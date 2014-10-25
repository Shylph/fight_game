#include "player.h"
extern int life;
extern struct box missile[20];

void Player::winAnimation(){
	static double changeSeta = 0;
	bodyMovement[3].v.x -= 2;
	bodyMovement[4].v.x -= 2;
	for (int i = 3; i < 7; i++){
		bodyMovement[i].v.y -= 4 * sin(changeSeta);
		changeSeta += 0.01;
		if (changeSeta > 3.141592){
			changeSeta = 0;
		}
	}
}

void Player::attackAnimation(){
	int cntLimt;
	vector power;
	vector ** ani;
	if ((state & PS_HAND_ATTACK_UPPERCUT) == PS_HAND_ATTACK_UPPERCUT){
		if ((direction & PD_ViEW_LEFT)>0){
			ani = attAni_uppercut_l;
		}
		else{
			ani = attAni_uppercut_r;
		}
		power = { 0.25, -0.85 };
		cntLimt = att_uppercut_cnt;
	}
	else if ((state & PS_HAND_ATTACK) == PS_HAND_ATTACK){
		if ((direction & PD_ViEW_LEFT)>0){
			ani = attAni_l;
		}
		else{
			ani = attAni_r;
		}
		power = { 1.3, 0 };
		cntLimt = attCnt;
	}

	static bool collsion = false;

	if ((!isCancelAttack()) && (collsion == false) && (currentAttCnt <= cntLimt)){
		setBodyArm(ani[currentAttCnt++]);
		body[3].v.x += -1 * power.x;
		body[3].v.y += power.y;
		body[6].v += power;
	}
	else if (collsion){
		//뺄떄 힘도 뺄 것!!!!!!!!
		body[3].v.x *= 0.7;
		body[3].v.y *= 0.7;
		body[6].v.x *= 0.7;
		body[6].v.y *= 0.7;
		//
		setBodyArm(ani[currentAttCnt--]);
	}
	else{
		collsion = true;
		setBodyArm(ani[currentAttCnt--]);
	}
	if ((currentAttCnt == -1) || (currentAttCnt == cntLimt)){
		currentAttCnt = 0;
		collsion = false;
		state = state&(~(PS_ATTACK | PS_HAND_ATTACK | PS_HAND_ATTACK_UPPERCUT));
		state = state | ((PS_ATTACK_CANCEL) | (PS_ATTACK_FINISH));
		body[3].v = { 0, 0 };
		body[6].v = { 0, 0 };
	}

}

void Player::deadlyBlowAnimation(){
	int cntLimt;
	static bool firstFlag = true;
	vector power;
	vector ** ani;
	if ((firstFlag) && (direction & PD_ViEW_LEFT)>0){
		base.v.x = -15;
		base.v.y = 0;
		firstFlag = false;
	}
	else if ((firstFlag) && (direction & PD_ViEW_RIGHT)>0){
		base.v.x = 15;
		base.v.y = 0;
		firstFlag = false;
	}
	if ((state & PS_DEADLY_BLOW) == PS_DEADLY_BLOW){
		if ((direction & PD_ViEW_LEFT)>0){
			ani = attAni_deadly_blow_l;
		}
		else{
			ani = attAni_deadly_blow_r;
		}
		power = { 0.5, 0};
		cntLimt = attCnt_deadly_blow;
	}
	static bool collsion = false;
	
	if (!isCancelAttack() && (collsion == false)){
		setBody(ani[currentdeadlyBlowCnt++]);
		if ((direction & PD_ViEW_LEFT)>0){
			base.v.x -= 1;
		}
		else{
			base.v.x += 1;
		}
	}
	else if (collsion){
		setBody(ani[currentdeadlyBlowCnt++]);
	}
	else{
		collsion = true;
		setBody(ani[currentdeadlyBlowCnt++]);
	}
	if ((direction & PD_ViEW_LEFT)>0){
		body[3].v.x += 0.8 * power.x;
		body[6].v.x += 0.8 * power.x;
		body[7].v.x += 0.5 * power.x;
		body[10].v.x += -0.5 * power.x;
	}
	else{
		body[3].v.x += -0.8 * power.x;
		body[6].v.x += -0.8 * power.x;
		body[7].v.x += -0.5 * power.x;
		body[10].v.x += 0.5 * power.x;
	}
	
	if ((currentdeadlyBlowCnt == cntLimt)){
		currentdeadlyBlowCnt = 0;
		firstFlag = true;
		collsion = false;
		state = state&(~(PS_DEADLY_BLOW));
		state = state | ((PS_ATTACK_CANCEL) | (PS_ATTACK_FINISH));
		body[3].v = { 0, 0 };
		body[6].v = { 0, 0 };
		body[7].v = { 0, 0 };
		body[10].v = { 0, 0 };

		for (int i = 0; i < 20; i++){
			missile[i].position = { -50, -50 };
		}
	}
}

