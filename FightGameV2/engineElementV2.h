#ifndef ENGINE_ELEMENT_V2_H
#define ENGINE_ELEMENT_V2_H
#include "MathUtils.h"

struct box{
	vector position;
	vector width;
	double rotation;
	vector v;

	box(){}
	box(vector position_, vector width_){
		position = position_;
		width = width_;
		rotation = 0;
		v = { 0, 0 };
	}
	void getVertexs(vector *v){
		double x = width.y / 2.0;
		double y = width.x / 2.0;
		/*
		v1 - v2
		ㅣ   ㅣ
		v4 - v3
		*/
		vector vertexs[] = { { -1 * x, -1 * y }, { x, -1 * y }, { x, y }, { -1 * x, y } };
		for (int i = 0; i < 4; i++){
			v[i] = vertexs[i];
		}
	}
	void getEdge(vector *edge){
		double c = cos(rotation);
		double s = sin(rotation);
		vector e1 = { width.x / 2.0, 0 };
		vector e2 = { 0, width.y / 2.0 };
		vector te[2] = { { e1.x*c + e1.y*s, -1 * e1.x*s + e1.y*c },
		{ e2.x*c + e2.y*s, -1 * e2.x*s + e2.y*c }
		};
		edge[0] = te[0];
		edge[1] = te[1];
	}
	void getRotationVertexs(vector *v_){
		vector v[4];
		getVertexs(v);
		double c = cos(rotation);
		double s = sin(rotation);
		//회전 변환
		vector tv[4] = { { v[0].x*c + v[0].y*s, -1 * v[0].x*s + v[0].y*c },
		{ v[1].x*c + v[1].y*s, -1 * v[1].x*s + v[1].y*c },
		{ v[2].x*c + v[2].y*s, -1 * v[2].x*s + v[2].y*c },
		{ v[3].x*c + v[3].y*s, -1 * v[3].x*s + v[3].y*c }
		};
		for (int i = 0; i < 4; i++){
			v_[i] = tv[i];
		}
	}
	double getBottom(){
		vector tv[4];
		getRotationVertexs(tv);
		double height1 = abs(tv[0].y) + abs(tv[2].y);
		double height2 = abs(tv[1].y) + abs(tv[3].y);
		double result = 0;
		height1 < height2 ? result = height2 : result = height1;
		result *= 0.5;
		return result;
	}
};

#endif