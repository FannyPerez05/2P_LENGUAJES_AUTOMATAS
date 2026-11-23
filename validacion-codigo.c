#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINEA 1024
#define MAX_LINEAS 1000

typedef struct {
    char contenido[MAX_LINEA];
    int numero;
} Linea;
