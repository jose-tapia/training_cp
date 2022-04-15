#include <iostream>
#include <vector>
using namespace std;
typedef long long int lli;
// Problem source: https://omegaup.com/arena/problem/Descomposicion-Fibonacci/#problems

const int MaxN = 89;

vector<lli> fibo(2, 0);

void precalculo_fibonacci() {
    fibo[0] = 1;
    fibo[1] = 2;
    for (int i = 2; i < MaxN; i++) {
        fibo.push_back(fibo[i-1] + fibo[i-2]);
    }
}

vector<int> descomposicion_canonica(lli n) {
    // Existe una unica descomposicion canonica:
    //  - Esta se encuentra usando la menor cantidad de numeros fibonacci
    //  - Observemos que no usamos dos numeros de fibonacci consecutivos en
    //    la descomposicion canonica; en caso de hacerlo, es posible
    //    juntarlas para hacer un numero de fibonacci (en caso de que haya tres
    //    numeros fibonacci, juntamos los dos mas grandes)
    vector<int> idx_canonica;
    for (int i = (int) fibo.size() - 1; i >= 0; i--) {
        if (fibo[i] <= n) {
            idx_canonica.push_back(i);
            n -= fibo[i];
        }
    }
    reverse(idx_canonica.begin(), idx_canonica.end());
    return idx_canonica;
}

lli DP(vector<int> idx_canonica) {
    // Tenemos la descomposicion canonica del numero 'n'.  En la posicion
    // 'i' es 0 si no se esta sumando, o 1 si si
    //
    // * Observemos que un '1' en 'i' lo podemos descomponer en dos '1' en
    //   las posiciones 'i-1' y 'i-2' (por fibo[i] = fibo[i-1]+fibo[i-2])
    // * No podemos 'expandir' la posicion 'i' si alguna de las posiciones
    //   'i-1' o 'i-2' ya tiene un '1'.
    //
    // Consideremos el i-esimo numero fibonacci:
    //   Este lo podemos expandir y poner un '1' en 'i-1' y 'i-2'. El numero
    //   en la posicion 'i-2' lo podemos volver a expandir ocupando la
    //   posicion 'i-3' y 'i-4'. Sin embargo, no importa cuales expandamos,
    //   no podremos expandir 'i-1'. Este argumento se repite para las
    //   posiciones 'i-3', 'i-5', 'i-7', ..., etc.
    //
    //   Podriamos expandir 'i', 'i-2', 'i-4', ..., etc. Entonces, si tenemos
    //   'x' ceros y un uno, podemos expandir a lo mucho x/2 (redondeado para
    //   abajo)
    //
    //  (Denotemos como 'x//y' como el cociente de la division de 'x' por 'y')
    //
    // Si tuvieramos un numero 'n' que sea suma de dos numeros fibonacci,
    // analicemos como calcular este caso:
    //  - Sea i y j los indices de estos numeros fibonacci
    //  - Para expandir 'i', tenemos que habria 1 + (i-1)//2 formas
    //  - No podemos expandir tan facil a 'j', pues depende de la cantidad de
    //    ceros entre el ultimo numero fibonacci
    //  - Esta la posibilidad que expandamos 'i' o no: En caso de expandirlo,
    //    habria (j-i-1) ceros, y si no, serian (j-i) ceros.
    //  - Entonces, podemos contar dependiendo si expandimos o no la posicion 'i'
    //
    // Podemos repetir el proceso anterior para tres o mas numeros fibonacci, la
    // idea seria guardar la cantidad de formas de expandir los primeros 'x',
    // haciendo la diferencia si se expande el x-esimo numero o no.
    
    vector<lli> dp(2, 0); // Guardaremos para el ultimo
    
    // Caso base (indice idx_canonica[0])
    dp[0] = idx_canonica[0] / 2; // Expandir el numero fibonacci
    dp[1] = 1;                   // No expandir el numero fibonacci
    
    for (int i = 1; i < idx_canonica.size(); i++) {
        // Consideramos ambos casos
        lli sin_expandir = dp[0] + dp[1];
        
        // Expandimos la posicion idx_canonica[i]
        int num_ceros = (idx_canonica[i] - idx_canonica[i-1] - 1);
        lli expandiendo = (num_ceros/2) * dp[1] + ((num_ceros+1)/2) * dp[0];
        
        // Guardamos los calculos para el siguiente indice
        dp[1] = sin_expandir;
        dp[0] = expandiendo;
    }
    return dp[0] + dp[1];
}

int main(){
    cin.tie(0);
    ios_base::sync_with_stdio(0);
    
    // Realizar precalculo
    precalculo_fibonacci();

    int T;
    cin >> T;
    for (int t = 1; t <= T; t++) {
        lli n;
        cin >> n;
        // Generar descomposicion canonica de 'n' (con la menor cantidad de #fib's)
        vector<int> idx_canonica = descomposicion_canonica(n);
        // Aplicar DP
        cout << DP(idx_canonica) << "\n";
    }
}
