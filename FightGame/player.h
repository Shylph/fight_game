#ifndef _PLAYER_H
#define _PLAYER_H
#pragma warning(disable:4996)

#include <Windows.h>
#include <stdio.h>
#include <MathUtils.h>
#include "engineElement.h"
#include <engineElementV2.h>

#define PD_RIGHT 0x1L
#define PD_LEFT 0x2L
#define PD_ViEW_RIGHT 0x4L
#define PD_ViEW_LEFT 0x8L

#define PS_WAIT 0x1L
#define PS_ON_THE_GROUND 0x100L
#define PS_JUMPING 0x200L
#define PS_ATTACK_FINISH 0x400L
#define PS_ATTACK 0x1000L
#define PS_ATTACK_CANCEL 0x2000L
#define PS_DEFENSE 0x4000L
#define PS_DEFENSE_CANCEL 0x8000L
#define PS_WALK 0x100000L
#define PS_FOOT_ATTACK 0x10000L
#define PS_KNEE_KICK_ATTACK 0x20000L
#define PS_HAND_ATTACK 0x1000000L
#define PS_HAND_ATTACK_UPPERCUT 0x2000000L
#define PS_THROW_BOX 0x4000000L
#define PS_DEADLY_BLOW 0x8000000L


extern CRITICAL_SECTION CS;

class Player
{
public:
	Player();
	~Player();
	int getLife(){
		return life;
	}
	void addLife(int m){
		life += m;
		if (life > 1000)
			life = 1000;
	}
	void subLife(int m){
		life -= m;
		if (life < 0)
			life = 0;
	}
	int getMana(){
		return mana;
	}
	void addMana(int m){
		mana += m;
		if (mana > 1000)
			mana = 1000;
	}
	void subMana(int m){
		mana -= m;
		if (mana < 0)
			mana = 0;
	}
	void updateAnimation();
	void setKey(TCHAR left, TCHAR right, TCHAR up, TCHAR down, TCHAR hand, TCHAR foot,TCHAR defense);
	void checkKey();
	physicalElement base;
	physicalElement* getBodys(){
		return body;
	}
	void setBody(vector* b){
		EnterCriticalSection(&CS);
		for (int i = 0; i < 11; i++){
			body[i].position = b[i];
		}
		LeaveCriticalSection(&CS);
	}
	void setBodyArm(vector* b){
		EnterCriticalSection(&CS);
		int temp[4] = { 3, 4, 5, 6 };
		for (int i = 0; i < 4; i++){
			body[temp[i]].position = b[temp[i]];
		}
		LeaveCriticalSection(&CS);
	}
	void setBodyLeg(vector* b){
		EnterCriticalSection(&CS);
		if (b == NULL)
			return;
		int temp[4] = { 7, 8, 9, 10 };
		for (int i = 0; i < 4; i++){
			body[temp[i]].position = b[temp[i]];
		}
		LeaveCriticalSection(&CS);
	}
	line* getBodyLine(){
		return bodyLine;
	}
	int headSize;
	int handSize;
	COLORREF color = RGB(0,0,0);
	unsigned int getDirection(){
		return direction;
	}
	bool isAttack(){
		return PS_ATTACK == (state & PS_ATTACK);
	}
	void cancelAttack(){
		state = state | PS_ATTACK_CANCEL;
	}
	bool isCancelAttack(){
		return (state & PS_ATTACK_CANCEL) == PS_ATTACK_CANCEL;
	}
	physicalElement* getBodyMovement(){
		return bodyMovement;
	}
	int damagedPlayer(vector attV, line l);

	void fallPlayer(){
		state = state | PS_ON_THE_GROUND;
		state = state & ~PS_JUMPING;
	}
	double getBottom(){
		double bottom;
		body[7].position.y > body[10].position.y ?
			bottom = body[7].position.y :
			bottom = body[10].position.y;
		return bottom;
	}
	void setDirection(unsigned int d){
		if (d == PD_ViEW_LEFT){
			direction = direction & ~PD_ViEW_RIGHT;
		}
		else if (d == PD_ViEW_RIGHT){
			direction = direction & ~PD_ViEW_LEFT;
		}
		direction = direction | d;
	}
	void initBody(){
		if (direction & PD_ViEW_LEFT){
			setBody(walkAni_l[0]);
		}
		else{
			setBody(walkAni_r[0]);
		}

	}
	void findAndSwayMovement(vector p, vector f){
		physicalElement *pe = findBodyMovement(p);
		vector temp = pe->v + f;
		while(temp.length() > 5){
			temp = temp*0.8;
		}
		pe->v = temp;
	}
	void winAnimation();
private:
	void loadAnimationAll();
	physicalElement* findBodyMovement(vector p);
	int currentAttCancelCnt = 0;
	int currentFootAttCnt = 0;
	int currentAttCnt = 0;
	int currentWalkCnt = 0;
	int currentDefenseCnt=0;
	int currentdeadlyBlowCnt = 0;
		
	bool beforeInitFlag=false;
	LARGE_INTEGER before, after, freq; 
	double elapse;
	bool damagedDelay();

	int life=1000;
	int mana = 0;
	TCHAR left, right, up, down, hand, foot,defense;
	unsigned long state;
	unsigned long direction;
	struct vector *hand1, *hand2;
	struct vector *foot1, *foot2;
	physicalElement body[11];
	physicalElement bodyMovement[11];
	line bodyLine[10];
	/*
	0 머리
	1 상체
	2 하체
	3,4왼손
	6,5오른손
	7,8왼발
	10,9오른발
	*/
	void walkAnimation();
	vector **walkAni_l, **walkAni_r;
	vector **back_walkAni_l, **back_walkAni_r;
	int walkCnt;

	void attackAnimation();
	vector **attAni_l, **attAni_r;
	int attCnt;
	vector **attAni_uppercut_r, **attAni_uppercut_l;
	int att_uppercut_cnt;

	void attackFootAnimation();
	vector **attAni_foot_l, **attAni_foot_r;
	int attCnt_foot;

	vector **att_knee_kick_r, **att_knee_kick_l;
	int att_knee_kick_cnt;

	void defenseAnimation();
	vector **defenseAni_r, **defenseAni_l;
	int defense_cnt;

	void deadlyBlowAnimation();
	vector **attAni_deadly_blow_l, **attAni_deadly_blow_r;
	int attCnt_deadly_blow;

	void throwBoxAnimation();

	void waitAnimation();


};

#endif