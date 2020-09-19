#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <mqueue.h>
#include "mensaje.h"
#include "funcionalidad.h"
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>

mqd_t q_cliente;
mqd_t q_servidor;
int e;
struct mensaje m;
struct mq_attr attr;
pthread_mutex_t mutex_mens;
pthread_mutex_t lista_mutex;
bool no_copiado = true;
pthread_cond_t cond_mens;
pthread_t thid;
pthread_attr_t t_attr;

//Prototipo de la funcion que ejecuta los servicios de la lista
void tratar_mensaje(struct mensaje *mes);

int main(int argc, char **argv){
  //atributos de la cola
  attr.mq_maxmsg= 10;
  attr.mq_msgsize= sizeof(struct mensaje);

  //abrimos la cola del servidor
  q_servidor = mq_open("/SERVIDOR", O_CREAT|O_RDONLY, 0664, &attr);
  if (q_servidor == -1) {
  	perror("No se puede crear la cola de servidor");
  	return -1;
  }

  //inicializamos el mutex y las condiciones de los hilos
  pthread_mutex_init(&mutex_mens, NULL);
  pthread_cond_init(&cond_mens, NULL);
  pthread_attr_init(&t_attr);
  pthread_mutex_init(&lista_mutex, NULL);

  /* atributos de los threads, threads independientes*/
  pthread_attr_setdetachstate(&t_attr, PTHREAD_CREATE_DETACHED);

  while(1){
    printf("Esperando solicitud...\n");
    //esperamos a recibir las solicitudes de los clientes
  	e= mq_receive(q_servidor, (char *) &m, sizeof(struct mensaje), 0);
    if (e == -1){
      perror("Ha ocurrido un error al recibir datos del cliente");
      break;
    }

    //imprimimos el codigo de operacion para comprobar que se hacen todas las solicitudes
    printf("Cod op: %d\n", m.cod_op);

    //creamos los thread
    if(pthread_create(&thid, &t_attr, (void *) tratar_mensaje, &m) == -1){
      printf("Error al crear el thread\n");
      return -1;
    }

    /*Espera a que el thread copie el mensaje y le despierte*/
    pthread_mutex_lock(&mutex_mens);
      while (no_copiado == true){
        pthread_cond_wait(&cond_mens, &mutex_mens);
      }
      no_copiado = true;
    pthread_mutex_unlock(&mutex_mens);

  }//fin while

 return 0;
} //fin main

void tratar_mensaje(struct mensaje *mes){
	struct mensaje mensaje;  /* mensaje local */
  struct respuesta r;

  pthread_mutex_lock(&mutex_mens); //seccion critica
    memcpy((char *) &mensaje, (char *)mes, sizeof(struct mensaje));
    no_copiado = false;
    pthread_cond_signal(&cond_mens); //se envia la señal de activacion
  pthread_mutex_unlock(&mutex_mens);

	/* ejecutar la petición del cliente y preparar respuesta */
  pthread_mutex_lock(&lista_mutex);   //seccion critica
	if(mensaje.cod_op== 1){ //init  1
    r.cod_error= init_local(mensaje.nombre, mensaje.N);
	}else if(mensaje.cod_op== 2){ //set  2
		r.cod_error= set_local(mensaje.nombre, mensaje.i, mensaje.valor);
	}else if(mensaje.cod_op== 3){ //get  3
		r.cod_error= get_local(mensaje.nombre, mensaje.i, &(r.valor));
	}else if(mensaje.cod_op== 4){ //destroy  4
		r.cod_error= destroy_local(mensaje.nombre);
	}
  pthread_mutex_unlock(&lista_mutex);

  //abrimos la cola de respuesta del cliente
  q_cliente = mq_open(mensaje.cola, O_WRONLY);

  //enviamos respuesta
  if (q_cliente == -1){
        perror("Ha ocurrido un error al intentar abrir la cola del cliente");
  }else{
        //printf("estoy respondiendo al cliente %s\n", mensaje.cola);
        int err = mq_send(q_cliente, (const char*) &r, sizeof(r), 0);
        if(err==-1) fprintf(stderr, "error");
        mq_close(q_cliente);
  }

    pthread_exit(0);




}
