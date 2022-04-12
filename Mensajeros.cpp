#include <iostream>
#include <vector>
using namespace std;
typedef long long int lli;
typedef pair<int, lli> pii;
// Problem source: https://omegaup.com/arena/problem/mensajeros/#problems

const int MaxN = 1e5 + 10;

int N;
// Lista de adyacencia con pesos
vector<pii> ady[MaxN];
// Constantes por ciudad
lli S[MaxN];    // Tiempo de preparacion
lli V[MaxN];    // Velocidad de mensajero
lli Dist[MaxN]; // Distancia a la central(1)
// Lista de adyacencia sin ciclos
vector<int> adj_dfs[MaxN];
// Respuesta para cada ciudad
lli ans[MaxN];

// Estructura para el ConvexHull Trick
struct line {
    lli m;    // Pendiente
    lli c;    // Constante
    int id;   // Identificador
    double x; // x optimo (Conviene usar esta recta para 'v' tal que x <= v
};

double interseccion(line a, line b) {
    // Supongamos que coinciden en el valor de 'x'
    // Entonces y = a.m * x + a.c
    //          y = b.m * x + b.c
    // Igualamos las ecuaciones y tendriamos que
    //     a.m * x + a.c = b.m * x + b.c
    //   (a.m - b.m) * x = b.c - a.c
    //                 x = (b.c - a.c) / (a.m - b.m)
    // Entonces, deben intersectar en (b.c - a.c) / (a.m - b.m)
    return (double (a.c-b.c+0.0)/(b.m-a.m));
}

// Implementacion sobre un arreglo para soportar stack persistente
int sz_stack;
line pila[MaxN];

int best_line(lli x) { // O(logN)
    // Encontrar la linea que cubre el valor de 'x'

    // Rango en que vamos a buscar
    int in = 0, fn = sz_stack;
    int m;
    // in es 'valido'     -> pila[0].x = 0 <= x
    // fn es 'invalido'   -> pila[fn].x = inf > x
    while (in + 1 < fn) {
        m = (in+fn)/2;
        if (pila[m].x <= x) {
            // [in, in+1, ..., m] cubren a 'x'
            in = m;
        } else {
            // [m, m+1, ..., fn] no cubren a 'x'
            fn = m;
        }
    }
    // pila[in].x <= x < pila[fn].x
    return pila[in].id;
}

int find_position(line new_line) { // O(logN)
    // Encontrar la posicion en la que debe estar la nueva linea
    // Una observacion importante:
    //   * Todos los ancestros estan mas cercas a 1 (Dist[v] < Dist[u])
    //   * Por lo tanto, la pendiente -Dist[u] es conveniente para valores grandes de 'x'
    //   * Hay que buscar desde que posicion empieza a ganar 'u'
    
    
    // Rango en que vamos a buscar
    int in = 0, fn = sz_stack;
    int m;
    double x;
    while (in < fn) {
        m = (in+fn)/2;
        // Valor donde new_line es conveniente
        x = interseccion(new_line, pila[m]);
        if (pila[m].x < x) {
            // [in, in+1, ..., m] convienen mas que 'new_line' para valores anteriores
            in = m + 1;
        } else {
            // [m, m+1, ..., fn] ya no convienen, puesto 'new_line' puede suplantar
            fn = m;
        }
    }
    // 'in-1' es la ultima posicion conveniente
    // 'in' es donde debe ir 'new_line'
    return in;
}

// Precalculo de distancias
void DFS_distancias(lli nodo, lli anterior) {
    // Recorremos todo el arbol para calcular Dist[] y adj_dfs[]
    for (pii arista: ady[nodo]) {
        int sig_nodo = arista.first;
        lli sig_dist = arista.second;
        if (sig_nodo == anterior) continue;
        
        Dist[sig_nodo] = Dist[nodo] + sig_dist;
        DFS_distancias(sig_nodo, nodo);
    }
}

// Solucion O(N^2)
void DP_cuadrada(int u, int prev_u, vector<int>& ancestros) {
    // Justificacion de esta dp en 'DP'
    ans[u] = 0;
    for (int v: ancestros) {
        // Comparar con ancestros
        ans[u] = min(ans[u], V[u] * (-Dist[v]) + ans[v]);
    }
    ans[u] += S[u] + V[u] * Dist[u]; // Sumar constantes
    
    ancestros.push_back(u);
    for (int v: adj_dfs[u]) {
        if (v == prev_u) continue;
        DP_cuadrada(v, u, ancestros);
    }
    ancestros.pop_back();
}

