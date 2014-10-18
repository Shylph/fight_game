#ifndef _ENGINEV2_H
#define _ENGINEV2_H
#include "MathUtils.h"
#include "engineElementV2.h"
#include <list>

class EngineV2
{
private:
	double pixelRate;
	int screenLeft, screenRight, screenBottom;
	double interval;
	std::list<box*> list;
public:
	EngineV2();
	~EngineV2();
	void setInterval(int milliisecond);
	void physicalProcess();
	void addBox(box *element);
	void setScreenArea(int left, int right, int bottom);
	void setRate1mPerPixel(int pixel);
	double getInterval(){
		return interval;
	}
};

#endif