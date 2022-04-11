#include <iostream>
#include <vector>
using namespace std;
typedef int lli;
typedef pair<int,int> pii;
const int MaxN=1e6+10;
// Problem source: https://omegaup.com/arena/problem/Pieza-bola-de-billar/#problems

lli N, M;
int vis[MaxN];

enum Direcciones {
    Noreste = 0,  // Norte Este
    Noroeste = 1, // Norte Oeste
    Suroeste = 2, // Sur Oeste
    Sureste = 3   // Sur Este
};

pair<pii, int> rebota(pii actual, int dir) {
    /// Noreste
    if (dir == Direcciones::Noreste) {
        // (x, y) -> (x + 1, y + 1)
        // Queremos que vaya hacia el techo
        
        // Desde la posicion 'y' a la 'M-1' hay M-1-y pasos
        // (x, y) -> (x + M-1-y, M-1)
        int a = actual.first + M - 1 - actual.second;
        int b = M - 1;
        int nueva_dir = Direcciones::Sureste;
        if (a < N) {
            // Si logro llegar al techo
            if (a == N - 1) {
                // Llego a la esquina
                nueva_dir = Direcciones::Suroeste;
            }
        } else {
            // No llego al techo
            // Entonces choco con la pared de la derecha
            // Desde la posicion 'x' a la 'N-1' hay N-1-x pasos
            // (x, y) -> (N-1, y + N-1-x)
            a = N - 1;
            b = actual.second + N - 1 - actual.first;
            nueva_dir = Direcciones::Noroeste;
        }
        return {{a, b}, nueva_dir};
    }
    
    /// Noroeste
    if (dir == Direcciones::Noroeste) {
        // (x, y) -> (x - 1, y + 1)
        // Queremos que choque con el techo
        // De 'y' a 'M-1' hay M-1-y pasos
        // (x, y) -> (x -M+1+y, M-1)
        int a = actual.first - M + 1 + actual.second;
        int b = M - 1;
        int nueva_dir = Direcciones::Suroeste;
        if (0 <= a) {
            // Si logro llegar al techo
            if (a == 0) {
                // Esta en la esquina
                nueva_dir = Direcciones::Sureste;
            }
        } else {
            // No logro llegar al techo
            // Entonces choco con la pared izquierda
            // De 'x' a '0' hay x pasos
            // (x, y) -> (0, y+x)
            a = 0;
            b = actual.first + actual.second;
            nueva_dir = Direcciones::Noreste;
        }
        return {{a, b}, nueva_dir};
    }
    
    /// Suroeste
    if (dir == Direcciones::Suroeste) {
        // (x, y) -> (x-1, y-1)
        // Queremos que choque con el suelo
        // De 'y' a '0' hay y pasos
        // (x, y) -> (x-y, 0)
        int a = actual.first - actual.second;
        int b = 0;
        int nueva_dir = Direcciones::Noroeste;
        if (0 <= a) {
            // Logro llegar al suelo
            if (a == 0) {
                // Esta en la esquina
                nueva_dir = Direcciones::Noreste;
            }
        } else {
            // No logro llegar al suelo
            // Choco con la pared izquierda
            // De 'x' a '0' hay x pasos
            // (x, y) -> (0, y-x)
            a = 0;
            b = actual.second - actual.first;
            nueva_dir = Direcciones::Sureste;
        }
        return {{a, b}, nueva_dir};
    }
    
    /// Sureste
    if (dir == Direcciones::Sureste) {
        // (x, y) -> (x+1, y-1)
        // Queremos que choque con el suelo
        // De 'y' a '0' hay y pasos
        // (x, y) -> (x+y, 0)
        int a = actual.first + actual.second;
        int b = 0;
        int nueva_dir = Direcciones::Noreste;
        if (a < N) {
            // Logro llegar al suelo
            if (a == N - 1) {
                // Esta en la esquina
                nueva_dir = Direcciones::Noroeste;
            }
        } else {
            // No logro llegar al suelo
            // Choco con la pared derecha
            // De 'x' a 'N-1' hay N-1-x pasos
            // (x, y) -> (N-1, y -N+1+x)
            a = N - 1;
            b = actual.second - N + 1 + actual.first;
            nueva_dir = Direcciones::Suroeste;
        }
        return {{a, b}, nueva_dir};
    }
    // Direccion desconocida
    return {{0, 0}, 0};
}

void find_cycle(lli x){
    // Inicio
    pii inicio = {x, 0};
    // Actual
    pii actual = {x, 0};
    // Direcciones posibles
    vector<int> dirs = {Direcciones::Noreste, Direcciones::Noroeste};
    
    for (int dir: dirs) {
        // Realizar ciclo
        do{
            if (actual.second == 0) {
                // Llegamos a la primer fila
                vis[actual.first] = true;
            }
            // Rebotamos
            auto nueva_pos = rebota(actual, dir);
            // Actualizamos posicion
            actual = nueva_pos.first;
            dir = nueva_pos.second;
        }while(inicio != actual);
    }
}

int main(){
    // Para este problema vamos a visualizarlo como si fuera un plano cartesiano
    // Esto es, la esquina inferior izquierda es la (0, 0)
    // La esquina superior derecha es (N, M)
    
    lli ans = 0;
    cin >> N >> M;
    if (N > M) {
        // Mantener N < M
        swap(N, M);
    }
    
    // Podemos ver la matriz como la union de ciclos
    // Si existe un ciclo, este debe pasar por el primer renglon
    for (int i = 0; i < N; i++) {
        if(!vis[i]){
            // Encontramos un ciclo nuevo
            // Visitamos todas las posiciones que cubre en las orillas
            find_cycle(i);
            ans++;
        }
    }
    // A lo mucho visitaria las 2*N + 2*M - 4 posiciones de las orillas
    cout<<ans;
}
