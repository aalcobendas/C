#include <stdio.h>              /* Cabecera llamada al sistema printf  */
#include <unistd.h>             /* Cabecera llamada al sistema gtcwd  */
#include <sys/types.h>          /* Cabeceras llamadas al sistema opendir, readdir y closedir  */
#include <dirent.h>
#include <string.h>
#include <linux/limits.h>

int main(int argc, char *argv[]){
struct dirent *dp; /* para recibir estructuras del directorio*/
DIR *dir; /*puntero directorio actual*/
char cwd [PATH_MAX];
if (argc < 2){/*No se introduce archivo*/
	
	getcwd(cwd, PATH_MAX);
	dir = opendir(cwd);
  if (dir == NULL){
	printf("El directorio no se ha podido abrir\n");
	return -1;
  }else{
	dp = readdir (dir);
        while(dp != NULL){
		printf("%s\n", dp-> d_name);
		dp = readdir (dir);

}
}
}
else {/*Si se ha introducido archivo*/
  
	dir = opendir(argv[1]);
  	if (dir == NULL){
		printf("El directorio no se ha podido abrir\n");
		return -1;
  	}else{
		dp = readdir (dir);
        	while(dp != NULL){
			printf("%s\n", dp-> d_name);
			dp = readdir (dir);

}
}
}
closedir(dir);
return 0;
}

