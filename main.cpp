#include <iostream>
#include <limits>

using namespace std;

struct Minors {
    float oneMinor;
    float infinityMinor;
};

float generateMatrixAndCountMinor (float* matrix, int n) {
    std::srand(time(NULL));
    float maxMinor = 0;
    for (int i=0; i<n; i++) {
        float minor = 0;
        for (int j=0; j<n; j++){
            matrix[i*n+j] = ((float)rand())/((float)RAND_MAX)*10;
            minor+=matrix[i*n+j];
        }
        if(minor > maxMinor)
            maxMinor = minor;
    }
    return maxMinor;
}


int main() {
    int n,m;
    cin >> n >> m;
    float matrix[n*n];
    float minor = generateMatrixAndCountMinor(matrix, n);
    cout << "minor  is " << minor << "\n";
    for (int i=0; i<n; i++) {
        for (int j = 0; j < n; j++)
            cout << matrix[i*n+j] << " ";
        cout << "\n";
    }
    // Matrix generation and minor ||A||1 counting is correct
    return 0;
}