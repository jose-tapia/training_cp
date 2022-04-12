#include <iostream>
#include <vector>
using namespace std;
typedef long long int lli;
typedef pair<int, int> pii;
// Problem source: https://omegaup.com/arena/problem/Arboles-Chistosos/#problems

const int MaxN=2e5+10;

int N, M;
lli color[MaxN];       // Color de cada nodo
vector<int> ady[MaxN]; // Grafo
int altura[MaxN];      // Profundidad desde '1' a 'nodo'

// Recorrido pre-orden (primero visita el 'nodo' luego sus hijos)
pii rango[MaxN];       // Rango que cubre el nodo
int pos_arr[MaxN];     // Posicion del nodo en el arreglo


int dfs_preorden(int u, int prev_u, int num_nodos) {
    // DFS por 'u'
    altura[u] = altura[prev_u] + 1; // Guardar profundidad
    rango[u].first = num_nodos;     // Inicio del rango
    pos_arr[u] = num_nodos;         // Posicion en el arreglo
    
    num_nodos++;
    for (int v: ady[u]) {
        if (v == prev_u) {
            continue;
        }
        num_nodos = dfs_preorden(v, u, num_nodos);
    }
    rango[u].second = num_nodos;    // Fin del rango
    // Cubrimos [fst, snd)
    return num_nodos;               // Fin para ancestros
}


struct BIT {
    vector<lli> bit;
    // Initializer
    BIT(int sz = MaxN) {
        bit.resize(sz);
    }
    // Update
    void update(int pos, lli val) {
        for (pos++; pos < bit.size(); pos += (pos&-pos)) {
            bit[pos] += val;
        }
    }
    // Query
    lli query(int pos) {
        lli sum = 0;
        for (pos++; pos > 0; pos -= pos&(-pos)) {
            sum += bit[pos];
        }
        return sum;
    }
}bit[2];


lli query(int n){
    // Contar la cantidad
    int paridad = altura[n]%2;
    lli query_ans = 0;
    query_ans += bit[paridad].query(pos_arr[n]);
    query_ans -= bit[paridad^1].query(pos_arr[n]);
    return query_ans + color[n];
}
void update(int n, lli v) {
    // Agregar a cada posicion del rango [fst, snd) el valor 'v'
    int paridad = altura[n]%2;
    bit[paridad].update(rango[n].first, v);
    bit[paridad].update(rango[n].second, -v);
}

int main(){
    cin.tie(0);
    ios_base::sync_with_stdio(0);
    
    // Leer entrada
    cin >> N >> M;
    for (int i = 1; i <= N; i++) {
        cin >> color[i];
    }
    for (int i = 1; i < N; i++) {
        int p, q;
        cin >> p >> q;
        ady[p].push_back(q);
        ady[q].push_back(p);
    }
    
    // Realizar pre-orden
    dfs_preorden(1, 0, 0);
    
    // Contestar preguntas
    int operacion, nodo;
    lli val;
    for (int i = 0; i < M; i++) {
        cin >> operacion;
        if (operacion == 1) {
            // Update
            cin >> nodo >> val;
            update(nodo, val);
        } else {
            // Query
            cin >> nodo;
            cout << query(nodo) << "\n";
        }
    }
}
