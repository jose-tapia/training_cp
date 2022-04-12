#include <iostream>
using namespace std;
typedef long long int lli;
typedef pair<int, int> pii;
// Problem source: https://codeforces.com/contest/1469/problem/C
 
const int MaxN = 2e5+10;
int h[MaxN]; // Alturas

int main() {
    cin.tie(0);
    ios_base::sync_with_stdio(0);
    // Cantidad de testcases
    int T;
    cin >> T;

    int N, K, max_rango, min_rango;
    bool possible;
    while(T--) {
        // Entrada
        cin >> N >> K;
        for(int i = 0; i < N; i++) {
            cin >> h[i];
        }
        
        // Checar en que rango pueden estar los numeros
        possible = true;
        // Al principio solo podemos usar el rango [h[0], h[0]]
        max_rango = h[0];
        min_rango = h[0];
        for (int i = 1 ; i < N; i++) {
            // Que tanto podemos subir
            max_rango = min(h[i]+K-1, max_rango+K-1);
            // Que tanto podemos bajar
            min_rango = max(h[i], min_rango-K+1);
            // Si ya no existe alguna posicion valida, regresamos false
            if (max_rango < min_rango) {
                possible = false;
                break;
            }
        }
        
        if (min_rango == h[N-1] && possible) {
            cout << "YES\n";
        } else {
            cout << "NO\n";
        }
    }
}
