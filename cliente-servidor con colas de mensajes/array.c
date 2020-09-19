#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <string.h>
#include <pthread.h>
#include "array.h"
#include "mensaje.h"
#include <sys/types.h>
#include <unistd.h>

mqd_t q_cliente;
mqd_t q_servidor;
int e,s;
struct mensaje m;
struct respuesta r;
struct mq_attr attr;
char queuename [MAX];

int init (char *nombre, int N){
  m.cod_op = 1;
  attr.mq_maxmsg= 1;
  attr.mq_msgsize= sizeof(r);
  //damos un nombre de cola  a cada cliente
  sprintf (queuename, "/Cola-%d", getpid());

  //abrimos colas
  q_cliente = mq_open(queuename, O_CREAT|O_RDONLY, 0700, &attr);
  if(q_cliente == -1){
    perror("Hubo un error al abrir la cola cliente");
    return -1;
  }
  q_servidor = mq_open("/SERVIDOR", O_WRONLY);
  if(q_servidor == -1){
    perror("Hubo un error al abrir la cola servidor");
    return -1;
  }

  //rellenamos la peticion
  strcpy(m.nombre, nombre);
  m.N = N;
  strcpy(m.cola, queuename);

  //enviamos la peticion al servidor
  s = mq_send(q_servidor, (const char *) &m, sizeof(m), 0);
  if(s == -1){
    perror("Hubo un error al enviar la peticion al servidor");
    return -1;
  }

 //recibimos respuesta del servidor
  e = mq_receive(q_cliente,(char *) &r, sizeof(r), 0);
  if (e==-1){
    perror("Hubo un error al recibir datos del cliente");
    return -1;
  }else{
    if(r.cod_error == -1) printf(" %s: ERROR en la funcion INIT\n", queuename);
    else if(r.cod_error == 0) printf("  %s: EL VECTOR YA EXISTE CON EL MISMO NUM DE COMPONENTES\n", queuename);
    else printf(" %s: Vector creado con EXITO\n", queuename);
  }

	mq_close(q_servidor);
	mq_close(q_cliente);
	mq_unlink(queuename);

  return r.cod_error;
}


int set (char *nombre, int i, int valor){
  m.cod_op = 2;
  attr.mq_maxmsg= 10;
  attr.mq_msgsize= sizeof(r);

  //damos un nombre de cola por cliente
  sprintf (queuename, "/Cola-%d", getpid());

  //abrimos colas
  q_cliente = mq_open(queuename, O_CREAT|O_RDONLY, 0700, &attr);
  if(q_cliente == -1){
    perror("Hubo un error al abrir la cola cliente");
    return -1;
  }
  q_servidor = mq_open("/SERVIDOR", O_WRONLY);
  if(q_servidor == -1){
    perror("Hubo un error al abrir la cola servidor");
    return -1;
  }

  //rellenamos la peticion
  strcpy(m.nombre, nombre);
  m.i = i;
  m.valor = valor;
  strcpy(m.cola, queuename);
  //printf("Array.c: El cliente imprime su nombre de cola.....%s\n",m.cola );

  //enviamos la peticion al servidor
  s = mq_send(q_servidor, (const char *) &m, sizeof(m), 0);
  if(s == -1){
    perror("Hubo un error al enviar la peticion al servidor");
    return -1;
  }

  //recibimos la respuesta del servidor
  e = mq_receive(q_cliente,(char *) &r, sizeof(r), 0);
  if (e ==-1){
    perror("Hubo un error al recibir");
    return -1;
  }else{
    if(r.cod_error == -1) printf(" %s: ERROR en la funcion SET\n", queuename);
    else printf(" %s: Funcion SET realizada con EXITO\n", queuename);
  }

  mq_close(q_servidor);
  mq_close(q_cliente);
  mq_unlink(queuename);

  return r.cod_error;
}


int get (char *nombre, int i, int *valor){
  m.cod_op = 3;
  attr.mq_maxmsg= 10;
  attr.mq_msgsize= sizeof(r);

  //damos un nombre de cola por cliente
  sprintf (queuename, "/Cola-%d", getpid());

  //abrimos colas
  q_cliente = mq_open(queuename, O_CREAT|O_RDONLY, 0700, &attr);
  if(q_cliente == -1){
    perror("Hubo un error al abrir la cola cliente");
    return -1;
  }
  q_servidor = mq_open("/SERVIDOR", O_WRONLY);
  if(q_servidor == -1){
    perror("Hubo un error al abrir la cola servidor");
    return -1;
  }

  //rellenamos la peticion
  strcpy(m.nombre, nombre);
  m.i = i;
  strcpy(m.cola, queuename);
  //printf("Array.c: El cliente imprime su nombre de cola.....%s\n",m.cola );

  //enviamos la peticion al servidor
  s = mq_send(q_servidor, (const char *) &m, sizeof(m), 0);
  if(s == -1){
    perror("Hubo un error al enviar la peticion al servidor");
    return -1;
  }

  //recibimos la respuesta del servidor
  e = mq_receive(q_cliente,(char *) &r, sizeof(r), 0);
  *valor = r.valor;

  if (e ==-1){
    perror("Hubo un error al recibir");
    return -1;
  }else{
    if(r.cod_error == -1) printf(" %s: ERROR en la funcion GET \n", queuename);
    else printf(" %s: Funcion GET realizada con EXITO \n", queuename);
  }

  mq_close(q_servidor);
  mq_close(q_cliente);
  mq_unlink(queuename);


  return r.cod_error;
}

int destroy (char *nombre){
  m.cod_op = 4;
  attr.mq_maxmsg= 10;
  attr.mq_msgsize= sizeof(r);

  //damos un nombre de cola por cliente
  sprintf (queuename, "/Cola-%d", getpid());

  //abrimos colas
  q_cliente = mq_open(queuename, O_CREAT|O_RDONLY, 0700, &attr);
  if(q_cliente == -1){
    perror("Hubo un error al abrir la cola cliente");
    return -1;
  }
  q_servidor = mq_open("/SERVIDOR", O_WRONLY);
  if(q_servidor == -1){
    perror("Hubo un error al abrir la cola servidor");
    return -1;
  }

  //rellenamos la peticion
  strcpy(m.nombre, nombre);
  strcpy(m.cola, queuename);
  //printf("Array.c: El cliente imprime su nombre de cola.....%s\n",m.cola );

  //enviamos la peticion al servidor
  s = mq_send(q_servidor, (const char *) &m, sizeof(m), 0);
  if(s == -1){
    perror("Hubo un error al enviar la peticion al servidor");
    return -1;
  }

  //recibimos la respuesta del servidor
  e = mq_receive(q_cliente,(char *) &r, sizeof(r), 0);
  if (e==-1){
    perror("Hubo un error al recibir");
    return -1;
  }else{
    if(r.cod_error == -1) printf(" %s: ERROR en la funcion DESTROY \n", queuename);
    else printf(" %s: Funcion DESTROY realizada con EXITO\n", queuename);
  }

  mq_close(q_servidor);
  mq_close(q_cliente);
  mq_unlink(queuename);


  return r.cod_error;
}
