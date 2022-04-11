#include <iostream>
using namespace std;
// Problem source: https://omegaup.com/arena/problem/GCD-Array/#problems

const int MaxN=1e1 + 10;

int cant[MaxN * 2]; // suma de prefijos
int K;

bool possible(int A){
    // Checa si es posible que todos los numeros sean divisibles por A
    if (cant[A-1] > 0) {
        // Hay valores que no podran alcanzar al numero A
        return false;
    }
    if (A - 1 <= K) {
        // El numero es mas chico que la K, siempre se podran mover los numeros
        return true;
    }
    
    for (int i = A; i < MaxN; i += A) {
        if (cant[i+K] < cant[i+A-1]) {
            // Existen numeros entre (i+K, i+A) que no podran moverse al multiplo de A (i)
            return false;
        }
    }
    // Todos los numeros alcanzaron a un multiplo de A
    return true;
}

int main(){
    cin.tie(0);
    ios_base::sync_with_stdio(0);
    
    // Leer entrada
    int N;
    cin >> N >> K;
    int min_valor = MaxN * 2;
    for (int i = 0; i < N; i++) {
        int valor;
        cin >> valor;
        min_valor = min(min_valor, valor);
        // Guardamos en la cubeta
        cant[valor]++;
    }
    // Hacemos un barrido para tener la suma de prefijos
    for (int i = 1; i < MaxN * 2; i++) {
        cant[i] += cant[i-1];
    }
    // Checamos desde el maximo valor posible hasta el 1
    for (int i = min_valor; i >= 1; i--) {
        if(possible(i)){
            // Se logro con el valor 'i'
            cout << i;
            break;
        }
    }
    return 0;
}
