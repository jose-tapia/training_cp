#include <iostream>
using namespace std;
typedef pair<int, int> pii;
// Problem source: https://dmoj.ca/problem/ioi07p4


int convert_food(char shipment) {
    if (shipment == 'F') {
        return 1;
    } else if (shipment == 'M') {
        return 2;
    } else {
        return 3;
    }
}

pii next_state(int curr_state, int new_food) {
    int fst_food = curr_state%4;
    int snd_food = curr_state/4;
    
    int new_state = snd_food + 4 * new_food;
    if (snd_food == 0) {
        // 'new_food' es la primer comida que llega
        return {new_state, 1};
    }
    if (fst_food == 0 || fst_food == new_food) {
        // Solo hay una comida, o bien, la primer comida es igual a la nueva
        return {new_state, 1 + (snd_food != new_food)};
    }
    if (snd_food == new_food) {
        // La comida mas reciente es igual a la nueva
        return {new_state, 1 + (fst_food != new_food)};
    } else {
        // La nueva comida es nueva, el costo depende de los anteriores
        return {new_state, 2 + (fst_food != snd_food)};
    }
}
pii next_states[16][4];

bool vis[2][16][16];
int dp[2][16][16];
int DP(string& shipments) {
    // Caso base
    vis[0][0][0] = true;
    dp[0][0][0] = 0;
    
    int paridad = 1;
    for (char shipment: shipments) {
        int val_shipment = convert_food(shipment);
        
        for (int i = 0; i < 16; i++) {
            for (int j = 0; j < 16; j++) {
                if (0 < i && i < 4) {
                    // No hay comida asignada en el ultimo dia
                    // pero si en el penultimo en la primer mina
                    break;
                }
                if (0 < j && j < 4) {
                    // No hay comida asignada en el ultimo dia
                    // pero si en el penultimo en la segunda mina
                    continue;
                }
                if (!vis[paridad^1][i][j]) continue;
                // Agregar a la primer mina
                pii next_i = next_states[i][val_shipment];
                vis[paridad][next_i.first][j] = true;
                dp[paridad][next_i.first][j] = max(dp[paridad][next_i.first][j], dp[paridad^1][i][j] + next_i.second);
                
                // Agregar a la segunda mina
                pii next_j = next_states[j][val_shipment];
                vis[paridad][i][next_j.first] = true;
                dp[paridad][i][next_j.first] = max(dp[paridad][i][next_j.first], dp[paridad^1][i][j] + next_j.second);
            }
        }
        paridad ^= 1;
    }
    
    int ans = 0;
    for (int i = 0; i < 16; i++) for (int j = 0; j < 16; j++) {
        if (vis[paridad^1][i][j]) {
            ans = max(ans, dp[paridad^1][i][j]);
        }
    }
    return ans;
}

void precalculo_states() {
    for (int i = 0; i < 16; i++) for (int food = 1; food < 4; food++) {
        if (0 < i && i < 4) {
            // No hay comida asignada en el ultimo dia
            // pero si en el penultimo en la primer mina
            continue;
        }
        next_states[i][food] = next_state(i, food);
    }
}

int main(){
    cin.tie(0);
    ios_base::sync_with_stdio(0);
        
    // Leer entrada
    int N;
    string shipments;
    cin >> N >> shipments;
    
    // Precalculo de costos
    precalculo_states();
    
    cout << DP(shipments) << "\n";
}
