#include <windows.h>
#include <stdio.h>
#include <math.h>
#include "resource.h"
#include "SimplePlayer.h"
#include "drawPerson.h"
#include <collisionCheck.h>


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("animationTool");


SimplePlayer model;
int sceneNum = 0;
vector scene[100][11];
bool sceneFlag[100][11] = { false, };
int bodySize[10];

CRITICAL_SECTION CS;

VOID CALLBACK drawModel(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime){
	static int cnt = 0;

	model.setBody(scene[cnt]);
	InvalidateRect(hWnd, NULL, true);
	 
	cnt++;
	if (cnt == sceneNum){
		KillTimer(hWnd, 1);
		cnt = 0;
	}
}


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
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, 1030, 850, NULL, (HMENU)NULL, hInstace, NULL);
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, NULL, 0, 0)){
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}
int g_dlgNum;
BOOL CALLBACK digProc(HWND hDig, UINT iMessage, WPARAM wParam, LPARAM lParam){
	switch (iMessage)
	{
	case WM_INITDIALOG:
		SetDlgItemInt(hDig, IDC_EDIT1, g_dlgNum, false);
		return true;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			g_dlgNum = GetDlgItemInt(hDig, IDC_EDIT1, NULL, FALSE);
			EndDialog(hDig, IDOK);
			return true;
		case IDCANCEL:
			EndDialog(hDig, IDCANCEL);
			return true;
		}
		break;
	}
	return false;
}

