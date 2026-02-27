#include <stdio.h>

#define PRECIO_CHICA 100.0
#define PRECIO_MEDIANA 150.0
#define PRECIO_GRANDE 200.0
#define PRECIO_TOPPING 15.0
#define MAX_PIZZAS 100

typedef struct {
    int tamano; 
    int toppings;
    double precio;
} Pizza;

void leerPizza(Pizza *p, int indice) {
    int opcion;
    printf("\nPizza %d\n", indice + 1);
    printf("Elige tamano (1=Chica, 2=Mediana, 3=Grande): ");
    scanf("%d", &opcion);
    while (opcion < 1 || opcion > 3) {
        printf("Opcion invalida. Elige 1, 2 o 3: ");
        scanf("%d", &opcion);
    }
    p->tamano = opcion;
    printf("Cantidad de toppings: ");
    scanf("%d", &p->toppings);
    if (p->toppings < 0) p->toppings = 0;

    double base = 0.0;
    if (p->tamano == 1) base = PRECIO_CHICA;
    else if (p->tamano == 2) base = PRECIO_MEDIANA;
    else base = PRECIO_GRANDE;

    p->precio = base + p->toppings * PRECIO_TOPPING;
}

const char *nombreTamano(int t) {
    if (t == 1) return "Chica";
    if (t == 2) return "Mediana";
    return "Grande";
}

double calcularDescuentoPorTamano(Pizza pizzas[], int n, int tamano) {
    double precios[MAX_PIZZAS];
    int cuenta = 0;

    for (int i = 0; i < n; i++) {
        if (pizzas[i].tamano == tamano) {
            precios[cuenta] = pizzas[i].precio;
            cuenta++;
        }
    }

    if (cuenta < 2) return 0.0;

    for (int i = 0; i < cuenta - 1; i++) {
        for (int j = i + 1; j < cuenta; j++) {
            if (precios[i] > precios[j]) {
                double tmp = precios[i];
                precios[i] = precios[j];
                precios[j] = tmp;
            }
        }
    }

    int pares = cuenta / 2;
    double descuento = 0.0;
    for (int i = 0; i < pares; i++) {
        descuento += precios[i];
    }

    return descuento;
}

int main() {
    
    int n;
    Pizza pizzas[MAX_PIZZAS];

    printf("Cuantas pizzas quieres? ");
    scanf("%d", &n);
    if (n <= 0) {
        printf("No se ordenaron pizzas.\n");
        return 0;
    }
    if (n > MAX_PIZZAS) n = MAX_PIZZAS;

    for (int i = 0; i < n; i++) {
        leerPizza(&pizzas[i], i);
    }

    double subtotal = 0.0;
    for (int i = 0; i < n; i++) {
        subtotal += pizzas[i].precio;
    }

    double descuentoChica = calcularDescuentoPorTamano(pizzas, n, 1);
    double descuentoMediana = calcularDescuentoPorTamano(pizzas, n, 2);
    double descuentoGrande = calcularDescuentoPorTamano(pizzas, n, 3);
    double descuentoTotal = descuentoChica + descuentoMediana + descuentoGrande;
    double total = subtotal - descuentoTotal;

    printf("\n========== TICKET ==========\n");
    for (int i = 0; i < n; i++) {
        printf("Pizza %d: %s, toppings: %d, precio: $%.2f\n",
               i + 1,
               nombreTamano(pizzas[i].tamano),
               pizzas[i].toppings,
               pizzas[i].precio);
    }
    printf("----------------------------\n");
    printf("Subtotal: $%.2f\n", subtotal);
    printf("Descuento promocion 2x1: -$%.2f\n", descuentoTotal);
    printf("Total a pagar: $%.2f\n", total);
    printf("============================\n");

    return 0;
}
