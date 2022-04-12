#include <iostream>
using namespace std;
// Problem source: https://omegaup.com/arena/problem/Freddievecithan/

int main(){
    cin.tie(0);
    ios_base::sync_with_stdio(0);
    
    int N;
    string A;
    cin >> N >> A;
    
    if (N%2 == 0) {
        // No puede ser par
        cout << "NOT POSSIBLE\n";
        return 0;
    }
    
    int sz = N/2;
    int posibilidades = 0;
    
    int errores = 0;
    for (int idx_a = 0, cnt_b = 0; cnt_b < sz; idx_a++) {
        // Indice en la segunda mitad
        int idx_b = cnt_b + sz + 1;
        if (A[idx_a] == A[idx_b]) {
            // Todo bien
            cnt_b++;
        } else {
            // Un error extra
            errores++;
        }
        if (errores > 1) {
            // Muchos errores
            break;
        }
    }
    if (errores <= 1) {
        posibilidades |= 1;
    }
    // Distinto inicio
    errores = 0;
    for (int cnt_a = 0, idx_b = 0; idx_b < sz; cnt_a++) {
        // Indice en la segunda mitad
        int idx_a = cnt_a + sz;
        if (A[idx_a] == A[idx_b]) {
            // Todo bien
            idx_b++;
        } else {
            // Un error extra
            errores++;
        }
        if (errores > 1) {
            // Muchos errores
            break;
        }
    }
    if (errores <= 1) {
        posibilidades |= 2;
    }    
    
    if (posibilidades == 3) {
        // Hay dos posibilidades
        int errores = 0;
        for (int i = 0; i < sz; i++) {
            if (A[i] != A[i+sz+1]) errores++;
        }
        
        if (errores == 0) {
            // Es palindromo
            posibilidades -= 2;
        } else {
            // Ambas son distintas
            cout << "NOT UNIQUE\n";
            return 0;
        }
    }
    if (posibilidades == 0) {
        // No existe caso
        cout << "NOT POSSIBLE\n";
        return 0;
    }
    if (posibilidades == 1) {
        for (int i = sz+1; i < N; i++) {
            cout << A[i];
        }
        cout << "\n";
        return 0;
    }
    if (posibilidades == 2) {
        for (int i = 0; i < sz; i++) {
            cout << A[i];
        }
        cout << "\n";
        return 0;
    }
}
