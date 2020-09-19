#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista2.h"

//crea un nuevo vector en la lista
void insertar(lista_num  *l , int n, char *nombre){ //init: crear un nuevo vector
struct  elemento *ptr;
ptr = (struct  elemento *)  malloc(sizeof(struct  elemento ));// new  element
if (ptr==NULL) return;
if (*l == NULL) {// lista vacia
	strcpy(ptr ->nombre, nombre);
	ptr->n = n;
	ptr->v = (int *)  calloc(n, sizeof(int));
	ptr->enlace = NULL;
	*l = ptr;
}else {//  inserta al principio
	strcpy(ptr ->nombre, nombre);
	ptr->n = n;
	ptr->v = (int *)  calloc(n, sizeof(int));
	ptr ->enlace = *l;
	*l = ptr;
}

	return;
}

//devuelve el elemento del vector que le indiquemos
int buscar_lista(lista_num *ptr, char* name, int posicion){ //get
	lista_num p1;
	p1= *ptr;
	while (p1 != NULL) {
		if (strcmp(p1->nombre, name)== 0){
			return *((p1->v)+posicion);
		}
		p1 = p1 ->enlace;
	}
	return 0;
}

//se devuelve la posicion que ocupa el vector en la lista
int posicion_lista(lista_num ptr, char *name){
	int posicion=0;
	if((strcmp(ptr->nombre, name)==0)){
		posicion++;
	}
	ptr= ptr->enlace;
	return posicion;
}

//devuelve la longitud de la lista
int longitud_lista(lista_num ptr){
  int contador=0;
  while (ptr != NULL) {
		contador++;
	 	ptr = ptr->enlace;
  }
	return contador;
}

//se busca un vector en la lista
int busca_vector(lista_num ptr, char* name, int n){
  int resultado;
  while (ptr != NULL) {
	if ((strcmp(ptr-> nombre, name)== 0) && (ptr-> n == n)){ //se tiene en cuenta el tamaño del vector
		resultado=0; //el vector ya existe en la lista
		printf("El vector ya existe.");
	}else if((strcmp(ptr-> nombre, name)== 0) && !(ptr-> n == n)){
		printf("Se intento crear un vector que ya existe con un tamaño distinto.");
		resultado=-1; //el vector existe pero se intenta crear con otro tamaño
	}else{
		resultado=1; //el vector es nuevo
	}
 	ptr = ptr->enlace;
  }
  return resultado;

}

//se guarda un valor en la posicion i del vector indicado
int set_lista(char* nombre, int i, int valor, lista_num *a){
	lista_num  aux;
	aux = *a;
  int resultado= -1;

	while (aux!= NULL) {
		if ((strcmp(aux->nombre, nombre)== 0)){
			if(i <0 || i >= aux->n){
				printf("Set: Se accede a posicion invalida\n");
				return -1;
			}
			aux->v[i] = valor;
			resultado= 0;
		}
		  	aux = aux->enlace;
	}
  return resultado;

}

//devuelve el valor del vector en la posicion indicada
int get_lista(char *nombre, int i, lista_num a){
  int resultado= -1;
	while (a != NULL) {
		if (strcmp(a -> nombre, nombre)== 0){
			if(i <0 || i >= a->n){
				printf("Get: Se accede a posicion invalida\n");
				return -1;
			}
			resultado= *(a -> v+i);
		}
			 a = a ->enlace;
	}
  return resultado;
}

//elimina de la lista el vector indicado
int destroy_lista(char  *nombre, lista_num *a){
	lista_num aux , back;
	if (a==NULL) return -1;
	aux = *a;
	back = *a;
	int resultado= -1;

	if (longitud_lista(*a)==1 || posicion_lista(*a, nombre)== 1){
		if (strcmp(aux -> nombre, nombre)== 0){
			*a= aux->enlace;
			free (aux);
			resultado=1;
		}
	}else{
		while (aux !=NULL) {
			if (strcmp(aux->nombre, nombre)== 0) {
				back->enlace = aux->enlace;
				free (aux);
				resultado=1;
			}else {
				back = aux;
				aux = aux->enlace;
			}
		}
	}
	return resultado;
}
