#include <iostream>

using namespace std;

/// OK!
float generateMatrixAndCountMinor (float* matrix, int n) {
    std::srand(time(NULL));
    float maxMinor = 0;
    for (int i=0; i<n; i++) {
        float minor = 0;
        for (int j=0; j<n; j++){
            matrix[i*n+j] = ((float)rand())/((float)RAND_MAX)*100;
            minor+=matrix[i*n+j];
        }
        if(minor > maxMinor)
            maxMinor = minor;
    }
    return maxMinor;
}

/// OK!
void generateOneMatrix(float* matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            matrix[i*n + j] = (i==j);
    }
}

/// OK!
float transposeAndCountMinor(const float* matrix, float *transp, int n) {
    float maxMinor = 0;
    for (int j=0; j<n; j++) {
        float minor = 0;
        for (int i=0; i<n; i++){
            transp[j*n + i] = matrix[i*n + j];
            minor += matrix[i*n + j];
        }
        if(minor>maxMinor)
            maxMinor=minor;
    }
    return maxMinor;
}

/// OK!
void divideToScalarAndCountResultTranspose(const float* matrix, float* result, float* resultTrans, int n, float scalar){
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            resultTrans[j*n+i] = result[i * n + j] = matrix[i * n + j] / scalar;
    }
}

/**
 * You must send transposed matrix in this function
 * @param matrix
 * @param transposed
 * @param n
 */
void multiplyToTransposed(const float* matrix, const float* transposed, float* result, int n){
    for (int i=0; i<n; i++) {
        for (int j=0; j<n; j++) {
            float currentSum = 0;
            for(int k=0; k<n; k++){
                //For debug
                auto a = matrix[j*n+k];
                auto b = transposed[i*n+k];
                currentSum+= matrix[j*n+k]*transposed[i*n+k];
            }
            result[j*n+i] = currentSum;
        }
    }
}

/// OK!
void countR(const float *one, const float *b, const float *transposed, float *r, int n){
    float t[n*n];

    cout << "\n b inside \n";
    for(int i=0; i<n; i++) {
        for (int j = 0; j < n; j++)
            cout << b[i * n + j] << " ";
        cout << "\n";
    }
    cout << "\n";

    multiplyToTransposed(b, transposed, t, n);
    cout << "\n PROM \n";
    for(int i=0; i<n; i++) {
        for (int j = 0; j < n; j++)
            cout << t[i * n + j] << " ";
        cout << "\n";
    }
    cout << "\n";

    for(int i=0; i<n; i++)
        for(int j=0; j<n; j++)
            r[i*n+j] = one[i*n+j] - t[i*n+j];
}

void countSeries(const float *one, const float *r, float *result, int n, int m) {
    //Todo Refactor
    if(m == 1) {
        for(int i=0; i<n; i++)
            for(int j=0; j<n; j++)
                result[i*n+j] = one[i*n+j];
        return;
    } else if(m == 2) {
        for(int i=0; i<n; i++)
            for(int j=0; j<n; j++)
                result[i*n+j] = one[i*n+j] + r[i*n+j];
        return;
    } else {
        float current[n*n];
        for(int i=0; i<n; i++)
            for(int j=0; j<n; j++) {
                result[i * n + j] = one[i * n + j] + r[i * n + j];
                current[j * n + i] = r[i * n + j];
            }


        for (int k=0; k<m-2; k++) {
            float multed[n*n];
            multiplyToTransposed(r, current, multed, n);



            cout << "\n MULTED \n";
            for(int i=0; i<n; i++) {
                for (int j = 0; j < n; j++)
                    cout << multed[i*n + j] << " ";
                cout << "\n";
            }
            cout << "\n";

            for(int i=0; i<n; i++)
                for(int j=0; j<n; j++) {
                    result[i * n + j] += multed[i * n + j];
                    current[j * n + i] = multed[i * n + j];
                }
        }

    }
    cout << "\n  inside res \n";
    for(int i=0; i<n; i++) {
        for (int j = 0; j < n; j++)
            cout << result[i*n + j] << " ";
        cout << "\n";
    }
    cout << "\n";
}

int main() {
    int n,m;
    cin >> n >> m;
    float one[n*n], transp[n*n], b[n*n], bTrans[n*n], r[n*n];
    // float minor = generateMatrixAndCountMinor(matrix, n);
    float matrix[] = {5,8,6,4,3,2,0,1,4};
    float minor = 19;
    cout << "minor  is " << minor << "\n";
    for (int i=0; i<n; i++) {
        for (int j = 0; j < n; j++)
            cout << matrix[i*n+j] << " ";
        cout << "\n";
    }
    // Matrix generation and minor ||A||1 counting is correct
    generateOneMatrix(one, n);

    //For matrix multiplication test

    /*float t[] = {5,8,6,4,3,2,0,1,4};
    float l[] = {4,2,1,3,6,4,1,7,5};
    float q[3*3];
    multiplyToTransposed(t, l, q, 3);
    for (int i=0; i<n; i++) {
        for (int j = 0; j < n; j++)
            cout << q[i*n+j] << " ";
        cout << "\n";
    }*/

    float q[3*3];

    float minor2 = transposeAndCountMinor(matrix, q, n);
    cout << "minor is !!!" << minor2 << "\n";
    for (int i=0; i<n; i++) {
        for (int j = 0; j < n; j++)
            cout << q[i*n+j] << " ";
        cout << "\n";
    }
    cout << "\n";

    cout << "B:\n";
    divideToScalarAndCountResultTranspose(q, b, bTrans, n, minor * minor2);
    for (int i=0; i<n; i++) {
        for (int j = 0; j < n; j++)
            cout << b[i*n+j] << " ";
        cout << "\n";
    }
    cout << "\n B_trans \n";
    for (int i=0; i<n; i++) {
        for (int j = 0; j < n; j++)
            cout << bTrans[i*n+j] << " ";
        cout << "\n";
    }
    cout <<"\n R: \n";

    countR(one, b, q, r, n);
    for (int i=0; i<n; i++) {
        for (int j = 0; j < n; j++)
            cout << r[i * n + j] << " ";
        cout << "\n";
    }

    float series[n*n];
    countSeries(one, r, series, n, m);

    float result[n*n];
    multiplyToTransposed(series, bTrans, result, n);
    cout << "\n RES: \n";
    for (int i=0; i<n; i++) {
        for (int j = 0; j < n; j++)
            cout << result[i * n + j] << " ";
        cout << "\n";
    }

    // Checking
    float rt[n*n];
    transposeAndCountMinor(result, rt, n);
    float check[n*n];
    multiplyToTransposed(matrix, rt, check, n);

    cout << "\n CHECK: \n";
    for (int i=0; i<n; i++) {
        for (int j = 0; j < n; j++)
            cout << check[i * n + j] << " ";
        cout << "\n";
    }

    return 0;
}