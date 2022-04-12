#include <iostream>
#include <stack>
using namespace std;
// Problem source: https://codeforces.com/contest/1364/problem/C

const int MaxN=1e5+10;
int N, M;
int arr[MaxN];
int mex[MaxN];
 
int main(){
    cin.tie(0);
    ios_base::sync_with_stdio(0);
    
    // Lectura
    cin >> N;
    for (int i = 0; i < N; i++) {
        cin >> arr[i];
    }
    
    // Hacemos el primer valor manualmente:
    //   Si arr[0] == 1, entonces el primer valor debe ser 0
    //   En caso contrario, puede ser cualquier valor
    int ultimo_mex;
    if (arr[0] == 1) {
        mex[0] = 0;
        ultimo_mex = 1;
    } else {
        mex[0] = N+1;
        ultimo_mex = 0;
    }
    
    // Para posiciones consecutivas iguales, ponemos mex[i] = N+1
    // Debe cumplirse que MEX(mex[0], mex[1], ..., mex[i-1]) = arr[i-1] (Se encuentren los valores de 0, 1, 2, ..., arr[i-1]-1)
    // Entonces, para no afectar al MEX, hay que poner un valor mayor a arr[i-1] (Por el momento, N+1)
    // El valor N+1 indicara que podemos cambiarlo despues
    stack<int> buscar_posicion;
    for (int i = N-1; i > 0; i--) {
        if (arr[i-1] == arr[i]) {
            mex[i] = N+1;
        } else {
            // De afuerzas hay que cambiar el anterior MEX
            mex[i] = arr[i-1];
            // Indicamos que queremos conseguir arr[i]
            buscar_posicion.push(arr[i]);
        }
    }
    for (int i = 0; i < N; i++) {
        if (mex[i] == N+1) {
            // Quitar del stack si ya no son necesarios
            while (!buscar_posicion.empty()) {
                if (ultimo_mex + 1 == buscar_posicion.top()) {
                    ultimo_mex++;
                    buscar_posicion.pop();
                } else {
                    // Hay que asignar el valor en mex[i]
                    break;
                }
            }
            if (buscar_posicion.empty()) {
                // Todos los mex's cumplen!
                continue;
            }
            mex[i] = ultimo_mex + 1;
            ultimo_mex++;
        }
    }
    while (!buscar_posicion.empty()) {
        if (ultimo_mex + 1 == buscar_posicion.top()) {
            ultimo_mex++;
            buscar_posicion.pop();
        } else {
            // Hay que asignar el valor en mex[i]
            break;
        }
    }
    if (!buscar_posicion.empty()) {
        // Quedan mex's sin cumplir y ya asignamos todos los N+1
        // Entonces no es posible
        cout << "-1\n";
    } else {
        // Imprimimos los valores de mex's buscados
        for (int i = 0; i < N; i++) {
            cout << mex[i] << " ";
        }
        cout << "\n";
    }
}
