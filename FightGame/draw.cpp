#include "draw.h"

extern HWND g_hWnd;
extern Player person[2];
extern HBITMAP hBit; 
extern int fps;
extern int engineFps;
extern HINSTANCE g_hInst;
HBITMAP collisionBitmap[3];;

std::list<vector> collisionList;

void TransDrawBitmap(HDC hdc, int x, int y, HBITMAP hBit, COLORREF col)
{
	HDC MemDC;
	HBITMAP OldBitmap;
	int bx, by;
	BITMAP bit;

	MemDC = CreateCompatibleDC(hdc);
	OldBitmap = (HBITMAP)SelectObject(MemDC, hBit);

	GetObject(hBit, sizeof(BITMAP), &bit);
	bx = bit.bmWidth;
	by = bit.bmHeight;

	GdiTransparentBlt(hdc, x, y, bx, by, MemDC, 0, 0, bx, by, col);

	SelectObject(MemDC, OldBitmap);
	DeleteDC(MemDC);
}

vector colisionPosition[100];
int start = 0;
int end = 0;
void addColisionPosition(vector p){
	colisionPosition[end++] = p;
	if (end > 100){
		int cnt=0;
		for (int i = start; i < end; i++){
			colisionPosition[cnt] = colisionPosition[i];
			cnt++;
		}
		start = 0;
		end = cnt;
	}
	if (start == end){
		start = 0;
		end = 0;
	}
}

void drawColision(HDC hdc){
	static int delay[100] = { 0, };
	int select = rand() % 3;
	for (int i = start; i<end; i++){
		TransDrawBitmap(hdc, (int)colisionPosition[i].x - 24, (int)colisionPosition[i].y - 24, collisionBitmap[select], RGB(255, 0, 255));
		delay[i]++;
		if (delay[i]>10){
			delay[i]=0;
			colisionPosition[start] = { -50, -50 };
			start++;
		}
	}
}
struct box missile[20];
void drawMissile(HDC hdc){
	for (int i = 0; i < 20; i++){
		drawBox(hdc, missile[i]);
	}
}

VOID drawProcess(){
	HWND hWnd = g_hWnd;
	person[0].checkKey();
	person[1].checkKey();
	person[1].updateAnimation();
	person[0].updateAnimation();

	RECT crt;
	HDC hdc, hMemDC;
	HBITMAP oldBit;
	hdc = GetDC(hWnd);
	GetClientRect(hWnd, &crt);
	if (hBit == NULL) {
		hBit = CreateCompatibleBitmap(hdc, crt.right, crt.bottom);
	}
	hMemDC = CreateCompatibleDC(hdc);
	oldBit = (HBITMAP)SelectObject(hMemDC, hBit);

	//배경
	FillRect(hMemDC, &crt, GetSysColorBrush(COLOR_WINDOW));

	drawPerson(hMemDC, person[0]);
	drawPerson(hMemDC, person[1]);

	//background
	MoveToEx(hMemDC, 0, 0, NULL);
	LineTo(hMemDC, 0, 600);
	LineTo(hMemDC, 1000, 600);
	LineTo(hMemDC, 1000, 0);

	//test 체력
	TCHAR Mes[10];
	wsprintf(Mes, TEXT("생명=%d"), person[0].getLife());
	TextOut(hMemDC, 200, 30, Mes, lstrlen(Mes));

	wsprintf(Mes, TEXT("생명=%d"), person[1].getLife());
	TextOut(hMemDC, 600, 30, Mes, lstrlen(Mes));

	//체력바 틀
	Rectangle(hMemDC, 30, 50, 450, 90);
	Rectangle(hMemDC, 550, 50, 970, 90);
	//체력바의 현재 체력 표시
	HBRUSH hBrush = CreateSolidBrush(RGB(255,0,0));
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hMemDC, hBrush);
	int p1Life = (int)(person[0].getLife() * 420 / 1000.0);
	int p2Life = (int)(person[1].getLife() * 420 / 1000.0);
	Rectangle(hMemDC, 30, 50, 30+p1Life, 90);
	Rectangle(hMemDC, 970- p2Life, 50, 970, 90);
	(HBRUSH)SelectObject(hMemDC, hOldBrush);
	DeleteObject(hBrush);

	//마나바 
	hBrush = CreateSolidBrush(RGB(255, 224, 140));
	hOldBrush = (HBRUSH)SelectObject(hMemDC, hBrush);
	int p1Mana = (int)(person[0].getMana() * 420 / 1000.0);
	int p2Mana = (int)(person[1].getMana() * 420 / 1000.0);
	Rectangle(hMemDC, 30, 90, 30 + p1Mana, 110);
	Rectangle(hMemDC, 970 - p2Mana, 90, 970, 110);
	(HBRUSH)SelectObject(hMemDC, hOldBrush);
	DeleteObject(hBrush);

	//fps
	TCHAR fpsStr[30];
	wsprintf(fpsStr, TEXT("FPS=%d"), fps);
	TextOut(hMemDC, 30, 10, fpsStr, lstrlen(fpsStr));
	//enginefps
	wsprintf(fpsStr, TEXT("EngineFPS=%d"), engineFps);
	TextOut(hMemDC, 30, 30, fpsStr, lstrlen(fpsStr));


	drawColision(hMemDC);

	drawMissile(hMemDC);

	SelectObject(hMemDC, oldBit);
	DeleteDC(hMemDC);
	ReleaseDC(hWnd, hdc);
	InvalidateRect(hWnd, NULL, false);
}