// Solucion O(NlogN)
void DP(int u, int prev_u){
    // Analicemos la situacion para una linea:
    //  - De una ciudad 'u', sin hacer paradas, llegamos a la central en
    //       S[u] + V[u] * Dist[u]
    //  - Si la ciudad 'u' hace una parada en la ciudad 'v', entonces, el costo seria
    //       (S[u] + V[u] * (Dist[u] - Dist[v])) + (S[v] + V[v] * Dist[v])
    //
    //  - Observemos que podemos hacer la siguiente DP:
    //      * Definicion: dp[u] = 'Tiempo optimo para llegar a la central'
    //      * Transicion: dp[u] = min_{v=1,2,3,..., u-1}(S[u] + V[u] * (Dist[u] - Dist[v]) + dp[v])
    //      * Caso base:  dp[1] = 0 (ya llego a la central)
    //  - Calcular dicha DP la podemos ahcer en O(N^2) (Observemos la implementacion en DP_cuadrada)
    //
    // Para visualizar mejor nuestra funcion objetivo, vamos a organizar las variables:
    //    S[u] + V[u] * (Dist[u] - Dist[v]) + dp[v]
    //    S[u] + V[u] * Dist[u] - V[u] * Dist[v] + dp[v]
    //
    //   Al aplicar el minimo, observemos que no importa cual sea el valor de 'v',
    //   siempre estara S[u] + V[u] * Dist[u]. Por lo que podemos "sacarlo" del minimo y sumarlo al final
    //   La transicion entonces quedaria como:
    //    * dp[u] = S[u] + V[u] * Dist[u] + min_{v=1,2,3,..., u-1}(V[u] * (-Dist[v]) + dp[v])
    //
    //   Entonces, queremos optimizar
    //       V[u] * (-Dist[v]) + dp[v]
    //   Aqui ya podemos aplicar la optimizacion de DP 'Convexhull Trick'
    //
    //   Una dificultad mas es que debemos de guardar el stack de las lineas: en este problema tenemos que aplicar un stack persistente
    
    if (u != 1) {
        // Optimizar (V[u] * (-Dist[v]) + dp[v])
        int v = best_line(V[u]);
        // Calcular dp[u]
        ans[u] = S[u] + V[u] * (Dist[u] - Dist[v]) + ans[v];
    }
    
    // Variables para guardar persistencia
    int prev_sz_stack;
    line prev_city;
    
    if (u == 1) {
        // Agregamos al stack
        prev_sz_stack = sz_stack++;
        pila[0] = line({0, 0, 1, 0});
    }
    else {
        // Estructura de la linea: m*x + c
        //  * m = -Dist[u]
        //  * c = ans[u]
        //  * id = u
        //  * x = ?
        
        // Crear linea
        line new_city = line({-Dist[u], ans[u], u, 0});
        // Encontrar posicion
        int new_pos = find_position(new_city);
        // Asignar rango de 'x'
        new_city.x = interseccion(new_city, pila[new_pos - 1]);
        
        // Guardar valor previo
        prev_city = pila[new_pos];
        prev_sz_stack = sz_stack;
        
        // Actualizar stack
        pila[new_pos] = new_city;
        sz_stack = new_pos + 1;
    }
    // Visitar childs
    for (int v: adj_dfs[u]) {
        if (v == prev_u) continue;
        DP(v, u);
    }
    // Regresar a valor previo (backtracking) ~ Persistencia
    pila[sz_stack-1] = prev_city;
    sz_stack = prev_sz_stack;
}

int main(){
    cin.tie(0);
    ios_base::sync_with_stdio(0);

    // Lectura de entrada
    cin >> N;
    for (int i = 1; i < N; i++) {
        int u, v, d;
        cin >> u >> v >> d;
        // Con pesos
        ady[u].push_back({v, d});
        ady[v].push_back({u, d});
        // Sin pesos
        adj_dfs[u].push_back(v);
        adj_dfs[v].push_back(u);
    }
    for (int i = 2; i <= N; i++) {
        cin >> S[i] >> V[i];
    }
    
    // Precalcular distancia de la central a las ciudades
    DFS_distancias(1, 0);

    // Calcular la DP para cada valor del arbol
    // vector<int> ancestros;
    // DP_cuadrada(1, 0, ancestros); //O(N^2)
    DP(1, 0); // O(NlogN)
    
    // Imprimir respuestas
    for (int i = 2; i <= N; i++) {
        cout << ans[i] << " ";
    }
    cout << "\n";
}
