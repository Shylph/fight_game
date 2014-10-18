#ifndef _ENGINE_ELEMENT_H
#define _ENGINE_ELEMENT_H
#include <MathUtils.h>

struct physicalElement{
	vector position;
	vector v;
	physicalElement(){
		position = { 0.0, 0.0 };
		v = { 0.0, 0.0 };
	}
	bool operator ==(const physicalElement &p){
		return ((position == p.position) && (v == p.v));
	}
	bool operator !=(const physicalElement &p){
		return !((position == p.position) && (v == p.v));
	}
};


struct line{
	physicalElement *pE1, *pE2;
	line(){}
	line(physicalElement *p1,physicalElement *p2){
		pE1 = p1;
		pE2 = p2;
	}
};

#endif