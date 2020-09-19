#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

//nodos de la lista
struct  elemento{
	char nombre[250]; //nombre del vector
	int n; //tamaño del vector
	int *v; //vector
	struct  elemento *enlace; //puntero al siguiente elemento
};

//lista
typedef  struct  elemento  *lista_num;

//se inserta el vector en la lista
void insertar(lista_num  *l , int n, char *nombre);

//devuelve el elemento del vector que le indiquemos
int buscar_lista(lista_num *ptr, char* name, int posicion);

//se busca un vector en la lista
int busca_vector(lista_num ptr, char* name, int n);

//devuelve la longitud de la lista
int longitud_lista(lista_num ptr);

//se guarda un valor en la posicion i del vector indicado
int set_lista(char* nombre, int i, int valor, lista_num *a);

//devuelve el valor del vector en la posicion indicada
int get_lista(char *nombre, int i, lista_num a);

//elimina de la lista el vector indicado
int destroy_lista(char  *nombre, lista_num *a);
