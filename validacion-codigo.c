
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINEA 1024
#define MAX_LINEAS 1000
#define MAX_ERRORES 500

// Estructura para almacenar las lineas
typedef struct {
    char contenido[MAX_LINEA];
    int numero;
} Linea;

// Estructura para almacenar los errores
typedef struct {
    int linea;
    char tipo[30];
    char mensaje[200];
} Error;

// Variables globales
Error errores[MAX_ERRORES];
int totalErrores = 0;

// Palabras reservadas de C
const char *reservadas[] = {
    "auto", "break", "case", "char", "const",
    "continue", "default", "do", "double", "else",
    "enum", "extern", "float", "for", "goto",
    "if", "int", "long", "register", "return",
    "short", "signed", "sizeof", "static", "struct",
    "switch", "typedef", "union", "unsigned", "void",
    "volatile", "while", "_Bool"
};

// Funciones conocidas de biblioteca
const char *funciones[] = {
    "printf", "scanf", "getchar", "putchar",
    "puts", "gets", "fopen", "fclose", "fgets",
    "malloc", "calloc", "realloc", "free",
    "strlen", "strcpy", "strcmp", "strcat"
};

// Funcion para abrir el archivo
FILE *abrirArchivo(const char *nombre) {

    FILE *archivo = fopen(nombre, "r");

    if (archivo == NULL) {
        printf("Error: No se pudo abrir %s\n", nombre);
        return NULL;
    }

    return archivo;
}

// Funcion para leer el archivo
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

// Funcion para comprobar si una palabra es reservada
int esPalabraReservada(const char *palabra) {

    int total = sizeof(reservadas) / sizeof(reservadas[0]);

    for (int i = 0; i < total; i++) {
        if (strcmp(palabra, reservadas[i]) == 0) {
            return 1;
        }
    }

    return 0;
}

// Funcion para comprobar si es una funcion conocida
int esFuncionValida(const char *palabra) {

    int total = sizeof(funciones) / sizeof(funciones[0]);

    for (int i = 0; i < total; i++) {
        if (strcmp(palabra, funciones[i]) == 0) {
            return 1;
        }
    }

    return 0;
}

// Funcion para calcular la diferencia entre palabras
int distanciaPalabras(const char *a, const char *b) {

    int largoA = strlen(a);
    int largoB = strlen(b);
    int matriz[100][100];

    if (largoA >= 100 || largoB >= 100) {
        return 100;
    }

    for (int i = 0; i <= largoA; i++)
        matriz[i][0] = i;

    for (int j = 0; j <= largoB; j++)
        matriz[0][j] = j;

    for (int i = 1; i <= largoA; i++) {
        for (int j = 1; j <= largoB; j++) {

            int costo = (a[i - 1] == b[j - 1]) ? 0 : 1;

            int borrar = matriz[i - 1][j] + 1;
            int insertar = matriz[i][j - 1] + 1;
            int sustituir = matriz[i - 1][j - 1] + costo;

            int menor = borrar;

            if (insertar < menor)
                menor = insertar;

            if (sustituir < menor)
                menor = sustituir;

            matriz[i][j] = menor;
        }
    }

    return matriz[largoA][largoB];
}

// Funcion para buscar una palabra similar
const char *buscarPalabraSimilar(const char *palabra) {

    static char sugerencia[50];
    int menorDistancia = 3;
    int distancia;
    int totalReservadas = sizeof(reservadas) / sizeof(reservadas[0]);
    int totalFunciones = sizeof(funciones) / sizeof(funciones[0]);

    sugerencia[0] = '\0';

    for (int i = 0; i < totalReservadas + totalFunciones; i++) {

        const char *actual;

        if (i < totalReservadas)
            actual = reservadas[i];
        else
            actual = funciones[i - totalReservadas];

        distancia = distanciaPalabras(palabra, actual);

        if (distancia < menorDistancia) {
            menorDistancia = distancia;
            strcpy(sugerencia, actual);
        }
    }

    return sugerencia;
}

// Funcion para registrar errores
void agregarError(int linea, const char *tipo, const char *mensaje) {

    if (totalErrores >= MAX_ERRORES)
        return;

    errores[totalErrores].linea = linea;

    strcpy(errores[totalErrores].tipo, tipo);
    strcpy(errores[totalErrores].mensaje, mensaje);

    totalErrores++;
}

