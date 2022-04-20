#include <iostream>
#include <vector>
using namespace std;
typedef  long long int lli;
typedef pair<int, int> pii;
// Problem source: https://dmoj.ca/problem/ioi16p6

struct linea {
    lli m;
    lli c;
    int id;
    double x; // La interseccion con la linea anterior
    // Para el rango de [x, inf), esta linea es optima
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

vector<linea> lineas_vivas;

void agregar_linea(lli m, lli c, int id) {
    // La pendiente es mayor a las demas, entonces siempre agregaremos esta nueva linea
    
    linea nueva_linea = (linea){m, c, id, -1e18};
    
    // Quitar lineas que ya no sean optimas
    while (lineas_vivas.size() > 0) {
        double x = interseccion(lineas_vivas.back(), nueva_linea);
        nueva_linea.x = x;
        if (lineas_vivas.back().x <= x) {
            // Todo good
            // Nuestra neuva recta es optima despues
            lineas_vivas.push_back(nueva_linea);
            return;
        } else {
            // Nuestra nueva recta es optima antes
            // Ya no sera valida la ultima linea (nueva_linea le gana)
            lineas_vivas.pop_back();
        }
    }
    
    // Para agregar la primer linea
    lineas_vivas.push_back(nueva_linea);
}

lli query(lli x) {
    int in = 0, fin = (int) lineas_vivas.size(), mit;
    // linea[in].x <= x
    // linea[fin-1].x <= x ?    NO sabemos
    // linea[fin].x > x (no existe linea[fin].x pero supongamos esto)
    while (in + 1 < fin) {
        mit = (in + fin) / 2;
        if (lineas_vivas[mit].x <= x) {
            in = mit;
        } else {
            // x < lineas_vivas[mit].x
            // lineas_vivas[mit] no cubre a la x y por lo tanto no es optima ahi
            fin = mit;
        }
    }
    // La recta optima esta en lineas_vivas[in]
    return evaluar(lineas_vivas[in], x);
    // return lineas_vivas[in].id;
}

lli x_prev = -1e18; // Valor de 'x_prev'
int linea_prev = 0; // Linea optima de 'x_prev'
                    // lineas_vivas[linea_prev].x <= x_prev <= x
lli query_constante(lli x) {
    // x_prev < x
    // linea_prev ya cumple
    // lineas_vivas[linea_prev].x <= x
    linea_prev = min(linea_prev, (int) lineas_vivas.size() - 1);
    
    while (linea_prev + 1 < lineas_vivas.size()) {
        if (lineas_vivas[linea_prev+1].x <= x){
            // Encontramos una mejor
            linea_prev++;
        } else {
            // Ya encontramos la requete-optima
            break;
        }
    }
    x_prev = x;
    return evaluar(lineas_vivas[linea_prev], x);
}

void limpiar_CHT() {
    lineas_vivas.clear();
    x_prev = -1e18;
    linea_prev = 0;
}

const int MaxN = 1e5;

vector<pii> rango;

lli square(lli x) {
    return x * x;
}

lli costo(int a, int b) {
    // Cuadrado que cubre los puntos 'a', 'a+1', ..., 'b'
    lli cost = square(rango[b].second - rango[a].first + 1);
    if (a == 0) return cost;
    // Residuo de cuadrado que cubria el anterior punto 'a-1'
    lli minus_cost = square(max(0, rango[a-1].second - rango[a].first+1));
    return cost - minus_cost;
}

bool paridad;
lli dp[MaxN][2];

lli get_cost(int K) {
    // dp[i][k] = min(dp[j-1][k-1] + costo(j, i))
    //          = min(dp[j-1][k-1] + (r[i]-l[j]+1)^2 - max(0, r[j-1]-l[j]+1)^2
    //          = r[i]^2 + [(r[i])*(-2(l[j]-1))+(l[j]-1)^2 - max(0,r[j-1]-l[j]+1)^2+dp[j-1][k-1]
    //
    //  x = r[i]
    //  m = -2l[j]+2
    //  c = (l[j]-1)^2 - max(0,r[j-1]-l[j]+1)^2 + dp[j-1][k-1]
    
    for (int i = 0; i < (int) rango.size(); i++) {
        dp[i][paridad] = costo(0, i);
    }
    
    for (int k = 2; k <= K; k++) {
        limpiar_CHT();
        paridad ^= 1;
        for (int i = 0; i < (int) rango.size(); i++) {
            lli m = -2 * (rango[i].first - 1);
            lli c = square(rango[i].first - 1);
            if (i > 0) {
                c += - square(max(0, rango[i-1].second - rango[i].first + 1)) + dp[i-1][paridad^1];
            }
          
            agregar_linea(m, c, i);  
            dp[i][paridad] = square(rango[i].second) + query_constante(rango[i].second);
        }
    }
    return dp[(int)rango.size()-1][paridad];
}

long long int take_photos(int N, int M, int K, int R[], int C[]) {
    // Precalculo para preparar los rangos sin overlap
    vector<pii> uRango;
    for (int i = 0; i < N; i++) {
        uRango.push_back({min(R[i],C[i]), -max(R[i],C[i])});
    }
    sort(uRango.begin(), uRango.end());
    
    int limite = -1;
    for (int i = 0; i < N; i++) {
        if (limite < -uRango[i].second) {
            limite = -uRango[i].second;

            rango.push_back({uRango[i].first, limite});
        }
    }
    
    return get_cost(K);
}
