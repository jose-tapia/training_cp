#include <iostream>
#include <vector>
using namespace std;
// Problem source: https://omegaup.com/arena/problem/Subcadenas-Monotonas/

struct Segment {
    int total;
    int izq_decreciente, izq_creciente;
    int der_decreciente, der_creciente;
};
vector<Segment> Tree;
vector<int> arr;

void unir_segmento(int x, int y, int n) {
    Tree[n].total = Tree[2*n].total + Tree[2*n+1].total;
    
    Tree[n].izq_decreciente = Tree[2*n].izq_decreciente;
    Tree[n].izq_creciente = Tree[2*n].izq_creciente;

    Tree[n].der_decreciente = Tree[2*n+1].der_decreciente;
    Tree[n].der_creciente = Tree[2*n+1].der_creciente;

    int m = (x+y)/2;
    if (arr[m-1] < arr[m]) {
        // Creciente
        // Juntar segmentos entre los dos rangos
        Tree[n].total += Tree[2*n].der_creciente * Tree[2*n+1].izq_creciente;
        
        if (m-x == Tree[2*n].izq_creciente) {
            // Podemos extender mas de la mitad a la derecha
            Tree[n].izq_creciente = (m-x) + Tree[2*n+1].izq_creciente;
        }
        if (y-m == Tree[2*n+1].der_creciente) {
            // Podemos extender mas de la mitad a la izquierda
            Tree[n].der_creciente = Tree[2*n].der_creciente + y-m;
        }
    }
    
    if (arr[m-1] > arr[m]) {
        // Decreciente
        // Juntar segmentos entre los dos rangos
        Tree[n].total += Tree[2*n].der_decreciente * Tree[2*n+1].izq_decreciente;
        if (m-x == Tree[2*n].izq_decreciente) {
            // Podemos extender mas de la mitad a la derecha
            Tree[n].izq_decreciente = m-x + Tree[2*n+1].izq_decreciente;
        }
        if (y-m == Tree[2*n+1].der_decreciente) {
            // Podemos extender mas de la mitad a la izquierda
            Tree[n].der_decreciente = Tree[2*n].der_decreciente + y-m;
        }
    }
}

void build(int x, int y, int n) {
    if (x+1 == y) {
        Tree[n] = {1, 1, 1, 1, 1};
        return;
    }
    build(x, (x+y)/2, 2*n);
    build((x+y)/2, y, 2*n+1);
    unir_segmento(x, y, n);
}
void initialize() {
    Tree.resize(4*arr.size());
    build(0, (int) arr.size(), 1);
}

void _update(int x, int y, int n, int pos, int v) {
    if (x+1 == y) {
        arr[x] = v;
        return;
    }
    if (pos < (x+y)/2) {
        _update(x, (x+y)/2, 2*n, pos, v);
    } else {
        _update((x+y)/2, y, 2*n+1, pos, v);
    }
    unir_segmento(x, y, n);
}

void update(int pos, int v) {
    _update(0, (int) arr.size(), 1, pos, v);
}


int main(){
    cin.tie(0);
    ios_base::sync_with_stdio(0);
    
    int N;
    cin >> N;
    arr.resize(N);
    for (int i = 0; i < N; i++) {
        cin >> arr[i];
    }
    initialize();
    
    int Q;
    cin >> Q;
    for (int i = 0; i < Q; i++) {
        int operacion;
        cin >> operacion;
        if (operacion == 0) {
            cout << Tree[1].total << "\n";
        } else {
            int pos, v;
            cin >> pos >> v;
            update(pos-1, v);
        }
    }
}
