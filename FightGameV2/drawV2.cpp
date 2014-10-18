#include "drawV2.h"

void drawBox(HDC hdc, box b){
	double x = b.width.y / 2.0;
	double y = b.width.x / 2.0;
	double c = cos(b.rotation);
	double s = sin(b.rotation);
	/*
	v1 - v2
	¤Ó   ¤Ó
	v4 - v3
	*/
	vector v1 = { -1 * x, -1 * y };
	vector v2 = { x, -1 * y };
	vector v3 = { x, y };
	vector v4 = { -1 * x, y };

	//È¸Àü
	vector tv1 = { v1.x*c + v1.y*s, -1 * v1.x*s + v1.y*c };
	vector tv2 = { v2.x*c + v2.y*s, -1 * v2.x*s + v2.y*c };
	vector tv3 = { v3.x*c + v3.y*s, -1 * v3.x*s + v3.y*c };
	vector tv4 = { v4.x*c + v4.y*s, -1 * v4.x*s + v4.y*c };

	MoveToEx(hdc, (int)(tv1.x + b.position.x), (int)(tv1.y + b.position.y), NULL);
	LineTo(hdc, (int)(tv2.x + b.position.x), (int)(tv2.y + b.position.y));
	LineTo(hdc, (int)(tv3.x + b.position.x), (int)(tv3.y + b.position.y));
	LineTo(hdc, (int)(tv4.x + b.position.x), (int)(tv4.y + b.position.y));
	LineTo(hdc, (int)(tv1.x + b.position.x), (int)(tv1.y + b.position.y));
}

void testDrawRectangle(HDC hdc){
	for (int i = 0; i < 2; i++){

		static double r = 0;
		if (r < 3.1415926535897932*2.0)
			r += 0.01;
		else
			r = 0;
		//	test2.boxs[i].rotation = r;
		//	drawBox(hdc, test2.boxs[i]);
	}
}

void drawTest(HDC hdc){
	//drawBox(hdc, test2.boxs[0]);
	//drawBox(hdc, test2.boxs[1]);
	//	testDrawRectangle(hdc);
}