bool Player::damagedDelay(){
	//연속적인 데미지 방지
	
	if (!beforeInitFlag){
		QueryPerformanceCounter(&before);
		beforeInitFlag = true;
	}
	QueryPerformanceCounter(&after);
	QueryPerformanceFrequency(&freq);
	elapse = elapse + ((double)(after.QuadPart - before.QuadPart)) / ((double)freq.QuadPart);
	before = after;
	double interval = 0.075;
	if (elapse > interval){
		elapse = elapse - interval;
		return true;
	}
	return false;
}
int Player::damagedPlayer(vector attV, line l){
	if (damagedDelay()){
		attV = attV - base.v*0.1;
		double damage = (int)(abs(2 * (attV.x)) + abs(2 * (attV.y)));
		if ((state & PS_DEFENSE) == PS_DEFENSE){
			damage *= 0.1;
			attV.x = attV.x*0.1;
			attV.y = attV.y*0.1;
		}
		if (damage > 100){
			damage = 100;
		}
		subLife((int)damage);
		addMana((int)(damage));
		base.v = base.v + (attV)*0.5;
		findAndSwayMovement(l.pE1->position, (attV)*1.5);
		findAndSwayMovement(l.pE2->position, (attV)*1.5);
		return (int)damage;
	}
	return 0;
}
void Player::waitAnimation(){
	vector **ani;
	if ((direction & PD_ViEW_LEFT)>0){
		ani = walkAni_l;
	}
	else{
		ani = walkAni_r;
	}
	setBody(ani[currentWalkCnt]);
	if (currentWalkCnt > 7){
		currentWalkCnt++;
	}
	else if (currentWalkCnt > 0){
		currentWalkCnt--;
	}
	if ((currentWalkCnt == walkCnt )|| currentWalkCnt<0){
		currentWalkCnt = 0;
	}
	
}
void Player::throwBoxAnimation(){
	/*
	static int cnt = 0;
	static double r = 0;
	if (cnt > 20)
		cnt = 0;
	int temp[] = { 3, 6, 6, 3 };
	static int i = 0;
	i = i % 4;
	if ((direction & PD_ViEW_LEFT)>0){
		//for (int i = 0; i < 3; i++){
			missile[cnt].v = { 5, 0 };
			missile[cnt].width = { 30, 30 };
			missile[cnt].rotation = r;
			r += 0.2;
			missile[cnt].position = { body[temp[i]].position.x + base.position.x, body[temp[i]].position.y + base.position.y };
			cnt++;
		//}
	}
	else{
		//for (int i = 0; i < 3; i++){
			missile[cnt].v = { -5, 0 };
			missile[cnt].width = { 30, 30 };
			missile[cnt].rotation = r;
			r += 0.2;
			missile[cnt].position = { body[temp[i]].position.x + base.position.x, body[temp[i]].position.y + base.position.y };
			cnt++;
		//}
	}
	i++;
	state = state & ~(PS_THROW_BOX);
	*/
}

