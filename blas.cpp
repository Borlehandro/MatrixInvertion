#include <cblas.h>
#include <iostream>

using namespace std;

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



int main ( int argc, char* argv[] ) {

    long long n,m;
    cin >> n >> m;
    auto *matrix = (float*)malloc(n*n*sizeof(float)); auto *one = (float*)malloc(n*n*sizeof(float));  auto *transposed = (float*)malloc(n*n*sizeof(float));
    auto *b = (float*)malloc(n*n*sizeof(float)); auto *bTrans = (float*)malloc(n*n*sizeof(float)); auto *r = (float*)malloc(n*n*sizeof(float));

    float oneMinor = generateMatrixAndCountOneMinor(matrix, n);

    generateOneMatrix(one, n);
    float infinityMinor = transposeAndCountInfinityMinor(matrix, transposed, n);

    // Calculate B
    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, n, n, n, 1.0f/(infinityMinor*oneMinor), matrix, n, one, n, 0.0, b, n);

    // Calculate R
    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, n, n, n, -1.0f, b, n, matrix, n, 0.0, r, n);
    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, n, n, n, 1, one, n, one, n, 1, r, n);


    auto *test = (float*)malloc(n*n*sizeof(float));

    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, n, n, n, 1.0f, r, n, one, n, 0.0, test, n);

    auto *pow = (float*)malloc(n*n*sizeof(float));
    auto *series = (float*)malloc(n*n*sizeof(float));

    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, n, n, n, 1.0f, r, n, one, n, 0.0, pow, n);
    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, n, n, n, 1.0f, one, n, one, n, 0, series, n);

    for (int i=2; i<m; i++) {
        auto *t = (float*)malloc(n*n*sizeof(float));
        cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, n, n, n, 1.0f, pow, n, r, n, 0.0f, t, n);
        cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, n, n, n, 1.0f, t, n, one, n, 0.0f, pow, n);

        cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, n, n, n, 1.0f, pow, n, one, n, 1.0f, series, n);
    }

    auto *res = (float*)malloc(n*n*sizeof(float));
    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, n, n, n, 1.0f, series, n, b, n, 0.0, res, n);

    auto *check = (float*)malloc(n*n*sizeof(float));
    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, n, n, n, 1.0f, res, n, matrix, n, 0.0, check, n);

    // Clean up
    delete[] matrix;
    delete[] b;
    delete[] transposed;

    return 0;
}