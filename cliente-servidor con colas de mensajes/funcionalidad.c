#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <mqueue.h>
#include "lista.h"
#include "mensaje.h"
#include <unistd.h>
#include "funcionalidad.h"

lista_num lista = NULL; // lista  inicial  vacía

//Acciones complementarias a la lista y el servidor
int init_local(char *nombre, int N){
	int resultado=1;
	if(longitud_lista(lista)==0){
		insertar(&lista, N, nombre); //creamos el vector en la lista
	}else{
	  int existencia= busca_vector(lista, nombre, N);
		if(existencia== 0){ //el vector ya existe
		   resultado=0;
		}else if(existencia==1){ //el vector es nuevo
		   insertar(&lista, N, nombre); //creamos el vector vacio en la lista lista.c
		   resultado=1;
		}else{ //error
		   resultado=-1;
		}
	}
 return resultado;
}

int set_local(char* nombre, int i, int valor){
  int resultado = set_lista(nombre, i, valor, &lista);
  return resultado;

}

int get_local(char *nombre, int i, int *valor){
 int a;
 *valor= get_lista(nombre, i, lista);
  if( *valor != -1){
   a=0;
  }else{
   return -1;
  }
 return a;
}

int destroy_local(char  *nombre){
	return destroy_lista(nombre, &lista);

}
