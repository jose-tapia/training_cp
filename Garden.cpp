#include <iostream>
using namespace std;
// Problem source: https://dmoj.ca/problem/ioi05p1
const int MaxN = 300;
const int INF = (1 << 30);

// Prefijos de matriz para obtener subsumas en tiempo constante
int N, M, K;
int mat_orig[MaxN][MaxN];

void flip_axes() {
    for (int i = 0; i <= max(N, M); i++) {
        for (int j = i; j <= max(N, M); j++) {
            swap(mat_orig[i][j], mat_orig[j][i]);
        }
    }
    swap(N, M);
}

int mat[MaxN][MaxN];
void precalculo_matriz() {
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= M; j++) {
            mat[i][j] = mat_orig[i][j] + mat[i-1][j] + mat[i][j-1] - mat[i-1][j-1];
        }
    }
}
int compute_subsum(int x,int y,int p,int q){
    if(x>p||y>q)return 0;
    return mat[p][q]+mat[x-1][y-1]-mat[p][y-1]-mat[x-1][q];
}

int between_rows[MaxN][MaxN];
int up_to[MaxN], down_to[MaxN];

int solve_horizontal() {
    // La idea es la siguiente: Si tenemos dos rectangulos que no
    // se sobrelapan, entonces hay una linea horizontal o vertical
    // que las puede separar perfectamente.
    //
    // Vamos a enfocarnos en el caso de la linea horizontal
    //
    // El problema se simplifica y tenemos que buscar para cada
    // sufijo de filas (o prefijo de filas) el rectangulo con el
    // menor perimetro posible
    
    // Para obtener la suma de submatrices rapido
    precalculo_matriz();
    
    // Vamos a delimitar nuestro rectangulo entre las filas 'i'
    // y 'j'. Variaremos entre las posibles columnas para encontrar
    // el rectangulo con menor perimetro
    for (int i = 1; i <= N; i++) {
        for (int j = i; j <= N; j++) {
            // (1<<30) es un perimetro grande. Si se mantiene ese
            // valor, no encontramos un rectangulo valido
            between_rows[i][j] = INF;
            
            // Vamos a hacer un sliding-window para encontrar los
            // rectangulos con perimetros optimos
            for (int izq = 1, der = 1; der <= M; der++) {
                while (compute_subsum(i, izq, j, der) > K) {
                    // El rectangulo tiene muchas rosas, hay que hacerlo mas chico
                    izq++;
                }
                if (compute_subsum(i, izq, j, der) == K) {
                    // Es un rectangulo valido
                    // Vamos a hacerlo mas chico
                    while (compute_subsum(i, izq, j, der) == K) {
                        // El rectangulo tiene muchas rosas, hay que hacerlo mas chico
                        izq++;
                    }
                    izq--; // izq es invalido, izq-1 es valido
                    // Rectangulo con lados (j-i+1) y (der-izq+1)
                    int perimetro = 2 * (j-i+1) + 2 * (der-izq+1);
                    between_rows[i][j] = min(between_rows[i][j], perimetro);
                }
            }
        }
    }
    
    
    up_to[0] = down_to[N+1] = INF;
    
    // up_to tendra el perimetro mas chico entre todos los rectangulos
    // validos con filas menores o iguales a 'i'
    for (int i = 1; i <= N; i++) {
        up_to[i] = up_to[i-1];
        for (int j = 1; j <= i; j++) {
            // Cubrimos todos los que tengan como limite superior i]
            up_to[i] = min(up_to[i], between_rows[j][i]);
        }
    }
    
    // down_to tendra el perimetro mas chico entre todos los rectangulos
    // validos con filas mayores o iguales a 'i'
    for (int i = N; i >= 1; i--) {
        down_to[i] = down_to[i+1];
        for (int j = i; j <= N; j++) {
            // Cubrimos todos los que tengan como limite inferior [i
            down_to[i] = min(down_to[i], between_rows[i][j]);
        }
    }
    
    int ans = INF;
    for (int i = 1; i < N; i++) {
        // Un rectangulo con filas menores o iguales a 'i'
        // Y otro rectangulo con filas mayores o iguales a 'i+1'
        // (Linea divisoria 'i')
        ans = min(ans, up_to[i] + down_to[i+1]);
    }
    return ans;
}

int main(){
    cin.tie(0);
    ios_base::sync_with_stdio(0);
    
    // Lectura de entrada
    int Roses;
    cin >> N >> M >> Roses >> K;
    for (int i = 0; i < Roses; i++) {
        int x, y;
        cin >> x >> y;
        mat_orig[x][y]++;
    }
    
    // Cubrir filas horizontales
    int ans = solve_horizontal();
    // Volteamos la matriz para cubrir la fila vertical
    flip_axes();
    ans = min(ans, solve_horizontal());

    // Verificar respuesta
    if (ans == INF) {
        // No hay ningun rectangulo valido
        cout << "NO\n";
    } else {
        // Respuesta
        cout << ans << "\n";
    }
}
