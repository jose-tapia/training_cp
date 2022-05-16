#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>
using namespace std;
//Problem source: https://omegaup.com/arena/problem/Poder-del-subarreglo#problems

typedef pair<int, int> pii;
typedef pair<pii, pii> query;

const int MaxN = 2e5+1;
const int MaxT = 1e6+1;

vector<query> orden;
int arr[MaxN];
int bucket[MaxT];
int curr_ans;
int respuestas[MaxN];

int movercubeta(int in_nuevo, int fn_nuevo, int in, int fn) {
    if (in_nuevo <= in) {
        // Mover inicio a la izquierda (aumentar rango)
        --in;
        while (in_nuevo <= in) {
            int s = arr[in];
            int K_s = bucket[s];
            curr_ans = curr_ans - K_s*K_s*s + (K_s+1)*(K_s+1)*s;
            ++bucket[s];
            --in;
        }
    } else {
        while (in < in_nuevo) {
            // Mover inicio a la derecha (disminuir rango)
            int s = arr[in];
            int K_s = bucket[s];
            curr_ans = curr_ans - K_s*K_s*s + (K_s-1)*(K_s-1)*s;
            --bucket[s];
            ++in;
        }
    }
    
    if (fn <= fn_nuevo) {
        ++fn;
        while (fn <= fn_nuevo) {
            // Mover fin a la derecha (aumentar rango)
            int s = arr[fn];
            int K_s = bucket[s];
            curr_ans = curr_ans - K_s*K_s*s + (K_s+1)*(K_s+1)*s;
            ++bucket[s];
            ++fn;
        }
    } else {
        while (fn_nuevo < fn) {
            // Mover fin a la izquierda (disminuir rango)
            int s = arr[fn];
            int K_s = bucket[s];
            curr_ans = curr_ans - K_s*K_s*s + (K_s-1)*(K_s-1)*s;
            --bucket[s];
            --fn;
        }
    }
    return curr_ans;
}

int main(){
    // Optimizacion de entrada
    cin.tie(0);
    ios_base::sync_with_stdio(0);

    int N, T;
    cin >> N >> T;
    for (int i = 1; i <= N; i++) {
        cin >> arr[i];
    }
    
    int bucket_size = sqrt(N);
    
    for (int i = 0; i < T; i++) {
        int l, r;
        cin >> l >> r;
        int cubeta = l / bucket_size;
        orden.push_back({{cubeta, r},{l, i}});
    }

    sort(orden.begin(),orden.end());

    // Inicializar rangos actuales
    int in=1;
    int fn=0;
    // Responder todas las queries
    for (int i = 0; i < T; i++) {
        int in_nuevo = orden[i].second.first;
        int fn_nuevo = orden[i].first.second;
        int idx_query = orden[i].second.second;
        
        respuestas[idx_query] = movercubeta(in_nuevo, fn_nuevo, in, fn);
        in = in_nuevo;
        fn = fn_nuevo;
    }

    //Imprimir
    for(int i = 0; i < T; i++) {
        cout << respuestas[i] << "\n";
    }
}
