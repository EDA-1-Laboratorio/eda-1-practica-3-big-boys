#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    int valor;
    int palo;
} Carta;

typedef struct {
    int rango;
    int desempate[5];
} Mano;

void crearBaraja(Carta baraja[52]) {
    int i = 0;
    for (int palo = 0; palo < 4; palo++) {
        for (int valor = 1; valor <= 13; valor++) {
            baraja[i].valor = valor;
            baraja[i].palo = palo;
            i++;
        }
    }
}

void barajar(Carta baraja[52]) {
    for (int i = 51; i > 0; i--) {
        int j = rand() % (i + 1);
        Carta temp = baraja[i];
        baraja[i] = baraja[j];
        baraja[j] = temp;
    }
}

void imprimirCarta(Carta c) {
    char *valores[] = {"", "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
    char *palos[] = {"Treboles", "Espadas", "Corazones", "Diamantes"};
    printf("%s de %s\n", valores[c.valor], palos[c.palo]);
}

int max(int a, int b) {
    return a > b ? a : b;
}

void ordenarValores(int v[5]) {
    for (int i = 0; i < 4; i++) {
        for (int j = i + 1; j < 5; j++) {
            if (v[i] > v[j]) {
                int t = v[i];
                v[i] = v[j];
                v[j] = t;
            }
        }
    }
}

Mano evaluar5(Carta c[5]) {
    Mano m;
    m.rango = 0;
    for (int i = 0; i < 5; i++) m.desempate[i] = 0;

    int v[5];
    int palos[5];
    for (int i = 0; i < 5; i++) {
        int valor = c[i].valor;
        if (valor == 1) valor = 14;
        v[i] = valor;
        palos[i] = c[i].palo;
    }

    ordenarValores(v);

    int esColor = 1;
    for (int i = 1; i < 5; i++) {
        if (palos[i] != palos[0]) {
            esColor = 0;
            break;
        }
    }

    int esCorrida = 0;
    int altaCorrida = 0;
    int unica = 1;
    for (int i = 1; i < 5; i++) {
        if (v[i] == v[i - 1]) unica = 0;
    }
    if (unica && v[0] + 1 == v[1] && v[1] + 1 == v[2] && v[2] + 1 == v[3] && v[3] + 1 == v[4]) {
        esCorrida = 1;
        altaCorrida = v[4];
    }
    if (v[0] == 2 && v[1] == 3 && v[2] == 4 && v[3] == 5 && v[4] == 14) {
        esCorrida = 1;
        altaCorrida = 5;
    }

    int conteo[15];
    for (int i = 0; i < 15; i++) conteo[i] = 0;
    for (int i = 0; i < 5; i++) conteo[v[i]]++;

    int cuatro = 0;
    int tres = 0;
    int pares[2];
    pares[0] = pares[1] = 0;
    int numPares = 0;

    for (int val = 14; val >= 2; val--) {
        if (conteo[val] == 4) cuatro = val;
        if (conteo[val] == 3) tres = val;
        if (conteo[val] == 2) {
            if (numPares < 2) pares[numPares] = val;
            numPares++;
        }
    }

    if (esColor && esCorrida && altaCorrida == 14) {
        m.rango = 9;
        m.desempate[0] = 14;
        return m;
    }

    if (esColor && esCorrida) {
        m.rango = 8;
        m.desempate[0] = altaCorrida;
        return m;
    }

    if (cuatro) {
        m.rango = 7;
        m.desempate[0] = cuatro;
        for (int val = 14, idx = 1; val >= 2 && idx < 5; val--) {
            if (conteo[val] == 1) {
                m.desempate[idx] = val;
                idx++;
            }
        }
        return m;
    }

    if (tres && numPares >= 1) {
        m.rango = 6;
        m.desempate[0] = tres;
        m.desempate[1] = pares[0];
        return m;
    }

    if (esColor) {
        m.rango = 5;
        int idx = 0;
        for (int i = 4; i >= 0 && idx < 5; i--) {
            m.desempate[idx++] = v[i];
        }
        return m;
    }

    if (esCorrida) {
        m.rango = 4;
        m.desempate[0] = altaCorrida;
        return m;
    }

    if (tres) {
        m.rango = 3;
        m.desempate[0] = tres;
        int idx = 1;
        for (int val = 14; val >= 2 && idx < 5; val--) {
            if (conteo[val] == 1) {
                m.desempate[idx++] = val;
            }
        }
        return m;
    }

    if (numPares >= 2) {
        m.rango = 2;
        int parAlto = max(pares[0], pares[1]);
        int parBajo = pares[0] + pares[1] - parAlto;
        m.desempate[0] = parAlto;
        m.desempate[1] = parBajo;
        for (int val = 14, idx = 2; val >= 2 && idx < 5; val--) {
            if (conteo[val] == 1) {
                m.desempate[idx++] = val;
            }
        }
        return m;
    }

    if (numPares == 1) {
        m.rango = 1;
        m.desempate[0] = pares[0];
        int idx = 1;
        for (int val = 14; val >= 2 && idx < 5; val--) {
            if (conteo[val] == 1) {
                m.desempate[idx++] = val;
            }
        }
        return m;
    }

    m.rango = 0;
    int idx = 0;
    for (int i = 4; i >= 0 && idx < 5; i--) {
        m.desempate[idx++] = v[i];
    }
    return m;
}

int compararManos(Mano a, Mano b) {
    if (a.rango > b.rango) return 1;
    if (a.rango < b.rango) return -1;
    for (int i = 0; i < 5; i++) {
        if (a.desempate[i] > b.desempate[i]) return 1;
        if (a.desempate[i] < b.desempate[i]) return -1;
    }
    return 0;
}

Mano mejorMano(Carta cartas[7]) {
    Mano mejor;
    mejor.rango = -1;
    for (int i = 0; i < 5; i++) mejor.desempate[i] = 0;

    Carta combo[5];
    for (int a = 0; a < 7; a++) {
        for (int b = a + 1; b < 7; b++) {
            for (int c2 = b + 1; c2 < 7; c2++) {
                for (int d = c2 + 1; d < 7; d++) {
                    for (int e = d + 1; e < 7; e++) {
                        combo[0] = cartas[a];
                        combo[1] = cartas[b];
                        combo[2] = cartas[c2];
                        combo[3] = cartas[d];
                        combo[4] = cartas[e];
                        Mano actual = evaluar5(combo);
                        if (mejor.rango == -1 || compararManos(actual, mejor) > 0) {
                            mejor = actual;
                        }
                    }
                }
            }
        }
    }
    return mejor;
}

const char *nombreMano(Mano m) {
    switch (m.rango) {
        case 9: return "Corrida real";
        case 8: return "Corrida de color";
        case 7: return "Poker";
        case 6: return "Casa llena";
        case 5: return "Color";
        case 4: return "Corrida";
        case 3: return "Tercia";
        case 2: return "Dos pares";
        case 1: return "Un par";
        default: return "Carta mas alta";
    }
}

int main() {
    printf("/* Elaborado por Ollin Enrique Casañas de la Cruz */\n");

    srand((unsigned int)time(NULL));

    Carta baraja[52];
    crearBaraja(baraja);
    barajar(baraja);

    int indice = 0;
    Carta jugador1[2];
    Carta jugador2[2];
    Carta river[5];

    jugador1[0] = baraja[indice++];
    jugador2[0] = baraja[indice++];
    jugador1[1] = baraja[indice++];
    jugador2[1] = baraja[indice++];

    printf("Cartas jugador 1:\n");
    imprimirCarta(jugador1[0]);
    imprimirCarta(jugador1[1]);

    printf("Cartas jugador 2:\n");
    imprimirCarta(jugador2[0]);
    imprimirCarta(jugador2[1]);

    char r1, r2;
    int sigue1 = 1, sigue2 = 1;

    printf("Jugador 1, te quedas en el juego? (s/n): ");
    scanf(" %c", &r1);
    if (r1 != 's' && r1 != 'S') sigue1 = 0;

    printf("Jugador 2, te quedas en el juego? (s/n): ");
    scanf(" %c", &r2);
    if (r2 != 's' && r2 != 'S') sigue2 = 0;

    if (!sigue1 && !sigue2) {
        printf("Ambos jugadores se retiraron. No hay ganador.\n");
        return 0;
    }
    if (!sigue1 && sigue2) {
        printf("Jugador 2 gana porque el jugador 1 se retiro.\n");
        return 0;
    }
    if (sigue1 && !sigue2) {
        printf("Jugador 1 gana porque el jugador 2 se retiro.\n");
        return 0;
    }

    for (int i = 0; i < 3; i++) {
        river[i] = baraja[indice++];
    }

    printf("River (3 cartas):\n");
    for (int i = 0; i < 3; i++) imprimirCarta(river[i]);

    printf("Jugador 1, te quedas en el juego? (s/n): ");
    scanf(" %c", &r1);
    if (r1 != 's' && r1 != 'S') sigue1 = 0;

    printf("Jugador 2, te quedas en el juego? (s/n): ");
    scanf(" %c", &r2);
    if (r2 != 's' && r2 != 'S') sigue2 = 0;

    if (!sigue1 && !sigue2) {
        printf("Ambos jugadores se retiraron. No hay ganador.\n");
        return 0;
    }
    if (!sigue1 && sigue2) {
        printf("Jugador 2 gana porque el jugador 1 se retiro.\n");
        return 0;
    }
    if (sigue1 && !sigue2) {
        printf("Jugador 1 gana porque el jugador 2 se retiro.\n");
        return 0;
    }

    river[3] = baraja[indice++];

    printf("River (4 cartas):\n");
    for (int i = 0; i < 4; i++) imprimirCarta(river[i]);

    printf("Jugador 1, te quedas en el juego? (s/n): ");
    scanf(" %c", &r1);
    if (r1 != 's' && r1 != 'S') sigue1 = 0;

    printf("Jugador 2, te quedas en el juego? (s/n): ");
    scanf(" %c", &r2);
    if (r2 != 's' && r2 != 'S') sigue2 = 0;

    if (!sigue1 && !sigue2) {
        printf("Ambos jugadores se retiraron. No hay ganador.\n");
        return 0;
    }
    if (!sigue1 && sigue2) {
        printf("Jugador 2 gana porque el jugador 1 se retiro.\n");
        return 0;
    }
    if (sigue1 && !sigue2) {
        printf("Jugador 1 gana porque el jugador 2 se retiro.\n");
        return 0;
    }

    river[4] = baraja[indice++];

    printf("River completo (5 cartas):\n");
    for (int i = 0; i < 5; i++) imprimirCarta(river[i]);

    Carta total1[7];
    Carta total2[7];
    total1[0] = jugador1[0];
    total1[1] = jugador1[1];
    total2[0] = jugador2[0];
    total2[1] = jugador2[1];

    for (int i = 0; i < 5; i++) {
        total1[2 + i] = river[i];
        total2[2 + i] = river[i];
    }

    Mano mano1 = mejorMano(total1);
    Mano mano2 = mejorMano(total2);

    printf("Mejor mano jugador 1: %s\n", nombreMano(mano1));
    printf("Mejor mano jugador 2: %s\n", nombreMano(mano2));

    int cmp = compararManos(mano1, mano2);
    if (cmp > 0) {
        printf("Jugador 1 gana la partida.\n");
    } else if (cmp < 0) {
        printf("Jugador 2 gana la partida.\n");
    } else {
        printf("Empate.\n");
    }

    return 0;
}