// Funcion para analizar palabras de una linea
void analizarLexico(Linea linea) {

    char palabra[100];
    int i = 0;
    int j;

    while (linea.contenido[i] != '\0') {

        if (isalpha((unsigned char)linea.contenido[i]) ||
            linea.contenido[i] == '_') {

            j = 0;

            while (isalnum((unsigned char)linea.contenido[i]) ||
                   linea.contenido[i] == '_') {

                if (j < 99)
                    palabra[j++] = linea.contenido[i];

                i++;
            }

            palabra[j] = '\0';

            // Si es una palabra reservada, es correcta
            if (esPalabraReservada(palabra))
                continue;

            // Si es una funcion conocida, es correcta
            if (esFuncionValida(palabra))
                continue;

            // Si es main, tambien es valida
            if (strcmp(palabra, "main") == 0)
                continue;

            // Buscar una posible palabra similar
            const char *similar = buscarPalabraSimilar(palabra);

            if (similar[0] != '\0') {

                char mensaje[200];

                snprintf(mensaje, sizeof(mensaje),
                         "Palabra desconocida '%s'. Posible correccion: '%s'",
                         palabra, similar);

                agregarError(linea.numero, "LEXICO", mensaje);
            }
        }
        else {
            i++;
        }
    }
}

// Funcion para comprobar delimitadores
void verificarDelimitadores(Linea lineas[], int totalLineas) {

    char pila[MAX_LINEAS * 10];
    int lineasPila[MAX_LINEAS * 10];
    int tope = -1;

    for (int i = 0; i < totalLineas; i++) {

        char *texto = lineas[i].contenido;

        for (int j = 0; texto[j] != '\0'; j++) {

            char c = texto[j];

            if (c == '(' || c == '{' || c == '[') {

                if (tope < MAX_LINEAS * 10 - 1) {
                    tope++;
                    pila[tope] = c;
                    lineasPila[tope] = lineas[i].numero;
                }
            }

            else if (c == ')' || c == '}' || c == ']') {

                if (tope == -1) {

                    agregarError(lineas[i].numero,
                                 "SINTACTICO",
                                 "Delimitador de cierre sin apertura.");

                } else {

                    char apertura = pila[tope];

                    if ((c == ')' && apertura == '(') ||
                        (c == '}' && apertura == '{') ||
                        (c == ']' && apertura == '[')) {

                        tope--;

                    } else {

                        agregarError(lineas[i].numero,
                                     "SINTACTICO",
                                     "Los delimitadores no coinciden.");
                    }
                }
            }
        }
    }

    while (tope >= 0) {

        char mensaje[200];

        snprintf(mensaje, sizeof(mensaje),
                 "Falta cerrar el delimitador '%c'.",
                 pila[tope]);

        agregarError(lineasPila[tope], "SINTACTICO", mensaje);

        tope--;
    }
}

// Funcion para verificar punto y coma de forma basica
void verificarPuntoComa(Linea lineas[], int totalLineas) {

    for (int i = 0; i < totalLineas; i++) {

        char copia[MAX_LINEA];

        strcpy(copia, lineas[i].contenido);

        // Eliminar espacios del final
        int largo = strlen(copia);

        while (largo > 0 &&
              (copia[largo - 1] == '\n' ||
               copia[largo - 1] == '\r' ||
               isspace((unsigned char)copia[largo - 1]))) {

            copia[--largo] = '\0';
        }

        if (largo == 0)
            continue;

        // Ignorar comentarios y directivas
        if (strncmp(copia, "//", 2) == 0 ||
            copia[0] == '#')
            continue;

        // Ignorar estructuras que no requieren punto y coma
        if (copia[largo - 1] == '{' ||
            copia[largo - 1] == '}' ||
            copia[largo - 1] == ';' ||
            copia[largo - 1] == ':')
            continue;

        // Ignorar comentarios de cierre de linea
        if (strchr(copia, ';') == NULL &&
            strchr(copia, '=') != NULL) {

            agregarError(lineas[i].numero,
                         "SINTACTICO",
                         "Posible falta de punto y coma.");
        }
    }
}

// Funcion para mostrar todos los errores
void mostrarErrores() {

    printf("\n========== REPORTE DE ERRORES ==========\n");

    if (totalErrores == 0) {
        printf("No se encontraron errores en las revisiones realizadas.\n");
    }

    for (int i = 0; i < totalErrores; i++) {

        printf("\nError %d\n", i + 1);
        printf("Linea: %d\n", errores[i].linea);
        printf("Tipo: %s\n", errores[i].tipo);
        printf("Descripcion: %s\n", errores[i].mensaje);
    }

    printf("\nTotal de errores encontrados: %d\n", totalErrores);
    printf("========================================\n");
}

// Funcion principal
int main() {

    Linea lineas[MAX_LINEAS];

    FILE *archivo;
    int totalLineas;

    archivo = abrirArchivo("codigo.txt");

    if (archivo == NULL)
        return 1;

    totalLineas = leerArchivo(archivo, lineas);

    fclose(archivo);

    printf("\nArchivo leido correctamente.");
    printf("\nTotal de lineas: %d\n", totalLineas);

    // Analisis lexico
    for (int i = 0; i < totalLineas; i++) {
        analizarLexico(lineas[i]);
    }

    // Analisis de delimitadores
    verificarDelimitadores(lineas, totalLineas);

    // Revision basica de punto y coma
    verificarPuntoComa(lineas, totalLineas);

    // Mostrar reporte
    mostrarErrores();

    return 0;
}
