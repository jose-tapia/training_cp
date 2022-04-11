#include <iostream>
#include <vector>
using namespace std;
// Problem source: https://omegaup.com/arena/problem/Subdivision-del-Reino/

const int MaxN = 28;

int N;
// Representaciones del grafo
vector<int> ady[MaxN];
int matriz_suf[MaxN][MaxN];

// Variables para el backtracking
bool usando[MaxN];
int subconjunto_actual[MaxN];

// Mejor respuesta actual
int ans;
int subconjunto_invitado[MaxN];

void dfs(int nodo, int invitados, int regalos) {
    if (invitados == N / 2) {
        // Amigos no-invitados
        for (int i = 0; i < N/2; i++) {
            regalos += matriz_suf[subconjunto_actual[i]][nodo];
        }
        
        if (regalos > ans) {
            // No mejoramos la respuesta
            return;
        }
        // Actualizar mejor cantidad de regalos encontrados
        ans = regalos;
        // Guardar lista de invitados
        for (int i = 0; i < N/2; i++) {
            subconjunto_invitado[i] = subconjunto_actual[i];
        }
        return;
    }
    
    // No hemos alcanzado a la mitad de los invitados
    if (regalos >= ans) {
        // No obtendremos una mejor respuesta que la que ya tenemos
        return;
    }
    if (nodo > N) {
        // Ya no es una posicion valida
        return;
    }
    if (N - nodo + invitados + 1 < N/2) {
        // Restan N-nodo+1 personas
        // Si invitamos a todas y no alcanzamos N/2, es mejor no buscar
        return;
    }
    
    // Contar penalizacion si invitamos o no a 'nodo'
    int si_no_me_invitan = 0;
    int para_no_invitados = 0;
    for (int v: ady[nodo]) {
        if (v > nodo) break;
        if (usando[v]) si_no_me_invitan++;
        else para_no_invitados++;
    }
    
    // Invitar
    usando[nodo] = true;
    subconjunto_actual[invitados] = nodo;
    dfs(nodo + 1, invitados + 1, regalos + para_no_invitados);

    // No invitar
    usando[nodo] = false;
    dfs(nodo + 1, invitados, regalos + si_no_me_invitan);
}

int main(){
    int amistades;
    cin >> N >> amistades;    
    ans = amistades + 1;
  
    // Leer entrada
    for (int i = 0; i < amistades; i++) {
        int a, b;
        cin >> a >> b;
        ady[a].push_back(b);
        ady[b].push_back(a);
        matriz_suf[a][b] = 1;
        matriz_suf[b][a] = 1;
    }
    for (int i = 1; i <= N; i++) {
        // Ordenar adyacentes
        sort(ady[i].begin(), ady[i].end());
        // Sufijos
        // Para responder en constante cuantos amigos mayores o iguales a 'j' tiene 'i'
        for (int j = N-1; j >= 1; j--) {
            matriz_suf[i][j] += matriz_suf[i][j+1];
        }
        
    }
    
    // Invitamos siempre a Freddy
    usando[1] = 1;
    subconjunto_actual[0] = 1;
    // Empezamos en el indice 2, hay un invitado (F) y no hay regalos
    dfs(2, 1, 0);
    
    // Imprimir mejor subconjunto
    for (int i = 0; i < N/2; i++) {
        cout << subconjunto_invitado[i] << " ";
    }
}
