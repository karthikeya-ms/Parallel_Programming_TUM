#include "dgemm.h"
#include <cstdio>
#include <cstdlib>
#include <immintrin.h>



void dgemm(float alpha, const float *a, const float *b, float beta, float *c) {
    
    for (size_t i = 0; i < MATRIX_SIZE; i++) {
        
        for (int j = 0; j < MATRIX_SIZE; j++) {
            
            c[i * MATRIX_SIZE + j] *= beta;

            __m256 partialSum = _mm256_setzero_ps();

            float partialSumArr[8]  = {0, 0, 0, 0, 0, 0, 0, 0};

            int k = 0;
            
            for (; k+8 <= MATRIX_SIZE; k+=8) {
                
                //c[i * MATRIX_SIZE + j] += alpha * a[i * MATRIX_SIZE + k] * b[j * MATRIX_SIZE + k];
                __m256 aa = _mm256_loadu_ps(a + i * MATRIX_SIZE + k);
                __m256 bb = _mm256_loadu_ps(b + j * MATRIX_SIZE + k);
                //__m256 tmp = _mm256_mul_ps(aa, bb);
                partialSum = _mm256_add_ps(partialSum,  _mm256_mul_ps(aa, bb));

            }
            _mm256_store_ps(partialSumArr, partialSum);

            float sum = 0;
            
            for (int i = 0; i < 8; i++){

                sum += partialSumArr[i];
            }

            for(; k < MATRIX_SIZE; k++){

                sum += a[i * MATRIX_SIZE + k] * b[j * MATRIX_SIZE + k];
            }

            c[i * MATRIX_SIZE + j] += alpha * sum;
        }
    }
}



void dgemm_avx(float alpha, float* A, float* B, float beta, float* C) {
    for (int i = 0; i < MATRIX_SIZE ; i += 8) {
        for (int j = 0; j < MATRIX_SIZE ; j += 8) {
            __m256 c00 = _mm256_loadu_ps(C + i * MATRIX_SIZE  + j);
            __m256 c01 = _mm256_loadu_ps(C + (i + 1) * MATRIX_SIZE  + j);
            __m256 c02 = _mm256_loadu_ps(C + (i + 2) * MATRIX_SIZE  + j);
            __m256 c03 = _mm256_loadu_ps(C + (i + 3) * MATRIX_SIZE  + j);
            __m256 c04 = _mm256_loadu_ps(C + (i + 4) * MATRIX_SIZE  + j);
            __m256 c05 = _mm256_loadu_ps(C + (i + 5) * MATRIX_SIZE  + j);
            __m256 c06 = _mm256_loadu_ps(C + (i + 6) * MATRIX_SIZE  + j);
            __m256 c07 = _mm256_loadu_ps(C + (i + 7) * MATRIX_SIZE  + j);

            for (int k = 0; k < MATRIX_SIZE ; k += 8) {
                __m256 a0 = _mm256_loadu_ps(A + i * MATRIX_SIZE  + k);
                __m256 a1 = _mm256_loadu_ps(A + (i + 1) * MATRIX_SIZE  + k);
                __m256 a2 = _mm256_loadu_ps(A + (i + 2) * MATRIX_SIZE  + k);
                __m256 a3 = _mm256_loadu_ps(A + (i + 3) * MATRIX_SIZE  + k);
                __m256 a4 = _mm256_loadu_ps(A + (i + 4) * MATRIX_SIZE  + k);
                __m256 a5 = _mm256_loadu_ps(A + (i + 5) * MATRIX_SIZE  + k);
                __m256 a6 = _mm256_loadu_ps(A + (i + 6) * MATRIX_SIZE  + k);
                __m256 a7 = _mm256_loadu_ps(A + (i + 7) * MATRIX_SIZE  + k);

                for (int l = 0; l < 8; ++l) {
                    __m256 b = _mm256_set1_ps(B[(k + l) * MATRIX_SIZE  + j]);

                    c00 = _mm256_add_ps(_mm256_mul_ps(a0, b), c00);
                    c01 = _mm256_add_ps(_mm256_mul_ps(a1, b), c01);
                    c02 = _mm256_add_ps(_mm256_mul_ps(a2, b), c02);
                    c03 = _mm256_add_ps(_mm256_mul_ps(a3, b), c03);
                    c04 = _mm256_add_ps(_mm256_mul_ps(a4, b), c04);
                    c05 = _mm256_add_ps(_mm256_mul_ps(a5, b), c05);
                    c06 = _mm256_add_ps(_mm256_mul_ps(a6, b), c06);
                    c07 = _mm256_add_ps(_mm256_mul_ps(a7, b), c07);
                }
            }

            __m256 alpha_vec = _mm256_set1_ps(alpha);
            __m256 beta_vec = _mm256_set1_ps(beta);

            c00 = _mm256_add_ps(_mm256_mul_ps(alpha_vec, c00),
                                _mm256_mul_ps(beta_vec, _mm256_loadu_ps(C + i * MATRIX_SIZE  + j)));
            c01 = _mm256_add_ps(_mm256_mul_ps(alpha_vec, c01),
                                _mm256_mul_ps(beta_vec, _mm256_loadu_ps(C + (i + 1) * MATRIX_SIZE  + j)));
            c02 = _mm256_add_ps(_mm256_mul_ps(alpha_vec, c02),
                                _mm256_mul_ps(beta_vec, _mm256_loadu_ps(C + (i + 2) * MATRIX_SIZE  + j)));
            c03 = _mm256_add_ps(_mm256_mul_ps(alpha_vec, c03),
                                _mm256_mul_ps(beta_vec, _mm256_loadu_ps(C + (i + 3) * MATRIX_SIZE  + j)));
            c04 = _mm256_add_ps(_mm256_mul_ps(alpha_vec, c04),
                                _mm256_mul_ps(beta_vec, _mm256_loadu_ps(C + (i + 4) * MATRIX_SIZE  + j)));
            c05 = _mm256_add_ps(_mm256_mul_ps(alpha_vec, c05),
                                _mm256_mul_ps(beta_vec, _mm256_loadu_ps(C + (i + 5) * MATRIX_SIZE  + j)));
            c06 = _mm256_add_ps(_mm256_mul_ps(alpha_vec, c06),
                                _mm256_mul_ps(beta_vec, _mm256_loadu_ps(C + (i + 6) * MATRIX_SIZE  + j)));
            c07 = _mm256_add_ps(_mm256_mul_ps(alpha_vec, c07),
                                _mm256_mul_ps(beta_vec, _mm256_loadu_ps(C + (i + 7) * MATRIX_SIZE  + j)));

            _mm256_storeu_ps(C + i * MATRIX_SIZE  + j, c00);
            _mm256_storeu_ps(C + (i + 1) * MATRIX_SIZE  + j, c01);
            _mm256_storeu_ps(C + (i + 2) * MATRIX_SIZE  + j, c02);
            _mm256_storeu_ps(C + (i + 3) * MATRIX_SIZE  + j, c03);
            _mm256_storeu_ps(C + (i + 4) * MATRIX_SIZE  + j, c04);
            _mm256_storeu_ps(C + (i + 5) * MATRIX_SIZE  + j, c05);
            _mm256_storeu_ps(C + (i + 6) * MATRIX_SIZE  + j, c06);
            _mm256_storeu_ps(C + (i + 7) * MATRIX_SIZE  + j, c07);
        }
    }
}


