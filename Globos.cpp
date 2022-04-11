#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;
typedef long long int lli;
const int MaxN=2e5+10;
// Problem source: https://omegaup.com/arena/problem/Globos/


double posicion[MaxN];
int stack_anterior[MaxN];
double radio[MaxN];
double maximo_radio[MaxN];

double obtener_radio(double x, double r1, double y, double max_r2) {
    //      ____
    //     /    \
    //    |      |
    //     \    /
    // -------x-------y----
    // Hay un circulo que esta en la posicion 'x' y tiene radio r1
    //
    // Queremos saber el maximo radio r2 para un circulo que esta
    // en la posicion 'y'
    
    // Podemos pensar en el triangulo con los siguientes lados:
    //   - Hipotenusa = r1+r2
    //   - Cateto horizontal = y-x
    //   - Cateto vertical = r2-r1
    // Este triangulo se forma considerando como dos de los vertices a lo centros
    // de los dos circulos
    //
    // Por el teorema de pitagoras:
    //   (r1+r2)^2 = (y-x)^2 + (r2-r1)^2
    //   (r1^2 + 2r1r2 + r2^2) = (y-x)^2 + (r1^2 - 2r1r2 + r2^2)
    //   4r1r2 = (y-x)^2
    //   r2 = (y-x)^2 / 4r1
    return min((y-x)*(y-x)/(4*r1), max_r2);
}


int main(){
    cin.tie(0);
    ios_base::sync_with_stdio(0);
    int N;
    cin >> N;
    
    for (int i = 0; i < N; i++) {
        cin >> posicion[i] >> maximo_radio[i];
    }
    // Inicializar stack
    radio[0] = maximo_radio[0];
    stack_anterior[0] = -1;
    
    double x, y;
    double r1, max_r2;
    
    for (int i = 1; i < N; i++) {
        // Valores del nuevo globo
        y = posicion[i];
        max_r2 = maximo_radio[i];

        for (int j = i - 1; j != -1; j = stack_anterior[j]) {
            // Globo anterior
            x = posicion[j];
            r1 = radio[j];

            // Nuevo limite
            double nuevo_radio = obtener_radio(x, r1, y, max_r2);
            // Actualizar mejor radio
            max_r2 = min(nuevo_radio, max_r2);
                
            // Detenernos
            if (r1 >= max_r2) {
                break;
            }
        }
        radio[i] = max_r2;
        
        int j = i - 1;
        for (; j != -1 && radio[j] <= max_r2; j = stack_anterior[j]);
        stack_anterior[i] = j;
    }
    
    cout << fixed << setprecision(6);
    for (int i = 0; i < N; i++) {
        cout << radio[i] << "\n";
    }
}
