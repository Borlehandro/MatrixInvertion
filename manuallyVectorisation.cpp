#include <iostream>

typedef float v4sf __attribute__ ((vector_size (16)));

using namespace std;


union float4 {
    v4sf vector;
    float array[4];
};

/// OK!
void generateMatrixAndCountTranspose(v4sf *matrix, v4sf *transposed, int n) {
    std::srand(time(NULL));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n / 4; j++) {
            for (int k = 0; k < 4; k++) {
                auto random = ((float) rand()) / ((float) RAND_MAX) * 100;
                int a = j * 8 + k * (n / 4) + (i / 4);
                int b = i % 4;
                matrix[i * n / 4 + j][k] = random;
                cout << "into " << a << "," << b << " " << random << "\n";
                transposed[j * 8 + k * (n / 4) + (i / 4)][i % 4] = random;
            }

            cout << "\n==================== MATRIX ==================== \n";
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n / 4; j++) {
                    for (int k = 0; k < 4; k++) {
                        cout << matrix[i * (n / 4) + j][k] << " ";

                    }

                }
                cout << "\n";
            }
            cout << "\n";

            cout << "\n==================== TRANSPOSED ==================== \n";
            for (int m = 0; m < n; m++) {
                for (int q = 0; q < n / 4; q++) {
                    for (int k = 0; k < 4; k++) {
                        cout << transposed[m * (n / 4) + q][k] << " ";
                    }

                }
                cout << "\n";
            }
            cout << "\n";
        }
    }
}

/// OK!
void generateOneMatrix(float *matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            matrix[i * n + j] = (i == j);
    }
}

/// OK!
float transposeAndCountInfinityMinor(const float *matrix, float *transp, int n) {
    float maxMinor = 0;
    for (int j = 0; j < n; j++) {
        float minor = 0;
        for (int i = 0; i < n; i++) {
            transp[j * n + i] = matrix[i * n + j];
            minor += matrix[i * n + j];
        }
        if (minor > maxMinor)
            maxMinor = minor;
    }
    return maxMinor;
}

/// OK!
void
divideToScalarAndCountResultTranspose(const float *matrix, float *result, float *resultTrans, int n, float scalar) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            resultTrans[j * n + i] = result[i * n + j] = matrix[i * n + j] / scalar;
    }
}

/**
 * You must send transposed matrix in this function
 * @param matrix
 * @param transposed
 * @param n
 */
void multiplyToTransposed(const float *matrix, const float *transposed, float *result, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            float currentSum = 0;
            for (int k = 0; k < n; k++) {
                currentSum += matrix[j * n + k] * transposed[i * n + k];
            }
            result[j * n + i] = currentSum;
        }
    }
}

/// OK!
void countR(const float *one, const float *b, const float *transposed, float *r, int n) {
    float t[n * n];

    multiplyToTransposed(b, transposed, t, n);

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            r[i * n + j] = one[i * n + j] - t[i * n + j];
}

void countSeries(const float *one, const float *r, float *result, int n, int m) {
    //Todo Refactor
    if (m == 1) {
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                result[i * n + j] = one[i * n + j];
        return;
    } else if (m == 2) {
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                result[i * n + j] = one[i * n + j] + r[i * n + j];
        return;
    } else {
        float current[n * n];
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++) {
                result[i * n + j] = one[i * n + j] + r[i * n + j];
                current[j * n + i] = r[i * n + j];
            }

        for (int k = 0; k < m - 2; k++) {

            float multed[n * n];
            multiplyToTransposed(r, current, multed, n);

            for (int i = 0; i < n; i++)
                for (int j = 0; j < n; j++) {
                    result[i * n + j] += multed[i * n + j];
                    current[j * n + i] = multed[i * n + j];
                }
        }
    }
}

void countMinors(v4sf *matrix, v4sf *transposed, float *minorOne, float *minorInfinity, int n) {
    float maxOneMinor = 0;
    float maxInfMinor = 0;
    v4sf vectorSumInf[n / 4], vectorSumOne[n / 4];
    // I - Столбец, J - Вектор в столбце
    for (int j = 0; j < n / 4; j++) {
        for (int i = 0; i < n; i++) {
            vectorSumInf[j] += transposed[i + j];
            vectorSumOne[j] += matrix[i + j];
        }
    }

    for (int i = 0; i < n / 4; i++) {
        for (int j = 0; j < 4; j++) {
            maxOneMinor = max(maxOneMinor, vectorSumOne[i][j]);
            maxInfMinor = max(maxInfMinor, vectorSumInf[i][j]);
        }
    }
    *minorOne = maxOneMinor;
    *minorInfinity = maxInfMinor;
}

int main() {
    int n, m;
    cin >> n >> m;

    v4sf matrix[n * n / 4], one[n * n], transposed[n * n / 4], b[n * n], bTrans[n * n], r[n * n];
    float minorInfinity, minorOne = 0.0f;
    generateMatrixAndCountTranspose(matrix, transposed, n);
    countMinors(matrix, transposed, &minorOne, &minorInfinity, n);

    cout << "minors : " << minorOne << " " << minorInfinity << endl;

    cout << "\n==================== MATRIX ==================== \n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n / 4; j++) {
            for (int k = 0; k < 4; k++) {
                cout << matrix[i * (n / 4) + j][k] << " ";

            }

        }
        cout << "\n";
    }
    cout << "\n";


    cout << "\n==================== TRANSPOSED ==================== \n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n / 4; j++) {
            for (int k = 0; k < 4; k++) {
                cout << transposed[i * (n / 4) + j][k] << " ";
            }

        }
        cout << "\n";
    }
    cout << "\n";


    /*generateOneMatrix(one, n);

    float infinityMinor = transposeAndCountInfinityMinor(matrix, transposed, n);

    cout << "oneMinor: " << oneMinor << "\n";
    cout << "infinityMinor: " << infinityMinor << "\n";


    divideToScalarAndCountResultTranspose(transposed, b, bTrans, n, oneMinor * infinityMinor);

    countR(one, b, transposed, r, n);

    float series[n*n];
    countSeries(one, r, series, n, m);

    float result[n*n];
    multiplyToTransposed(series, bTrans, result, n);
    cout << "\n==================== RESULT ==================== \n";
    for (int i=0; i<n; i++) {
        for (int j = 0; j < n; j++)
            cout << result[i * n + j] << " ";
        cout << "\n";
    }

    // Checking
    float rt[n*n];
    transposeAndCountInfinityMinor(result, rt, n);
    float check[n*n];
    multiplyToTransposed(matrix, rt, check, n);

    cout << "\n==================== CHECK ==================== \n";
    for (int i=0; i<n; i++) {
        for (int j = 0; j < n; j++)
            cout << check[i * n + j] << " ";
        cout << "\n";
    }*/

    return 0;
}