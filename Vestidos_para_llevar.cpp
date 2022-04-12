#include "vestidos.h"
// Problem source: https://omegaup.com/arena/problem/Vestidos-para-llevar/

const int MaxN = 151;

int cnt_vestidos;
int reunion[MaxN];

int festejar(int x, int y, int persona) {
    // Organizamos una fiesta con las personas entre [x, y) y la nueva persona
    int mini_fiesta[y-x+1];
    for (int i = x; i < y; i++) {
        mini_fiesta[i-x] = reunion[i];
    }
    mini_fiesta[y-x] = persona;
    return fiesta(y-x+1, mini_fiesta);
}

int encontrar_vestido(int persona) {
    // Hay un vestido similar a 'vestido' y vamos a encontrarlo
    int in = 0, fn = cnt_vestidos, m;
    // En [in, fn) esta el 'vestido'
    while (in + 1 < fn) {
        m = (in+fn)/2;
        if (festejar(in, m, persona) == m-in) {
            // El vestido se encuentra en la mitad izquierda
            fn = m;
        } else {
            // El vestido se encuentra en la mitad derecha
            in = m;
        }
    }
    // El vestido que es igual a 'vestido' esta en 'in' (Entonces el color es 'in+1'
    return in + 1;
}

int colorear_vestido(int persona) {
    // Con una query podemos saber si es un vestido nuevo o repetido
    if (festejar(0, cnt_vestidos, persona) == cnt_vestidos) {
        // Se repite el vestido
        return encontrar_vestido(persona);
    } else {
        // Es un vestido nuevo
        reunion[cnt_vestidos] = persona;
        cnt_vestidos += 1;
        return cnt_vestidos;
    }
}

void determinaVestidos(int N) {
    int color_vestido[N+1];

    // Caso base
    // Asignar valores para el primer vestido
    cnt_vestidos = 1;     // Hay un solo vestido
    color_vestido[1] = 1; // El color para la persona '1' es '1'
    reunion[0] = 1;       // La unica persona con ese color es '1'
    
    // Vamos buscando el color para cada persona
    for (int persona = 2; persona <= N; persona++) {
        color_vestido[persona] = colorear_vestido(persona);
    }
    
    respuesta(color_vestido);
}
