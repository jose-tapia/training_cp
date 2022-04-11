#include <iostream>
#include <queue>
using namespace std;
// Problem source: https://omegaup.com/arena/problem/Portal/#problems
typedef pair<int,int> pii;

const int MaxN=11;

// Mapa
int N, M;
char mapa[MaxN][MaxN];
int inicio_x, inicio_y;
int fin_x, fin_y;


int mov[4][2] = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}}; // Movimientos N E S O

pii lookWall[MaxN][MaxN][4];// Pared mas cercana hacia dicha direccion
int distWall[MaxN][MaxN];   // Minima distancia a la pared
bool vis[MaxN][MaxN];       // Visitados en BFS
int dijkstra[MaxN][MaxN];   // Valores alcanzados

bool valid_position(int x, int y) {
    return 0 <= x && x < N && 0 <= y && y < M && !vis[x][y] && mapa[x][y] != '#';
}

void precalculo_distWall() {
    // Iniciar en las paredes con distancia 0
    queue<pii> bsq;
    for (int i = 0; i < N; i++) for (int j = 0; j < M; j++) {
        if (mapa[i][j] == '#') {
            vis[i][j] = true;
            bsq.push({i, j});
        }
    }
    
    // Visitar resto del mapa
    while(!bsq.empty()){
        pii pos = bsq.front();
        bsq.pop();
        for (int k = 0; k < 4; k++) {
            int a = pos.first + mov[k][0];
            int b = pos.second + mov[k][1];
            if (!valid_position(a, b)) {
                continue;
            }
            // Visitar
            vis[a][b] = true;
            distWall[a][b] = distWall[pos.first][pos.second] + 1;
            bsq.push({a, b});
        }
    }
}

void precalculo_lookWall() {
    // N E
    for (int i = 0; i < N; i++) for (int j = 0; j < M; j++) {
        if (mapa[i][j] == '#') {
            for (int k = 0; k < 2; k++) {
                lookWall[i][j][k] = {i-mov[k][0], j-mov[k][1]};
            }
        } else {
            for (int k = 0; k < 2; k++) {
                lookWall[i][j][k] = lookWall[i+mov[k][0]][j+mov[k][1]][k];
            }
        }
    }
    // S O
    for (int i = N - 1; i >= 0; i--) for (int j = M - 1; j >= 0; j--) {
        if (mapa[i][j] == '#') {
            for (int k = 2; k < 4; k++) {
                lookWall[i][j][k] = {i-mov[k][0], j-mov[k][1]};
            }
        } else {
            for (int k = 2; k < 4; k++) {
                lookWall[i][j][k] = lookWall[i+mov[k][0]][j+mov[k][1]][k];
            }
        }
    }
}

int dijkstra_cake() {
    // Limpiar matriz de visitados
    for (int i = 0; i < N; i++) for (int j = 0; j < M; j++) {
        vis[i][j] = false;
    }
    
    // Inicializar
    priority_queue<pair<int, pii> > bsq;
    bsq.push({0, {inicio_x, inicio_y}});

    while(!bsq.empty()){
        int costo = -bsq.top().first;
        pii pos = bsq.top().second;
        bsq.pop();
        
        if (vis[pos.first][pos.second]) {
            // Ya fue visitado
            continue;
        }
        if (pos == (pii){fin_x, fin_y}) {
            // Llegamos
            return costo;
        }
        // Visitar
        vis[pos.first][pos.second] = true;
        
        dijkstra[pos.first][pos.second] = costo;

        // Caminar normal
        int costo_normal = costo + 1;
        for (int k = 0; k < 4; k++) {
            int a = pos.first + mov[k][0];
            int b = pos.second + mov[k][1];
            if (!valid_position(a, b)) {
                continue;
            }
            bsq.push({-costo_normal, {a, b}});
        }
        
        // Usar portal
        int costo_portal = costo + distWall[pos.first][pos.second];
        for (int k = 0; k < 4; k++) {
            int a = lookWall[pos.first][pos.second][k].first;
            int b = lookWall[pos.first][pos.second][k].second;
            if (!valid_position(a, b)) {
                continue;
            }
            bsq.push({-costo_portal, {a, b}});
            
        }
    }
    // No alcanzamos a llegar al cake
    return -1;
}



int main(){
    cin.tie(0);
    ios_base::sync_with_stdio(0);

    // Lectura
    cin >> N >> M;
    for (int i = 0; i < N; i++) for (int j = 0; j < M; j++) {
        cin >> mapa[i][j];
        if (mapa[i][j] == 'O') {
            inicio_x = i;
            inicio_y = j;
        } else if (mapa[i][j] == 'X') {
            fin_x = i;
            fin_y = j;
        }
    }
    
    // Calcular la distancia a la pared mas cercana
    precalculo_distWall();
    // Guardar la pared mas cercana a cada posicion
    precalculo_lookWall();

    // Dijkstra
    int ans = dijkstra_cake();
    if (ans == -1) {
        cout << "THE CAKE IS A LIE\n";
    } else {
        cout << ans << "\n";
    }
}
