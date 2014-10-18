#ifndef _MATHUTILS_H
#define _MATHUTILS_H
#include <math.h>

struct vector{
	double x, y;
	vector(){ x = 0; y = 0; }
	~vector(){}
	vector(double _x,double _y){
		x = _x;
		y = _y;
	}
	vector operator -(const vector &v){
		return vector( x - v.x, y - v.y );
	}
	vector operator +(const vector &v){
		return vector(x + v.x, y + v.y);
	}
	vector operator +=(const vector &v){
		return vector(x=x + v.x, y=y + v.y);
	}
	vector operator *(const double &v){
		return vector(x * v, y * v);
	}
	vector operator /(const double &v){
		return vector(x / v, y / v);
	}
	bool operator ==(const vector &v){
		return (x == v.x) && (y == v.y);
	}
	bool operator !=(const vector &v){
		return !((x == v.x) && (y == v.y));
	}
	double length(){
		return sqrt(x*x + y*y);
	}

	vector Normalize(){
		double l = length();
		return vector(x / l, y / l);
	}
};


inline double dot(vector a, vector b){
	return a.x*b.x + a.y * b.y;
}



#endif