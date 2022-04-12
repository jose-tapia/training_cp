#include <iostream>
#include <vector>
using namespace std;
typedef pair<int, int> pii;
typedef pair<int, pii> piii;
// Problem source: https://omegaup.com/arena/problem/Ethankovich/#problems

const int MaxN = 2e5+10;

// Grafo
vector<int> ady[MaxN];
vector<piii> aristas;

// Union-Find modificado
int sz[MaxN], representante[MaxN];
int suma[MaxN];

pii buscar_representante(int u) {
    // (representante, suma_acarreada)
    if (representante[u] == u) {
        // 'u' se representa a si mismo
        return {u, 0};
    } else {
        pii repre_u = buscar_representante(representante[u]);
        // Aqui aplicamos la optimizacion de tiempo
        representante[u] = repre_u.first;
        // Como nos vamos a esquivar varios nodos, sumamos los valores respectivos que nos faltan
        suma[u] += repre_u.second;
        return {repre_u.first, suma[u]};
    }
}

void agregar_arista(int u, int v, int d) {
    // Al agregar una arista, este tiene dos formas de "atravesar"
    // Dependiendo de como se atraviese es la cantidad que deberiamos de sumar
    // Para ello, idee lo siguiente:
    //   - La premicia es que el representante tenga la cantidad correcta
    //   - Y alguien que no sea representante, al sumarse con el representante, si sea correcta su cantidad
    // Con ello, podemos calcular el numero de manera optima
    int repre_u = buscar_representante(u).first;
    int repre_v = buscar_representante(v).first;
    if (sz[repre_u] < sz[repre_v]) {
        swap(repre_u, repre_v);
        swap(u, v);
    }
    // Cantidad de caminos con capacidad 'd' que irian a todos los que estan en la componente de 'u'
    suma[repre_u] += sz[repre_v] * d;
    // Cantidad de caminos con capacidad 'd' que irian a todos los que estan en la componente de 'v'
    suma[repre_v] += sz[repre_u] * d - suma[repre_u]; // restamos suma['repre_u'] para tener la respuesta correcta al sumar suma[repre_u] y suma[repre_v] si es que preguntamos por 'v'
    sz[repre_u] += sz[repre_v];
    representante[repre_v] = repre_u;
}

int main(){
    cin.tie(0);
    ios_base::sync_with_stdio(0);

    // Leer entrada
    int N;
    cin >> N;
    for (int i = 1; i < N; i++) {
        int u, v, d;
        cin >> u >> v >> d;
        aristas.push_back({d, {u, v}});
        ady[u].push_back(v);
        ady[v].push_back(u);
    }
    sort(aristas.rbegin(), aristas.rend());
    
    // Inicializar union-find
    for (int i = 1; i <= N; i++) {
        sz[i] = 1;
        representante[i] = i;
    }
    
    // Agregar aristas
    for (auto arista: aristas) {
        int u, v, d;
        d = arista.first;
        u = arista.second.first;
        v = arista.second.second;
        agregar_arista(u, v, d);
    }
    
    int ans = 0;
    for (int i = 1; i <= N; i++) {
        int repre_i = buscar_representante(i).first;
        if (i == repre_i) {
            ans = max(ans, suma[i]);
        } else {
            ans = max(ans, suma[i] + suma[repre_i]);
        }
    }
    cout << ans << "\n";
}
