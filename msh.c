/*-
 * msh.c
 *
 * Minishell C source
 * Show how to use "obtain_order" input interface function
 *
 * THIS FILE IS TO BE MODIFIED
 */

#include <stddef.h>			/* NULL */
#include <stdio.h>
#include <stdlib.h>	
#include <unistd.h> 	
#include <sys/wait.h>
#include <sys/types.h> //Para implementar las funciones de redirecciones
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h> //Para comparar strings
#include <dirent.h>
#include <math.h>
#include <sys/time.h>
	/* setbuf, printf */

extern int obtain_order();		/* See parser.y for description */

int main(void)
{
	char ***argvv;
	int num_commands;
	char *filev[3];
	int bg;
	int ret;
	int pid,i;
	int p1[2], p2[2]; /*Array de dos para cada una de las piper creadas
			será de dos ya que [0] será para escritura y [1] para lectura*/
	char cwd [PATH_MAX]; //Array usado para guardarel directorio actual
	double startt, endt;
	setbuf(stdout, NULL);			/* Unbuffered */
	setbuf(stdin, NULL);

	while (1) 
	{
		fprintf(stderr, "%s", "msh> ");	/* Prompt */
		ret = obtain_order(&argvv, filev, &bg); /*Numero de comandos introducidos*/
		if (ret == 0) break;		/* EOF */
		if (ret == -1) continue;	/* Syntax error */
		num_commands = ret - 1;		/* Line */
		if (num_commands == 0) continue;	/* Empty line */
		
		//Comando interno para MYPWD
		//
		if(strcmp(argvv[0][0], "mypwd") == 0){
			 /*obtencion del directorio actual*/
			if (argvv[0][1] != NULL){ // en caso de que se introduzca un comando hay error
				printf("Error en secuencia de mandatos\n");
				return -1;
			}
			if (getcwd(cwd, PATH_MAX) == NULL){ 
			/*En caso de haberse producido un error al intentar conseguir el directorio actual, la
			función dejará el cwd en null*/
				printf("Mypwd error\n");
				return -1;
			}else{
			/*En caso de hque haya encontrado el directorio actual sin problemas lo imprimirá
			por pantalla*/
				printf("Current dir: %s\n", cwd);
			}
			
		}
	
		//Comando interno para MYTIME
		//
		if(strcmp(argvv[0][0], "mytime")==0){	//si el comando que se ha introducido coincide con el mytime se ejecuta solo este comando interno
			if (argvv[0][1] == NULL){ //en caso de que no se haya introducido un comando hay error
				printf("Incorrecta sintaxis");
				return -1;
			}
			struct timeval initialTime, finalTime;
			if(gettimeofday(&initialTime, NULL) < 0){ //Cogemos el tiempo inicial y comprobamos que no hayan surgido errores
				printf("Usage: mytime %s\n", argvv[0][1]);
				return -1;
			}
			
			pid=fork(); //Creamos el proceso hijo
			if(pid==0){ //en caso de que sea el hijo ejecutamos el comando introducido
				if(execvp(argvv[0][1],  argvv[0]+1) < 0) {
					perror(argvv[0][1]);
					return -1;
				}
			}
			wait(NULL); //Esperamos que el proceso hijo termine, para poder calcular el tiempo despues de haberse ejecutado

			if(gettimeofday(&finalTime,  NULL) < 0) {   //Cogemos el tiempo final, y comprobamos que no hayan surgido errores
				printf("Usage: mytime %s\n", argvv[0][1]);
				return -1;
			}
			startt = (double) initialTime.tv_sec + (double) initialTime.tv_usec / (double) 1000000; //Calculamos el tiempo inicial total
			endt = (double) finalTime.tv_sec + (double) finalTime.tv_usec / (double) 1000000;//Calculamos el tiempo final total
			printf("Time spent: %f secs.\n" ,   endt - startt);//Imprimimos el tiempo final menos el inicial
			
			
		}
		
		/*En caso de que el número de comandos introducidos sea 1*/
		if(num_commands==1){
			pid = fork(); //Creamos un proceso hijo en el cual se ejecutará lo introducido por la terminal
			if(pid==0){//Hijo
				//Redirecciones
				//Primero se realizan las redirecciones para luego poder ejcutar el comando
				//Se tendrán en cuenta los modos en los que las redirecciones se llevarán a cabo
				//para cada uno de los distintos tipos	
				if (filev[0] != NULL){
					close(0);
					open(filev[0], O_RDONLY);
				}else if (filev[1] != NULL){
					close(1);
					open(filev[1], O_CREAT | O_RDWR | O_TRUNC, 0755);
				}else if (filev[2] != NULL){
					close(2);
					open(filev[2], O_CREAT | O_RDWR | O_TRUNC, 0755);
				}//Ejecutamos el comando introducido en el proceso hijo
				execvp(argvv[0][0], argvv[0]);
			}	
					
			if(bg){ //En caso de que haya background
				printf("[%d]\n", pid);
				bg = 0;
			}else wait(NULL);//el padre espera al hijo

			//Para esperar al hijo zombie en caso de que haya background
			struct sigaction sa;
			sa.sa_handler = SIG_IGN;
			sigemptyset(&sa.sa_mask);
			sa.sa_flags= 0;
			if (sigaction(SIGCHLD, &sa, 0) == 1){
				//perror(0);
				//exit(1);
			}
			
		}
		/*En caso de que se introduzca más de un comandocomo máximo 3
		se deberá de tener el cuenta el número variante de pipes que se deberán crear y cerrar*/
		else if(num_commands>1){
			//Proceso de creacón de pipes
			for (i=0; i< num_commands; i++){
				if (i==0){ 
					/*El hijo 1 hereda tan solo la primera pipe sin embargo, el resto de los hijos tambien
					la heredan, aunque no todos ellos la utilizarán*/
					pipe(p1);
				}else if (i==1 &&i<num_commands-1){ /*Se creará la pipe2 en caso de que xista hijo 3, será heredada
								por los hijos 2 y 3 */
					pipe(p2);
				}

				pid = fork(); //Creamos hijos
				if (pid==0){ //En caso de que sea proceso hijo
					if(i==0){
						//El primer tipo de redirección lo deberá tener en cuenta tan solo el primer hijo
						//y será de tipo solo RDONLY
						if (filev[0] != NULL && i==0){ 
							close(0);
							open(filev[0], O_RDONLY);
						}
						close(p1[0]); //indica que el proceso no va a leer de la pipe1
						dup2(p1[1], STDOUT_FILENO);//cierra la entrada estandar y duplica 
						//pipe que la pone en el lugar que estaba la entrada estandar
						close(p1[1]);			
					}else if(i==1){
						//El segundo y tercer tipo de redirección lo deberá tener en cuenta tan solo el ultimo hijo
						//por lo tanto, en nuestro caso deberemos comprobar tanto para cuando haya 2 hijos como para
						//si hay 3 y será de tipo solo RDONLY
						if (i==(num_commands-1) && filev[1] != NULL){
							close(1);
							open(filev[1], O_CREAT | O_RDWR | O_TRUNC, 0755);
						}else if (i==(num_commands-1) && filev[2] != NULL){
							close(2);
							open(filev[2], O_CREAT | O_RDWR | O_TRUNC, 0755);
						}
						close(p1[1]); //indica que el proceso no va escribir a la pipe1
						dup2(p1[0], STDIN_FILENO);//cierra la salida estandar y duplica
						close(p1[0]);
								
						if (i != num_commands-1){ // En caso de que haya mas de 2 comandos
							close(p2[0]); //indica que el proceso no va a leer de la pipe 2
							dup2(p2[1], STDOUT_FILENO);//cierra la entrada estandar y duplica 
							//pipe que la pone en el lugar que estaba la entrada estandar
							close(p2[1]);
							}
					}else if(i==2){
						//Si se crea un tercer hijo, este será el encargado de realizar uno de los últimos tipos de 
						//redirecciones
						if (filev[1] != NULL){
							close(1);
							open(filev[1], O_CREAT | O_RDWR | O_TRUNC, 0755);
						}else if (filev[2] != NULL){
							close(2);
							open(filev[2], O_CREAT | O_RDWR | O_TRUNC, 0755);
						}
						close(p2[1]); //indica que el proceso no va escribir a la pipe2
						dup2(p2[0], STDIN_FILENO);
						close(p2[0]);
						close(p1[1]);
					}else{
					}
					execvp(argvv[i][0], argvv[i]); //Hacemos exec, lo que implica que el hijo pasa a ejecutar eso
				}else{
					
				}
			}
			//Cerramos todas las pipes del padre
			close(p1[0]); //Cierras la primera pipe
			close(p1[1]); //Tanto entrada como salida
			if (num_commands>2){ //Tan solo cerraras la segunda si se ha hecho previamente
				close(p2[0]);
				close(p2[1]);
			}

			if (bg){ // Si hay background, el padre no espera a los hijos
				printf("[%d]\n", pid);
				bg=0;
			}else{
				for(i=0; i<num_commands;i++){
					wait(NULL);
				}
			}
				
			//Para esperar a los hijos zombies en caso de que haya background
			struct sigaction sa;
			sa.sa_handler = SIG_IGN;
			sigemptyset(&sa.sa_mask);
			sa.sa_flags= 0;
			if (sigaction(SIGCHLD, &sa, 0) == 1){
				perror(0);
				exit(1);
			}
						
		}
	}
	/*fin while*/ 

	return 0;

} /*end main*/

