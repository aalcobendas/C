#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "array.h"

int main (int argc, char **argv){

  int valor;

  init("vector2", 7);
  init("vector2", 3);

  set("vector2", 1, 40);
  set("vector2", 50, 5);

  get("vector2", 1, &valor);

  printf(" Valor %d\n", valor);

  init("vector6", 6);

  set("vector6", 0, 0);
  set("vector6", 1, 1);
  set("vector6", 2, 5);

  get("vector6", 2, &valor);
  set("vector6", 4, 4);


  init("vector3", 20);
  set("vector3", 10, 10);
  get("vector3", 1, &valor);
  printf(" Valor %d\n", valor);

  destroy("vector3");
  destroy("vector6");
  destroy("vector2");
}
