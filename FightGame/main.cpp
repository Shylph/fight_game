#include <windows.h>
#include "resource.h"
#include "player.h"
#include "collisionCheck.h"
#include "keyCheck.h"
#include "draw.h"
#include "engine.h"

extern Player person[2];
extern HBITMAP hBit;
extern HWND g_hWnd;
extern int fps;
extern HBITMAP collisionBitmap[3];
int tempFps = 0;
int engineTempFps = 0;
int engineFps = 0;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("FightGame");

int APIENTRY WinMain(HINSTANCE hInstace, HINSTANCE hPrevInstace, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstace;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstace;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, 1015, 680, NULL, (HMENU)NULL, hInstace, NULL);
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, NULL, 0, 0)){
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}


VOID CALLBACK checkFps(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime){
	fps = tempFps;
	tempFps = 0;
	engineFps = engineTempFps;
	engineTempFps = 0;
}

HANDLE hDrawThread;

DWORD WINAPI drawThread(LPVOID pro){
	static LARGE_INTEGER before, after, freq;
	static double elapse;
	QueryPerformanceCounter(&before);
	while (true){
		QueryPerformanceCounter(&after);
		QueryPerformanceFrequency(&freq);
		elapse = elapse + ((double)(after.QuadPart - before.QuadPart)) / ((double)freq.QuadPart);
		before = after;
		
		double interval =0.0165;
		if (elapse > interval){
			elapse = elapse - interval;
			drawProcess();
		}

		//무한 루프로 인한 CPU사용 완화 코드?
		static double delay = 0.08;
		static int targetFPS = (int)(1 / interval);
		if (fps < targetFPS){
			delay -= 0.00001;
		}
		else {
			delay += 0.00001;
		}
		if (elapse < delay)
			WaitForSingleObject((void *)hDrawThread, 1);
	}
	return 0;
}

void drawStart(){
	DWORD ThreadID;
	hDrawThread = CreateThread(NULL, 0, drawThread, NULL, 0, &ThreadID);
}
Engine *engine;

void initGame(HWND hWnd){

	collisionBitmap[0] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));
	collisionBitmap[1] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP2));
	collisionBitmap[2] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP3));

	person[0].setDirection(PD_ViEW_RIGHT);
	person[0].addLife(1000);
	person[0].subMana(1000);
	person[0].base.position = { 100, -100 };
	person[0].base.v = { 0, 0 };
	person[0].setKey('A', 'D', 'W', 'S', 'F', 'G', 'H');
	person[0].color = RGB(255, 0, 0);
	person[0].initBody();

	person[1].setDirection(PD_ViEW_LEFT);
	person[1].addLife(1000);
	person[1].subMana(1000);
	person[1].base.position = { 850, -100 };
	person[1].base.v = { 0, 0 };
	person[1].setKey(VK_LEFT, VK_RIGHT, VK_UP, VK_DOWN, VK_OEM_COMMA, VK_OEM_PERIOD, VK_OEM_2);
	person[1].color = RGB(0, 255, 0);
	person[1].initBody();
}

CRITICAL_SECTION CS;
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam){
	HDC hdc;
	PAINTSTRUCT ps;
	g_hWnd = hWnd;
	switch (iMessage){
	case WM_CREATE:{
		InitializeCriticalSection(&CS);
		engine = new Engine(10, 100, 0, 1000, 600);
		initGame(hWnd);
		engine->addPlayer1(&person[0]);
		engine->addPlayer2(&person[1]);
		engine->engineStart();
		drawStart();
		SetTimer(hWnd, 2, 1000, checkFps);
		return 0;
	}
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDM_GAME_START:
			if (engine->isEngineRunning())
				engine->exitEngine();
			initGame(hWnd);
			WaitForSingleObject(engine->hEngineThread, 2);
			engine->engineStart();
		}
		return 0;
	case WM_DESTROY:
		engine->exitEngine();
		SuspendThread(hDrawThread);
		CloseHandle(hDrawThread);
		DeleteObject(hBit);
		DeleteCriticalSection(&CS);
		PostQuitMessage(0);
		return 0;
	case WM_PAINT:
		tempFps++;
		hdc = BeginPaint(hWnd, &ps);
		if (hBit){
			DrawBitmap(hdc, 0, 0, hBit);
		}
		EndPaint(hWnd, &ps);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
