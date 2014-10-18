#include <Windows.h>
#include <MathUtils.h>


void drawPerson(HDC hdc, SimplePlayer p){
	HPEN hPen = CreatePen(PS_SOLID, 2, RGB(0,0,0));
	HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

	int baseX = (int)p.base.x;
	int baseY = (int)p.base.y;
	vector *temp = p.getBodys();
	vector body[11];
	for (int i = 0; i < 11; i++){
		body[i] = temp[i];
	}

	MoveToEx(hdc, (int)body[0].x +  baseX, (int)body[0].y +  baseY, NULL);
	LineTo(hdc, (int)body[1].x +  baseX, (int)body[1].y + baseY);
	LineTo(hdc, (int)body[2].x + baseX, (int)body[2].y +  baseY);

	MoveToEx(hdc, (int)body[3].x + baseX, (int)body[3].y + baseY, NULL);
	LineTo(hdc, (int)body[4].x + baseX, (int)body[4].y + baseY);
	LineTo(hdc, (int)body[1].x + baseX, (int)body[1].y +  baseY);
	LineTo(hdc, (int)body[5].x + baseX, (int)body[5].y +  baseY);
	LineTo(hdc, (int)body[6].x + baseX, (int)body[6].y + baseY);

	MoveToEx(hdc, (int)body[7].x + baseX, (int)body[7].y +  baseY, NULL);
	LineTo(hdc, (int)body[8].x + baseX, (int)body[8].y +  baseY);
	LineTo(hdc, (int)body[2].x +baseX, (int)body[2].y +  baseY);
	LineTo(hdc, (int)body[9].x +baseX, (int)body[9].y +  baseY);
	LineTo(hdc, (int)body[10].x + baseX, (int)body[10].y + baseY);

	Ellipse(hdc, (int)body[0].x - p.headSize +  baseX, (int)body[0].y - p.headSize +  baseY, (int)body[0].x + p.headSize +baseX, (int)body[0].y + p.headSize  + baseY);

	Ellipse(hdc, (int)body[3].x - p.handSize +baseX, (int)body[3].y - p.handSize +  baseY, (int)body[3].x + p.handSize +  baseX, (int)body[3].y + p.handSize + baseY);
	Ellipse(hdc, (int)body[6].x - p.handSize + baseX, (int)body[6].y - p.handSize +  baseY, (int)body[6].x + p.handSize + baseX, (int)body[6].y + p.handSize  + baseY);

	SelectObject(hdc, hOldPen);
	DeleteObject(hPen);
}
