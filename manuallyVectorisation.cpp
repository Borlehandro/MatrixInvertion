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
                transposed[j * n + k * (n / 4) + (i / 4)][i % 4] = random;
            }

            cout << "\n==================== MATRIX_INTO ==================== \n";
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n / 4; j++) {
                    for (int k = 0; k < 4; k++) {
                        cout << matrix[i * (n / 4) + j][k] << " ";

                    }

                }
                cout << "\n";
            }
            cout << "\n";

            cout << "\n==================== TRANSPOSED_INTO ==================== \n";
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
void generateOneMatrix(v4sf *matrix, int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < (n / 4); j++)
            for (int k = 0; k < 4; k++) {
                matrix[i * (n/4) + j][k] = (j*4+k == i);
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
 * @param matrix
 * @param transposed
 * @param n
 */
void multiplyToTransposed(const v4sf *matrix, const v4sf *transposed, v4sf *result, int n) {
    for (int j = 0; j < n; j++) {
        v4sf multed[n * n / 4];
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
            // cout << "SUM " << sum << endl;
            result[j * (n / 4) + p / 4][p % 4] = sum;

            /*cout << "\n==================== RESsssss ==================== \n";
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n / 4; j++) {
                    for (int k = 0; k < 4; k++) {
                        cout << result[i * (n / 4) + j][k] << " ";

                    }

                }
                cout << "\n";
            }
            cout << "\n";*/
        }
    }
}

/// OK!
void countR(const v4sf *one, const v4sf *b, const v4sf *transposed, v4sf *r, int n) {
    v4sf t[n *(n/4)];

    multiplyToTransposed(b, transposed, t, n);

    for (int i=0; i<n*(n/4); i++)
            r[i] = one[i] - t[i];
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
            //multiplyToTransposed(r, current, multed, n);

            for (int i = 0; i < n; i++)
                for (int j = 0; j < n; j++) {
                    result[i * n + j] += multed[i * n + j];
                    current[j * n + i] = multed[i * n + j];
                }
        }
    }
}

void countMinors(v4sf *matrix, v4sf *transposed, float *minorOne, float *minorInfinity, int n) {

    float maxOneMinor = 0.0f, maxInfMinor = 0.0f;
    v4sf vectorSumInf[n / 4], vectorSumOne[n / 4];

    for (int v = 0; v < n / 4; v++) {
        vectorSumOne[v] = v4sf{0, 0, 0, 0};
        vectorSumInf[v] = v4sf{0, 0, 0, 0};
        for (int l = 0; l < n; l++) {
            vectorSumOne[v] += transposed[v + l * (n / 4)];
            vectorSumInf[v] += matrix[v + l * (n / 4)];
        }
    }

    for (int k = 0; k < n / 4; k++)
        for (int p = 0; p < 4; p++)
            cout << "vector " << k << "," << p << ":" << vectorSumOne[k][p] << endl;

    for (int i = 0; i < n / 4; i++) {
        for (int j = 0; j < 4; j++) {
            maxOneMinor = max(maxOneMinor, vectorSumOne[i][j]);
            cout << "maxOne: " << maxOneMinor << endl;
            maxInfMinor = max(maxInfMinor, vectorSumInf[i][j]);
        }
    }
    *minorOne = maxOneMinor;
    *minorInfinity = maxInfMinor;
}

int main() {
    int n, m;
    cin >> n >> m;

    v4sf matrix[n * (n / 4)], res[n * (n / 4)], one[n * (n / 4)], transposed[n * (n / 4)], b[n * (n / 4)], bTrans[
            n * (n / 4)], r[n * (n / 4)];

    float minorInfinity, minorOne = 0.0f;
    generateMatrixAndCountTranspose(matrix, transposed, n);
    generateOneMatrix(one, n);

    cout << "\n==================== 1 ==================== \n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n / 4; j++) {
            for (int k = 0; k < 4; k++) {
                cout << one[i * (n / 4) + j][k] << " ";
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

    countMinors(matrix, transposed, &minorOne, &minorInfinity, n);
    cout << "================ Minors: " << minorOne << " , " << minorInfinity << endl;

    divideToScalarAndCountResultTranspose(matrix, b, bTrans, n, minorInfinity * minorOne);

    cout << "================ Minors: " << minorOne << " , " << minorInfinity << endl;

    // multiplyToTransposed(b, transposed, res, n);

    b[0] = v4sf{5,2,3,1};
    b[1] = v4sf{4,1,2,3};
    b[2] = v4sf{1,2,3,4};
    b[3] = v4sf{5,4,3,2};

    transposed[0] = v4sf{1,4,2,4};
    transposed[1] = v4sf{2,1,1,1};
    transposed[2] = v4sf{3,2,3,3};
    transposed[3] = v4sf{4,3,4,2};

    countR(one, b, transposed, r, n);


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


    cout << "\n==================== B ==================== \n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n / 4; j++) {
            for (int k = 0; k < 4; k++) {
                cout << b[i * (n / 4) + j][k] << " ";

            }

        }
        cout << "\n";
    }
    cout << "\n";

    cout << "\n==================== B_Trans ==================== \n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n / 4; j++) {
            for (int k = 0; k < 4; k++) {
                cout << bTrans[i * (n / 4) + j][k] << " ";

            }

        }
        cout << "\n";
    }
    cout << "\n";

    cout << "\n==================== R ==================== \n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n / 4; j++) {
            for (int k = 0; k < 4; k++) {
                cout << r[i * (n / 4) + j][k] << " ";

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