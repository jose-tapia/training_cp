#include <iostream>
using namespace std;
const int MaxN = 2e6 + 10;
// Problem source: https://omegaup.com/arena/problem/Similaridad/#problems

int cubeta[256];

int main(){
    cin.tie(0);
    ios_base::sync_with_stdio(0);
    
    // Leer entrada
    string A, B;
    cin >> A >> B;
    int N = (int) A.size();
    int M = (int) B.size();
    long long int ans = 0;
    
    // Haremos un sliding-window
    // Para cada caracter de A, veremos en que posiciones puede estar en B
    // Con esto, podemos encontrar rapido la cantidad de matchs que aportara
    //   - El primer caracter 0 puede ser [0, 1, ..., M-N] en B
    //   - El segundo caracter 1 puede ser [1, 2, ..., M-N+1] en B
    //   - ..
    //   - ..
    //   - El caracter i puede ser [i, i+1, ..., M-N+i] en B
    //   - ..
    //   - ..
    //   - El caracter N-1 puede ser [N-1, N, ..., M-1] en B
    
    // Contamos manualmente los primeros M-N+1 caracteres
    for (int i = 0; i <= M - N; i++) {
        cubeta[B[i]]++;
    }

    for (int i = 0; i < N; i++) {
        // Cubeta cubre [i, ..., M-N+i]
        ans += cubeta[A[i]];
        
        // Actualizamos el siguiente rango
        if (i < N - 1) {
            // Cubeta debe cubrir [i+1, ..., M-N+i+1]
            cubeta[B[i]]--;
            cubeta[B[M-N+i+1]]++;
        }
    }
    
    // Respuesta
    cout << ans << "\n";
}
