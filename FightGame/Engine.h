#ifndef _ENGINE_H
#define _ENGINE_H
#include <Windows.h>
#include <list>
#include <mmsystem.h>
#include <time.h>
#include "resource.h"
#include "MathUtils.h"
#include "collisionCheck.h"
#include "player.h"
#include "engineElement.h"
#include "draw.h"
/*
FPS 변수들 정리할 것
*/

class Engine
{
private:
	LARGE_INTEGER before, after, freq;
	double elapse=0;
	bool exitFlag = false;
	Player *player1, *player2;
	double pixelRate;
	int screenLeft, screenRight, screenBottom;
	double interval;
	std::list<physicalElement*> list;
	std::list<line*> lineList;
	std::list<line*> lineList2;

	void physicalProcess();
	void setInterval(int milliisecond);
	void setScreenArea(int left, int right, int bottom);
	void setRate1mPerPixel(int pixel);
	static DWORD WINAPI engineThread(LPVOID pro);
	void engineProcess();
	double getInterval(){return interval;}
	void basePhysicalProcess();
	void playAttackedSound();
public:
	HANDLE hEngineThread;
	Engine(int interval, int pixel, int left, int right, int bottom);
	~Engine();
	void addElement(physicalElement *element);
	void addPlayer1(Player *line);
	void addPlayer2(Player *line);
	void engineStart();
	void engineStop();
	void exitEngine();
	bool isEngineRunning(){
		return !exitFlag;
	}
};

#endif