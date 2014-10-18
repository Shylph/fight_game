#include "SimplePlayer.h"


SimplePlayer::SimplePlayer()
{

	handSize = 10;
	headSize = 30;

	body[0] = { 0, 0 };
	body[1]= { 0, 34 };

	body[2]= { 0, 130 };

	body[3]= { 33, 25 };
	body[4]= { -15, 78 };

	body[5]= { 20, 96 };
	body[6]= { 47, 42 };

	body[7]= { -47, 314 };
	body[8]= { -1, 236 };

	body[9]= { 40, 228 };
	body[10]= { 11, 314 };

	foot1 = &body[7];
	foot2 = &body[10];

	hand1 = &body[3];
	hand2 = &body[6];

}


SimplePlayer::~SimplePlayer()
{
}
