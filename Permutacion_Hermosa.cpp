#include <iostream>
using namespace std;
// Problem source: https://omegaup.com/arena/problem/Permutacion-Hermosa/#problems

int main(){
    int N, K;
    cin >> N >> K;
    if (N <= K) {
        // No se puede (faltan numeros o el '1' no sera bueno)
        // Hay a lo mucho N-1 numeros buenos
        cout << "-1\n";
        return 0;
    }
    
    int diff = N - K;
    // Hacer shift a los primeros 'N-K' numeros
    for (int i = 1; i < diff; i++) {
        cout << i+1 << " ";
    }
    cout << "1 ";
    
    // Los ultimos K numeros conservan su posicion
    for (int i = diff+1; i <= N; i++) {
        cout << i << " ";
    }
    return 0;
}
