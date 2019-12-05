#include <iostream>

using namespace std;

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

void generateOneMatrix(float* matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            matrix[i*n + j] = (i==j);
    }
}

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

void divideToScalar(const float* matrix, float* result, int n, float scalar){
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            result[i*n+j] = matrix[i*n+j]/scalar;
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

int main() {
    int n,m;
    cin >> n >> m;
    float matrix[n*n], one[n*n], transp[n*n], b[n*n], test[n*n];
    float minor = generateMatrixAndCountMinor(matrix, n);
    cout << "minor  is " << minor << "\n";
    for (int i=0; i<n; i++) {
        for (int j = 0; j < n; j++)
            cout << matrix[i*n+j] << " ";
        cout << "\n";
    }
    // Matrix generation and minor ||A||1 counting is correct
    generateOneMatrix(one, n);
    cout << "\n";
    for (int i=0; i<n; i++) {
        for (int j = 0; j < n; j++)
            cout << one[i*n+j] << " ";
        cout << "\n";
    }
    cout << "\n";
    float minor2 = transposeAndCountMinor(matrix, transp, n);
    cout << "minor is " << minor2 << "\n";
    for (int i=0; i<n; i++) {
        for (int j = 0; j < n; j++)
            cout << transp[i*n+j] << " ";
        cout << "\n";
    }
    cout << "\n";
    divideToScalar(matrix, b, n, minor*minor2);
    for (int i=0; i<n; i++) {
        for (int j = 0; j < n; j++)
            cout << b[i*n+j] << " ";
        cout << "\n";
    }
    cout << "\n";

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



    return 0;
}