#include <iostream>
using namespace std;
typedef long long int lli;
// Problem source: https://omegaup.com/arena/problem/Pseudo-Attractio/#problems
const int MaxN = 5020;

lli A[MaxN], B[MaxN];
lli dp[2][MaxN];

int main(){
    // Leer entrada
    lli N;
    cin >> N;
    for (int i = 1; i <= N; i++) {
        cin >> A[i];
        B[i] = A[i]; // Copiar arreglo
    }
    // Obtener los valores ordenados
    sort(B+1,B+N+1);
    // La observacion importante es que los valores que
    // al final van a aparecer son los que tiene el arreglo original
    
    // DP (i, v) = minimo costo para que en la posicion 'i' se encuentre el valor 'v'
    // dp[i][v] = min_{j = 1, 2, 3, ..., v} (dp[i-1][j] + abs(A[i] - B[v]))
    // Esto es, la transicion es con la posicion anterior y los valores 'j' menores a 'v'
    for (int i = 1; i <= N; i++) {
        int i_1 = (i-1)%2;
        lli current_min = dp[i_1][1];
        for (int v = 1; v <= N; v++) {
            // Para calcular rapido la transicion, hacemos un barrido 
            current_min = min(dp[i_1][v], current_min);
            dp[i%2][v] = abs(A[i] - B[v]) + current_min;
        }
    }
    
    // Mejor valor posible en la ultima posicion
    lli ans = dp[N%2][1];
    for (int i = 1; i <= N; i++) {
        ans = min(ans, dp[N%2][i]);
    }
    cout << ans << "\n";
}
