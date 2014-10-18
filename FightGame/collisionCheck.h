#ifndef _COLLISION_CHECK_H
#define _COLLISION_CHECK_H
#include <Windows.h>
#include <math.h>
#include "engineElement.h"
//isCollisionLineToLine�Լ��� �Ʒ� V2�� �ִ� box OBB�浹 ó���� ������ �����
#include <collisionCheckV2.h>
#include <EngineV2.h>
#include <drawV2.h>



double getDistanceBetweenPointAndLine(double px, double py, double lx1, double ly1, double lx2, double ly2);
bool isCollisionLineToLine(struct line l1,struct line l2);
vector findCollisionLineToLine(line l1, line l2);
vector findCollisionLineToBox(line l1, box b2);

#endif