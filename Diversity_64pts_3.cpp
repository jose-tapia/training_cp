#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>
#include <map>
//#include <fstream>
using namespace std;
typedef long long int lli;
typedef pair<lli, lli> pii;
//Problem source: https://evaluator.hsin.hr/tasks/SC2021CEOI11diversity/

lli N, Q;
vector<int> arr;

void leer_entrada() {
    // Optimizacion de entrada
    cin.tie(0);
    ios_base::sync_with_stdio(0);

    // Leer entrada
    cin >> N >> Q;
    if (Q > 1) {
        // Not supported on this solution
        exit(0);
    }
    
    arr.resize(N);
    for (int i = 0; i < N; i++) {
        cin >> arr[i];
    }
    
    int p, q;
    cin >> p >> q;
}

lli sum_cuadrados_aritmetica(lli A, lli B, lli M) {
    // A^2 + (A+B)^2 + (A+2B)^2 + ... + (A + (M-1)B)^2
    // (A+tB)^2 = A^2 + 2tAB + t^2B^2
    // sum_t (A+tB)^2 = A^2(sum_t 1) + (2AB)(sum_t t) + B^2(sum_t t^2)
    //                = A^2(M) + (2AB)((M-1)M/2) + B^2((M-1)M(2M-1)/6)
    lli sum = A * A * M;
    sum +=  A * B * (M - 1) * M;
    sum += B * B * (((M - 1) * M * (2 * M - 1)) / 6);
    return sum;
}

int main(){

    // La primer observacion importante es que todos los numeros iguales deben
    // estar consecutivos
    
    // Ahora hay que resolver dos problemas: cómo acomodarlos y cómo responder
    // una pregunta
    
    // Haciendo varios ejemplos podemos observar que ordenarlo de 'menor a
    // mayor' no es el mejor acomodo. Por la cantidad de numeros, debemos de
    // encontrar una forma 'rapida' de ordenar los numeros
    // Podemos probar varias ideas y convencernos que hacer zig-zag podria ser
    // el mejor acomodo

    leer_entrada();
    
    // Responder una query
    
    // Ordenamos para que esten consecutivos
    sort(arr.begin(), arr.end());
    
    map<lli, lli> bucket;
    for (int i = 0; i < N; ) {
        int num = arr[i];
        int cnt = 0;
        while (i < N && num == arr[i]) {
            ++i;
            ++cnt;
        }
        bucket[cnt]++;
    }
    
    // Cambiemos el enfoque de la pregunta que nos hacen:
    //   En vez de contar para cada subarreglo cuantos numeros diferentes tienen,
    //   contamos para cada numero en cuántos subarreglos aportaría un número
    //   diferente; este enfoque nos permite calcular más rápido la respuesta.
    //
    // En este caso, para un número "k" tenemos todas sus ocurrencias contiguas,
    // facilitando el cálculo para contar cuántos subarreglos tienen al menos una
    // ocurrencia "k". Hay dos formas de contar esto:
    //   1) Contar directamente cuántos subarreglos contienen al menos un "k".
    //   2) Contar cuántos subarreglos no tienen al menos un "k" y restar al total
    //      de subarreglos.
    //
    // La ventaja de la segunda forma es que, teniendo en cuenta las ocurrencias,
    // podemos hacer el calculo de los cuadrados más fácil, rápido y de forma independiente
    //
    // En particular, este último aspecto: independiente. Al ser independiente podemos
    // hacer varios cálculos de manera simultánea, lo que significa mejor complejidad.
    //
    // La mas grande observación sobre este cálculo es que hay que sumar N(N+1)/2 la
    // cantidad de numeros distintos que tengamos, y restarle todos los prefijos al cuadrado
    // y todos los sufijos al cuadrado (sin incluir el prefijo completo y el sufijo
    // completo)
    //
    // Podemos tener una cantidad muy grande de numeros distintos (hasta N), pero no hay
    // una gran cantidad de tamaños diferentes (hay a lo mucho sqrt(N) distintos).
    //
    // A diferencia de la última forma de contar, en esta vamos a acomodar los numeros en
    // el momento
    
    // Obtener repeticiones de las ocurrencias
    vector<pii> ocurrencias;
    for (pii val: bucket) {
        ocurrencias.push_back(val);
    }
    //sort(ocurrencias.begin(), ocurrencias.end());
    
    // Total de numeros distintos a considerar
    int K = 0;
    for (pii val: ocurrencias) {
        K += val.second;
    }
    // Constantes iniciales
    lli ans = N * (N + 1) * K - N * (K - 1);
    
    // Procedemos a restar
    int agregar_primero = 1; // 1 izq - 0 der
    lli sum_izq = 0, sum_der = 0;
    for (pii val: bucket) {
        lli cnt_izq = (val.second + agregar_primero) / 2;
        lli cnt_der = (val.second + (1-agregar_primero)) / 2;
        
        // La izquierda (Prefijo)
        ans -= sum_cuadrados_aritmetica(sum_izq, val.first, cnt_izq);
        // La izquierda (Sufijo)
        ans -= sum_cuadrados_aritmetica(N - sum_izq - val.first, -val.first, cnt_izq);
        
        // La derecha (Sufijo)
        ans -= sum_cuadrados_aritmetica(sum_der, val.first, cnt_der);
        // La derecha (Prefijo)
        ans -= sum_cuadrados_aritmetica(N - sum_der - val.first, -val.first, cnt_der);
        
        sum_izq += val.first * cnt_izq;
        sum_der += val.first * cnt_der;
        agregar_primero = (agregar_primero + val.second) % 2;
    }
    cout << ans / 2 << "\n";
}
