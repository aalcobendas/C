#include <stdio.h>              /* Cabecera llamada al sistema printf  */
#include <unistd.h>             /* Cabecera llamada al sistema getcwd  */
#include <sys/types.h>          /* Cabeceras llamadas al sistema opendir, readdir y closedir  */
#include <dirent.h>
#include <string.h>
#include<fcntl.h>
//VARIABLES
char buffer[1024];
int numbytes, tamanio, fd;

////////////
    int main(int argc, char *argv[]){
        if(argc >2){			//porque si meten algo mas que el comando y el directorio, no lo estan haciendo bien
          //  printf("Error \n");
            return-1;
          }
    DIR *dir;				// creamos un puntero a un directorio
    struct dirent *direntp; 		// una estructura con un puntero a un directorio


    if( argc == 1){ 			//si NO pasan un argumento, se coge la raiz del mismo

   	getcwd(buffer, PATH_MAX);  	//La funcion getcwd escribe el nombre del directorio actual en una memoria dada, buffer. Envia NULL si el nombre
    //	printf("%s\n",buffer);		//del directorio supera el tamanio de la memoria (error ERANGE), proporcionado por el parametro PATH_MAX. Si tiene exito envia buffer.

   	dir = opendir(buffer); 		//aqui va lo que queremos abrir

	    if (dir == NULL) {
	       //		printf ("No existe\n");
			return -1;
		}
	    else{
      		direntp = readdir(dir);
      		while(direntp != NULL){

            fd = open(direntp->d_name , O_RDONLY , 0777); //abrimos el fichero fuera del bucle, para poder leerlo y sacar el valor.
                  if (direntp->d_type == DT_REG){       //Aqui chequeamos que sea un fichero regular
                             printf("%s" , direntp->d_name);
                             tamanio = lseek(fd, 0, 2); //sacamos el valor del tamanio del fichero, para imprimirlo
                             printf("\t");
                             printf("%d",tamanio);
                             printf("\n");
                  }
                  direntp = readdir(dir);
              }
             }
       close(fd);
       closedir(dir);
    }
return 0;
}
