#ifndef MATRIX_H
#define MATRIX_H
#include <intrin.h>
#include <cstring>
#include <algorithm>

class Matrix
{
public:
	int kc, rc;
	int* bufferA, * bufferB, * bufferC;
	void CalMatrix4Mul4L2(int* a, int* b, int* c, int len);
	void CalMatrix4Mul4L2_256(int* a, int* b, int* c, int len);
	void CalMatrix4Mul4L1(int* a, int* b, int* c, int len);
	void CalMatrix4Mul4L1_256(int* a, int* b, int* c, int len);
	void CalMatrix4Mul4Ln(int* a, int* b, int* c, int len);
	void CalMatrix4Mul4Ln_256(int* a, int* b, int* c, int len);
};

#endif MATRIX_H