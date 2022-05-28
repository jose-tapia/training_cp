#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>
#include <unordered_map>
#include <set>
using namespace std;
typedef long long int lli;
typedef pair<lli, lli> pii;
typedef pair<pair<int, int>, int> piii;
typedef pair<pii, pii> piiii;
//Problem source: https://evaluator.hsin.hr/tasks/SC2021CEOI11diversity/

const int MaxN = 3e5 + 10;
const int MaxSqrtN = 3e3 + 10;
lli N, Q;
vector<int> arr;
vector<piii> queries;
vector<piiii> queries_unordered;
int total_freq[MaxN];
int sqrt_n;
vector<int> large_values;

void leer_entrada() {
    // Optimizacion de entrada
    cin.tie(0);
    ios_base::sync_with_stdio(0);

    // Leer entrada
    cin >> N >> Q;
    
    arr.resize(N+1);
    for (int i = 1; i <= N; i++) {
        cin >> arr[i];
        ++total_freq[arr[i]];
    }
    sqrt_n = sqrt(N+10);
    for (int i = 0; i < MaxN; i++) {
        if (total_freq[i] > sqrt_n) {
            large_values.push_back(i);
        }
    }
    
    queries_unordered.resize(Q);
    for (int i = 0; i < Q; i++) {
        int p, q;
        cin >> p >> q;
        // Sort by (bucket -> end -> start -> index)
        queries_unordered[i] = {{p / sqrt_n, q}, {p, i}};
    }
    sort(queries_unordered.begin(), queries_unordered.end());
    for (piiii query: queries_unordered) {
        // ((start, end), idx)
        queries.push_back({{(int) query.second.first, (int) query.first.second}, (int) query.second.second});
    }
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

int small_frq[MaxSqrtN];
set<int> big_freq;
int bucket[MaxN];
int freqs[MaxN];
bool cmp_freqs(const int &a, const int &b) {
    return bucket[a] < bucket[b];
}
vector<pii> obtener_ocurrencias() {
    vector<pii> ocurrencias;
    // Ya estaran en orden porque estamos recorriendo un set ordenado
    for (int i = 1; i <= sqrt_n; i++) {
        if (freqs[i] > 0) {
            ocurrencias.push_back({i, freqs[i]});
        }
    }
    // O(sqrtNlogN) worst-case
    sort(large_values.begin(), large_values.end(), cmp_freqs);
    // O(sqrtN)
    for (int value: large_values) {
        if (bucket[value] > sqrt_n) {
            ocurrencias.push_back({bucket[value], 1});
        }
    }
    return ocurrencias;
}
void actualizar_bucket(int val, int cambio) {
    freqs[bucket[val]] -= 1;
    bucket[val] += cambio;
    freqs[bucket[val]] += 1;
}

int L, R;
void mover_rango(int l, int r) {
    while (R < r) {
        actualizar_bucket(arr[++R], 1);
    }
    while (R > r) {
        actualizar_bucket(arr[R--], -1);
    }
    while (L < l) {
        actualizar_bucket(arr[L++], -1);
    }
    while (L > l) {
        actualizar_bucket(arr[--L], 1);
    }
}

lli query_rango() {
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
    
    // Obtenemos las ocurrencias de las frecuencias de los numeros que aparecen en el
    // subrango
    vector<pii> ocurrencias = obtener_ocurrencias();

    // Total de numeros distintos a considerar
    lli N_sub = 0, K = 0;
    for (pii val: ocurrencias) {
        N_sub += val.first * val.second;
        K += val.second;
    }
    // Constantes iniciales
    lli ans = N_sub * (N_sub + 1) * K - N_sub * (K - 1);

    // Procedemos a restar
    int agregar_primero = 1; // 1 izq - 0 der
    lli sum_izq = 0, sum_der = 0;
    for (pii val: ocurrencias) {
        lli cnt_izq = (val.second + agregar_primero) / 2;
        lli cnt_der = (val.second + (1-agregar_primero)) / 2;
        
        // La izquierda (Prefijo)
        ans -= sum_cuadrados_aritmetica(sum_izq, val.first, cnt_izq);
        // La izquierda (Sufijo)
        ans -= sum_cuadrados_aritmetica(N_sub - sum_izq - val.first, -val.first, cnt_izq);
        
        // La derecha (Sufijo)
        ans -= sum_cuadrados_aritmetica(sum_der, val.first, cnt_der);
        // La derecha (Prefijo)
        ans -= sum_cuadrados_aritmetica(N_sub - sum_der - val.first, -val.first, cnt_der);
        
        sum_izq += val.first * cnt_izq;
        sum_der += val.first * cnt_der;
        agregar_primero = (agregar_primero + val.second) % 2;
    }
    return ans / 2;
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
    
    // Ya vimos como responder una query y nos aseguramos de hacerlo de una forma optima
    // usando la idea de MO's algorithm (ya ordenamos las queries en 'leer_entrada')
    
    freqs[0] = (int) N;
    L = 1;
    R = 0;
    vector<lli> ans_offline(Q);
    for (piii query: queries) {
        mover_rango(query.first.first, query.first.second);
        ans_offline[query.second] = query_rango();
    }
    for (lli ans: ans_offline) {
        cout << ans << "\n";
    }
}
