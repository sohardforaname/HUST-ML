#include "matrix.h"

#define cal(a, b) (((a) - (b)) * ((a) - (b)))

void Matrix::CalMatrix4Mul4(int* a, int* b, int* c, int len)
{
	register int c_00_reg = 0, c_01_reg = 0, c_02_reg = 0, c_03_reg = 0;
	register int c_10_reg = 0, c_11_reg = 0, c_12_reg = 0, c_13_reg = 0;
	register int c_20_reg = 0, c_21_reg = 0, c_22_reg = 0, c_23_reg = 0;
	register int c_30_reg = 0, c_31_reg = 0, c_32_reg = 0, c_33_reg = 0;

	int* a_ptr_0 = a, * a_ptr_1 = a + len, * a_ptr_2 = a_ptr_1 + len, * a_ptr_3 = a_ptr_2 + len;
	int* b_ptr_0 = b, * b_ptr_1 = b + len, * b_ptr_2 = b_ptr_1 + len, * b_ptr_3 = b_ptr_2 + len;

	for (int i = 0; i < len; ++i)
	{
		c_00_reg += cal(*a_ptr_0, *b_ptr_0);
		c_01_reg += cal(*a_ptr_0, *b_ptr_1);
		c_02_reg += cal(*a_ptr_0, *b_ptr_2);
		c_03_reg += cal(*a_ptr_0, *b_ptr_3);

		c_10_reg += cal(*a_ptr_1, *b_ptr_0);
		c_11_reg += cal(*a_ptr_1, *b_ptr_1);
		c_12_reg += cal(*a_ptr_1, *b_ptr_2);
		c_13_reg += cal(*a_ptr_1, *b_ptr_3);

		c_20_reg += cal(*a_ptr_2, *b_ptr_0);
		c_21_reg += cal(*a_ptr_2, *b_ptr_1);
		c_22_reg += cal(*a_ptr_2, *b_ptr_2);
		c_23_reg += cal(*a_ptr_2, *b_ptr_3);

		c_30_reg += cal(*a_ptr_3, *b_ptr_0);
		c_31_reg += cal(*a_ptr_3, *b_ptr_1);
		c_32_reg += cal(*a_ptr_3, *b_ptr_2);
		c_33_reg += cal(*a_ptr_3, *b_ptr_3);

		++a_ptr_0;
		++a_ptr_1;
		++a_ptr_2;
		++a_ptr_3;

		++b_ptr_0;
		++b_ptr_1;
		++b_ptr_2;
		++b_ptr_3;
	}

	int* c_ptr_0 = c, * c_ptr_1 = c + 4, * c_ptr_2 = c_ptr_1 + 4, * c_ptr_3 = c_ptr_2 + 4;

	*(c_ptr_0++) = c_00_reg;
	*(c_ptr_1++) = c_10_reg;
	*(c_ptr_2++) = c_20_reg;
	*(c_ptr_3++) = c_30_reg;

	*(c_ptr_0++) = c_01_reg;
	*(c_ptr_1++) = c_11_reg;
	*(c_ptr_2++) = c_21_reg;
	*(c_ptr_3++) = c_31_reg;

	*(c_ptr_0++) = c_02_reg;
	*(c_ptr_1++) = c_12_reg;
	*(c_ptr_2++) = c_22_reg;
	*(c_ptr_3++) = c_32_reg;

	*(c_ptr_0++) = c_03_reg;
	*(c_ptr_1++) = c_13_reg;
	*(c_ptr_2++) = c_23_reg;
	*(c_ptr_3++) = c_33_reg;
}

void Matrix::CalMatrix4Mul4_256(int* a, int* b, int* c, int len)
{
	__m256i c_256_1 = _mm256_set_epi64x(0, 0, 0, 0);
	__m256i c_256_2 = _mm256_set_epi64x(0, 0, 0, 0);
	__m256i a_256_1, a_256_2, b_256_1, b_256_2, mask;
	mask = _mm256_set_epi64x(-1, -1, -1, -1);

	int* a_ptr_0 = a, * a_ptr_1 = a + len, * a_ptr_2 = a_ptr_1 + len, * a_ptr_3 = a_ptr_2 + len;
	int* b_ptr_0 = b, * b_ptr_1 = b + len, * b_ptr_2 = b_ptr_1 + len, * b_ptr_3 = b_ptr_2 + len;

	for (int i = 0; i < len; ++i)
	{
		a_256_1 = _mm256_set_epi32
		(
			*a_ptr_1, *a_ptr_1, *a_ptr_1, *a_ptr_1,
			*a_ptr_0, *a_ptr_0, *a_ptr_0, *a_ptr_0
		);
		a_256_2 = _mm256_set_epi32
		(
			*a_ptr_3, *a_ptr_3, *a_ptr_3, *a_ptr_3,
			*a_ptr_2, *a_ptr_2, *a_ptr_2, *a_ptr_2
		);

		b_256_1 = _mm256_set_epi32
		(
			*b_ptr_3, *b_ptr_2, *b_ptr_1, *b_ptr_0,
			*b_ptr_3, *b_ptr_2, *b_ptr_1, *b_ptr_0
		);

		b_256_2 = b_256_1;

		a_256_1 = _mm256_sub_epi32(a_256_1, b_256_1);
		a_256_1 = _mm256_mullo_epi32(a_256_1, a_256_1);
		c_256_1 = _mm256_add_epi32(c_256_1, a_256_1);

		a_256_2 = _mm256_sub_epi32(a_256_2, b_256_2);
		a_256_2 = _mm256_mullo_epi32(a_256_2, a_256_2);
		c_256_2 = _mm256_add_epi32(c_256_2, a_256_2);

		++a_ptr_0;
		++a_ptr_1;
		++a_ptr_2;
		++a_ptr_3;

		++b_ptr_0;
		++b_ptr_1;
		++b_ptr_2;
		++b_ptr_3;

	}

	_mm256_maskstore_epi32(c, mask, c_256_1);
	_mm256_maskstore_epi32(c + 8, mask, c_256_2);
}