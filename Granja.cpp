#include <iostream>
#include <numeric>
using namespace std;
typedef long long int lli;
// Problem source: https://omegaup.com/arena/problem/OMI2014LENGUAJE2#problems

const int MaxN = 5e4 + 10;
bool granja_visitada[MaxN];
int siguiente_granja[MaxN];

int main(){
    cin.tie(0);
    ios_base::sync_with_stdio(0);

    // Leer entrada
    int N;
    cin >> N;
    for (int i = 1; i <= N; i++) {
        cin >> siguiente_granja[i];
    }
    
    lli mcm = 1;
    for (int i = 1; i <= N; i++) {
        if (!granja_visitada[i]) {
            // Visitamos un nuevo ciclo
            
            // Calculamos el sz del ciclo
            int granja_actual = i;
            int sz_ciclo = 0;
            while (!granja_visitada[granja_actual]) {
                granja_visitada[granja_actual] = true;
                granja_actual = siguiente_granja[granja_actual];
                sz_ciclo++;
            }
            
            // Incrementamos el mcm
            lli mcd = gcd(mcm, sz_ciclo);
            lli faltante = sz_ciclo / mcd;
            mcm *= faltante;
        }
    }
    
    // Imprimimos la respuesta
    cout << mcm << "\n";
}
