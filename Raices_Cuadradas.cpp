#include <iostream>
#include <vector>
using namespace std;
typedef long long int lli;
// Problem source: https://omegaup.com/arena/problem/Raices-Cuadradas/

const int MaxN = 2e5+10;
const int MaxRaiz = 6;

lli sqrt_int(lli num) {
    // Calculamos la raiz con una binaria
    lli in = 1, fn = num, m;
    while (in+1 < fn) {
        m = (in+fn)/2;
        if (m*m <= num) {
            in = m;
        } else {
            fn = m;
        }
    }
    return in;
}

struct Segment {
    int lazy_level;
    lli sum_raiz[MaxRaiz];
};

vector<int> arr;
Segment Tree[MaxN*4];

void build(int x, int y, int n) {
    if (x+1 == y) {
        // Calcular las siguinetes MaxRaiz (8) raices
        Tree[n].lazy_level = 0;       // No hay raices pendientes
        Tree[n].sum_raiz[0] = arr[x]; // Numero base
        for (int r = 1; r < MaxRaiz; r++) {
            Tree[n].sum_raiz[r] = sqrt_int(Tree[n].sum_raiz[r-1]);
        }
        return;
    }
    build(x, (x+y)/2, 2*n);
    build((x+y)/2, y, 2*n+1);
    // Unir las sumas
    Tree[n].lazy_level = 0; // No hay raices pendientes
    // Actualizamos rango [x, y)
    for (int r = 0; r < MaxRaiz; r++) {
        Tree[n].sum_raiz[r] = Tree[2*n].sum_raiz[r] + Tree[2*n+1].sum_raiz[r];
    }
}

void apply_lazy(int x, int y, int n) {
    if (Tree[n].lazy_level == 0) {
        // No hay raices pendientes
        return;
    }
    // Hacemos shift de las raices (aplicamos 'lazy_level' raices)
    for (int r = 0; r < MaxRaiz; r++) {
        int shift_r = r + Tree[n].lazy_level;
        if (shift_r < MaxRaiz) {
            Tree[n].sum_raiz[r] = Tree[n].sum_raiz[shift_r];
        } else {
            // Aplicamos muchas veces la raiz, todos los numeros seran 1
            Tree[n].sum_raiz[r] = y - x;
        }
    }
    if (x+1 == y) {
        // El segmento es una hoja del ST
        // No es necesario propagarse
        Tree[n].lazy_level = 0;
        return;
    }
    // Propagar cantidad de raices
    Tree[2*n].lazy_level += Tree[n].lazy_level;
    Tree[2*n+1].lazy_level += Tree[n].lazy_level;
    Tree[n].lazy_level = 0;
}

void _update_raiz(int x, int y, int n, int I, int D) {
    // IMPORTANTE: Aplicar siempre el lazy
    apply_lazy(x, y, n);
    if (I <= x && y <= D) {
        // Aplicar raiz al segmento
        Tree[n].lazy_level++;
        apply_lazy(x, y, n);
        return;
    }
    if (y <= I || D <= x) {
        // Fuera de rango
        return;
    }
    _update_raiz(x, (x+y)/2, 2*n, I, D);
    _update_raiz((x+y)/2, y, 2*n+1, I, D);
    // Actualizar segmento [x, y)
    for (int r = 0; r < MaxRaiz; r++) {
        Tree[n].sum_raiz[r] = Tree[2*n].sum_raiz[r] + Tree[2*n+1].sum_raiz[r];
    }
}

void _update_valor(int x, int y, int n, int pos, int v) {
    // IMPORTANTE: Aplicar siempre el lazy
    apply_lazy(x, y, n);
    if (pos < x || y <= pos) {
        // No estamos cubriendo a 'pos'
        return;
    }
    if (x+1 == y) {
        arr[x] = v;
        Tree[n].lazy_level = 0;  // No hay raices pendientes
        Tree[n].sum_raiz[0] = v; // Numero base
        for (int r = 1; r < MaxRaiz; r++) {
            Tree[n].sum_raiz[r] = sqrt_int(Tree[n].sum_raiz[r-1]);
        }
        return;
    }
    // Hay que hacer las dos llamadas para propagar correctamente
    _update_valor(x, (x+y)/2, 2*n, pos, v);
    _update_valor((x+y)/2, y, 2*n+1, pos, v);
    // Actualizar segmento [x, y)
    for (int r = 0; r < MaxRaiz; r++) {
        Tree[n].sum_raiz[r] = Tree[2*n].sum_raiz[r] + Tree[2*n+1].sum_raiz[r];
    }
}

lli _query(int x, int y, int n, int I, int D) {
    // IMPORTANTE: Aplicar siempre el lazy
    apply_lazy(x, y, n);
    if (I <= x && y <= D) {
        return Tree[n].sum_raiz[0];
    }
    if (y <= I || D <= x) {
        return 0;
    }
    lli izq = _query(x, (x+y)/2, 2*n, I, D);
    lli der = _query((x+y)/2, y, 2*n+1, I, D);
    return izq + der;
}

void update_raiz(int I, int D) {
    _update_raiz(0, (int) arr.size(), 1, I, D);
}
void update_valor(int pos, int v) {
    _update_valor(0, (int) arr.size(), 1, pos, v);
}
lli query(int I, int D) {
    return _query(0, (int) arr.size(), 1, I, D);
}


int main(){
    cin.tie(0);
    ios_base::sync_with_stdio(0);
    
    // Leer entrada
    int N, Q;
    cin >> N >> Q;
    arr.resize(N);
    for (int i = 0; i < N; i++) {
        cin >> arr[i];
    }
    
    // Inicializar
    build(0, N, 1);

    for (int i = 0; i < Q; i++) {
        int operacion;
        cin >> operacion;
        if (operacion == 1) {
            // Aplicar raiz a [I-1, D)
            int I, D;
            cin >> I >> D;
            update_raiz(I-1, D);
        } else if (operacion == 2) {
            // Cambiar el valor de 'pos' por 'v'
            int pos, v;
            cin >> pos >> v;
            update_valor(pos-1, v);
        } else if (operacion == 3) {
            // Responder la suma de [I-1, D)
            int I, D;
            cin >> I >> D;
            cout << query(I-1, D) << "\n";
        }
    }
}
