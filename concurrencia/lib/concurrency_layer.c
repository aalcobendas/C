#include "../include/concurrency_layer.h"
#include <stdio.h>
#include <pthread.h>
int num_lectores = 0; //numero de lectores
pthread_mutex_t mutex;
pthread_mutex_t mutex_brokers;
pthread_mutex_t mutex_lectores;


pthread_cond_t executer;
pthread_cond_t brokers;
pthread_cond_t readers;


void init_concurrency_mechanisms(){
//Inicializamos los mutex necesarios
	pthread_mutex_init(&mutex,NULL);
	pthread_mutex_init(&mutex_lectores,NULL);
	pthread_cond_init(&executer,NULL);
	pthread_cond_init(&readers,NULL);
	pthread_cond_init(&brokers,NULL);

}

void destroy_concurrency_mechanisms(){
//Destruimos los mutex que hemos utilizado

	pthread_mutex_destroy(&mutex);
	pthread_mutex_destroy(&mutex_lectores);
	pthread_cond_destroy(&executer);
	pthread_cond_destroy(&readers);
	pthread_cond_destroy(&brokers);


}

void* broker(void * args){
	//Cogemos la informacion pasada por parametro que es del tipo broker info
	broker_info brok =(*(broker_info *) args); 
	char *fichero_batch = brok.batch_file; 
	stock_market *mercado = brok.market;
	int enqueue;
	char id[11];
	int type;
	int num_shares;
	int price;
	int share_price;

	iterator *iterador = new_iterator(brok.batch_file);
	//Comprobamos si hay operaciones pendientes
	while(next_operation(iterador,id,&type,&num_shares,&price) >=0){ 
		operation oper;		
		new_operation(&oper,id,type,num_shares,share_price);//Creamos una nueva operacion

		pthread_mutex_lock(&mutex); //Bloqueamos el hilo 
		if(operations_queue_full(mercado->stock_operations)==1){//Comprobamos si la cola esta llena de operaciones y no se pueden añadir mas operaciones
			pthread_cond_wait(&brokers, &mutex); //Dormimos al proceso para que se active cuando le llegue la señal de que puede ejecutarse
		}//fin if

		
	         enqueue = enqueue_operation(mercado->stock_operations, &oper);//Encola en la cola la operacion
		if(enqueue == -1){//Comprobamos si la operacion ha sido bien encolada o no
			printf("No se ha podido encolar la operacion en la cola");
		}//Se ha podido encolar la operacion
			
		pthread_cond_signal(&executer);//Despertamos el proceso executer
		pthread_cond_signal(&readers);//Despertamos el proceso readers
		pthread_mutex_unlock(&mutex);//Desbloqueamos mutex
		
						
		
	} //fin while
		destroy_iterator(iterador); //Destruimos iterador

}

void* operation_executer(void * args){
	int dequeue;
	int process;
	int exit;
//Cogemos la informacion pasada por parametro que es del tipo executer info
	exec_info *exec = (exec_info *) args;
	stock_market *mercado = exec->market;
	operation op;

	pthread_mutex_lock(exec->exit_mutex); //Bloqueamos el mutex del exit para acceder a la variable exit
	exit = *(exec->exit);
	pthread_mutex_unlock(exec->exit_mutex);//Desbloqueamos el mutex del exit
	while(exit == 0 || operations_queue_empty(mercado->stock_operations)==0){//Mientras el flag exit no este activo o la cola no este vacia
		pthread_mutex_lock(&mutex); //Bloqueamos el hilo para acceder
		if(operations_queue_empty(mercado->stock_operations)==1){ //Si la cola esta vacia
			pthread_cond_wait(&executer, &mutex); //Dormimos al proceso para que se active cuando le llegue la señal de que puede ejecutarse
		}//fin if
		dequeue = dequeue_operation(mercado->stock_operations, &op);//Desencola la operacion
		if(dequeue == -1){//Comprobamos si ha sido bien desencolada
			printf("No se ha podido desencolar la operacion");
		}
		process = process_operation(mercado, &op);//Procesamos la operacion desencolada
		if(process == -1){//Comprobamos si ha sido procesada
			printf("No se ha procesado la operacion");
		}//fin if
		pthread_cond_signal(&readers);//Despertamos el proceso readers
		pthread_cond_signal(&brokers);//Despertamos el proceso brokers
		pthread_mutex_unlock(&mutex);//Desbloqueamos el mutex

		pthread_mutex_lock(exec->exit_mutex); //Accedemos otra vez para ver si la variable sigue siendo 0
		exit = *(exec->exit);
		pthread_mutex_unlock(exec->exit_mutex);
		

	}//fin while
}
void* stats_reader(void * args){
//Cogemos la informacion pasada por parametro que es del tipo reader info
	int exit;
	reader_info *reader = (reader_info *) args;
	stock_market *mercado = reader->market;

	pthread_mutex_lock(reader->exit_mutex); //Bloqueamos el mutex del exit para acceder a la variable exit
	exit = *(reader->exit);
	pthread_mutex_unlock(reader->exit_mutex);//Desbloqueamos el mutex del exit

	while(exit == 0){//Mientras el flag exit no este activo
		

		pthread_mutex_lock(&mutex_lectores);//Bloqueamos el mutex para acceder al numero de lectores
		num_lectores++;
		if(num_lectores == 1){//Si el numero de lectores es 1 se bloquea el mutex general para que así si viene otro reader no cancele el proceso al intentar coger el mutex ya que deben leer
					//varios readers a la vez
			pthread_mutex_lock(&mutex);
		}
		pthread_mutex_unlock(&mutex_lectores);//Desbloquea

		print_market_status(mercado);//Imprime el mercado

		pthread_mutex_lock(&mutex_lectores); //Accede al numero de lectores
		num_lectores--;
		if(num_lectores == 0){//Si es 0 el mutex general se desbloquea porque ya ha acabado
			pthread_mutex_unlock(&mutex);
		}
		pthread_mutex_unlock(&mutex_lectores);
		usleep(reader->frequency);

		pthread_mutex_lock(reader->exit_mutex); //Accedemos otra vez para ver si la variable sigue siendo 0
		exit = *(reader->exit);
		pthread_mutex_unlock(reader->exit_mutex);
		
	}//fin while
	
}

		














