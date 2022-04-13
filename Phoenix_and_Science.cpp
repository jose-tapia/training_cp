#include <iostream>
#include <vector>
using namespace std;
typedef long long int lli;
typedef pair<int,int> pii;
// Problem source: https://codeforces.com/contest/1348/problem/D

vector<lli> obtain_days(lli n) {
    // Si tenemos 'x' moleculas en un dia, despues de una noche
    // podemos sumar entre 'x' y '2x'. Si sumamos 1, 2, 4, ...,
    // 2^k, estaremos cerca de tener 'n'. Entonces agregamos las
    // potencias que podamos y el complemento de 'n'

    vector<lli> key_values;
    lli suma_pot = 0;
    for (lli pot = 1; suma_pot + pot <= n; pot *= 2) {
        suma_pot += pot;
        key_values.push_back(pot);
    }
    if (suma_pot < n) {
        key_values.push_back(n - suma_pot);
    }
    sort(key_values.begin(), key_values.end());

    // Para que en una noche sume 'x', hay que indicar cuantas
    // moleculas generar. Si sumo 'x' en una noche y 'y' el dia
    // siguiente, entonces se dividieron 'y-x' moleculas

    vector<lli> days;
    for (int i = 1; i < key_values.size(); i++) {
        days.push_back(key_values[i] - key_values[i-1]);
    }
    return days;
}
 
int main(){
    int T;
    cin >> T;
    for (int t = 1; t <= T; t++) {
        // Cantidad de masa requerida
        lli n;
        cin >> n;
        
        // Respuesta
        vector<lli> days = obtain_days(n);
        cout << days.size() << "\n";
        for (lli day: days) {
            cout << day << " ";
        }
        cout << "\n";
    }
}