void drawPerson(HDC hdc, Player p){
	HPEN hPen = CreatePen(PS_SOLID, 2, p.color);
	HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

	int baseX = (int)p.base.position.x;
	int baseY = (int)p.base.position.y;
	
	physicalElement *bodyElement = p.getBodys();
	vector body[11];
	for (int i = 0; i < 11; i++){
		body[i] = bodyElement[i].position;
	}
	physicalElement *bm = p.getBodyMovement();

	MoveToEx(hdc, (int)body[0].x + (int)bm[0].position.x + baseX, (int)body[0].y + (int)bm[0].position.y + baseY, NULL);
	LineTo(hdc, (int)body[1].x + (int)bm[1].position.x + baseX, (int)body[1].y + (int)bm[1].position.y + baseY);
	LineTo(hdc, (int)body[2].x + (int)bm[2].position.x + baseX, (int)body[2].y + (int)bm[2].position.y + baseY);
	
	MoveToEx(hdc, (int)body[3].x + (int)bm[3].position.x + baseX, (int)body[3].y + (int)bm[3].position.y + baseY, NULL);
	LineTo(hdc, (int)body[4].x + (int)bm[4].position.x + baseX, (int)body[4].y + (int)bm[4].position.y + baseY);
	LineTo(hdc, (int)body[1].x + (int)bm[1].position.x + baseX, (int)body[1].y + (int)bm[1].position.y + baseY);
	LineTo(hdc, (int)body[5].x + (int)bm[5].position.x + baseX, (int)body[5].y + (int)bm[5].position.y + baseY);
	LineTo(hdc, (int)body[6].x + (int)bm[6].position.x + baseX, (int)body[6].y + (int)bm[6].position.y + baseY);

	MoveToEx(hdc, (int)body[7].x + (int)bm[7].position.x + baseX, (int)body[7].y + (int)bm[7].position.y + baseY, NULL);
	LineTo(hdc, (int)body[8].x + (int)bm[8].position.x + baseX, (int)body[8].y + (int)bm[8].position.y + baseY);
	LineTo(hdc, (int)body[2].x + (int)bm[2].position.x + baseX, (int)body[2].y + (int)bm[2].position.y + baseY);
	LineTo(hdc, (int)body[9].x + (int)bm[9].position.x + baseX, (int)body[9].y + (int)bm[9].position.y + baseY);
	LineTo(hdc, (int)body[10].x + (int)bm[10].position.x + baseX, (int)body[10].y + (int)bm[10].position.y + baseY);

	Ellipse(hdc, (int)body[0].x - p.headSize + (int)bm[0].position.x + baseX, (int)body[0].y - p.headSize + (int)bm[0].position.y + baseY, (int)body[0].x + p.headSize + (int)bm[0].position.x + baseX, (int)body[0].y + p.headSize + (int)bm[0].position.y + baseY);

	Ellipse(hdc, (int)body[3].x - p.handSize + (int)bm[3].position.x + baseX, (int)body[3].y - p.handSize + (int)bm[3].position.y + baseY, (int)body[3].x + p.handSize + (int)bm[3].position.x + baseX, (int)body[3].y + p.handSize + (int)bm[3].position.y + baseY);
	Ellipse(hdc, (int)body[6].x - p.handSize + (int)bm[6].position.x + baseX, (int)body[6].y - p.handSize + (int)bm[6].position.y + baseY, (int)body[6].x + p.handSize + (int)bm[6].position.x + baseX, (int)body[6].y + p.handSize + (int)bm[6].position.y + baseY);

	SelectObject(hdc, hOldPen);
	DeleteObject(hPen);
	
	hPen = CreatePen(PS_SOLID, 5, p.color);
	hOldPen = (HPEN)SelectObject(hdc, hPen);

	if ((p.getDirection()&PD_ViEW_LEFT)>0){
		MoveToEx(hdc, (int)body[0].x - 20 + baseX, (int)body[0].y - 5 + baseY, NULL);
		LineTo(hdc, (int)body[0].x - 7 + baseX, (int)body[0].y - 6 + baseY);
	}
	else{
		MoveToEx(hdc, (int)body[0].x + 20 + baseX, (int)body[0].y - 5 + baseY, NULL);
		LineTo(hdc, (int)body[0].x + 7 + baseX, (int)body[0].y - 6 + baseY);
	}

	
	SelectObject(hdc, hOldPen);
	DeleteObject(hPen);
}



void DrawBitmap(HDC hdc, int x, int y, HBITMAP hBit)
{
	HDC MemDC;
	HBITMAP OldBitmap;
	int bx, by;
	BITMAP bit;

	MemDC = CreateCompatibleDC(hdc);
	OldBitmap = (HBITMAP)SelectObject(MemDC, hBit);

	GetObject(hBit, sizeof(BITMAP), &bit);
	bx = bit.bmWidth;
	by = bit.bmHeight;

	BitBlt(hdc, x, y, bx, by, MemDC, 0, 0, SRCCOPY);

	SelectObject(MemDC, OldBitmap);
	DeleteDC(MemDC);
}