void Player::attackFootAnimation(){
	vector ** ani;
	int cntLimt;
	vector power;
	static bool collsion = false;
	
	if ((state & PS_FOOT_ATTACK) == PS_FOOT_ATTACK){
		if ((direction & PD_ViEW_LEFT)>0){
			ani = attAni_foot_l;
		}
		else{
			ani = attAni_foot_r;
		}
		power = { 1, 0 };
		cntLimt = attCnt_foot;
	}
	else if ((state & PS_KNEE_KICK_ATTACK) == PS_KNEE_KICK_ATTACK){
		if ((direction & PD_ViEW_LEFT)>0){
			ani = att_knee_kick_l;
		}
		else{
			ani = att_knee_kick_r;
		}
		power = { -0.6, -0.8 };
		cntLimt = att_knee_kick_cnt;
	}

	
	if (!isCancelAttack() && (collsion == false) && (currentFootAttCnt <= cntLimt)){
		setBodyLeg(ani[currentFootAttCnt++]);
		body[10].v.x += -1 * power.x;
		body[10].v.y += power.y;
		body[7].v += power;
	}
	else if (collsion){
		body[10].v.x *= 0.7 ;
		body[10].v.y *= 0.7;
		body[7].v.x *= 0.7;
		body[7].v.y *= 0.7;
		setBodyLeg(ani[currentFootAttCnt--]);
	}
	else{
		collsion = true;
		setBodyLeg(ani[currentFootAttCnt--]);
	}
	if ((currentFootAttCnt == -1) || (currentFootAttCnt == cntLimt)){
		currentFootAttCnt = 0;
		collsion = false;
		state = state & ~(PS_ATTACK | PS_KNEE_KICK_ATTACK | PS_FOOT_ATTACK);
		state = state | ((PS_ATTACK_CANCEL) | (PS_ATTACK_FINISH));
		body[7].v = { 0, 0 };
		body[10].v = { 0, 0 };
	}

}



void Player::walkAnimation(){
	vector **ani;
	if ((direction & PD_ViEW_LEFT)>0){
		if ((direction & PD_LEFT) > 0)
			ani = walkAni_l;
		else if ((direction & PD_RIGHT )> 0)
			ani = back_walkAni_r;
	}
	else{
		if ((direction & PD_LEFT) > 0)
			ani = back_walkAni_l;
		else if ((direction & PD_RIGHT) > 0)
			ani = walkAni_r;
	}
	
	setBody(ani[currentWalkCnt]);
	currentWalkCnt++;
	if (currentWalkCnt == walkCnt){
		currentWalkCnt = 0;
	}
}

void Player::defenseAnimation(){
	vector **ani;
	if ((direction & PD_ViEW_LEFT)>0){
		ani = defenseAni_l;
	}
	else{
		ani = defenseAni_r;
	}
	setBody(ani[currentDefenseCnt]);
	if ((state &PS_DEFENSE) == PS_DEFENSE){
		if (currentDefenseCnt != defense_cnt / 2){
			currentDefenseCnt++;
		}
	}
	else if ((state &PS_DEFENSE_CANCEL) == PS_DEFENSE_CANCEL){
		currentDefenseCnt++;
	}
	if (currentDefenseCnt == defense_cnt){
		currentDefenseCnt = 0;
		state = state&(~(PS_DEFENSE | PS_DEFENSE_CANCEL));
	}
}
void Player::updateAnimation(){
	if ((state&PS_WAIT) == PS_WAIT && (state&(PS_WALK | PS_DEFENSE | PS_DEFENSE_CANCEL | PS_HAND_ATTACK | PS_HAND_ATTACK_UPPERCUT | PS_FOOT_ATTACK | PS_KNEE_KICK_ATTACK | PS_DEADLY_BLOW)) == 0){
		waitAnimation();
	}

	if ((state & PS_WALK) == PS_WALK){
		walkAnimation();
	}

	if ((state & (PS_DEFENSE | PS_DEFENSE_CANCEL)) >0){
		defenseAnimation();
	}

	if ((state & (PS_HAND_ATTACK | PS_HAND_ATTACK_UPPERCUT))>0){
		attackAnimation();
	}

	if ((state & (PS_FOOT_ATTACK | PS_KNEE_KICK_ATTACK)) > 0){
		attackFootAnimation();
	}
	/*
	if ((state & (PS_THROW_BOX)) > 0){
		throwBoxAnimation();
	}
	*/
	if ((state & (PS_DEADLY_BLOW)) > 0){
		throwBoxAnimation();
		deadlyBlowAnimation();
	}
	
	//공격후 일정 시간동안 공격 불가 코드
	if ((state & (PS_ATTACK_FINISH | PS_ATTACK_CANCEL))>0){
		currentAttCancelCnt++;
		if (!(currentAttCancelCnt %= 9)){
			state = state& ~(PS_ATTACK_FINISH | PS_ATTACK_CANCEL);
		}
	}

	addMana(1);
}

