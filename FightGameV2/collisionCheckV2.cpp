#include "collisionCheckV2.h"

bool isCollision(struct box A, struct box B){
	//OBBÃæµ¹
	vector vA[4];
	A.getRotationVertexs(vA);
	vector vB[4];
	B.getRotationVertexs(vB);
	vector edgeA[2];
	A.getEdge(edgeA);
	vector edgeB[2];
	B.getEdge(edgeB);

	vector seperatingAxisA[] = { { vA[2] - vA[1] }, { vA[1] - vA[0] } };
	vector seperatingAxisB[] = { { vB[2] - vB[1] }, { vB[1] - vB[0] } };
	for (int i = 0; i < 2; i++){
		seperatingAxisA[i] = seperatingAxisA[i].Normalize();
		seperatingAxisB[i] = seperatingAxisB[i].Normalize();
	}

	double rAx1 = abs(dot(seperatingAxisB[0], edgeA[0])) + abs(dot(seperatingAxisB[0], edgeA[1]));
	double rBx1 = abs(dot(seperatingAxisB[0], edgeB[0])) + abs(dot(seperatingAxisB[0], edgeB[1]));
	double rAy1 = abs(dot(seperatingAxisB[1], edgeA[0])) + abs(dot(seperatingAxisB[1], edgeA[1]));
	double rBy1 = abs(dot(seperatingAxisB[1], edgeB[0])) + abs(dot(seperatingAxisB[1], edgeB[1]));

	double rAx2 = abs(dot(seperatingAxisA[0], edgeA[0])) + abs(dot(seperatingAxisA[0], edgeA[1]));
	double rBx2 = abs(dot(seperatingAxisA[0], edgeB[0])) + abs(dot(seperatingAxisA[0], edgeB[1]));
	double rAy2 = abs(dot(seperatingAxisA[1], edgeA[0])) + abs(dot(seperatingAxisA[1], edgeA[1]));
	double rBy2 = abs(dot(seperatingAxisA[1], edgeB[0])) + abs(dot(seperatingAxisA[1], edgeB[1]));

	vector gap = B.position - A.position;

	double gapAx = abs(dot(seperatingAxisA[1], gap));
	double gapAy = abs(dot(seperatingAxisA[0], gap));

	double gapBx = abs(dot(seperatingAxisB[1], gap));
	double gapBy = abs(dot(seperatingAxisB[0], gap));

	if (gapBx > (rAx1 + rBx1)){
		return false;
	}
	if (gapBy > (rAy1 + rBy1)){
		return false;
	}
	if (gapAx > (rAx2 + rBx2)){
		return false;
	}
	if (gapAy > (rAy2 + rBy2)){
		return false;
	}

	return true;
}