int countAnimationFrame(FILE *fp){
		fseek(fp, 0, 0);
		int cnt = 0;
		char ch[1000];
		while (fgets(ch, 1000, fp) != NULL){
			cnt++;
		}
		return cnt;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam){
	HDC hdc;
	PAINTSTRUCT ps;
	static int selection;
	static bool drawFlag = false;
	static bool rSetFlag = false;


	switch (iMessage){
	case WM_CREATE:{
		CreateWindow(TEXT("button"), TEXT("저장"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 20, 20, 100, 25, hWnd, (HMENU)0, g_hInst, NULL);
		CreateWindow(TEXT("button"), TEXT("캡쳐"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 120, 20, 100, 25, hWnd, (HMENU)3, g_hInst, NULL);

		CreateWindow(TEXT("button"), TEXT("반전"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 220, 20, 100, 25, hWnd, (HMENU)1, g_hInst, NULL);
		CreateWindow(TEXT("button"), TEXT("재생"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 320, 20, 100, 25, hWnd, (HMENU)2, g_hInst, NULL);
		CreateWindow(TEXT("button"), TEXT("역전"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 420, 20, 100, 25, hWnd, (HMENU)4, g_hInst, NULL);

		model.base = { 200, 300 };

		int bodyLine[20] = { 0, 1, 1, 2, 3, 4, 4, 1, 1, 5, 5, 6, 8, 2, 7, 8, 2, 9, 9, 10 };
		for (int i = 0; i < 20; i = i + 2){
			vector l1 = model.getBodys()[bodyLine[i]];
			l1.x += (int)model.base.x;
			l1.y += (int)model.base.y;
			vector l2 = model.getBodys()[bodyLine[i + 1]];
			l2.x += (int)model.base.x;
			l2.y += (int)model.base.y;

			int d = sqrt(pow(l1.x - l2.x, 2) + pow(l1.y - l2.y, 2));
			bodySize[i / 2] = d;
		}

		return 0;
	}
	case WM_COMMAND:
		switch (LOWORD(wParam)){
		case 0:{
			//MessageBox(hWnd, TEXT("저장"), TEXT("저장"), MB_OK);
			FILE *fp = fopen("test", "w+");

		//	Point* b = model.getBodys();
		//	fprintf(fp, "%d, %d, %d, %d , %d, %d , %d, %d , %d, %d , %d, %d , %d, %d , %d, %d , %d, %d , %d, %d , %d, %d\n" \
//				, b[0].x, b[0].y, b[1].x, b[1].y, b[2].x, b[2].y, b[3].x, b[3].y, b[4].x, b[4].y, b[5].x, b[5].y, b[6].x, b[6].y, b[7].x, b[7].y, b[8].x, b[8].y, b[9].x, b[9].y, b[10].x, b[10].y);
			for (int i = 0; i < sceneNum; i++){
				for (int j = 0; j < 11; j++){
					fprintf(fp, "%d, %d, ", (int)scene[i][j].x, (int)scene[i][j].y);
				}
				fprintf(fp, "\n");
			}
			/*
			Player temp;
			b = temp.getBodys();
			fseek(fp, 0, 0);
			fscanf(fp, "%d, %d, %d, %d , %d, %d , %d, %d , %d, %d , %d, %d , %d, %d , %d, %d , %d, %d , %d, %d , %d, %d "\
				, &b[0].x, &b[0].y, &b[1].x, &b[1].y, &b[2].x, &b[2].y, &b[3].x, &b[3].y, &b[4].x, &b[4].y, &b[5].x, &b[5].y, &b[6].x, &b[6].y, &b[7].x, &b[7].y, &b[8].x, &b[8].y, &b[9].x, &b[9].y, &b[10].x, &b[10].y);
		
			TCHAR str[1000];
			wsprintf(str, TEXT("body[0] = { %d, %d };\nbody[1] = { %d, %d };\nbody[2] = { %d, %d };\nbody[3] = { %d, %d };\nbody[4] = { %d, %d };\nbody[5] = { %d, %d };\nbody[6] = { %d, %d };\nbody[7] = { %d, %d };\nbody[8] = { %d, %d };\nbody[9] = { %d, %d };\nbody[10] = { %d, %d };\n") 
				, b[0].x, b[0].y, b[1].x, b[1].y, b[2].x, b[2].y, b[3].x, b[3].y, b[4].x, b[4].y, b[5].x, b[5].y, b[6].x, b[6].y, b[7].x, b[7].y, b[8].x, b[8].y, b[9].x, b[9].y, b[10].x, b[10].y);
			MessageBox(hWnd, str, TEXT("저장"), MB_OK);
			*/
			fclose(fp);
			break;
		}
		case 1:
		{
			FILE *fp = fopen("test", "r");

			int cnt = countAnimationFrame(fp);

			vector** walkAni;
			vector b[11];
			fseek(fp, 0, 0);
			walkAni = new vector*[cnt];
			for (int i = 0; i < cnt; i++){
				walkAni[i] = new vector[11];
			}
			int j = 0;
			while (j < cnt){
				for (int i = 0; i < 11; i++){
					fscanf(fp, "%lf, %lf,", &b[i].x, &b[i].y);
					walkAni[j][i] = b[i];
				}
				fscanf(fp, "\n");
				j++;
			}

			fclose(fp);
			int sceneNum = 0;
			while (sceneNum < cnt){
				vector temp;
				temp = walkAni[sceneNum][3];
				walkAni[sceneNum][3] = walkAni[sceneNum][6] ;
				walkAni[sceneNum][6] = temp;


				temp = walkAni[sceneNum][4];
				walkAni[sceneNum][4] = walkAni[sceneNum][5];
				walkAni[sceneNum][5] = temp;


				temp = walkAni[sceneNum][8];
				walkAni[sceneNum][8] = walkAni[sceneNum][9];
				walkAni[sceneNum][9] = temp;


				temp = walkAni[sceneNum][7];
				walkAni[sceneNum][7] = walkAni[sceneNum][10];
				walkAni[sceneNum][10] = temp;

				walkAni[sceneNum][3].x *= -1;
				walkAni[sceneNum][6].x *= -1;
				walkAni[sceneNum][4].x *= -1;
				walkAni[sceneNum][5].x *= -1;
				walkAni[sceneNum][8].x *= -1;
				walkAni[sceneNum][9].x *= -1;
				walkAni[sceneNum][7].x *= -1;
				walkAni[sceneNum][10].x *= -1;

				sceneNum++;
			}
			fp = fopen("test2", "w+");

			for (int i = 0; i < sceneNum; i++){
				for (int j = 0; j < 11; j++){
					fprintf(fp, "%d, %d, ", (int)walkAni[i][j].x, (int)walkAni[i][j].y);
				}
				fprintf(fp, "\n");
			}
			fclose(fp);
			break;
		}
		case 2:{
			SetTimer(hWnd, 1, 100, drawModel);
			break;
		}
		case 3:{
			vector* b = model.getBodys();

			for (int i = 0; i < 11; i++){
				scene[sceneNum][i].x = b[i].x;
				scene[sceneNum][i].y = b[i].y;
			}
			sceneNum++;
			break;
		}
		case 4:
			int tempNum = sceneNum;
			int limit = sceneNum;

			for (int i = 0; i <limit; i++){
				for (int j = 0; j < 11;j++)
					 scene[i+limit][j] = scene[tempNum-1][j];
				sceneNum++;
				tempNum--;
			}
		}
		return 0;
	case WM_MOUSEMOVE:
		if (drawFlag == true){
			int x = LOWORD(lParam) - (int)model.base.x;
			int y = HIWORD(lParam) - (int)model.base.y;

			double inLine, outLine;
			vector selctPoint = model.getBodys()[selection];

			vector leftPoint;
			int leftPointIndex;
			vector fixPoint;
			if (!((selection < 3) || (selection >6))){
				fixPoint = model.getBodys()[1];
				if (selection == 3){
					leftPoint = model.getBodys()[4];
					leftPointIndex = 4;
					inLine = bodySize[3];
					outLine = bodySize[2];
				}
				else if (selection == 4){
					leftPoint = model.getBodys()[3];
					leftPointIndex = 3; 
					inLine = bodySize[3];
					outLine = bodySize[2];
				}
				else if (selection == 5){
					leftPoint = model.getBodys()[6];
					leftPointIndex = 6;
					inLine = bodySize[4];
					outLine = bodySize[5];
				}
				else if (selection == 6){
					leftPoint = model.getBodys()[5];
					leftPointIndex = 5;
					inLine = bodySize[4];
					outLine = bodySize[5];
				}

			}
			else if (!((selection < 7) || (selection >10))){
				fixPoint = model.getBodys()[2];
				if (selection == 7){
					leftPoint = model.getBodys()[8];
					leftPointIndex = 8;
					inLine = bodySize[7];
					outLine = bodySize[6];
				}
				else if (selection == 8){
					leftPoint = model.getBodys()[7];
					leftPointIndex = 7;
					inLine = bodySize[7];
					outLine = bodySize[6];
				}
				else if (selection == 9){
					leftPoint = model.getBodys()[10];
					leftPointIndex = 10;
					inLine = bodySize[8];
					outLine = bodySize[9];
				}
				else if (selection == 10){
					leftPoint = model.getBodys()[9];
					leftPointIndex = 9;
					inLine = bodySize[8];
					outLine = bodySize[9];
				}
			}

			if (selection == 3 || selection == 6 || selection == 7 || selection == 10){
				vector cA = fixPoint, cB = selctPoint;
				static double rA, rB;
				if (!rSetFlag){
					rA = sqrt(pow(fixPoint.x - leftPoint.x, 2) + pow(fixPoint.y - leftPoint.y, 2));
					rB = sqrt(pow(selctPoint.x - leftPoint.x, 2) + pow(selctPoint.y - leftPoint.y, 2));
					rSetFlag = true;
				}
				
				cB = { (double)x, (double)y };
				if ((rA + rB) > sqrt(pow(x - fixPoint.x, 2) + pow(y - fixPoint.y, 2))){
					//두원의 교점의 직선의 방정식 Ax+By+C=0
					double A = -2 * cA.x + 2 * cB.x;
					double B = -2 * cA.y + 2 * cB.y;
					if (!(B<-0.0000000001 || B>0.0000000001)){
						break;
					}
					double C = pow(cA.x, 2) + pow(cA.y, 2) - pow(cB.x, 2) - pow(cB.y, 2) - pow(rA, 2) + pow(rB, 2);

					//직선의 방정식과 선택한 원의 교점에 대한 이차 방정식 ax^2+bx+c=0
					double a = 1 + (pow(A, 2) / pow(B, 2));
					double b = -2 * cB.x + 2 * (A*C / pow(B, 2)) + 2 * cB.y*A / B;
					double c = pow(cB.x, 2) + (pow(C, 2) / pow(B, 2)) + 2 * cB.y*C / B + pow(cB.y, 2) - pow(rB, 2);

					//근의 공식
					double temp = sqrt(pow(b, 2) - 4 * a*c);
					double x1 = (-1 * b + temp) / (2*a);
					double y1 = (-1 * x1*A - C) / B;
					double x2 = (-1 * b - temp) / (2*a);
					double y2 = (-1 * x2*A - C) / B;
					//부위에 따른 x,y결정 
					vector select;
					if (selection == 3 || selection == 6){
						y2 < y1 ? select = { x1, y1 } : select = { x2, y2 };
					}
					else if (selection == 7 ){
						x2 < x1 ? select = { x1, y1 } : select = { x2, y2 };
					}
					else if (selection == 10){
						y1 < y2 ? select = { x1, y1 } : select = { x1, y1 };
					}
					model.getBodys()[leftPointIndex].x = select.x ;
					model.getBodys()[leftPointIndex].y =  select.y;
					model.getBodys()[selection].x = (double)x;
					model.getBodys()[selection].y = (double)y;
				}
				else{
					
				}
			}

			InvalidateRect(hWnd, NULL, true);
		}
		return 0;
	case WM_LBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		//관절 선택
		for (int i = 0; i < 11; i++){
			int tempX = model.getBodys()[i].x + (int)model.base.x - x;
			int tempY = model.getBodys()[i].y + (int)model.base.y - y;
			if (!((6 < tempX || tempX < -6) || (6 < tempY || -6 > tempY))){
				drawFlag = true;
				selection = i;
				return 0;
			}
		}
		//몸 선택
		int bodyLine[20] = {0,1,1,2,3,4,4,1,1,5,5,6,8,2,7,8,2,9,9,10};
		for (int i = 0; i < 20; i=i+2){
			vector l1 = model.getBodys()[bodyLine[i]];
			l1.x += (int)model.base.x;
			l1.y += (int)model.base.y;
			vector l2 = model.getBodys()[bodyLine[i+1]];
			l2.x += (int)model.base.x;
			l2.y += (int)model.base.y;

			int t = getDistanceBetweenPointAndLine(x, y, l1.x, l1.y, l2.x, l2.y);
			if (getDistanceBetweenPointAndLine(x, y, l1.x, l1.y, l2.x, l2.y) < 10){
				g_dlgNum = bodySize[i / 2];
			//	DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, digProc);
				bodySize[i / 2] = g_dlgNum;
			}
		}

		return 0;
	}
	case WM_LBUTTONUP:
		drawFlag = false;
		rSetFlag = false;
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
	
		drawPerson(hdc, model);

		MoveToEx(hdc, 0, 615, NULL);
		LineTo(hdc, 400, 615);
		EndPaint(hWnd, &ps);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}