//gcc lista2.c pruebas.c
#include <stdio.h>
#include <stdlib.h>
#include "lista2.h"

int main(){

	lista_num *lista = NULL;// lista  inicial  vacía
	int  numero= 5;
	int *vec;
	vec = (int *)  malloc(numero*sizeof(int ));
	char *c= "vector1";
	*vec=0;
	*(vec+1)=1;
	*(vec+2)=2;
	*(vec+3)=3;
	*(vec+4)=4;	
	
	insertar(&lista, vec, numero, c);
	
	int valor_posicion= buscar_lista(lista, c, 3);
	int que= busca_vector(lista, c, numero);	
	printf("%d", que);
	borrar(lista, c);

	
	return 0;

}
