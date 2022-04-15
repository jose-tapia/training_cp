#include <iostream>
#include <vector>
using namespace std;
typedef long long int lli;
typedef pair<int, int> pii;
typedef pair<lli, pii> piii;
// Problem source: https://omegaup.com/arena/problem/don-porfirio/#problems

const int MaxN = 1e5 + 10;
const int MOD = 1e9 + 7;

vector<piii> aristas;
vector<int> repre;
vector<int> sz;

int buscar_repre(int u) {
    if (repre[u] != u) {
        repre[u] = buscar_repre(repre[u]);
    }
    return repre[u];
}

lli unir_componentes(int u, int v, lli peso) {
    int repre_u = buscar_repre(u);
    int repre_v = buscar_repre(v);
    if (repre_u == repre_v) {
        // No hay aportacion (esto no debe suceder porque el grafo es un arbol)
        return 0;
    }
    lli aportacion = (peso * ((lli) sz[repre_u] * sz[repre_v])%MOD)%MOD;
    if (sz[repre_u] < sz[repre_v]) {
        // Mantener sz[repre_u] >= sz[repre_v]
        swap(repre_u, repre_v);
        swap(u, v);
    }
    // Juntar componente de 'v' a 'u'
    repre[repre_v] = repre_u;
    sz[repre_u] += sz[repre_v];
    // Regresar aportacion
    return aportacion;
}

int main(){
    cin.tie(0);
    ios_base::sync_with_stdio(0);
    
    // Leer entrada
    int N;
    cin >> N;
    for (int i = 1; i < N; i++) {
        int u, v;
        lli peso;
        cin >> u >> v >> peso;
        aristas.push_back({peso, {u, v}});
    }
    
    // Inicializar union-find
    for (int i = 0; i <= N; i++) {
        repre.push_back(i);
        sz.push_back(1);
    }
    
    // Ordenar aristas por peso (menor a mayor)
    sort(aristas.begin(), aristas.end());
    
    // Unimos aristas y contamos la cantidad de caminos + sus bellezas
    lli ans = 0;
    for (piii arista: aristas) {
        ans += unir_componentes(arista.second.first, arista.second.second, arista.first);
        ans %= MOD;
    }
    // Imprimir respuesta
    cout << ans << "\n";
}
