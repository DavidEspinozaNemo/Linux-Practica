//Rutina debug para imprimir los bloque de memoria del malloc modificado
#include <stdio.h>
#include <unistd.h>
#include "malloc.h"

void print_bloque(unsigned long nbytes);

int main() {
  int *arreglo;
  arreglo = (int *)malloc(sizeof(int) * 10);
  for (int i = 0; i < 10; i++)
    arreglo[i] = 10 * i + 10;
  for (int i = 0; i < 10; i++)
    printf("Numero: %i, Direccion: %p\n", arreglo[i], &arreglo[i]);
}
