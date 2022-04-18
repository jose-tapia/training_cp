#include <iostream>
#include <vector>
using namespace std;
typedef long long int lli;
typedef pair<int, int> pii;
// Problem source: https://dmoj.ca/problem/ioi16p6
const int MaxN = 1e5;

vector<pii> rango;

lli square(lli x) {
    return x * x;
}

lli costo(int a, int b) {
    // Cuadrado que cubre los puntos 'a', 'a+1', ..., 'b'
    lli cost = square(rango[b].second-rango[a].first + 1);
    if (a == 0) return cost;
    // Residuo de cuadrado que cubria el anterior punto 'a-1'
    lli minus_cost = square(max(0, rango[a-1].second - rango[a].first+1));
    return cost - minus_cost;
}

bool paridad;
lli dp[MaxN][2];

// Calcule dp[i][k] para x <= i <= y
// De antemano ya sabemos que los valores optimos estaran [L, R]
void realizar_dp(int x, int y, int L_opt, int R_opt) {
    // dp[i][k] = min (dp[j][k-1] + costo(j+1, i))  L <= j <= R
    if (y < x) {
        return;
    }
    
    // Hacemos de la forma clasica a dp[mitad][k]
    int mitad = (x+y)/2;
    int opt_m = mitad;
    lli valor_alcanzado = dp[mitad][paridad^1];
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

lli take_photos(int N, int M, int K, int R[], int C[]) {
    // Precalculo para preparar los rangos sin overlap
    vector<pii> uRango;
    for (int i = 0; i < N; i++) {
        uRango.push_back({min(R[i],C[i]), -max(R[i],C[i])});
    }
    sort(uRango.begin(), uRango.end());
    
    int limite = -1;
    for (int i = 0; i < N; i++) {
        if (limite < -uRango[i].second) {
            limite = -uRango[i].second;

            rango.push_back({uRango[i].first, limite});
        }
    }
    
    // Optimizacion generica de DP DyV
        
    // Caso base: Hacer grupos de 1
    paridad = false;
    for (int i = 0; i < (int) rango.size(); i++) {
        dp[i][paridad] = costo(0, i);
    }

    for (int k = 2; k <= K; k++) {
        // Haga los grupos de 2
        // luego de 3
        // ...
        // hasta hacer los de G
        paridad ^= true;
        realizar_dp(0, (int)rango.size()-1, 0, (int)rango.size()-1); // O(NlogN)
    }
    
    // Respuesta
    return dp[(int)rango.size()-1][paridad];
}
