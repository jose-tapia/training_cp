#include <iostream>
#include <vector>
using namespace std;
// Problem source: https://dmoj.ca/problem/ioi16p4

const int MaxN = 2e5 + 10;
const int MaxK = 101;

int K;
vector<int> bloques(1, 0);
int N;
string fixed_positions;

int prefix_negro[MaxN];  // Barrido de bloques negros
int prefix_blanco[MaxN]; // Cantidad de 'blancos' fijos
bool dp[MaxN][MaxK];     // El estado (idx, k) nos dice si usando las primeras
                         //  'idx' posiciones es posible cubrir los primeros 'k'
                         //  bloques
bool vis[MaxN][MaxK];    // Para checar visitados
int posibilidades[MaxN]; // Posibilidades para cada valor

void precalculo_fixed_blancos() {
    for (int i = 0; i < N; i++) {
        prefix_blanco[i+1] = fixed_positions[i] == '_';
        prefix_blanco[i+1] += prefix_blanco[i];
    }
}

inline int cantidad_blanco(int x, int y) {
    return prefix_blanco[y] - prefix_blanco[x-1];
}

bool DP(int pos, int k) {
    if (pos == 0) {
        // Solo es posible
        return k == 0;
    }
    if (vis[pos][k]) {
        // Memoizacion
        return dp[pos][k];
    }
    vis[pos][k] = true;
    dp[pos][k] = false;
    // Intentar poner blanco
    if (fixed_positions[pos-1] != 'X') {
        if (DP(pos-1, k)) {
            // Marcar posibilidad blanco
            posibilidades[pos] = 1;
            dp[pos][k] = true;
        }
    }
    // Intentar poner bloque negro
    if (k > 0 && pos >= bloques[k]) {
        int ini_bloque = pos - bloques[k] + 1;
        if (cantidad_blanco(ini_bloque, pos) == 0) {
            if (ini_bloque == 1 || fixed_positions[ini_bloque-2] != 'X') {
                if (DP(ini_bloque-2 + (ini_bloque == 1), k-1)) {
                    // Marcar bloque posible
                    prefix_negro[ini_bloque]++;
                    prefix_negro[pos+1]--;
                    // Poner '_' antes del bloque
                    posibilidades[ini_bloque-1] = 1;
                    dp[pos][k] = true;
                }
            }
        }
    }
    // Regresar si es posible o no
    return dp[pos][k];
}

string solve_puzzle(string s, int k, int c[]) {
    // Guardar las variables globales
    fixed_positions = s;
    K = k;
    bloques.resize(K+1);
    for (int i = 0; i < K; i++) {
        bloques[i+1] = c[i];
    }

    N = (int) fixed_positions.size();
    precalculo_fixed_blancos();
    DP(N, K);
    // Considerar bloques negros
    for (int i = 1; i <= N; i++) {
        prefix_negro[i] += prefix_negro[i-1];
        if (prefix_negro[i]) {
            posibilidades[i] |= 2;
        }
    }
    // Respuesta
    string ans;
    char posible[4] = {' ', '_', 'X', '?'};
    for (int i = 1; i <= N; i++) {
        ans.push_back(posible[posibilidades[i]]);
    }
    
    return ans;
}
