#include <iostream>
#include <vector>
using namespace std;

struct linea {
    float m;
    float c;
    int id;
    float x; // La interseccion con la linea anterior
    // Para el rango de [x, inf), esta linea es optima
};

float interseccion(linea A, linea B) {
    // y = A.m * x + A.c
    // y = B.m * x + B.c
    //
    // (A.m * x  +A.c)   =  (B.m * x + B.c)
    // (A.m * x) - (B.m * x)   = B.c - A.c
    // (A.m - B.m) * x = (B.c - A.c)
    //  x = (B.c - A.c) / (A.m - B.m)
    // Supongamos que nunca habra dos pendientes iguales
    return (B.c - A.c) / (A.m - B.m);
}
float evaluar(linea L, float x) {
    return L.m * x + L.c;
}

vector<linea> lineas_vivas;

void agregar_linea(float m, float c, int id) {
    // La pendiente es mayor a las demas, entonces siempre agregaremos esta nueva linea
    
    linea nueva_linea = (linea){m, c, id, -1e18};
    
    // Quitar lineas que ya no sean optimas
    while (lineas_vivas.size() > 0) {
        float x = interseccion(lineas_vivas.back(), nueva_linea);
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

float query(float x) {
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
                    // lineas_vivas[linea_prev].x <= x_prev   <= x
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

int main () {
    // Vamos a recibir Q queries
    //    1 m c  (una nueva linea)   (las pendientes siempre iran aumentando)
    //    2 x    (una query)
    int Q;
    cin >> Q;
    for (int i = 0; i < Q; i++) {
        int operacion;
        cin >> operacion;
        if (operacion == 1) {
            float m, c;
            cin >> m >> c;
            agregar_linea(m, c, i);
        } else {
            float x;
            cin >> x;
            cout << query(x) << "\n";
        }
    }
}
