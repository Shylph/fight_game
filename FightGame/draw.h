#ifndef _DRAW_H
#define _DRAW_H
#include <Windows.h>
#include <math.h>
#include <list>
#include "resource.h"
#include <MathUtils.h>
#include "player.h"
#include <drawV2.h>
#include <engineElementV2.h>

void addColisionPosition(vector p);
void DrawBitmap(HDC hdc, int x, int y, HBITMAP hBit);
void drawPerson(HDC hdc, Player p);
VOID drawProcess();
void TransDrawBitmap(HDC hdc, int x, int y, HBITMAP hBit, COLORREF col);
#endif