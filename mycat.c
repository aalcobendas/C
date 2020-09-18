#include <stdio.h>              /* Cabecera llamada al sistema printf  */
#include <sys/types.h>          /* Cabeceras llamada al sistema open  */
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>             /* Cabecera llamadas al sistema read, write y close  */


int main(int argc, char *argv[]){
int tam = 1024; /* Inicializamos variable para recoger el tamaño del read, le otorgamos el valor inicial de 1024, para que pueda empezar el bucle de escritura por pantalla*/
if (argc < 2){
	printf("No has introducido archivo\n");
	return -1;
}else if (argc > 2){
	printf("Comando erróneo\n");
	return -1;
}else{
int fd = open (argv[1], O_RDONLY); /*Abro el archivo*/
if (fd == -1){
	printf("Error al abrir el archivo\n");
	return -1; /*Si no hemos abierto el archivo, nos salimos*/	
}
char bufferT [1024]; /*Creamos la variable en la cual meteremos el contenido del archivo*/
while (tam == sizeof(bufferT)){ /* En caso de que el archivo ocupe mas de 1024 bytes, debemos hacer un bucle para que se imprima todo*/
tam = read(fd, bufferT, sizeof (bufferT)); /*El contenido del archivo lo introducimos en la variable anteriormente creada, y nos da un valor de la cantidad de bytes leidos para tam*/
write(STDOUT_FILENO, bufferT, tam); /*El archivo lo imprimimos por pantalla*/
}
close(fd); 
return 0;
}
}
