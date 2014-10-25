#include "collisionCheck.h"

bool isCollisionEllipse(int cx, int cy, int cr, int px, int py){
	double deltaX = cx - px;
	double deltaY = cy - py;
	if (cr*cr > deltaX*deltaX + deltaY*deltaY){
		return true;
	}
	return false;
}

double getDistanceBetweenPointToPoint(double x1, double y1, double x2, double y2){
	double deltaY = y2 - y1;
	double deltaX = x2 - x1;
	return sqrt((double)(deltaX*deltaX + deltaY*deltaY));
}


double getShortDistanceBetweenPointToTwoPoint(double px, double py, double lx1, double ly1, double lx2, double ly2){
	double distance1 = getDistanceBetweenPointToPoint(px, py, lx1, ly1);
	double distance2 = getDistanceBetweenPointToPoint(px, py, lx2, ly2);
	if (distance1 > distance2){
		return distance2;
	}
	else{
		return distance1;
	}
}

double getDistanceBetweenPointAndLine(double px, double py, double lx1, double ly1, double lx2, double ly2){
	if (lx1 == lx2){
		if ((max(ly1, ly2) > py && py > min(ly1, ly2))){
			return abs(lx1 - px);
		}
		else{
			return getShortDistanceBetweenPointToTwoPoint(px, py, lx1, ly1, lx2, ly2);
		}
	}
	else if (ly1 == ly2){
		if ((max(lx1, lx2) > px && px > min(lx1, lx2))){
			return abs(ly1 - py);
		}
		else{
			return getShortDistanceBetweenPointToTwoPoint(px, py, lx1, ly1, lx2, ly2);
		}
	}
	else{
		if (!(max(ly1, ly2) > py && py > min(ly1, ly2))) {
			return getShortDistanceBetweenPointToTwoPoint(px, py, lx1, ly1, lx2, ly2);
		}
		else{
			double m = (double)(ly2 - ly1) / (lx2 - lx1);
			double distance = (abs((-1 * m *px) + py + (m*lx1 - ly1))) / (sqrt(m*m + 1));
			return distance;
		}
	}
}

bool isCollisionLineToLine(line l1, line l2){
	vector l1_1 = l1.pE1->position;
	vector l1_2 = l1.pE2->position;
	vector l2_1 = l2.pE1->position;
	vector l2_2 = l2.pE2->position;
	if (2 < getDistanceBetweenPointAndLine(l1_1.x, l1_1.y, l2_1.x, l2_1.y, l2_2.x, l2_2.y))
		true;
	if (2 < getDistanceBetweenPointAndLine(l1_2.x, l1_2.y, l2_1.x, l2_1.y, l2_2.x, l2_2.y))
		true;
	if (2 < getDistanceBetweenPointAndLine(l2_1.x, l2_1.y, l1_1.x, l1_1.y, l1_2.x, l1_2.y))
		true;
	if (2 < getDistanceBetweenPointAndLine(l2_2.x, l2_2.y, l1_1.x, l1_1.y, l1_2.x, l1_2.y))
		true;
	return false;
}
//test용
extern HWND g_hWnd;
vector findCollisionLineToLine(line l1, line l2){
	vector l[] = { l1.pE1->position ,
		l1.pE2->position ,
		l2.pE1->position ,
		l2.pE2->position 
	};
	//일차 충돌 예측
	vector c1 = (l[0] + l[1]) / 2;
	double r1 = (l[0] - l[1]).length()/2;
	vector c2 = (l[1] + l[3]) / 2;
	double r2 = (l[2] - l[3]).length()/2;
	double d = (c1 - c2).length();
	if (d > r1 + r2){
		return vector();
	}
	//
	box b1;
	b1.position = { ((l[0].x + l[1].x) / 2), ((l[0].y + l[1].y) / 2) };
	if ((l[0].x - l[1].x) == 0)
		b1.rotation = 0;
	else
		b1.rotation = atan2((l[0].x - l[1].x), (l[0].y - l[1].y));

	b1.width = { sqrt(pow(l[0].x - l[1].x, 2) + pow(l[0].y - l[1].y, 2)), 2.0 };

	box b2;
	b2.position = { ((l[2].x + l[3].x) / 2), ((l[2].y + l[3].y) / 2) };
	if ((l[2].x - l[3].x) == 0)
		b2.rotation = 0;
	else
		b2.rotation = atan2((l[2].x - l[3].x), (l[2].y - l[3].y));
	b2.width = { sqrt(pow(l[2].x - l[3].x, 2) + pow(l[2].y - l[3].y, 2)), 2.0 };
	
	/*
	//test용
	static HDC hdc = GetDC(g_hWnd);
	static int a = 0;
	a++;
	a=a % 60;
	if (a == 0){
		drawBox(hdc, b1);
		drawBox(hdc, b2);
	}
	*/

	if (isCollision(b1, b2)){
		vector l[] = { l1.pE1->position ,
			l1.pE2->position ,
			l2.pE1->position ,
			l2.pE2->position 
		};
		double temp[] = { getDistanceBetweenPointAndLine(l[0].x, l[0].y, l[2].x, l[2].y, l[3].x, l[3].y),
			getDistanceBetweenPointAndLine(l[1].x, l[1].y, l[2].x, l[2].y, l[3].x, l[3].y),
			getDistanceBetweenPointAndLine(l[2].x, l[2].y, l[0].x, l[0].y, l[1].x, l[1].y),
			getDistanceBetweenPointAndLine(l[3].x, l[3].y, l[0].x, l[0].y, l[1].x, l[1].y)
		};
		double shortDistance = temp[0];
		int index = 0;
		for (int i = 1; i < 4; i++){
			if (shortDistance>temp[i]){
				shortDistance = temp[i];
				index = i;
			}
		}
		return l[index];
	}
	
	return vector();

}

vector findCollisionLineToBox(line l1, box b2){
	vector l[] = { l1.pE1->position,
		l1.pE2->position,
	};
	box b1;
	b1.position = { ((l[0].x + l[1].x) / 2), ((l[0].y + l[1].y) / 2) };
	if ((l[0].x - l[1].x) == 0)
		b1.rotation = 0;
	else
		b1.rotation = atan2((l[0].x - l[1].x), (l[0].y - l[1].y));

	b1.width = { sqrt(pow(l[0].x - l[1].x, 2) + pow(l[0].y - l[1].y, 2)), 2.0 };

	

	if (isCollision(b1, b2)){
		vector vertexs[4];
		b2.getVertexs(vertexs);
		vector l[] = { l1.pE1->position,
			l1.pE2->position,
			vertexs[1],
			vertexs[2],
			vertexs[3],
			vertexs[0]
		};
		double temp[] = { getDistanceBetweenPointAndLine(l[0].x, l[0].y, l[2].x, l[2].y, l[3].x, l[3].y),
			getDistanceBetweenPointAndLine(l[1].x, l[1].y, l[2].x, l[2].y, l[3].x, l[3].y),
			getDistanceBetweenPointAndLine(l[2].x, l[2].y, l[0].x, l[0].y, l[1].x, l[1].y),
			getDistanceBetweenPointAndLine(l[3].x, l[3].y, l[0].x, l[0].y, l[1].x, l[1].y)
		};
		double shortDistance = temp[0];
		int index = 0;
		for (int i = 1; i < 4; i++){
			if (shortDistance>temp[i]){
				shortDistance = temp[i];
				index = i;
			}
		}
		return l[index];
	}

	return vector();

}