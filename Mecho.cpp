#include <iostream>
#include <queue>
using namespace std;
typedef pair<int, int> pii;
// Problem source: https://dmoj.ca/problem/ioi09p6

const int MaxN = 801;
const int mov[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

int N, S;
char mapa[MaxN][MaxN];
int inicio_x, inicio_y;

int bee[MaxN][MaxN];
bool vis[MaxN][MaxN];

bool basic_check(int x, int y) {
    if (x < 0 || y <0 || x >= N || y >= N) {
        // Fuera del mapa
        return false;
    }
    if (mapa[x][y] == 'T' || vis[x][y]) {
        // Posiciones no disponibles
        return false;
    }
    return true;
}

bool valid_position(int x, int y, int t) {
    if (!basic_check(x, y)) {
        // No es posicion valida
        return false;
    }
    if (bee[x][y] <= t && mapa[x][y] != 'D') {
        // Abejas atacan
        return false;
    }
    // Posicion valida
    return true;
}

bool possible(int t) {
    if (bee[inicio_x][inicio_y] <= t * S) {
        // Alcanzaron las abejas a Mecho mientras comia
        return false;
    }
    
    // Limpiar visitados
    for (int i = 0; i < N; i++) for (int j = 0; j < N; j++) {
        vis[i][j] = false;
    }
    
    // BFS
    queue<pair<int, pii> > bsq; // (tiempo, posicion)
    bsq.push({t*S, {inicio_x, inicio_y}});
    vis[inicio_x][inicio_y] = true;
    
    while (!bsq.empty()) {
        int tiempo = bsq.front().first;
        pii pos = bsq.front().second;
        bsq.pop();
        
        if (mapa[pos.first][pos.second] == 'D') {
            // Logro llegar al destino
            return true;
        }
        for (int k = 0; k < 4; k++) {
            int a = pos.first + mov[k][0];
            int b = pos.second + mov[k][1];
            if (valid_position(a, b, tiempo + 1)) {
                vis[a][b] = true;
                bsq.push({tiempo + 1, {a, b}});
            }
        }
    }
    // No llego al destino
    return false;
}

int binaria() {
    int inicio = 0, fin = N * N * 2;
    // inicio - 1 = Y
    // inicio = ?
    // fin = X
    int m;
    while (inicio < fin) {
        m = (inicio + fin) / 2;
        if (possible(m)) {
            inicio = m + 1;
        } else {
            fin = m;
        }
    }
    return inicio - 1;
}

void precalcular_bee() {
    // Inicializar BFS
    queue<pii> bsq;
    for (int i = 0; i < N; i++) for (int j = 0; j < N; j++) {
        if (mapa[i][j] == 'H') {
            bsq.push({i, j});
            vis[i][j] = true;
        }
    }
    
    // Correr BFS
    while (!bsq.empty()) {
        int x = bsq.front().first;
        int y = bsq.front().second;
        bsq.pop();
        int t = bee[x][y];
        
        for (int k = 0; k < 4; k++) {
            int a = x + mov[k][0];
            int b = y + mov[k][1];
            if (!basic_check(a, b) || mapa[a][b] == 'D') {
                // No es posicion valida
                // O marca al destino
                continue;
            }
            bee[a][b] = t + S;
            vis[a][b] = true;
            bsq.push({a, b});
        }
    }
}

int main(){
    cin >> N >> S;
    for (int i = 0; i < N; i++) for (int j = 0; j < N; j++) {
        cin >> mapa[i][j];
        if (mapa[i][j] == 'M') {
            inicio_x = i;
            inicio_y = j;
        }
    }
    precalcular_bee();
    cout << binaria() << "\n";
}
