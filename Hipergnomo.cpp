#include <iostream>
#include <vector>
using namespace std;
typedef long long int lli;
typedef pair<lli, int> pii;
// Problem source: https://omegaup.com/arena/problem/Hipergnomo/#problems

vector<pii> bucket;
vector<lli> arr;
int contar_repeticiones(lli v, int lim) {
    int a = 0, b = lim, m;
    while (a+1 < b) {
        m = (a+b)/2;
        if (bucket[m].first <= v) {
            a = m;
        } else {
            b = m;
        }
    }
    if (bucket[a].first == v) {
        // Si existe 'v' y regresamos la cantidad
        return bucket[a].second;
    } else {
        // No existe 'v'
        return 0;
    }
}
void precalculo_prefijos() {
    // Ordenamos los prefijos
    sort(arr.begin(), arr.end());
    
    for (int i = 0; i < arr.size();) {
        int j = i;
        while (j < arr.size() && arr[i] == arr[j]) {
            j++;
        }
        // Contamos la cantidad de 'arr[i]' en el arreglo
        bucket.push_back({arr[i], j-i});
        i = j;
    }
}

int main(){
    cin.tie(0);
    ios_base::sync_with_stdio(0);
    
    // Leer entrada
    int N;
    cin >> N;
    lli prefix_bitmask = 0;
    arr.push_back(prefix_bitmask);
    for (int i = 0; i < N; i++) {
        char caracter;
        cin >> caracter;
        
        // Convertimos el caracter a numero entre [0, 52)
        int caracter_val;
        if ('a' <= caracter && caracter <= 'z') {
            caracter_val = caracter - 'a';
        } else {
            caracter_val = caracter - 'A' + 'z'-'a'+1;
        }
        
        // Guardamos la paridad de la aparicion de cada letra
        // desde el 0 al indice 'i' con bitmask
        prefix_bitmask ^= (1LL << caracter_val);
        // Consideramos el nuevo prefijo
        arr.push_back(prefix_bitmask);
    }
    
    precalculo_prefijos();
    
    lli ans = 0;
    lli curr_prefix, curr_cnt, cmp_prefix, cmp_cnt;
    for (int i = 0; i < bucket.size(); i++) {
        curr_prefix = bucket[i].first;
        curr_cnt = bucket[i].second;

        // Contamos rangos que generen palindromos pares
        ans += (curr_cnt) * (curr_cnt - 1) / 2;
        
        for (int j = 0; j < 52; j++) {
            // Contamos rangos que generen palindromos impares
            if (!(curr_prefix & (1LL << j))) {
                // Contaremos con parejas tales que cmp_prefix < curr_prefix
                // Las otras parejas se contaran despues
                continue;
            }
            cmp_prefix = curr_prefix ^ (1LL << j);
            
            // Aprovechamos que 'cmp_prefix < curr_prefix' y ya sabemos un
            // limite superior
            cmp_cnt = contar_repeticiones(cmp_prefix, i);
            if (cmp_cnt == 0) {
                // Significa que no existe 'cmp_prefix'
                continue;
            }
            
            // Contamos la cantidad de palindromos impares
            ans += curr_cnt * cmp_cnt;
        }
    }
    
    cout << ans  << "\n";
}
