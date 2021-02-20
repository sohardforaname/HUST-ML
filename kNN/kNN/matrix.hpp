#pragma once

#include <intrin.h>
#include <cstring>
#include <algorithm>

class Matrix {

    int wa, wb, lb;
    int *packa, *packb, *packc;

#define cal_e(a, b) (((a) - (b)) * ((a) - (b)))

private:
    inline void CalMatrix4Mul4L2(int* a, int* b, int* c, int len)
    {
        register int c_00_reg = 0, c_01_reg = 0, c_02_reg = 0, c_03_reg = 0;
        register int c_10_reg = 0, c_11_reg = 0, c_12_reg = 0, c_13_reg = 0;
        register int c_20_reg = 0, c_21_reg = 0, c_22_reg = 0, c_23_reg = 0;
        register int c_30_reg = 0, c_31_reg = 0, c_32_reg = 0, c_33_reg = 0;

        int *a_ptr_0 = a, *a_ptr_1 = a + len;
        int *a_ptr_2 = a_ptr_1 + len, *a_ptr_3 = a_ptr_2 + len;
        int* b_ptr_0 = b;
        register int b_ptr_v;

        for (int i = 0; i < len; ++i) {

            b_ptr_v = *(b_ptr_0++);

            c_00_reg += cal_e(*a_ptr_0, b_ptr_v);
            c_10_reg += cal_e(*a_ptr_1, b_ptr_v);
            c_20_reg += cal_e(*a_ptr_2, b_ptr_v);
            c_30_reg += cal_e(*a_ptr_3, b_ptr_v);

            b_ptr_v = *(b_ptr_0++);

            c_01_reg += cal_e(*a_ptr_0, b_ptr_v);
            c_11_reg += cal_e(*a_ptr_1, b_ptr_v);
            c_21_reg += cal_e(*a_ptr_2, b_ptr_v);
            c_31_reg += cal_e(*a_ptr_3, b_ptr_v);

            b_ptr_v = *(b_ptr_0++);

            c_02_reg += cal_e(*a_ptr_0, b_ptr_v);
            c_12_reg += cal_e(*a_ptr_1, b_ptr_v);
            c_22_reg += cal_e(*a_ptr_2, b_ptr_v);
            c_32_reg += cal_e(*a_ptr_3, b_ptr_v);

            b_ptr_v = *(b_ptr_0++);

            c_03_reg += cal_e(*a_ptr_0, b_ptr_v);
            c_13_reg += cal_e(*a_ptr_1, b_ptr_v);
            c_23_reg += cal_e(*a_ptr_2, b_ptr_v);
            c_33_reg += cal_e(*a_ptr_3, b_ptr_v);

            ++a_ptr_0;
            ++a_ptr_1;
            ++a_ptr_2;
            ++a_ptr_3;
        }

        int* ptrc = c;

        *(ptrc++) = c_00_reg;
        *(ptrc++) = c_10_reg;
        *(ptrc++) = c_20_reg;
        *(ptrc++) = c_30_reg;

        *(ptrc++) = c_01_reg;
        *(ptrc++) = c_11_reg;
        *(ptrc++) = c_21_reg;
        *(ptrc++) = c_31_reg;

        *(ptrc++) = c_02_reg;
        *(ptrc++) = c_12_reg;
        *(ptrc++) = c_22_reg;
        *(ptrc++) = c_32_reg;

        *(ptrc++) = c_03_reg;
        *(ptrc++) = c_13_reg;
        *(ptrc++) = c_23_reg;
        *(ptrc++) = c_33_reg;
    }

#undef cal_e

    inline void CalMatrix4Mul4L2_256(int* a, int* b, int* c, int len)
    {
        __m256i c01 = _mm256_setzero_si256();
        __m256i c23 = _mm256_setzero_si256();

        int* a_ptr_0 = a;
        int* b_ptr_0 = b;

        for (int i = 0; i < len; ++i) {

            __m256i ma0 = _mm256_loadu2_m128i((__m128i*)a_ptr_0, (__m128i*)a_ptr_0);
            __m256i ma1 = ma0;

            __m256i mb0 = _mm256_set1_epi32(*(b_ptr_0++));
            __m256i mb1 = _mm256_set1_epi32(*(b_ptr_0++));

            __m256i mb = _mm256_blend_epi32(mb1, mb0, 0xf);

            ma0 = _mm256_sub_epi32(ma0, mb);
            ma0 = _mm256_mullo_epi32(ma0, ma0);
            c01 = _mm256_add_epi32(c01, ma0);

            mb0 = _mm256_set1_epi32(*(b_ptr_0++));
            mb1 = _mm256_set1_epi32(*(b_ptr_0++));

            mb = _mm256_blend_epi32(mb1, mb0, 0xf);

            ma1 = _mm256_sub_epi32(ma1, mb);
            ma1 = _mm256_mullo_epi32(ma1, ma1);
            c23 = _mm256_add_epi32(c23, ma1);

            a_ptr_0 += 4;
        }

        int* ptrc = c;

        _mm256_storeu_epi32(c, c01);
        _mm256_storeu_epi32(c + 8, c23);
    }

public:
    Matrix() = delete;
    Matrix(int _wa, int _wb, int _lb)
        : wa(_wa)
        , wb(_wb)
        , lb(_lb)
    {
        packb = new int[wb << 2];
        packa = new int[wa << 2];
        packc = new int[lb << 2];
    }

