#include <stdio.h>
#include <stdlib.h>

#define MAX 256

struct mensaje{
  int cod_op;
  char cola[MAX];
  char nombre[MAX];
  int valor;
  int i;
  int N;
};
struct respuesta{
  int cod_error;
  int valor;
};