void dgemm_avx2(float alpha, float* A, float* B, float beta, float* C) {
    __m256 alpha_vec = _mm256_set1_ps(alpha);
    __m256 beta_vec = _mm256_set1_ps(beta);

    for (int i = 0; i < MATRIX_SIZE; i += 8) {
        for (int j = 0; j < MATRIX_SIZE; j += 8) {
            __m256 c[8];

            for (int l = 0; l < 8; ++l) {
                c[l] = _mm256_loadu_ps(C + (i + l) * MATRIX_SIZE + j);
            }

            for (int k = 0; k < MATRIX_SIZE; k += 8) {
                __m256 a[8];

                for (int l = 0; l < 8; ++l) {
                    a[l] = _mm256_set1_ps(A[(i + l) * MATRIX_SIZE + k]);
                }

                for (int l = 0; l < 8; ++l) {
                    __m256 b = _mm256_loadu_ps(B + (k + l) * MATRIX_SIZE + j);

                    for (int m = 0; m < 8; ++m) {
                        c[m] = _mm256_add_ps(_mm256_mul_ps(a[m], _mm256_set1_ps(B[(k + l) * MATRIX_SIZE + (j + m)])), c[m]);
                    }
                }
            }

            for (int l = 0; l < 8; ++l) {
                c[l] = _mm256_add_ps(_mm256_mul_ps(alpha_vec, c[l]), _mm256_mul_ps(beta_vec, _mm256_loadu_ps(C + (i + l) * MATRIX_SIZE + j)));
                _mm256_storeu_ps(C + (i + l) * MATRIX_SIZE + j, c[l]);
            }
        }
    }
}

int main(int, char **) {
    float alpha, beta;

    // mem allocations
    int mem_size = MATRIX_SIZE * MATRIX_SIZE * sizeof(float);
    auto a = (float *) malloc(mem_size);
    auto b = (float *) malloc(mem_size);
    auto c = (float *) malloc(mem_size);

    // check if allocated
    if (nullptr == a || nullptr == b || nullptr == c) {
        printf("Memory allocation failed\n");
        if (nullptr != a) free(a);
        if (nullptr != b) free(b);
        if (nullptr != c) free(c);
        return 0;
    }

    generateProblemFromInput(alpha, a, b, beta, c);

    std::cerr << "Launching dgemm step." << std::endl;
    // matrix-multiplication
    dgemm_avx2(alpha, a, b, beta, c);

    outputSolution(c);

    free(a);
    free(b);
    free(c);
    return 0;
}