Player::Player()
{
	state = (PS_WAIT);

	handSize = 10;
	headSize = 30;

	body[0].position = { 0, 0 };
	body[1].position = { 0, 34 };

	body[2].position = { 0, 130 };
	
	body[3].position = { 33, 25 };
	body[4].position = { -15, 78 };

	body[5].position = { 20, 96 };
	body[6].position = { 47, 42 };

	body[7].position = { -47, 314 };
	body[8].position = { -1, 236 };

	body[9].position = { 40, 228 };
	body[10].position = { 11, 314 };

	foot1 = &body[7].position;
	foot2 = &body[10].position;

	hand1 = &body[3].position;
	hand2 = &body[6].position;

	int temp[] =  { 0, 1, 1, 2, 3, 4, 4, 1, 1, 5, 5, 6, 8, 2, 7, 8, 2, 9, 9, 10 }; 
	for (int i = 0; i < 20; i=i+2){
		bodyLine[i/2] = { &body[temp[i]], &body[temp[i+1]]};
	}

	base.v = { 0, 0 };
	loadAnimationAll();
}

Player::~Player()
{
}

void Player::setKey(TCHAR left, TCHAR right, TCHAR up, TCHAR down, TCHAR hand, TCHAR foot, TCHAR defense){
	this->left = left;
	this->right = right;
	this->up = up;
	this->down = down;
	this->hand = hand;
	this->foot = foot;
	this->defense = defense;
}

void Player::checkKey(){
	if ((state & PS_ATTACK_CANCEL) != PS_ATTACK_CANCEL){
		if (GetKeyState(hand) < 0){
			int manaConsumption = 50;
			int manaConsumption2 = 650;

			if ((GetKeyState(down) < 0) && (mana>manaConsumption)){
				if (((state&PS_ATTACK) != PS_ATTACK) ){
					state = state|(PS_HAND_ATTACK_UPPERCUT|PS_ATTACK);
					subMana(manaConsumption);
				}
			}
			else if (((GetKeyState(left) < 0)) && (mana>manaConsumption2)){
				if ((state&PS_DEADLY_BLOW) != PS_DEADLY_BLOW){
					state = state | (PS_DEADLY_BLOW);
					subMana(manaConsumption2);
				}
			}
			else if ((GetKeyState(right) < 0) && (mana>manaConsumption2)){
				if ((state&PS_DEADLY_BLOW) != PS_DEADLY_BLOW){
					state = state | (PS_DEADLY_BLOW);
					subMana(manaConsumption2);
				}
			}
			else if (((state&PS_ATTACK) != PS_ATTACK) && (mana>manaConsumption)){
				state = state | (PS_HAND_ATTACK | PS_ATTACK);
				subMana(manaConsumption);
			}
		}
		if (GetKeyState(foot) < 0){
			int manaConsumption = 50;
			if ((GetKeyState(down) < 0)&&(mana>manaConsumption)){
				if ((state&PS_ATTACK) != PS_ATTACK){
					state = state | (PS_KNEE_KICK_ATTACK| PS_ATTACK);
					subMana(manaConsumption);
				}
			}
			else if (((state&PS_ATTACK) != PS_ATTACK) && (mana>manaConsumption)){
				state = state | (PS_FOOT_ATTACK | PS_ATTACK);
				subMana(manaConsumption);
			}
		}
	}
	if (GetKeyState(defense) < 0){
		if ((state&PS_DEFENSE) != PS_DEFENSE)
			state = state | (PS_DEFENSE );
	}
	else if (GetKeyState(defense) == 1){
		if ((state & PS_DEFENSE) == PS_DEFENSE){
			state = state & ~PS_DEFENSE;
			state = state | PS_DEFENSE_CANCEL;
		}
	}
	if (GetKeyState(left)<0){
		if (((state & PS_JUMPING) != PS_JUMPING) ){
			if ((base.v.x > -6))
				base.v.x = base.v.x - 0.7;
			direction = direction & ~PD_RIGHT;
			direction = direction | PD_LEFT;
			state = state | PS_WALK;
		}
	}else{
		if ((direction & PD_LEFT)>0)
			state = state & ~PS_WALK;
	}
	if ((GetKeyState(right)<0)){
		if (((state & PS_JUMPING) != PS_JUMPING)){
			if ((base.v.x < 6))
				base.v.x = base.v.x + 0.7;
			direction = direction & ~PD_LEFT;
			direction = direction | PD_RIGHT;
			state = state | PS_WALK;
		}
	}
	else{
		if ((direction & PD_RIGHT)>0)
			state = state & ~PS_WALK;
	}
	if (GetKeyState(up)<0){
		if ((state & PS_ON_THE_GROUND) == PS_ON_THE_GROUND){
			base.v.y = -9.5;
			state = state | PS_JUMPING;
			state = state & ~PS_ON_THE_GROUND;
		}
	}

	if (GetKeyState(down)<0){
		//person.vector.y = person.vector.y + 100;
	}

	if ((state & (PS_ATTACK | PS_WALK | PS_JUMPING)) > 0){
		if ((state & PS_DEFENSE) == PS_DEFENSE){
			state = state & ~PS_DEFENSE;
			state = state | PS_DEFENSE_CANCEL;
		}
	}
}

