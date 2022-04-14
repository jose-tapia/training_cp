#include <iostream>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>
using namespace std;
// Problem source: https://omegaup.com/arena/problem/La-secuencia-de-Turing#problems

const int MaxN = 1e6+10;
// DAG con unordered_map para solo crear los vectores necesarios (minimizar la memoria)
unordered_map<int,vector<int> > ady;
// Cantidad de ancestros que tiene cada numero
unordered_map<int, int> degree_out;
// Set de numeros que hay en la secuencia
unordered_set<int> existen;

int main(){
    cin.tie(0);
    ios_base::sync_with_stdio(0);

    // Leer entrada
    int N; // Cantidad de secuencias
    cin >> N;
    for (int i = 0; i < N; i++) {
        // Secuencia
        int K, prev_val, curr_val;
        cin >> K;
        for (int j = 0; j < K; j++) {
            cin >> curr_val;
            existen.insert(curr_val);
            if (0 < j) {
                // Indicamos que antes de la variable 'curr_val' debe estar 'prev_val'
                ady[prev_val].push_back(curr_val);
                // Aumentamos la cantidad de predecesores de 'curr_val'
                degree_out[curr_val]++;
            }
            prev_val = curr_val;
        }
    }
    
    // Usamos priority_queue para generar la secuencia menor lexicograficamente
    priority_queue<int, vector<int>, greater<int> > sequence_queue;
    for (int v: existen) {
        if (degree_out[v] == 0) {
            // Aquellos valores con degree_out igual a cero son los unicos que
            // pueden ser incluidos sin generar conflicos
            sequence_queue.push(v);
        }
    }
    
    while (!sequence_queue.empty()) {
        int next_value = sequence_queue.top();
        sequence_queue.pop();
        
        // Incluir en la secuencia / imprimir directamente (asi ahorramos memoria)
        cout << next_value << " ";
        for (int child_value: ady[next_value]) {
            degree_out[child_value]--;
            if (degree_out[child_value] == 0) {
                // Ya arreglamos todos los conflictos
                sequence_queue.push(child_value);
            }
        }
    }
}
