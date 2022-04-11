#include <iostream>
using namespace std;
// Problem source: https://omegaup.com/arena/problem/Enredados/#problems

const int MaxN=1.5e6 + 10;

int bit[MaxN];
int pos[MaxN];

int main(){
    cin.tie(0);
    ios_base::sync_with_stdio(0);
    // Leer entrada
    int N;
    cin >> N;
    long int ans = 0;
    
    int n = N / 2;
    // Guardar primer mitad
    for (int i = 1; i <= n; i++) {
        int p;
        cin >> p;
        pos[p] = i;
    }
    
    // Enlazar segunda mitad
    for (int i = 1; i <= n; i++) {
        int q;
        cin>>q;
        // Query de BIT - Con cuántos se enrueda
        for (int p = n-pos[q]+1; p > 0; p -= (p&-p)) {
            ans+=bit[p];
        }
        // Update de BIT - Considerar para futuros casos
        for (int p = n-pos[q]+1; p <= n; p += (p&-p)) {
            bit[p]++;
        }
    }
    // Imprimir respuesta
    cout << ans << "\n";
}
