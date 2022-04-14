#include <iostream>
#include <vector>
using namespace std;
typedef pair<int, int> pii;
// Problem source: https://omegaup.com/arena/problem/Lluvia/#problems

const int MaxN = 1e5+10;

struct UnionFind {
    vector<int> representante;
    vector<int> sz;
    UnionFind(int N = MaxN) {
        for (int i = 0; i <= N; i++) {
            representante.push_back(i);
            sz.push_back(1);
        }
    }
    int buscar_repre(int u) {
        if (representante[u] != u) {
            representante[u] = buscar_repre(representante[u]);
        }
        return representante[u];
    }
    bool unir_componentes(int u, int v) {
        int repre_u = buscar_repre(u);
        int repre_v = buscar_repre(v);
        if (repre_u == repre_v) {
            // Ya estaban en la misma componente
            return false;
        }
        if (sz[repre_u] < sz[repre_v]) {
            // Siempre sz[repre_u] >= sz[repre_v]
            swap(repre_u, repre_v);
        }
        // Unir comp_v a comp_u
        representante[repre_v] = repre_u;
        sz[repre_u] += sz[repre_v];
        // Si se unieron las componentes
        return true;
    }
};

// Subgrafo
int N;
vector<int> ady[MaxN];
bool visitado[MaxN];
// Puentes del grafo
vector<pii> puentes;

int curr_num_nodo;
int bloque[MaxN];
void DFS_tarjan(int u, int prev_u) {
    visitado[u] = true;          // Marcamos como visitado
    int valor_u = curr_num_nodo; // Asignamos un valor a 'u'
    bloque[u] = curr_num_nodo++; // El menor valor que hemos visitado por ahora
    int cnt_prev_u = 0;          // Cantidad de aristas (u, prev_u)
    for (int v: ady[u]) {
        if (prev_u == v) {
            cnt_prev_u++;
            if (cnt_prev_u > 1) {
                // Actualizamos el valor del bloque porque
                // podriamos regresar a 'prev_u'
                bloque[u] = min(bloque[u], bloque[v]);
            }
            continue;
        }
        if (!visitado[v]) {
            // Visitamos
            DFS_tarjan(v, u);
        }
        // El numero mas 'chico' que pueden visitar
        bloque[u] = min(bloque[u], bloque[v]);
        if (valor_u < bloque[v]) {
            // Significa que visito una zona sin explorar
            // por lo que '(u, v)' es un puente
            puentes.push_back({min(u, v), max(u, v)});
        }
    }
}

int main() {
    cin.tie(0);
    ios_base::sync_with_stdio(0);
    
    int M;
    cin >> N >> M;
    // No consideraremos todas las aristas M, solo las necesarias
    //  - Vamos a considerar aristas que conecten componentes
    //    distintas (en el union find de 'arbol')
    //  - Si una arista forma un ciclo de 'arbol', puede que nos sirva
    //    para diferenciar 'puentes' y 'no puentes', por eso la agregamos
    //    a 'ciclos'
    //  - Aquellas aristas que tambien hagan ciclo en 'ciclos' estan de
    //    mas (ya nos sobrarian ciclos) y no las consideramos
    UnionFind arbol(N);
    UnionFind ciclos(N);
    
    for (int i = 0; i < M; i++) {
        int u, v;
        cin >> u >> v;
        if (arbol.unir_componentes(u, v)) {
            // Arista de arbol
            ady[u].push_back(v);
            ady[v].push_back(u);
        } else if (ciclos.unir_componentes(u, v)) {
            // Podemos usar (u, v) para hacer ciclos
            ady[u].push_back(v);
            ady[v].push_back(u);
        }
    }

    // Tarjan's algorithm
    for (int i = 1; i <= N; i++) {
        if (!visitado[i]) {
            // Hay que visitar todas las componentes
            DFS_tarjan(i, 0);
        }
    }
    // Imprimir respuesta
    sort(puentes.begin(), puentes.end());
    for (pii puente: puentes) {
        cout << puente.first << " " << puente.second << "\n";
    }
}
