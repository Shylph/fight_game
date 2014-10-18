#include "engine.h"
//가드, 기술 한두개, 에네르기파??????

extern struct box missile[10];

Engine::Engine(int interval, int pixel, int left, int right, int bottom)
{
	setInterval(interval);
	setScreenArea(left, right,bottom);
	setRate1mPerPixel(pixel);
}

Engine::~Engine()
{
	exitEngine();
}
extern HINSTANCE g_hInst;

void Engine::playAttackedSound(){
	srand((unsigned int)time(NULL));
	unsigned int sound[] = { IDR_WAVE1, IDR_WAVE2, IDR_WAVE3 };
	PlaySound(MAKEINTRESOURCE(sound[rand() % 3]), g_hInst, SND_RESOURCE | SND_ASYNC);
}

void Engine::basePhysicalProcess(){
	Player *p[2] = { player1, player2};
	for (int i=0; i<2; i++){
		//중력가속도 9.8
		p[i]->base.v.y = p[i]->base.v.y + 9.8 * interval;
		
		p[i]->base.v.y *= 0.99;
		p[i]->base.v.x *= 0.99;

		if ((screenBottom < (p[i]->base.position.y + p[i]->getBottom())) && !((p[i]->base.v.y<0))){
			p[i]->base.v.y *= -0.3;
			//마찰 0.9
			p[i]->base.v.x = p[i]->base.v.x * 0.9;
			p[i]->fallPlayer();
		}
		//왼쪽 충돌
		if ((screenLeft >(p[i]->base.position.x)) && !((p[i]->base.v.x > 0))){
			p[i]->base.v.x *= -0.2;
		}
		//오른쪽 충돌
		if ((screenRight < (p[i]->base.position.x)) && !((p[i]->base.v.x < 0))){
			p[i]->base.v.x *= -0.2;
		}
		if (player1->base.position.x  > player2->base.position.x ){
			player1->setDirection(PD_ViEW_LEFT);
			player2->setDirection(PD_ViEW_RIGHT);
		}
		else{
			player1->setDirection(PD_ViEW_RIGHT);
			player2->setDirection(PD_ViEW_LEFT);
		}
		/*
		//player1과 player2는 서로 넘어 갈 수 없음
		if (player1->base.position.x +20 >= player2->base.position.x -20){
			double gap = abs(player1->base.position.x - player2->base.position.x);
			player1->base.v.x -= 0.4*(gap);
			player2->base.v.x += 0.4*(gap);
		}
		//player1과 player2는 일정 높이 이상 올라 갈 수 없음
		if (p[i]->base.position.y <= 150){
			p[i]->base.v.y += 0.5;
		}
		*/
		
		//속도에 따른 이동
		p[i]->base.position.x += p[i]->base.v.x * interval * pixelRate;
		p[i]->base.position.y += p[i]->base.v.y * interval * pixelRate;
	}
}
extern CRITICAL_SECTION CS;
extern HWND g_hWnd;
void Engine::physicalProcess(){
	basePhysicalProcess();

	physicalElement *base1 = &player1->base, *base2 = &player2->base;

	std::list<vector> collsionList;
	std::list<line> collsionL1List;
	std::list<line> collsionL2List;


	std::list<line*>::iterator lineIter;
	std::list<line*>::iterator lineIter2;

	//충돌 검사
	for (lineIter = lineList.begin(); lineIter != lineList.end(); lineIter++){
		line standard = *(*lineIter);
		physicalElement pE1[] = { *standard.pE1, *standard.pE2 };
		pE1[0].position += base1->position;
		pE1[1].position += base1->position;
		for (lineIter2 = lineList2.begin(); lineIter2 != lineList2.end(); lineIter2++){
			line temp = *(*lineIter2);
			physicalElement pE2[] = { *temp.pE1, *temp.pE2 };
			pE2[0].position += base2->position;
			pE2[1].position += base2->position;
			vector collsionPosition = findCollisionLineToLine(line(&pE1[0], &pE1[1]), line(&pE2[0], &pE2[1]));
			if (collsionPosition != vector()){
				collsionList.push_back(collsionPosition);
				collsionL1List.push_back(line(standard.pE1, standard.pE2));
				collsionL2List.push_back(line(temp.pE1,temp.pE2));
				break;
			}
		}
	}
	std::list<vector>::iterator collsionIter;
	std::list<line>::iterator l1Iter = collsionL1List.begin();
	std::list<line>::iterator l2Iter = collsionL2List.begin();;
	for (collsionIter = collsionList.begin(); collsionIter != collsionList.end(); collsionIter++){
		////
		vector l1V = l1Iter->pE1->v + l1Iter->pE2->v + player1->base.v;
		vector l2V = l2Iter->pE1->v + l2Iter->pE2->v + player2->base.v;
		line big;
		vector *bigBase, *littleBase;
		line little;
		Player *target;
		if (l1V.length() > l2V.length()){
			big = *l1Iter;
			bigBase = &player1->base.v;
			little = *l2Iter;
			target = player2;
			littleBase = &player2->base.v;
		}
		else{
			big = *l2Iter;
			bigBase = &player2->base.v;
			little = *l1Iter;
			target = player1;
			littleBase = &player1->base.v;
		}
		//반작용 적용 할 것
		// 힘 전달 코드 필요
		vector attPower = { big.pE1->v.x + big.pE2->v.x + bigBase->x*0.1, big.pE1->v.y + big.pE2->v.y + bigBase->y*0.1 };
	//	addColisionPosition(*collsionIter);
		int damaged = target->damagedPlayer(attPower, little);
		if (damaged>5){
				playAttackedSound();
				addColisionPosition(*collsionIter);
				player1->cancelAttack();
				player2->cancelAttack();
		}else if (player1->base.position.x > player2->base.position.x){
			player1->base.v.x += 0.1 ;
			player2->base.v.x += -0.1;
		}
		else{
			player1->base.v.x += -0.1;
			player2->base.v.x += 0.1;
		}
		bigBase->x +=  littleBase->x *interval;
		bigBase->y += littleBase->y*interval;

		l1Iter++;
		l2Iter++;
	}
	
	physicalElement *body1 = player1->getBodys();
	physicalElement *body2 = player2->getBodys();
	physicalElement *pE1 = player1->getBodyMovement();
	physicalElement *pE2 = player2->getBodyMovement();
	physicalElement voidPE;
	for (int i = 0; i < 11; i++){
		body1[i].v.x *= 0.99;
		body1[i].v.y *= 0.99;
		pE1[i].position.x += (pE1[i].v.x * interval * pixelRate);
		pE1[i].position.y += (pE1[i].v.y * interval * pixelRate);
		pE1[i].position.x *= 0.8;
		pE1[i].position.y *= 0.8;
		pE1[i].v.x *= 0.8;
		pE1[i].v.y *= 0.8;

		body2[i].v.x *= 0.99;
		body2[i].v.y *= 0.99;
		pE2[i].position.x += (pE2[i].v.x * interval * pixelRate);
		pE2[i].position.y += (pE2[i].v.y * interval * pixelRate);
		pE2[i].position.x *= 0.8;
		pE2[i].position.y *= 0.8;
		pE2[i].v.x *= 0.8;
		pE2[i].v.y *= 0.8;
		
	}

	
	if (player1->getLife() <= 0 || player2->getLife() <= 0){
		exitEngine();
		MessageBox(g_hWnd, TEXT("게임이 끝났습니다. 다시 시작하려면 게임/시작")
			TEXT(" 항목을 선택해 주십시오."), TEXT("알림"), MB_OK);
	}
}

