#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>
using namespace std;
typedef pair<int, int> pii;
typedef long long int lli;
//Problem source: https://evaluator.hsin.hr/tasks/SC2021CEOI11diversity/

int N, Q;
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
    vector<int> bucket;
    for (int i = 0; i < N; ) {
        int num = arr[i];
        int cnt = 0;
        while (i < N && num == arr[i]) {
            ++i;
            ++cnt;
        }
        bucket.push_back(cnt);
    }

    // Contamos la cantidad de cada numero; no nos importa que numero es, nos
    // importa la cantidad que hay
    sort(bucket.begin(), bucket.end());
    
    // Acomodamos en zig-zag
    vector<lli> izq, der;
    for (int i = 0; i < bucket.size(); i++) {
        if (i%2 == 0) {
            izq.push_back(bucket[i]);
        } else {
            der.push_back(bucket[i]);
        }
    }
    vector<lli> ocurrencias = izq;
    while(!der.empty()) {
        ocurrencias.push_back(der.back());
        der.pop_back();
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
    // Para la primer forma, podemos ver que se haría de la siguiente forma:
    lli izq_sum = 0, der_sum = N;
    lli ans = 0;
    for (int i = 0; i < ocurrencias.size(); i++) {
        der_sum -= ocurrencias[i]; // Actualizar lado derecho
        ans += (izq_sum * der_sum); // Los extremos de los subarreglos estan fuera
                                    // del rango de las ocurrencias
        ans += (izq_sum * ocurrencias[i]); // El extremo derecho es una de las occ
        ans += (der_sum * ocurrencias[i]); // El extremo izquierdo es una de las occ
        ans += (ocurrencias[i] * (ocurrencias[i] + 1))/2; // Completamente contenido
        izq_sum += ocurrencias[i]; // Actualizar lado izquierdo
    }
    cout << ans << "\n";
}
