#include <iostream>
using namespace std;
typedef long long int lli;
// Problem source: https://dmoj.ca/problem/ioi05p2

int main(){
    cin.tie(0);
    ios_base::sync_with_stdio(0);
    
    // La primer observacion es que al determinar el valor de s_1, toda
    // la secuencia s_2, s_3, ..., s_{n+1} esta definida
    // 
    // Si s_1 = v, entonces podemos calcular s_i usando m_i's:
    //  - Observemos que si m_i = (s_i + s_{i+1})/2
    //  - s_{i+1} = 2 * m_i - s_i
    //  - s_{i+1} = 2 * m_i - (2 * m_{i-1} - s_{i-1})
    //  - s_{i+1} = 2 * (m_i - m_{i-1} + m_{i-2} - m_{i-3} + ... +- m_1) -+ v
    //
    // Observemos que para que la secuencia sea no-decreciente, debe
    // cumplirse que
    //     s_i <= s_{i+1}
    //     s_i <= 2 * m_i - s_i
    //     s_i <= m_i
    //
    // Sea suma_acum_i = 2 * (m_i - m_{i-1} + ... +- m_1)
    // Entonces, para 'i' impar tenemos que
    //   s_i = suma_acum_i + s_1
    // Y para 'i' par tendriamos que
    //   s_i = suma_acum_i - s_1
    //
    //
    // Aunado con s_i <= m_i, tenemos que para 'i' impar
    //     suma_acum_i + s_1 <= m_i
    //     s_1 <= m_i - suma_acum_i
    //
    //   para 'i' par seria lo siguiente
    //     suma_acum_i - s_1 <= m_i
    //     suma_acum_i - m_i <= s_1
 
    int N;
    cin >> N;
    
    // Declaramos el rango en que pueden estar el primer numero s_1
    lli min_rango = -2e9, max_rango = 2e9;

    lli suma_acum = 0;
    lli m_i;
    for (int i = 1; i <= N; i++) {
        cin >> m_i;
        // Usamos las limitaciones que encontramos
        if (i%2 == 1) {
            max_rango = min(max_rango, m_i - suma_acum);
        } else {
            min_rango = max(min_rango, suma_acum - m_i);
        }
        suma_acum = 2 * m_i - suma_acum;
    }
    
    if (max_rango < min_rango) {
        // No existen numeros disponibles
        cout << "0\n";
    } else {
        // Contamos los valores posibles de s_1
        cout << max_rango - min_rango + 1 << "\n";
    }
}
