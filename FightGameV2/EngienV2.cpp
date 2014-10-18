#include "EngineV2.h"


EngineV2::EngineV2()
{
}


EngineV2::~EngineV2()
{
}
void EngineV2::physicalProcess(){
	//test
	//if (isCollision(test2.boxs[0], test2.boxs[1]))
	//	Sleep(10000);

	//---

	std::list<box*>::iterator iter;

	for (iter = list.begin(); iter != list.end(); iter++){

		//�߷°��ӵ� 9.8
		(*iter)->v.y = (*iter)->v.y + 9.8 * interval;

		int bottom = 10;

		//�ٴ� �浹
		if ((screenBottom < ((*iter)->position.y + (*iter)->getBottom())) && !(((*iter)->v.y<0))){
			(*iter)->v.y *= -0.05;
			//���� 0.9
			(*iter)->v.x = (*iter)->v.x * 0.9;
		}

		//���� �浹
		if ((screenLeft >((*iter)->position.x)) && !(((*iter)->v.x>0))){
			(*iter)->v.x *= -0.05;
		}
		//������ �浹
		if ((screenRight < ((*iter)->position.x)) && !(((*iter)->v.x<0))){
			(*iter)->v.x *= -0.05;
		}
		//�ӵ��� ���� �̵�
		(*iter)->position.x += (*iter)->v.x * interval * pixelRate;
		(*iter)->position.y += (*iter)->v.y * interval * pixelRate;

	}
}

void EngineV2::addBox(box *element){
	list.push_back(element);
}

void EngineV2::setInterval(int milliisecond){
	interval = milliisecond / 1000.0;
}

void EngineV2::setScreenArea(int left, int right, int bottom){
	screenLeft = left;
	screenRight = right;
	screenBottom = bottom;
}

void EngineV2::setRate1mPerPixel(int pixel){
	pixelRate = pixel;
}