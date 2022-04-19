#include <iostream>
#include <vector>
using namespace std;
typedef long long int lli;
// Problem source: https://codeforces.com/gym/103536/problem/A

const int MaxN = 8010;
//const int MaxK = 3010;

lli prefix_sum[MaxN];
lli costo(int a, int b) {
    return (prefix_sum[b]-prefix_sum[a-1]) * (b-a+1);
}

lli S[MaxN];

bool paridad;
lli dp[MaxN][2];


// Calcula dp[i][k] para x <= i <= y
// De antemano ya sabemos que los valores optimos estaran [L, R]
void realizar_dp(int x, int y, int L_opt, int R_opt) {
    // dp[i][k] = min (dp[j][k-1] + costo(j+1, i))  L <= j <= R
    if (y < x) {
        return;
    }
    
    // Hacemos de la forma clasica a dp[mitad][k]
    int mitad = (x+y)/2;
    int opt_m = 0;
    lli valor_alcanzado = 1e18;
    // Aprovechamos el rango donde debe estar la respuesta [L_opt, R_opt]
    for (int j = L_opt; j <= min(R_opt, mitad-1); j++) {
        lli nueva_transicion = dp[j][paridad^1] + costo(j+1, mitad);
        
        if (nueva_transicion < valor_alcanzado) {
            valor_alcanzado = nueva_transicion;
            opt_m = j;
        }
    }
    // Guardamos la respuesta
    dp[mitad][paridad] = valor_alcanzado;
    
    // Hacer los demas, aprovechando el nuevo pivote
    realizar_dp(x, mitad-1, L_opt, opt_m);
    realizar_dp(mitad+1, y, opt_m, R_opt);
}



int main () {
    cin.tie(0);
    ios_base::sync_with_stdio(0);
    
    
    // Leer entrada
    int N, G;
    cin >> N >> G;
    for (int i = 1; i <= N; i++) {
        cin >> S[i];
        prefix_sum[i] = prefix_sum[i-1] + S[i];
    }
    
    
    // Hacer grupos de 1
    paridad = false;
    for (int i = 1; i <= N; i++) {
        dp[i][paridad] = costo(1, i);
    }

    for (int k = 2; k <= G; k++) {
        // Haga los grupos de 2
        // luego de 3
        // ...
        // hasta hacer los de G
        paridad ^= true;
        realizar_dp(1, N, 1, N); // O(NlogN)
    }
    
    cout << dp[N][paridad] << "\n";
}
