#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "array.h"

int main (int argc, char **argv){

  int valor;
  init("vector4", 6);

  set("vector4", 0, 0);
  set("vector4", 1, 1);
  set("vector4", 2, 5);
  set("vector4", 3, 3);
  set("vector4", 4, 4);

  for(int i=0; i<100000; i++){
    get("vector4", 2, &valor);


  }
  destroy("vector4");
}
