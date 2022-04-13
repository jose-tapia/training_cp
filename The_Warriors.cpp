#include <iostream>
#include <vector>
using namespace std;
typedef pair<int, int> pii;
typedef long long int lli;
// Problem source: https://codeforces.com/gym/272878/problem/L

const int MaxN = 1001;
const int MaxK = 1001;

int N, K;
int is_warrior[MaxN];
lli costo[MaxN][MaxN];
lli dp[MaxK][MaxN];

void precalculo_costo() {
    // Precalculo de prefijos para obtener suma de rangos
    int acum[N+1];
    acum[0] = 0;
    for (int i = 1; i <= N; i++) {
        acum[i] = acum[i-1] + is_warrior[i];
    }
    for (int i = 1; i <= N; i++) {
        for (int j = i; j <= N; j++) {
            // Calcular el costo de todos los subrangos
            int cnt_warriors = acum[j] - acum[i-1];
            int cnt_citizens = (j-i+1) - cnt_warriors;
            costo[i][j] = cnt_warriors * cnt_citizens;
        }
    }
}

void realizar_dp(int k, int idx_L, int idx_R, int possible_L, int possible_R){
    if (idx_R < idx_L) {
        // No hay indices por calcular
        return;
    }
    
    // Encontrar el valor optimo para el punto medio de [idx_L, idx_R]
    int mit = (idx_L + idx_R) / 2;
    lli costo_mit = (1 << 30);
    int opt_mit = mit;
    // Solo buscamos en el rango de indices posibles [possible_L, possible_R]
    for (int i = possible_L; i <= min(possible_R, mit-1); i++) {
        // El 'k' tren tendra a las personas [i+1, mit]
        lli costo_temporal = dp[k-1][i] + costo[i+1][mit];
        if (costo_temporal < costo_mit) {
            costo_mit = costo_temporal;
            opt_mit = i;
        }
    }
    
    // Guardar mejor respuesta para 'mit'
    dp[k][mit] = costo_mit;
    // Realizar dp para el resto de indices
    realizar_dp(k, idx_L, mit-1, possible_L, opt_mit);
    realizar_dp(k, mit+1, idx_R, opt_mit, possible_R);
}
int main() {
    // Lectura de entrada
    cin >> N >> K;
    for (int i = 1; i <= N; i++) {
        cin >> is_warrior[i];
    }
    // Realizar precalculo
    precalculo_costo();
    
    // Consideremos la siguiente DP:
    //   dp[k][n] = 'minimo costo de acomodar las primeras n personas en k trenes'
    //   dp[k][n] = min_{j=k, k+1, ..., n} (dp[k-1][j-1] + costo[j][n])
    // Esta DP tiene complejidad O(N^2K)
    //
    // Optimizamos la DP usando la estrategia de Divide y Venceras
    // bajando la complejidad a O(NKlogN)
 
    // Caso base (k=1)
    for (int i = 1; i <= N; i++) {
        dp[1][i] = costo[1][i];
    }
    
    // Iterar por cada k = 2, 3, ..., K (Agregar un tren por iteracion)
    for (int k = 2; k <= K; k++) {
        realizar_dp(k, 1, N, 0, N - 1);
    }
    // Respuesta
    cout << dp[K][N] << "\n";
}
