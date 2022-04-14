#include <iostream>
#include <vector>
using namespace std;
typedef pair<int, int> pii;
// Problem source: https://omegaup.com/arena/problem/llaves/#problems

// La principal estrategia para resolver este problema es la siguiente:
//   - Para cada posicion (i), encontrar el mayor indice (j) tal que [i, j]
//     sean llaves unicas
//   - De esta forma, cada pregunta se podria responder en constante
//
// El vector 'limits' guardaremos el valor de 'j' para cada 'i' (j = limits[i])
// Vamos a analizar como conseguir cada valor 'j':
//   - Observemos que hay que fijarnos en las posiciones de cada valor posible
//   - Para un valor V, consideremos sus posiciones (idx_1, idx_2, ..., idx_n)
//   - Las posiciones en el rango [idx_(i-1)+1, idx_i] pueden llegar hasta
//     idx_(i+1)-1
//   - Para una posicion 'i', hay que ver en que rangos pertenece y ver el limite
//     mas estricto
//
// Esta situacion la podemos visualizar como el siguiente problema
//   Tenemos un arreglo de N numeros
//   Hay dos tipos de queries:
//     1. Aplicar la funcion 'min' en un rango [A, B] con el valor 'v'
//     2. Preguntar por el valor en la posicion 'pos'
//
// Podemos resolver el problema con un Segment Tree con Lazy Propagation
// Si ignoramos la query 2, podemos aplicar un barrido

int N;
vector<int> limits;
vector<pii> arr;

void precalculo_barrido() {
    fill(limits.begin(), limits.end(), N);
    for (int i = 0; i+1 < N; i++) {
        if (arr[i].first == arr[i+1].first) {
            limits[arr[i].second] = arr[i+1].second;
        }
    }
    for (int i = N-2; i>= 0; i--) {
        limits[i] = min(limits[i], limits[i+1]);
    }
}

vector<int> Tree;

void apply_lazy(int x, int y, int n) {
    if (x+1 == y) {
        return;
    }
    Tree[2*n] = min(Tree[2*n], Tree[n]);
    Tree[2*n+1] = min(Tree[2*n+1], Tree[n]);
}

void _update(int x, int y, int n, int A, int B, int v) {
    apply_lazy(x, y, n);
    if (A <= x && y <= B) {
        // Cubrimos todo el rango
        Tree[n] = min(v, Tree[n]);
        return;
    }
    if (y <= A || B <= x) {
        // No cubrimos el rango
        return;
    }
    _update(x, (x+y)/2, 2*n, A, B, v);
    _update((x+y)/2, y, 2*n+1, A, B, v);
}

void update(int A, int B, int v) {
    _update(0, N, 1, A, B, v);
}

void fill_array(int x, int y, int n) {
    // IMPORTANTE: Siempre aplicar el lazy
    apply_lazy(x, y, n);
    if (x+1 == y) {
        limits[x] = Tree[n];
        return;
    }
    fill_array(x, (x+y)/2, 2*n);
    fill_array((x+y)/2, y, 2*n+1);
}

void precalculo_STLazy() {
    Tree.resize(4*N);
    fill(Tree.begin(), Tree.end(), N);
    for (int i = 0; i+1 < N; i++) {
        if (arr[i].first == arr[i+1].first) {
            int A = 0, B = arr[i].second+1;
            if (i > 0 && arr[i-1].first == arr[i].first) {
                A = arr[i-1].second + 1;
            }
            update(A, B, arr[i+1].second);
        }
    }
    fill_array(0, N, 1);
}

int main() {
    cin.tie(0);
    iostream::sync_with_stdio(0);
    
    // Leer entrada
    cin >> N;
    limits.resize(N);
    arr.resize(N);
    
    for (int i = 0; i < N; i++) {
        cin >> arr[i].first;
        arr[i].second = i;
    }
    sort(arr.begin(), arr.end());
    
    // Usar el barrido O(N)
    precalculo_barrido();

    // Usar ST con Lazy O(NlogN)
    // precalculo_STLazy();
    
    int Q;
    cin >> Q;
    for (int i = 0; i < Q; i++) {
        int A, B;
        cin >> A >> B;
        cout << (limits[A] <= B) << "\n";
    }

}
