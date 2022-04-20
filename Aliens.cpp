#include <iostream>
#include <vector>
using namespace std;
typedef long long int lli;
typedef pair<lli, int> pii;
// Problem source: https://dmoj.ca/problem/ioi16p6

struct linea {
    lli m;
    lli c;
    int id;
    double x; // La interseccion con la linea anterior
    // Para el rango de [x, inf), esta linea es optima
    int cnt;
};

double interseccion(linea A, linea B) {
    // y = A.m * x + A.c
    // y = B.m * x + B.c
    //
    // (A.m * x  +A.c)   =  (B.m * x + B.c)
    // (A.m * x) - (B.m * x)   = B.c - A.c
    // (A.m - B.m) * x = (B.c - A.c)
    //  x = (B.c - A.c) / (A.m - B.m)
    // Supongamos que nunca habra dos pendientes iguales
    return (B.c - A.c + 0.0) / (A.m - B.m);
}

lli evaluar(linea L, lli x) {
    return L.m * x + L.c;
}

struct ConvexHull {
    // Lineas que pertenecen al convex hull
    vector<linea> lineas_vivas;
    
    // Variables para responer preguntas en O(1) amortizado
    lli x_prev = -1e18; // Valor de 'x_prev'
    int linea_prev = 0; // Linea optima de 'x_prev'
                        // lineas_vivas[linea_prev].x <= x_prev <= x

    void agregar_linea(lli m, lli c, int id=0, int cnt=0) {
        // Hay que agregar las pendientes en orden monotono
        //  - Si se agregan de menor a mayor -> Se calcula el maximo
        //  - Si se agregan de mayor a menor -> Se calcula el minimo
        
        linea nueva_linea = (linea){m, c, id, -1e18, cnt};
        
        // Actualizar conjunto de lineas en el convex hull
        while (lineas_vivas.size() > 0) {
            double x = interseccion(lineas_vivas.back(), nueva_linea);
            nueva_linea.x = x;
            if (lineas_vivas.back().x < x) {
                // Nuestra neuva recta es optima despues
                break;
            } else {
                // Nuestra nueva recta es optima antes de la interseccion
                // Ya no sera parte del convex hull la ultima linea
                lineas_vivas.pop_back();
            }
        }
        
        // Para agregar la nueva linea
        lineas_vivas.push_back(nueva_linea);
    }
    
    linea query_constante(lli x) {
        // Si tocas las preguntas (x_i) cumplen que x_i <= x_{i+1)
        // Con esta llamada se pueden lograr en tiempo O(1) amortizado
        if (x < x_prev) {
            // Reiniciar busqueda
            linea_prev = 0;
        }
        // Actualizar ultima linea tal que
        //     lineas_vivas[linea_prev].x <= x_prev
        linea_prev = min(linea_prev, (int) lineas_vivas.size() - 1);
        
        while (linea_prev + 1 < (int) lineas_vivas.size()) {
            if (lineas_vivas[linea_prev+1].x <= x){
                // Actualizar linea
                linea_prev++;
            } else {
                // Ya encontramos la requete-optima
                break;
            }
        }
        x_prev = x;
        return lineas_vivas[linea_prev];
    }
    
    lli query(lli x) {
        return evaluar(query_constante(x), x);
    }
    
    void clear() {
        lineas_vivas.clear();
        x_prev = -1e18;
        linea_prev = 0;
    }
};

ConvexHull CHTrick;
vector<pii> rango;
lli square(lli x) {
    return x * x;
}

// Solucion
//   DP Convex Hull Trick / O(NK)
//      dp[i][k] = min(dp[j-1][k-1] + costo(j, i))
//               = min(dp[j-1][k-1] + (r[i]-l[j]+1)^2 - max(0, r[j-1]-l[j]+1)^2)
//               = r[i]^2 +  min(   r[i] * (-2(l[j]-1)
//                                       + (l[j]-1)^2 - max(0,r[j-1]-l[j]+1)^2+dp[j-1][k-1])
//
//              x(i) = r[i]
//              m(j) = -2l[j]+2
//              c(j) = (l[j]-1)^2 - max(0,r[j-1]-l[j]+1)^2 + dp[j-1][k-1]
//
//      dp[i][k] = r[i]^2 +  min(x(i) * m(j) + c(j))
//
//   Lagrange optimization:
//      dp[i] = r[i]^2 + C + min(x(i) * m(j) + c(j))
//


lli solve_lagrange(lli C, int K) {
    // Limpiar convex hull
    CHTrick.clear();
    
    // Iterar los rangos en orden
    pii last_dp;
    for (int i = 0; i < (int) rango.size(); i++) {
        auto& [l, r] = rango[i];
        
        // Variables para la nueva linea
        lli m = -2 * (l - 1);
        lli c = square(l - 1);
        int cnt = 0;
        if (i > 0) {
            // Utilizar valores anteriores
            lli diff = rango[i-1].second - l + 1;
            if (diff > 0) {
                c -= square(diff);
            }
            c += last_dp.first; // DP[i-1]
            cnt = last_dp.second;
        }
        
        // Agregar nueva linea
        CHTrick.agregar_linea(m, c, i, cnt);
        // Preguntar por la linea optima actual
        linea mejor_opcion = CHTrick.query_constante(r);
        
        // Guardar valor de dp
        last_dp.first = square(r) + C + evaluar(mejor_opcion, r);
        last_dp.second = mejor_opcion.cnt + 1;
    }
    
    // Regresar solucion del problema
    return last_dp.first - K * C;
}

lli bin_search_lagrange(lli M, int K) {
    // Buscamos el valor de C tal que
    //    max_{C}{ solve_lagrange(C, K) }
    
    lli inicio = -1, fin = M * M, mitad;
    while (inicio + 1 < fin) {
        mitad = (inicio + fin)/2;
        lli costo_A = solve_lagrange(mitad, K);
        lli costo_B = solve_lagrange(mitad + 1, K);
        
        if (costo_B <= costo_A) {
            // El maximo esta mas cerca de 'mitad'
            fin = mitad;
        } else {
            // El maximo esta mas cerca de 'mitad + 1'
            inicio = mitad;
        }
    }
    return solve_lagrange(fin, K);
}

long long int take_photos(int N, int M, int K, int R[], int C[]) {
    // Precalculo para preparar los rangos sin overlap
    vector<pii> uRango;
    for (int i = 0; i < N; i++) {
        uRango.push_back({min(R[i],C[i]), -max(R[i],C[i])});
    }
    sort(uRango.begin(), uRango.end());
    
    // Barrido para filtrar rangos utiles
    int limite = -1;
    for (int i = 0; i < N; i++) {
        if (limite < -uRango[i].second) {
            limite = -uRango[i].second;
            rango.push_back({uRango[i].first, limite});
        }
    }
    
    // Actualizar cantidad maxima de K que vamos a usar
    K = min(K, (int) rango.size());
    // Conseguir respuesta
    return bin_search_lagrange(M, K);
}


int main() {
    int N, M, K;
    cin >> N >> M >> K;
    int R[N], C[N];
    for (int i = 0; i < N; i++) {
        cin >> R[i] >> C[i];
    }
    cout << take_photos(N, M, K, R, C) << "\n";
}
