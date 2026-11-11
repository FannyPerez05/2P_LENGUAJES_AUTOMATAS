
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINEA 1024
#define MAX_LINEAS 1000

// Estructura para almacenar cada linea del archivo
typedef struct {
    char contenido[MAX_LINEA];
    int numero;
} Linea;

// Funcion para abrir el archivo
FILE *abrirArchivo(const char *nombre) {

    FILE *archivo = fopen(nombre, "r");

    if (archivo == NULL) {
        printf("Error: No se pudo abrir el archivo %s\n", nombre);
        return NULL;
    }

    return archivo;
}

// Funcion para leer el archivo completo
int leerArchivo(FILE *archivo, Linea lineas[]) {

    int contador = 0;

    while (contador < MAX_LINEAS &&
           fgets(lineas[contador].contenido,
                 MAX_LINEA, archivo) != NULL) {

        lineas[contador].numero = contador + 1;

        contador++;
    }

    return contador;
}

// Funcion para mostrar el contenido leido
void mostrarArchivo(Linea lineas[], int totalLineas) {

    printf("\n========== CODIGO LEIDO ==========\n");

    for (int i = 0; i < totalLineas; i++) {

        printf("Linea %d: %s",
               lineas[i].numero,
               lineas[i].contenido);
    }

    printf("\n==================================\n");
}

// Funcion principal
int main() {

    Linea lineas[MAX_LINEAS];

    FILE *archivo;
    int totalLineas;

    // Abrir el archivo
    archivo = abrirArchivo("codigo.txt");

    if (archivo == NULL) {
        return 1;
    }

    // Leer el archivo
    totalLineas = leerArchivo(archivo, lineas);

    // Cerrar el archivo
    fclose(archivo);

    // Mostrar el contenido
    mostrarArchivo(lineas, totalLineas);

    printf("\nArchivo leido correctamente.\n");

    return 0;
}
