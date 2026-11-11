#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 200

/* Recibe como parametro c, el cual es un caracter que representa
 * un operador matematico.
 * Identifica la precedencia de los operadores matematicos,
 * los cuales pueden ser (^, *, /, +, -).
 * Retorna un numero entero que representa la prioridad de los
 * operadores: 3 para potencia, 2 para multiplicacion o division,
 * 1 para suma o resta y 0 si no es un operador reconocido.
 */
int prioridad(char c) {
    if (c == '^') return 3;
    else if (c == '*' || c == '/') return 2;
    else if (c == '+' || c == '-') return 1;
    return 0;
}


/* Recibe como parametro c, el cual es un caracter.
 * Comprueba si el caracter recibido corresponde a un operador
 * matematico, los cuales pueden ser (+, -, *, /, ^).
 * Retorna 1 si c es un operador y 0 en caso contrario.
 */
int es_operador(char c) {
    return c == '+' || c == '-' ||
           c == '*' || c == '/' || c == '^';
}


/* Recibe como parametro c, el cual es un caracter.
 * Comprueba si el caracter corresponde a un operando,
 * considerando letras, numeros y guion bajo (_).
 * Retorna 1 si c es un operando y 0 en caso contrario.
 */
int es_operando(char c) {
    return isalnum((unsigned char)c) || c == '_';
}


/* Recibe dos cadenas de texto: infija[] y posfija[].
 * infija[] contiene la expresion matematica original y posfija[]
 * es donde se almacenara el resultado de la conversion.
 * Convierte una expresion infija a posfija mediante el uso de una pila.
 * Recorre la expresion de izquierda a derecha.
 * Los operandos se agregan directamente al resultado.
 * Los operadores se almacenan en la pila considerando su prioridad.
 * Los parentesis permiten controlar el orden de las operaciones.
 * Finalmente, se extraen los operadores restantes de la pila.
 * Retorna 1 si la conversion se realiza correctamente y 0 si encuentra
 * algun caracter no valido, parentesis incorrectos o falta de espacio.
 */
int convertir(char infija[], char posfija[]) {

    // Pila utilizada para almacenar temporalmente operadores y parentesis.
    char pila[MAX];

    // Indica la posicion del ultimo elemento de la pila.
    // Se inicializa en -1 porque la pila comienza vacia.
    int tope = -1;

    // Indica la posicion donde se agregaran los caracteres al resultado.
    int j = 0;

    // Variable utilizada para recorrer la expresion.
    int i;

    // Se recorre la expresion infija caracter por caracter.
    for (i = 0; infija[i] != '\0'; i++) {

        // Se obtiene el caracter que se esta analizando.
        char c = infija[i];

        // Se comprueba si el caracter es un espacio en blanco.
        if (isspace((unsigned char)c))
            continue; // Se omite y se continua con el siguiente caracter.

        // Se comprueba si el caracter es un parentesis izquierdo.
        if (c == '(') {

            // Se verifica que la pila tenga espacio disponible.
            if (tope >= MAX - 1)
                return 0;

            // Se agrega el parentesis izquierdo a la pila.
            pila[++tope] = c;
        }

        // Se comprueba si el caracter es un parentesis derecho.
        else if (c == ')') {

            // Se extraen los operadores hasta encontrar el parentesis izquierdo.
            while (tope >= 0 && pila[tope] != '(') {

                // Se verifica que exista espacio en el resultado.
                if (j >= MAX - 1)
                    return 0;

                // Se agrega el operador de la cima de la pila a posfija.
                posfija[j++] = pila[tope--];
            }

            // Si la pila esta vacia, no existe un parentesis izquierdo.
            if (tope < 0)
                return 0;

            // Se elimina el parentesis izquierdo de la pila.
            // No se agrega a la expresion posfija.
            tope--;
        }

        // Se comprueba si el caracter es un operando.
        else if (es_operando(c)) {

            // Se verifica que exista espacio disponible en posfija.
            if (j >= MAX - 1)
                return 0;

            // Se agrega directamente el operando al resultado posfijo.
            posfija[j++] = c;
        }

        // Si no es parentesis ni operando, se comprueba si es operador.
        else if (es_operador(c)) {

            // Se comparan las prioridades del operador actual
            // y del operador que se encuentra en la cima de la pila.
            // Mientras la prioridad actual sea menor o igual,
            // se extraen los operadores de la pila.
            while (tope >= 0 && pila[tope] != '(' &&
                   prioridad(c) <= prioridad(pila[tope])) {

                // Se verifica que exista espacio en el resultado.
                if (j >= MAX - 1)
                    return 0;

                // Se agrega el operador de la pila a la expresion posfija.
                posfija[j++] = pila[tope--];
            }

            // Se verifica que la pila tenga espacio disponible.
            if (tope >= MAX - 1)
                return 0;

            // Se agrega el operador actual a la pila.
            pila[++tope] = c;
        }

        // Si el caracter no pertenece a ninguno de los casos anteriores,
        // significa que se encontro un caracter no reconocido.
        else {
            return 0;
        }
    }

    // Se extraen los operadores que quedaron pendientes en la pila.
    while (tope >= 0) {

        // Se comprueba si quedo un parentesis izquierdo sin cerrar.
        if (pila[tope] == '(')
            return 0;

        // Se verifica que exista espacio en el resultado posfijo.
        if (j >= MAX - 1)
            return 0;

        // Se agrega el operador de la pila al resultado.
        posfija[j++] = pila[tope--];
    }

    // Se agrega el caracter nulo para finalizar la cadena posfija.
    posfija[j] = '\0';

    // Se retorna 1 para indicar que la conversion fue exitosa.
    return 1;
}


