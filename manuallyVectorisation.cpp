#include <iostream>

typedef float v4sf __attribute__ ((vector_size (16)));

using namespace std;

/// OK!
void generateMatrixAndCountTranspose(v4sf *matrix, v4sf *transposed, int n) {
    std::srand(time(NULL));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n / 4; j++) {
            for (int k = 0; k < 4; k++) {
                //Todo Refactor
                auto random = ((float) rand()) / ((float) RAND_MAX) * 100;
                matrix[i * n / 4 + j][k] = random;
                transposed[j * n + k * (n / 4) + (i / 4)][i % 4] = random;
            }

        }
    }
}

/// OK!
void generateOneMatrix(v4sf *matrix, int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < (n / 4); j++)
            for (int k = 0; k < 4; k++) {
                matrix[i * (n/4) + j][k] = (j*4+k == i);
            }
}

/// OK!
void divideToScalarAndCountResultTranspose(const v4sf *matrix, v4sf *result, v4sf *resultTrans, int n, float scalar) {
    for (int i = 0; i < n * (n / 4); i++) {
        result[i] = matrix[i] / scalar;
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n / 4; j++) {
            for (int k = 0; k < 4; k++) {
                resultTrans[j * n + k * (n / 4) + (i / 4)][i % 4] = result[i * n / 4 + j][k];
            }
        }
    }
}

/**
 * @details You must send transposed matrix in this function
 */
void multiplyToTransposed(const v4sf *matrix, const v4sf *transposed, v4sf *result, int n) {
    for (int j = 0; j < n; j++) {
        v4sf *multed = (v4sf*)(malloc(n * (n / 4)* sizeof(v4sf)));
        for (int k = 0; k < n / 4; k++) {
            for (int i = 0; i < n; i++) {
                multed[i * (n / 4) + k] = matrix[j * (n / 4) + k] * transposed[i * (n / 4) + k];
            }
        }

        for (int p = 0; p < n; p++) {
            float sum = 0.0f;
            for (int q = 0; q < n / 4; q++) {
                for (int k = 0; k < 4; k++) {
                    sum += multed[p * (n / 4) + q][k];
                }
            }

            result[j * (n / 4) + p / 4][p % 4] = sum;
        }
        delete(multed);
    }
}

/// OK!
void countR(const v4sf *one, const v4sf *b, const v4sf *transposed, v4sf *r, int n) {
    v4sf *t = (v4sf*)(malloc(n * (n / 4)* sizeof(v4sf)));

    multiplyToTransposed(b, transposed, t, n);

    for (int i=0; i<n*(n/4); i++)
            r[i] = one[i] - t[i];

}

void countSeries(const v4sf *one, const v4sf *r, v4sf *result, int n, int m) {
    //Todo Refactor
    if (m == 1) {
        for (int i = 0; i < n*(n/4); i++)
                result[i] = one[i];
        return;
    } else if (m == 2) {
        for (int i = 0; i < n*(n/4); i++)
                result[i] = one[i] + r[i];
        return;
    } else {
        v4sf* current = (v4sf*)malloc(n * (n/4) * sizeof(v4sf));
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n / 4; j++) {
                result[i*(n/4) + j] = one[i*(n/4) + j] + r[i*(n/4) + j];
                for (int k = 0; k < 4; k++) {
                    current[j * n + k * (n / 4) + (i / 4)][i % 4] = r[i * n / 4 + j][k];
                }
            }
        }

        for (int k = 0; k < m - 2; k++) {

            v4sf *multed = (v4sf*)malloc(sizeof(v4sf)*(n*(n/4)));
            multiplyToTransposed(r, current, multed, n);

            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n / 4; j++) {
                    result[i*(n/4) + j]+= multed[i*(n/4)+j];
                    for (int p = 0; p < 4; p++) {
                        current[j * n + p * (n / 4) + (i / 4)][i % 4] = multed[i * n / 4 + j][p];
                    }
                }
            }
            free(multed);
        }
        free(current);
    }
}

void countMinors(v4sf *matrix, v4sf *transposed, float *minorOne, float *minorInfinity, int n) {

    float maxOneMinor = 0.0f, maxInfMinor = 0.0f;
    v4sf *vectorSumInf = (v4sf*)(malloc((n / 4)* sizeof(v4sf)));
    v4sf *vectorSumOne = (v4sf*)(malloc((n / 4)* sizeof(v4sf)));

    for (int v = 0; v < n / 4; v++) {
        vectorSumOne[v] = v4sf{0, 0, 0, 0};
        vectorSumInf[v] = v4sf{0, 0, 0, 0};
        for (int l = 0; l < n; l++) {
            vectorSumOne[v] += transposed[v + l * (n / 4)];
            vectorSumInf[v] += matrix[v + l * (n / 4)];
        }
    }

    for (int i = 0; i < n / 4; i++) {
        for (int j = 0; j < 4; j++) {
            maxOneMinor = max(maxOneMinor, vectorSumOne[i][j]);
            maxInfMinor = max(maxInfMinor, vectorSumInf[i][j]);
        }
    }
    free(vectorSumInf);
    free(vectorSumOne);
    *minorOne = maxOneMinor;
    *minorInfinity = maxInfMinor;
}

int main() {
    int n, m;
    cin >> n >> m;

    v4sf *matrix = (v4sf*)(malloc(n * (n / 4)* sizeof(v4sf))); v4sf *res = (v4sf*)(malloc(n * (n / 4)* sizeof(v4sf)));
    v4sf *one = (v4sf*)(malloc(n * (n / 4)* sizeof(v4sf))); v4sf *transposed = (v4sf*)(malloc(n * (n / 4)* sizeof(v4sf)));
    v4sf *b = (v4sf*)(malloc(n * (n / 4)* sizeof(v4sf)));  v4sf *bTrans = (v4sf*)(malloc(n * (n / 4)* sizeof(v4sf)));
    v4sf *r = (v4sf*)(malloc(n * (n / 4)* sizeof(v4sf)));

    float minorInfinity, minorOne = 0.0f;
    generateMatrixAndCountTranspose(matrix, transposed, n);
    generateOneMatrix(one, n);

    countMinors(matrix, transposed, &minorOne, &minorInfinity, n);
    cout << "================ Minors: " << minorOne << " , " << minorInfinity << endl;

    divideToScalarAndCountResultTranspose(matrix, b, bTrans, n, minorInfinity * minorOne);

    cout << "================ Minors: " << minorOne << " , " << minorInfinity << endl;

    countR(one, b, transposed, r, n);
    cout << "R OK" << endl;
    v4sf *series = (v4sf*)(malloc(n * (n / 4)* sizeof(v4sf)));
    countSeries(one, r, series, n, m);
    cout << "Series OK" << endl;
    multiplyToTransposed(series, bTrans, res, n);
    free(series);
    free(r);

    /*v4sf *check = (v4sf*)(malloc(n * (n / 4)* sizeof(v4sf)));
    multiplyToTransposed(res, transposed, check, n);
    free(check);*/

    free(matrix);
    free(b);
    free(bTrans);

    return 0;
}