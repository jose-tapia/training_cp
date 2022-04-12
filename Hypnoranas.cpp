#include <iostream>
#include <queue>
using namespace std;
// Problem source: https://omegaup.com/arena/problem/Hypnoranas/

const int mov[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

const int MaxN = 20;

int N, M;
int mapa[MaxN][MaxN];
int salida_x, salida_y;
// Visitados (6NM)^2
// (posicion de Karel, posicion de Gretel, tiempo de Karel inmovil, tiempo de Gretel inmovil)
bool vis[MaxN][MaxN][MaxN][MaxN][6][6];

struct estado {
    int karel_x, karel_y;
    int gretel_x, gretel_y;
    int karel_inmovil, gretel_inmovil;
    int tiempo;
};

int BFS(estado inicial) {
    queue<estado> bsq;
    bsq.push(inicial);
    while (!bsq.empty()) {
        estado actual = bsq.front();
        bsq.pop();
        
        if (actual.karel_x == salida_x && actual.karel_y == salida_y &&
            actual.gretel_x == salida_x && actual.gretel_y == salida_y) {
            // Logramos llegar
            return actual.tiempo;
        }
        
        for (int i = 0; i < 4; i++) for (int j = 0; j < 4; j++) {
            estado siguiente = actual;
            
            // Aplicamos el movimiento 'i' a Karel
            if (siguiente.karel_inmovil) {
                siguiente.karel_inmovil--;
            } else {
                siguiente.karel_x += mov[i][0];
                siguiente.karel_y += mov[i][1];
                if (siguiente.karel_x < 0 || siguiente.karel_x == N ||
                    siguiente.karel_y < 0 || siguiente.karel_y == M) {
                    // Posicion no valida para Karel
                    continue;
                }
                siguiente.karel_inmovil = mapa[siguiente.karel_x][siguiente.karel_y];
                if (siguiente.karel_inmovil == -1) {
                    // Posicion invalida
                    continue;
                }
            }
            
            // Aplicamos el movimiento 'j' a Gretel
            if (siguiente.gretel_inmovil) {
                siguiente.gretel_inmovil--;
            } else {
                siguiente.gretel_x += mov[j][0];
                siguiente.gretel_y += mov[j][1];
                if (siguiente.gretel_x < 0 || siguiente.gretel_x == N ||
                    siguiente.gretel_y < 0 || siguiente.gretel_y == M) {
                    // Posicion no valida para Gretel
                    continue;
                }
                siguiente.gretel_inmovil = mapa[siguiente.gretel_x][siguiente.gretel_y];
                if (siguiente.gretel_inmovil == -1) {
                    // Posicion invalida
                    continue;
                }
            }
            
            // No este visitado
            if (vis[siguiente.karel_x][siguiente.karel_y][siguiente.gretel_x][siguiente.gretel_y][siguiente.karel_inmovil][siguiente.gretel_inmovil]) {
                continue;
            }
            vis[siguiente.karel_x][siguiente.karel_y][siguiente.gretel_x][siguiente.gretel_y][siguiente.karel_inmovil][siguiente.gretel_inmovil] = true;
            
            siguiente.tiempo++;
            bsq.push(siguiente);
        }
    }
    // No encontramos la salida
    return -1;
}


int main(){
    char posicion;
    estado inicial;
    // Leer entrada
    cin >> N >> M;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            cin >> posicion;
            
            // Adaptar mapa
            if (posicion == '#') {
                mapa[i][j] = -1; // Invalida
            } else if ('0' <= posicion && posicion <= '9') {
                mapa[i][j] = posicion - '0'; // Paralizante
            } else {
                mapa[i][j] = 0; // Sin efecto
            }
            
            // Posiciones especiales
            if (posicion == 'S') {
                // Salida
                salida_x = i;
                salida_y = j;
            }
            if (posicion == 'K') {
                // Posicion inicial de Karel
                inicial.karel_x = i;
                inicial.karel_y = j;
            }
            if (posicion == 'G') {
                // Posicion inicial de Gretel
                inicial.gretel_x = i;
                inicial.gretel_y = j;
            }
        }
    }
    inicial.karel_inmovil = 0;
    inicial.gretel_inmovil = 0;
    inicial.tiempo = 0;
    cout << BFS(inicial) << "\n";
}
