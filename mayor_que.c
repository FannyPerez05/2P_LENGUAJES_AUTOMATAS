#include <stdio.h>

float mayor_que(float a, float b) {
    if (a > b) {
        return a;
    } else {
        return b;
    }
}

float mayor_que2(float mayor, float a) {
    if (mayor > a) {
        return mayor;
    } else {
        return a;
    }
}

int main() {
    float a, b, c, mayor, resultado;

    printf("Ingresa el primer numero: ");
    scanf("%f", &a);

    printf("Ingresa el segundo numero: ");
    scanf("%f", &b);

    mayor = mayor_que(a, b);

    printf("El mayor de los primeros dos es: %.2f\n", mayor);

    printf("Ingresa el tercer numero: ");
    scanf("%f", &c);

    resultado = mayor_que2(mayor, c);

    printf("El mayor de los tres numeros es: %.2f\n", resultado);

    return 0;
}
