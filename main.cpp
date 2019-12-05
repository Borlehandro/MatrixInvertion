#include <iostream>

using namespace std;

/// OK!
float generateMatrixAndCountOneMinor (float* matrix, int n) {
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
float transposeAndCountInfinityMinor(const float* matrix, float *transp, int n) {
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
                currentSum+= matrix[j*n+k]*transposed[i*n+k];
            }
            result[j*n+i] = currentSum;
        }
    }
}

/// OK!
void countR(const float *one, const float *b, const float *transposed, float *r, int n){
    float t[n*n];

    multiplyToTransposed(b, transposed, t, n);

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

            for(int i=0; i<n; i++)
                for(int j=0; j<n; j++) {
                    result[i * n + j] += multed[i * n + j];
                    current[j * n + i] = multed[i * n + j];
                }
        }
    }
}

int main() {
    int n,m;
    cin >> n >> m;
    float matrix[n*n], one[n*n], transposed[n*n], b[n*n], bTrans[n*n], r[n*n];
    float oneMinor = generateMatrixAndCountOneMinor(matrix, n);

    cout << "\n==================== MATRIX ==================== \n";
    for (int i=0; i<n; i++) {
        for (int j = 0; j < n; j++)
            cout << matrix[i*n+j] << " ";
        cout << "\n";
    }
    cout << "\n";
    generateOneMatrix(one, n);

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
    }

    return 0;
}