#include "mathUtil.h"
#include <cmath>

namespace util 
{
	void generateCircleVertices(float vRadius, int vFraction, float **voVertices, int *voVerticesNum) 
	{
		float Delta = 2 * PI / vFraction;
		float A, X, Y;
		int K = 0, Num = 0;
		for (A = 0.0f; A < 2 * PI; Num++, A += Delta);
		*voVerticesNum = Num;
		*voVertices = new float[3 * Num];
		for (A = 0.0f; A < 2 * PI; A += Delta) 
		{
			X = vRadius * cos(A);
			Y = vRadius * sin(A);
			(*voVertices)[K++] = X;
			(*voVertices)[K++] = Y;
			(*voVertices)[K++] = 0.0f;
		}
	}
}