    ~Matrix()
    {
        delete[] packa;
        delete[] packb;
        delete[] packc;
    }

    inline void CalMatrix(int* a, int la, int wa, int* b, int lb, int wb, int* c)
    {
        int* ptrb = packb;
        int* ptrc = packc;

        int *b_0_ptr, *b_1_ptr, *b_2_ptr, *b_3_ptr, *pb = b, *pa = a, *pc = c;
        int *ptr_c_0 = c, *ptr_c_1 = ptr_c_0 + lb;
        int *ptr_c_2 = ptr_c_1 + lb, *ptr_c_3 = ptr_c_2 + lb;

        for (int i = 0; i < la; i += 4) {

            // pack a
            memcpy(packa, pa, (wa << 4));
            pa += wa << 2;

            for (int j = 0; j < lb; j += 4) {
                b_0_ptr = pb;
                b_1_ptr = b_0_ptr + wb;
                b_2_ptr = b_1_ptr + wb;
                b_3_ptr = b_2_ptr + wb;
                pb += wb << 2;

                // pack b
                for (int k = 0; k < wb; ++k) {
                    *(ptrb++) = *(b_0_ptr++);
                    *(ptrb++) = *(b_1_ptr++);
                    *(ptrb++) = *(b_2_ptr++);
                    *(ptrb++) = *(b_3_ptr++);
                }

                CalMatrix4Mul4L2(packa, packb, ptrc, wb);

                ptrc += 16;
                ptrb = packb;
            }

            ptrc = packc;

            for (int j = 0; j < lb; ++j) {
                *(ptr_c_0++) = *(ptrc++);
                *(ptr_c_1++) = *(ptrc++);
                *(ptr_c_2++) = *(ptrc++);
                *(ptr_c_3++) = *(ptrc++);
            }

            ptrc = packc;
            pb = b;

            ptr_c_0 += lb + (lb << 1);
            ptr_c_1 += lb + (lb << 1);
            ptr_c_2 += lb + (lb << 1);
            ptr_c_3 += lb + (lb << 1);
        }
    }

    inline void CalMatrix_256(int* a, int la, int wa, int* b, int lb, int wb, int* c)
    {
        int* ptrb = packb;
        int* ptra = packa;
        int* ptrc = packc;

        int *b_0_ptr, *b_1_ptr, *b_2_ptr, *b_3_ptr;
        int *a_0_ptr, *a_1_ptr, *a_2_ptr, *a_3_ptr;

        int *pb = b, *pa = a, *pc = c;
        int *ptr_c_0 = c, *ptr_c_1 = ptr_c_0 + lb;
        int *ptr_c_2 = ptr_c_1 + lb, *ptr_c_3 = ptr_c_2 + lb;

        for (int i = 0; i < la; i += 4) {

            // pack a
            a_0_ptr = pa;
            a_1_ptr = a_0_ptr + wb;
            a_2_ptr = a_1_ptr + wb;
            a_3_ptr = a_2_ptr + wb;

            pa += wa << 2;

            for (int j = 0; j < wa; ++j) {
                *(ptra++) = *(a_0_ptr++);
                *(ptra++) = *(a_1_ptr++);
                *(ptra++) = *(a_2_ptr++);
                *(ptra++) = *(a_3_ptr++);
            }

            for (int j = 0; j < lb; j += 4) {
                b_0_ptr = pb;
                b_1_ptr = b_0_ptr + wb;
                b_2_ptr = b_1_ptr + wb;
                b_3_ptr = b_2_ptr + wb;
                pb += wb << 2;

                // pack b
                for (int k = 0; k < wb; ++k) {
                    *(ptrb++) = *(b_0_ptr++);
                    *(ptrb++) = *(b_1_ptr++);
                    *(ptrb++) = *(b_2_ptr++);
                    *(ptrb++) = *(b_3_ptr++);
                }

                CalMatrix4Mul4L2_256(packa, packb, ptrc, wb);

                ptrc += 16;
                ptrb = packb;
            }

            ptrc = packc;

            for (int j = 0; j < lb; ++j) {
                *(ptr_c_0++) = *(ptrc++);
                *(ptr_c_1++) = *(ptrc++);
                *(ptr_c_2++) = *(ptrc++);
                *(ptr_c_3++) = *(ptrc++);
            }

            ptrc = packc;
            ptra = packa;
            pb = b;

            ptr_c_0 += lb + (lb << 1);
            ptr_c_1 += lb + (lb << 1);
            ptr_c_2 += lb + (lb << 1);
            ptr_c_3 += lb + (lb << 1);
        }
    }
};