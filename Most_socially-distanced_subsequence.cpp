#include <iostream>
#include <vector>
using namespace std;
typedef pair<int,int> pii;
typedef long long int lli;
// Problem source: https://codeforces.com/contest/1364/problem/B

const int MaxN=1e5+10;
int N, M;
int arr[MaxN];
 
int main(){
    cin.tie(0);
    ios_base::sync_with_stdio(0);
    
    // Cantidad de testcases
    int T;
    cin>>T;
    while(T--){
        // Lectura
        cin >> N;
        for (int i = 0; i < N; i++) {
            cin>>arr[i];
        }
        
        // Vector para la respuesta
        vector<int> ans;
        // Siempre agregar al primero
        ans.push_back(arr[0]);
        bool direccion = (arr[0] >= arr[1]); // true: abajo  / false: arriba
        
        // Observemos que si hay tres numeros en secuencia (a, b, c)
        // tales que 'a <= b <= c' => '|a-b| + |b-c| = |a - c|'
        // * Tambien se cumple si 'a >= b >= c'
        //
        // Por lo que hay que buscar "picos" ('a < b > c' o 'a > b < c')
        for (int i = 1; i < N; i++) {
            bool nueva_direccion = (arr[i-1] >= arr[i]);
            if (nueva_direccion ^ direccion) {
                // Hay un pico y nos conviene agregarlo
                ans.push_back(arr[i-1]);
                direccion = nueva_direccion;
            }
        }
        // Siempre agregamos al ultimo
        ans.push_back(arr[N-1]);
        
        cout << ans.size() << "\n";
        for(int k: ans){
            cout << k << " ";
        }
        cout << "\n";
    }
}
