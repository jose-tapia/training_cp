//#include "jarras.h"
#include <iostream>
#include <queue>
#include <vector>
using namespace std;
typedef pair<int, int> pii;
// Problem source: https://omegaup.com/arena/problem/jarras/#problems

const int MaxN = 1001;
pii mov[7];
int memo[MaxN][MaxN];
pii source[MaxN][MaxN];
bool vis[MaxN][MaxN];

void verterAgua(int a, int b);

void rellenarAgua(pii jarra) {
    if (jarra == (pii){0, 0}) {
        // Caso base
        return;
    }
    // Valores precalculados
    int x = jarra.first, y = jarra.second;
    pii prev_jarras = source[x][y];
    int good_mov = memo[x][y];

    // Llenar las jarras al estado 'prev_jarras'
    rellenarAgua(prev_jarras);

    // De 'prev_jarras' pasar a 'jarra'
    verterAgua(mov[good_mov].first, mov[good_mov].second);
}

void programa(int O, int A, int B) {
    // Agregar a las jarras
    mov[1] = {0, 1};
    mov[2] = {0, 2};
    // Tirar las jarras
    mov[3] = {1, 3};
    mov[4] = {2, 3};
    // Mover entre jarras
    mov[5] = {1, 2};
    mov[6] = {2, 1};

    // Inicializar BFS
    queue<pii> bsq;
    bsq.push({0, 0});
    vis[0][0] = true;
    
    // Correr BFS
    while (!bsq.empty()) {
        int p = bsq.front().first;
        int q = bsq.front().second;
        bsq.pop();
        
        if (p == O) {
            // Logramos llegar a tener O litros en la primer jarra
            rellenarAgua({p, q});
            return;
        }
        
        // Movimientos
        vector<pii> mov_jarras = {{A, q}, // Llenar la jarra 1
                                  {p, B}, // Llenar la jarra 2
                                  {0, q}, // Tirar la jarra 1
                                  {p, 0}};// Tirar la jarra 2
        // Vaciar jarra 1 en 2
        if (p + q > B) {
            mov_jarras.push_back({p + q - B, B});
        } else {
            mov_jarras.push_back({0, p + q});
        }
        // Vaciar jarra 2 en 1
        if (p + q > A) {
            mov_jarras.push_back({A, p + q - A});
        } else {
            mov_jarras.push_back({p + q, 0});
        }
        
        for (int i = 0; i < 6; i++) {
            int a = mov_jarras[i].first, b = mov_jarras[i].second;
            if (vis[a][b]) {
                continue;
            }
            vis[a][b] = true;
            // Movimiento que genera {a, b}
            memo[a][b] = i + 1;
            // Jarra donde proviene {a, b}
            source[a][b] = {p, q};
            // Agregar a la BFS
            bsq.push({a, b});
        }
        
    }
}