void Engine::addElement(physicalElement *element){
	list.push_back(element);
}

void Engine::addPlayer1(Player *p){
	player1 = p;
	list.push_back(&p->base);
	for (int i = 0; i < 10; i++)
		lineList.push_back(&p->getBodyLine()[i]);
}
void Engine::addPlayer2(Player *p){
	player2 = p;
	list.push_back(&p->base);
	for (int i = 0; i < 10; i++)
		lineList2.push_back(&p->getBodyLine()[i]);
}
void Engine::setInterval(int milliisecond){
	interval = milliisecond/1000.0;
}

void Engine::setScreenArea(int left,int right,int bottom){
	screenLeft = left;
	screenRight = right;
	screenBottom = bottom;
}

void Engine::setRate1mPerPixel(int pixel){
	pixelRate = pixel;
}

extern int engineTempFps;
extern int engineFps;

void Engine::engineProcess(){

	QueryPerformanceCounter(&before);
	while (!exitFlag){
		QueryPerformanceCounter(&after);
		QueryPerformanceFrequency(&freq);
		elapse = elapse + ((double)(after.QuadPart - before.QuadPart)) / ((double)freq.QuadPart);
		before = after;
		double interval = this->getInterval();
		if (elapse > interval){
			elapse = elapse - interval;
			EnterCriticalSection(&CS);
			this->physicalProcess();
			LeaveCriticalSection(&CS);
			engineTempFps++;
		}
		
		//무한 루프로 인한 CPU사용 완화 코드
		static double delay = 0.08;
		static int targetFPS = (int)(1 / interval);
		if (engineFps < targetFPS){
			delay -= 0.00001;
		}
		else {
			delay += 0.00001;
		}
		if (elapse < delay)
			WaitForSingleObject((void *)hEngineThread, 1);
	}
}


DWORD WINAPI Engine::engineThread(LPVOID pro){
	((Engine*)pro)->engineProcess();
	return 0;
}


void Engine::engineStart(){
	exitFlag = false;
	elapse = 0;
	DWORD ThreadID;
	hEngineThread = CreateThread(NULL, 0, &Engine::engineThread, this, 0, &ThreadID);
}
void Engine::engineStop(){
	SuspendThread(hEngineThread);
}
void Engine::exitEngine(){
	exitFlag = true;
}