void loadAnimation(char *str, vector ***ani,int *saveCnt){
	FILE *fp = fopen(str, "r");
	if (fp == NULL)
		return;
	fseek(fp, 0, 0);
	int cnt = 0;
	char ch[1000];
	while (fgets(ch, 1000, fp) != NULL){
		cnt++;
	}
	*saveCnt = cnt;

	vector b[11];
	fseek(fp, 0, 0);
	ani[0] = new vector*[cnt];
	for (int i = 0; i < cnt; i++){
		ani[0][i] = new vector[11];
	}
	int j = 0;
	while (j < cnt){
		for (int i = 0; i < 11; i++){
			fscanf(fp, "%lf, %lf,", &b[i].x, &b[i].y);
			ani[0][j][i] = b[i];
		}
		fscanf(fp, "\n");
		j++;
	}

	fclose(fp);
}

void Player::loadAnimationAll(){
	loadAnimation(".\\ani\\walk_l", &walkAni_l,&walkCnt);
	loadAnimation(".\\ani\\walk_r", &walkAni_r, &walkCnt);
	loadAnimation(".\\ani\\back_walk_l", &back_walkAni_l, &walkCnt);
	loadAnimation(".\\ani\\back_walk_r", &back_walkAni_r, &walkCnt);
	loadAnimation(".\\ani\\att_l", &attAni_l, &attCnt);
	loadAnimation(".\\ani\\att_r", &attAni_r, &attCnt);
	loadAnimation(".\\ani\\att_foot_l", &attAni_foot_l, &attCnt_foot);
	loadAnimation(".\\ani\\att_foot_r", &attAni_foot_r, &attCnt_foot);
	loadAnimation(".\\ani\\att_uppercut_r", &attAni_uppercut_r, &att_uppercut_cnt);
	loadAnimation(".\\ani\\att_uppercut_l", &attAni_uppercut_l, &att_uppercut_cnt);
	loadAnimation(".\\ani\\defense_r", &defenseAni_r, &defense_cnt);
	loadAnimation(".\\ani\\defense_l", &defenseAni_l, &defense_cnt);
	loadAnimation(".\\ani\\att_knee_kick_r", &att_knee_kick_r, &att_knee_kick_cnt);
	loadAnimation(".\\ani\\att_knee_kick_l", &att_knee_kick_l, &att_knee_kick_cnt);
	loadAnimation(".\\ani\\att_deadly_blow_l", &attAni_deadly_blow_l, &attCnt_deadly_blow);
	loadAnimation(".\\ani\\att_deadly_blow_r", &attAni_deadly_blow_r, &attCnt_deadly_blow);
}

physicalElement* Player::findBodyMovement(vector p){
	for (int i = 0; i < 11; i++){
		if (((int)body[i].position.x == (int)p.x) && ((int)body[i].position.y == (int)p.y)){
			return &bodyMovement[i];
		}
	}
	return NULL;
}