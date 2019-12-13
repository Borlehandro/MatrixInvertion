#include <iostream>

using namespace std;

/// OK!
float generateMatrixAndCountOneMinor (float* matrix, long long n) {
    std::srand(time(NULL));
    float maxMinor = 0;
    for (long long i=0; i<n; i++) {
        float minor = 0;
        for (long long j=0; j<n; j++){
            matrix[i*n+j] = ((float)rand())/((float)RAND_MAX)*100;
            minor+=matrix[i*n+j];
        }
        if(minor > maxMinor)
            maxMinor = minor;
    }
    return maxMinor;
}

/// OK!
void generateOneMatrix(float* matrix, long long n) {
    for (long long i = 0; i < n; i++) {
        for (long long j = 0; j < n; j++)
            matrix[i*n + j] = (i==j);
    }
}

/// OK!
float transposeAndCountInfinityMinor(const float* matrix, float *transp, long long n) {
    float maxMinor = 0;
    for (long long j=0; j<n; j++) {
        float minor = 0;
        for (long long i=0; i<n; i++){
            transp[j*n + i] = matrix[i*n + j];
            minor += matrix[i*n + j];
        }
        if(minor>maxMinor)
            maxMinor=minor;
    }
    return maxMinor;
}

/// OK!
void divideToScalarAndCountResultTranspose(const float* matrix, float* result, float* resultTrans, long long n, float scalar){
    for (long long i = 0; i < n; i++) {
        for (long long j = 0; j < n; j++)
            resultTrans[j*n+i] = result[i * n + j] = matrix[i * n + j] / scalar;
    }
}

/**
 * You must send transposed matrix in this function
 * @param matrix
 * @param transposed
 * @param n
 */
void multiplyToTransposed(const float* matrix, const float* transposed, float* result, long long n){
    for (long long i=0; i<n; i++) {
        for (long long j=0; j<n; j++) {
            float currentSum = 0;
            for(long long k=0; k<n; k++){
                currentSum+= matrix[j*n+k]*transposed[i*n+k];
            }
            result[j*n+i] = currentSum;
        }
    }
}

/// OK!
void countR(const float *one, const float *b, const float *transposed, float *r, long long n){
    auto *t = (float*)malloc(n*n*sizeof(float));

    multiplyToTransposed(b, transposed, t, n);

    for(long long i=0; i<n; i++)
        for(long long j=0; j<n; j++)
            r[i*n+j] = one[i*n+j] - t[i*n+j];
}

void countSeries(const float *one, const float *r, float *result, long long n, long long m) {
    //Todo Refactor
    if(m == 1) {
        for(long long i=0; i<n; i++)
            for(long long j=0; j<n; j++)
                result[i*n+j] = one[i*n+j];
        return;
    } else if(m == 2) {
        for(long long i=0; i<n; i++)
            for(long long j=0; j<n; j++)
                result[i*n+j] = one[i*n+j] + r[i*n+j];
        return;
    } else {
        auto *current = (float*)malloc(n*n*sizeof(float));
        for(long long i=0; i<n; i++)
            for(long long j=0; j<n; j++) {
                result[i * n + j] = one[i * n + j] + r[i * n + j];
                current[j * n + i] = r[i * n + j];
            }

        for (long long k=0; k<m-2; k++) {

            auto *multed = (float*)malloc(n*n*sizeof(float));
            multiplyToTransposed(r, current, multed, n);

            for(long long i=0; i<n; i++)
                for(long long j=0; j<n; j++) {
                    result[i * n + j] += multed[i * n + j];
                    current[j * n + i] = multed[i * n + j];
                }
        }
    }
}

int main() {

    long long n,m;
    cin >> n >> m;

    auto *matrix = (float*)malloc(n*n*sizeof(float)); auto *one = (float*)malloc(n*n*sizeof(float));  auto *transposed = (float*)malloc(n*n*sizeof(float));
    auto *b = (float*)malloc(n*n*sizeof(float)); auto *bTrans = (float*)malloc(n*n*sizeof(float)); auto *r = (float*)malloc(n*n*sizeof(float));

    float oneMinor = generateMatrixAndCountOneMinor(matrix, n);

    generateOneMatrix(one, n);

    float infinityMinor = transposeAndCountInfinityMinor(matrix, transposed, n);

    divideToScalarAndCountResultTranspose(transposed, b, bTrans, n, oneMinor * infinityMinor);

    countR(one, b, transposed, r, n);

    auto *series = (float*)malloc(n*n*sizeof(float));
    countSeries(one, r, series, n, m);

    auto *result = (float*)malloc(n*n*sizeof(float));
    multiplyToTransposed(series, bTrans, result, n);

    // Checking
    /*auto *rt = (float*)malloc(n*n*sizeof(float));
    transposeAndCountInfinityMinor(result, rt, n);
    auto *check = (float*)malloc(n*n*sizeof(float));
    multiplyToTransposed(matrix, rt, check, n);

    cout << "\n==================== CHECK ==================== \n";
    for (long long i=0; i<n; i++) {
        for (long long j = 0; j < n; j++)
            cout << check[i * n + j] << " ";
        cout << "\n";
    }*/

    return 0;
}