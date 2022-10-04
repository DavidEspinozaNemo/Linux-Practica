//example.c
#include <stdio.h>
#include <unistd.h>
#include "malloc.h"
int main() {
  int *arreglo;

  arreglo = (int *)malloc(sizeof(int) * 10);
  for (int i = 0; i < 10; i++)
    arreglo[i] = 10 * i;
  for (int i = 0; i < 10; i++)
    printf("%i\n", arreglo[i]);
  free((char *)arreglo);
  arreglo = (int *)malloc(sizeof(int) * 10);
  for (int i = 0; i < 10; i++)
    arreglo[i] = 20 * i;
  for (int i = 0; i < 10; i++)
    printf("%i\n", arreglo[i]);

  debug();
}