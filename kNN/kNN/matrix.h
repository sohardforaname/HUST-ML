#ifndef MATRIX_H
#define MATRIX_H
#include <intrin.h>
#include <cstring>

class Matrix
{
public:
	int kc, rc;
	int* bufferA, * bufferB, * bufferC;
	void CalMatrix4Mul4(int* a, int* b, int* c, int len);
	void CalMatrix4Mul4_256(int* a, int* b, int* c, int len);
};

#endif MATRIX_H