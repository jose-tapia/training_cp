#include <iostream>
using namespace std;
typedef long long int lli;
const int MaxN = 2e6 + 10;
// Problem source: https://omegaup.com/arena/problem/Similaridad/#problems

lli sliding_window(string& A, string& B) {
    // Inicializar cubeta
    int cubeta[256];
    fill(cubeta, cubeta+256, 0);
    
    int N = (int) A.size();
    int M = (int) B.size();
    lli ans = 0;
    
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
    return ans;
}

lli sliding_window_per_letter(string& A, string& B) {
    lli in[26], fn[26], cub[26];
    for (int i = 0; i < 26; i++) {
        in[i] = fn[i] = cub[i] = 0;
    }
    int N = (int) A.size();
    int M = (int) B.size();
    
    // Vamos a hacer una estrategia parecida a la del problema de arriba
    // La diferencia es que, en vez de hacer una cubeta general, haremos
    // un sliding-window por cada letra
    
    // Cuando querramos la cantidad de caracteres para la posicion i,
    // actualizaremos el sliding window del caracter A[i]
    
    lli ans = 0;
    for (int i = 0; i < N; i++) {
        int letra = A[i] - 'a';
        // El caracter i puede estar en [i, M-N+i]
        
        // Mover inicio a 'i'
        while (in[letra] < i) {
            if (B[in[letra]] == A[i]) {
                cub[letra]--;
            }
            in[letra]++;
        }
        // Mover fin a 'M-N+i'
        while (fn[letra] <= M-N+i) {
            if (B[fn[letra]] == A[i]) {
                cub[letra]++;
            }
            fn[letra]++;
        }
        // Actualizar respuesta
        ans += cub[letra];
    }
    return ans;
}

int main(){
    cin.tie(0);
    ios_base::sync_with_stdio(0);
    
    // Leer entrada
    string A, B;
    cin >> A >> B;
    
    //sliding_window
    lli ans = sliding_window(A, B);
    // lli ans = sliding_window_per_letter(A, B);
    cout << ans << "\n";
}