/* No recibe parametros de entrada porque es la funcion principal.
 * Se encarga de abrir el archivo expresiones.txt y leer las expresiones
 * matematicas linea por linea.
 * Ignora las lineas vacias, llama a convertir() para transformar
 * cada expresion infija a posfija y muestra los resultados en pantalla.
 * Tambien comprueba si ocurren errores al abrir o leer el archivo.
 * Retorna 0 si el programa finaliza correctamente y 1 si no se pudo
 * abrir el archivo.
 */
int main(void) {

    // Se declara un puntero FILE para trabajar con el archivo.
    FILE *archivo;

    // Arreglo donde se almacenara cada expresion leida del archivo.
    char linea[MAX];

    // Arreglo donde se guardara el resultado de la conversion posfija.
    char posfija[MAX];

    // Contador que permite identificar el numero de cada expresion.
    int numero = 0;

    // Variable utilizada para recorrer los caracteres de cada linea.
    int i;

    // Se abre el archivo expresiones.txt en modo lectura.
    archivo = fopen("expresiones.txt", "r");

    // Se comprueba si el archivo se pudo abrir correctamente.
    if (archivo == NULL) {

        // Se muestra un mensaje indicando que no se pudo abrir el archivo.
        printf("Error: no se pudo abrir expresiones.txt\n");

        // Se retorna 1 para indicar que ocurrio un error.
        return 1;
    }

    // Se muestra el titulo del programa.
    printf("CONVERSION DE INFIJA A POSFIJA\n");

    // Se leen las expresiones del archivo linea por linea.
    while (fgets(linea, sizeof(linea), archivo) != NULL) {

        // Se eliminan los saltos de linea que pueda contener la expresion.
        linea[strcspn(linea, "\r\n")] = '\0';

        // Se inicializa el recorrido de la linea desde la primera posicion.
        i = 0;

        // Se avanzan las posiciones mientras existan espacios en blanco.
        while (isspace((unsigned char)linea[i]))
            i++;

        // Se comprueba si la linea esta vacia.
        if (linea[i] == '\0')
            continue; // Se ignora la linea y se lee la siguiente.

        // Se incrementa el contador de expresiones procesadas.
        numero++;

        // Se muestra el numero y el contenido de la expresion original.
        printf("\nExpresion %d: %s\n", numero, linea);

        // Se llama a convertir() para realizar la conversion a posfija.
        // Si retorna 1, significa que la conversion fue correcta.
        if (convertir(linea, posfija)) {

            // Se muestra el encabezado del resultado.
            printf("Expresion posfija: ");

            // Se recorre la cadena posfija hasta encontrar el final.
            for (i = 0; posfija[i] != '\0'; i++)

                // Se imprime cada caracter separado por un espacio.
                printf("%c ", posfija[i]);

            // Se realiza un salto de linea despues del resultado.
            printf("\n");
        }

        // Si convertir() retorna 0, se ejecuta este bloque.
        else {

            // Se informa que la expresion no pudo convertirse.
            printf("Error: expresion o parentesis incorrectos.\n");
        }
    }

    // Se comprueba si ocurrio algun error durante la lectura del archivo.
    if (ferror(archivo))
        printf("Error al leer el archivo.\n");

    // Se cierra el archivo para liberar los recursos utilizados.
    fclose(archivo);

    // Se retorna 0 indicando que el programa finalizo correctamente.
    return 0;
}
