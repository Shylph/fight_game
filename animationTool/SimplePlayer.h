#pragma once

#include <MathUtils.h>

class SimplePlayer
{
public:
	SimplePlayer();
	~SimplePlayer();

	vector base;
	vector* getBodys(){
		return body;
	}
	void setBody(vector* b){
		for (int i = 0; i < 11; i++){
			body[i] = b[i];
		}
	}


	int headSize;
	int handSize;
private:
	
	struct vector *hand1, *hand2;
	struct vector *foot1, *foot2;
	vector body[11];
		
};

