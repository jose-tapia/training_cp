#include <iostream>
using namespace std;

int main(){
    // Problem source: https://omegaup.com/arena/problem/manhattan/#problems 
  
    // Queremos buscar lo siguiente:
    // Dado N tuplas de 4 numeros  (A[i][4] de i = 1 a N)
    // Calcular max_{i, j = 1 a N} (sum_{p = 0, 1, 2, 3} abs(A[i][p]-A[j][p]) )
    //
    // Resolver el problema para 4 dimensiones esta dificil pensarlo
    // Vamos a pensarlo con dos dimensiones:
    //    - Cuando restamos dos numeros (a1, b1) y (a2, b2)    (abs(a1-a2) + abs(b1-b2)), hay cuatro posibles casos:
    //          * - (a1-a2) + -(b1-b2)
    //          *   (a1-a2) + -(b1-b2)
    //          * - (a1-a2) +  (b1-b2)
    //          *   (a1-a2) +  (b1-b2)
    //    - Entre estas cuatro opciones, la que equivale a abs(a1-a2) + abs(b1-b2) es la que tiene la maxima suma
    //    - Hago enfasis en que debe de quedarnos clara la observacion anterior, es super fundamental
    //    - Otra forma de ver los cuatro casos anteriores es la siguiete (distribuimos la resta y reacomodamos):
    //          *  -a1 + -b1  +  a2 +  b2
    //          *   a1 + -b1  + -a2 +  b2
    //          *  -a1 +  b1  +  a2 + -b2
    //          *   a1 +  b1  + -a2 + -b2
    //    - La siguiente observacion es que al aplicar - o + a los numeros a una tupla, y los signos - y + para la otra tupla son su complemento
    //
    // Observemos que estas propiedades las podemos extender a todas las dimensiones (la cantidad de casos seria 2^k, donde k es la dimension):
    //    - Para cada uno de los 2^k casos (de - y +), existe un complemento de + y -
    //    - Las observaciones anteriores tenian el objetivo de calcular la distancia manhattan sin necesidad de usar el valor absoluto o condiciones ifs
    //    - Ademas, **los calculos de la tupla i son independientes a los de la tupla j**
    //    - Buscamos dos cosas:
    //          * Entre los 2^k casos, encontrar la suma de parejas maxima (que equivaldria a la distancia con el valor absoluto)
    //          * Entre las N^2 parejas, encontrar la pareja que tenga distancia maxima
    //   - De hecho, el calculo de la respuesta cambia a lo siguiente:
    //          *  max_{i, j = 1 a N} ( max_{2^k casos} ( [aplicar - y + a A[i]]  + [aplicar + y - complemento a A[j]] ) )
    //   - Una propiedad que podemos utilizar es que, al ser dos maximos anidados, podemos intercambiar el orden
    //          * max_{i en C} ( max_{j en D} f(i, j) )  =  max_{j en D} ( max_{i en C} f(i, j) )
    //
    // - Entre las ultimas observaciones, ¿qué pasa si nos olvidamos de las N^2 parejas y guardamos el valor maximo que un caso de - y + puede tener?
    //    - De esta forma, podemos evitar el N^2, aunque parece curioso, no?

    
    cin.tie(0);
    ios_base::sync_with_stdio(0);
    int ans = 0;
    
    // Inicializamos los valores para los 2^k casos
    int k = 4;
    int n_casos = (1<<k);
    int D[n_casos];
    fill(D, D+n_casos, -1e9);
    
    // Leemos la entrada
    int N;
    cin>>N;
    for(int i = 0; i < N; i++){
        // Leemos la i-esima tupla
        int A[k];
        for(int j = 0; j < k; j++) cin >> A[j];
        
        // Calculamos los 2^k casos
        for(int d=0; d < n_casos/2; d++){
            int c = 0;
            for (int j = 0; j < k; j++) {
                c += A[j] * ((d&(1<<j))?-1:1);
            }
            D[d] = max(D[d], c);
            D[n_casos-1 - d] = max(D[n_casos-1 - d], -c);
            // c=A[0]*((d&1)?-1:1)+A[1]*((d&2)?-1:1)+A[2]*((d&4)?-1:1)+A[3];
            //D[d]=(D[d]<c?c:D[d]);
            //D[15-d]=(D[15-d]<-c?-c:D[15-d]);
        }
    }
    for(int i = 0; i < n_casos/2; i++)
        ans = max(ans, D[i] + D[15 - i]);
    cout << ans << endl;
}
