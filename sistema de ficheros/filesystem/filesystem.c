
/*
 *
 * Operating System Design / Diseño de Sistemas Operativos
 * (c) ARCOS.INF.UC3M.ES
 *
 * @file 	filesystem.c
 * @brief 	Implementation of the core file system funcionalities and auxiliary functions.
 * @date	Last revision 01/04/2020
 *
 */

 #include <stdlib.h>
 #include <stdio.h>
 #include <string.h>
 #include <stdint.h>

#include "filesystem/filesystem.h" // Headers for the core functionality
#include "filesystem/auxiliary.h"  // Headers for auxiliary functions
#include "filesystem/metadata.h"   // Type and structure declaration of the file system
#include "blocks_cache.c"

/*
 * @brief 	Generates the proper file system structure in a storage device, as designed by the student.
 * @return 	0 if success, -1 otherwise.
 */
//Busca i-nodo libre y devuleve identificador
 int ialloc( void ){
 	 int i;
 	 for (i=0; i<NUM_INODO; i++){
 				 if (i_map[i] == 0){
 						 i_map[i] = 1;
 						 memset(&(inodos[i]),0, sizeof(TipoInodoDisco));
 						 return i;
 				 }
 	 }
 	 return -1;
 }
//Busca bloque libre y devuleve identificador del bloque
 int bloque_alloc(void){
 	char buffer[BLOCK_SIZE];
 	int i;
 	for (i=0; i<sbloques[0].numBloquesDatos; i++){
 		if (sbloques[i].estadoBloques[sbloques[0].numBloquesDatos] == 0){
 			// bloque ocupado ahora
 			sbloques[i].estadoBloques[sbloques[0].numBloquesDatos] = 1;
 			memset(buffer, 0, BLOCK_SIZE);
 			bwrite(DEVICE_IMAGE, sbloques[0].primerBloqueDatos + i, buffer);
 			return i;
 		}
 	}

	return -1;
 }
 int ifree(int inodo_id){
 	 // comprobar validez de inodo_id
 	 if (inodo_id > NUM_INODO){
 			 return -1;
 	 }

 	 // liberar i-nodo
 	 i_map[inodo_id] = 0;

 	 return 1;
 }
 int bloque_free(int block_id){
 	 // comprobar validez de block_id
 	 if (block_id > sbloques[0].numBloquesDatos) {
 			 return -1;
 	 }
 	 // liberar bloque
 		 sbloques[0].estadoBloques[block_id] = 0;
 	 return 1;
 }
 int namei(char *fname){
 	int i;
 	// buscar i-nodo con nombre <fname>
 	for (i=0; i<NUM_INODO; i++){
 		if (!strcmp(inodos[i].nombre, fname)) {
 				return i;
 		}
 	}

 	return -1;
 }
 int bmap ( int inodo_id, int offset ){
 	 int bloque_logico ;
 	 // comprobar validez de inodo_id
 	 if (inodo_id > NUM_INODO) {
 			 return -1;
 	 }
 	 // bloque en el que se encuentra el i-nodo
 	 bloque_logico = offset / BLOCK_SIZE ;
 	 if (bloque_logico > 4) {
 			 return -1 ;
 	 }
 	 // devolver referencia a bloque directo
   return inodos[inodo_id].bloqueDirecto[bloque_logico];
 }

 int metadata_fromMemoryToDisk(void){
 	char b[BLOCK_SIZE] ;
  int err;
 	//escribir bloque 0 de sblock a disco
 	memset(b, 0, BLOCK_SIZE);
 	memmove(b, &(sbloques), sizeof(TipoSuperbloque));
  err =	bwrite(DEVICE_IMAGE, 0, b);
  if(err ==-1)return -1;
 	memset(b, 0, BLOCK_SIZE);
 	memmove(b, &(i_map), sizeof(inodo_map));
 	err = bwrite(DEVICE_IMAGE, 1, b);
  if(err ==-1)return -1;
 	//escribir los bloques para el mapa de bloques de datos
 	//escribir los i-nodos a disco
	int num_bloques_bucle = (NUM_INODO+INODOS_BLOQUE-1)/INODOS_BLOQUE;
 	for (int i=0; i<num_bloques_bucle; i++){
    int inodos_quedan_b=(INODOS_BLOQUE<NUM_INODO-INODOS_BLOQUE*i)?INODOS_BLOQUE:NUM_INODO-INODOS_BLOQUE*i;
 		memset(b, 0, BLOCK_SIZE) ;
 		memmove(b, &(inodos[i*INODOS_BLOQUE]), inodos_quedan_b*sizeof(TipoInodoDisco)) ;
 	  err =	bwrite(DEVICE_IMAGE, 1+i, b) ;
    if(err ==-1)return -1;
 	}
	return 0 ;
  }
 int  metadata_fromDiskToMemory(void){
	char b[BLOCK_SIZE] ;
  int err;
 	//escribir bloque 0 de sblock a disco
 	memset(b, 0, BLOCK_SIZE);
 	memmove(b, &(sbloques), sizeof(TipoSuperbloque));
 	err = bread(DEVICE_IMAGE, 0, b);
  if(err ==-1)return -1;
 	memset(b, 0, BLOCK_SIZE);
 	memmove(b, &(i_map), sizeof(inodo_map));
 	err = bread(DEVICE_IMAGE, 1, b);
  if(err ==-1)return -1;
 	// escribir los bloques para el mapa de bloques de datos
 	//escribir los i-nodos a disco
  int num_bloques_bucle = (NUM_INODO+INODOS_BLOQUE-1)/INODOS_BLOQUE;
 	for (int i=0; i<num_bloques_bucle; i++){
    int inodos_quedan_b=(INODOS_BLOQUE<NUM_INODO-INODOS_BLOQUE*i)?INODOS_BLOQUE:NUM_INODO-INODOS_BLOQUE*i;
 		memset(b, 0, BLOCK_SIZE) ;
 		memmove(b, &(inodos[i*INODOS_BLOQUE]), inodos_quedan_b*sizeof(TipoInodoDisco)) ;
 		err = bread(DEVICE_IMAGE, 1+i, b);
    if(err ==-1)return -1;
 	}
	return 0 ;
 }

int mkFS(long deviceSize)
{
	int err;
	char buffer[BLOCK_SIZE];
  //inicializamos variables
	sbloques[0].numMagico= 0x11111;
	sbloques[0].numBloquesDatos = ((deviceSize)/BLOCK_SIZE)-3;
	sbloques[0].primerBloqueDatos = 3;
	sbloques[0].tamDispositivo = deviceSize;
	for(int i=0; i<300; i++){
		sbloques[0].estadoBloques[i]=0;
	}
  for(int i = 0;i<NUM_INODO;i++)
    inodos_x[i].posicion=-1;

	for (int i=0; i<NUM_INODO; i++) {
			 i_map[i] = 0;
	}

	for (int i=0; i<NUM_INODO; i++) {
			 memset(&(inodos[i]),0, sizeof(TipoInodoDisco));
	}
  //guardamos los metadatos en disco
	err =	metadata_fromMemoryToDisk();
	if(err ==-1)return -1;
	memset(buffer,0,BLOCK_SIZE);
	for (int i=0; i < sbloques[0].numBloquesDatos; i++) {
		err = bwrite(DEVICE_IMAGE, sbloques[0].primerBloqueDatos + i, buffer);
		if(err ==-1){
    	 return -1;
		}
	}

	return 0;
}

/*
 * @brief 	Mounts a file system in the simulated device.
 * @return 	0 if success, -1 otherwise.
 */
int mountFS(void)
{
	if (montado == 1) return -1 ;

    // leer los metadatos del sistema de ficheros de disco a memoria
    metadata_fromDiskToMemory() ;
    //comprueba el número mágico
   if (0x11111 != sbloques[0].numMagico) {
       return -1 ;
   }
    montado = 1 ; // 0: falso, 1: verdadero

    return 0 ;
}

/*
 * @brief 	Unmounts the file system from the simulated device.
 * @return 	0 if success, -1 otherwise.
 */
int unmountFS(void)
{
	if (montado == 0) return -1 ;
	// si algún fichero está abierto -> error
	for (int i=0; i<NUM_INODO; i++) {
		if (inodos_x[i].posicion != -1){
			printf("i: %d\n",i);
			return -1 ;
		}
	}
	// escribir los metadatos del sistema de ficheros de memoria a disco
	metadata_fromMemoryToDisk();
	montado = 0 ; // 0: falso, 1: verdadero
	return 0 ;

}

/*
 * @brief	Creates a new file, provided it it doesn't exist in the file system.
 * @return	0 if success, -1 if the file already exists, -2 in case of error.
 */
int createFile(char *fileName)
{
  int inodo_id;
  if(sizeof(fileName)>33){ //comprobamos que el nombre no tiene el tamaño correcto
    return -2;
  }
	inodo_id = namei(fileName) ;
	if (inodo_id !=-1 ) {  //comprobamos que no existe el fichero
		return -1;
	}
  inodo_id = ialloc() ;
  if (inodo_id < 0) { //comprobamos que no devulve un identificador valido
		 return -2;
  }
	for(int i=0;i<5;i++)
		inodos[inodo_id].bloqueDirecto[i]=255;  //255 esta fuera de rango
  inodos[inodo_id].tipo = T_FICHERO ;
  strcpy(inodos[inodo_id].nombre, fileName);
  return 0;
}

/*
 * @brief	Deletes a file, provided it exists in the file system.
 * @return	0 if success, -1 if the file does not exist, -2 in case of error..
 */
int removeFile(char *fileName)
{
	int inodo_id ;

	if(sizeof(fileName)>33){//comprobamos que el nombre no tiene el tamaño correcto
		return -2;
	}

	inodo_id = namei(fileName) ;
	if (inodo_id < 0) { //comprobamos que no existe el fichero
		return -1 ;
	}
	int bloque_logico=inodos_x[inodo_id].posicion/BLOCK_SIZE; //bloque_logico es el bloque en el que esta el i-nodo
	bloque_free(inodos[inodo_id].bloqueDirecto[bloque_logico]);
	memset(&(inodos[inodo_id]), 0, sizeof(TipoInodoDisco));
	ifree(inodo_id) ;

	return 0;
}

/*
 * @brief	Opens an existing file.
 * @return	The file descriptor if possible, -1 if file does not exist, -2 in case of error..
 */
int openFile(char *fileName)
{
	if(sizeof(fileName)>33) //comprobamos que el nombre no tiene el tamaño correcto
		return -2;
	int inodo_id ;
	inodo_id = namei(fileName);
  if(inodo_id <0) return -1; //comprobamos que no existe el fichero
  if (inodos[inodo_id].tipo==T_ENLACE){
    inodo_id=inodos[inodo_id].apuntado;
    return inodo_id;
  }
  if(inodos[inodo_id].tipo==T_FICHERO){
  	inodos_x[inodo_id].posicion = 0;
}
	return inodo_id;
}

/*
 * @brief	Closes a file.
 * @return	0 if success, -1 otherwise.
 */
int closeFile(int fileDescriptor)
{
	if (fileDescriptor < 0) { //comprobamos que el parametro fileDescriptor se introduce correctamente
		return -1 ;
	}
	inodos_x[fileDescriptor].posicion = -1;
	return 0;
}


/*
 * @brief	Reads a number of bytes from a file and stores them in a buffer.
 * @return	Number of bytes properly read, -1 in case of error.
 */
int readFile(int fileDescriptor, void *buffer, int numBytes)
{
  char b[BLOCK_SIZE] ;
  int b_id ;
  int err;

  if((fileDescriptor<0) || (fileDescriptor>=NUM_INODO)){ //comprobamos que el parametro fileDescriptor se introduce correctamente
    return -1;
  }
  if(inodos_x[fileDescriptor].posicion<0){ //comprobamos que el fichero esta abierto
    return -1;
  }
  if (numBytes <= 0) { //comprobamos que el numBytes es valido
      return 0;
  }
  if (numBytes+inodos_x[fileDescriptor].posicion+1>inodos[fileDescriptor].size){ //comprobamos que la posicion no es mayor que el tamaño del fichero
    numBytes=inodos[fileDescriptor].size-(inodos_x[fileDescriptor].posicion+1); //actualizamos bytes a leer
  }
  int bytes_quedan_l=numBytes;
  int bytes_leidos=0;
  while(bytes_quedan_l>0){
    b_id = bmap(fileDescriptor, inodos_x[fileDescriptor].posicion);
    if(b_id==255){ //si el bloque no tiene un identificador asociado se le asocia un identificador
      b_id=bloque_alloc();
      if(-1==b_id){  //comprobar que el indentificador es valido
        return -1;
      }
      int bloque_logico=inodos_x[fileDescriptor].posicion/BLOCK_SIZE;
      inodos[fileDescriptor].bloqueDirecto[bloque_logico]=b_id;
    }
    err = bread(DEVICE_IMAGE, sbloques[0].primerBloqueDatos+b_id, b);
    if (err==-1)return -1;
    int bytes_a_empaquetar=BLOCK_SIZE-(inodos_x[fileDescriptor].posicion%BLOCK_SIZE);
    bytes_a_empaquetar=(bytes_a_empaquetar<bytes_quedan_l)?bytes_a_empaquetar:bytes_quedan_l;
    memmove(buffer,b+inodos_x[fileDescriptor].posicion%BLOCK_SIZE, bytes_a_empaquetar);
    buffer=buffer+bytes_a_empaquetar;
    inodos_x[fileDescriptor].posicion += bytes_a_empaquetar;
    bytes_quedan_l=bytes_quedan_l-bytes_a_empaquetar;
    bytes_leidos=bytes_leidos+bytes_a_empaquetar;
  }
    return bytes_leidos;
}

/*
 * @brief	Writes a number of bytes from a buffer and into a file.
 * @return	Number of bytes properly written, -1 in case of error.
 */
int writeFile(int fileDescriptor, void *buffer, int numBytes)
{
	char b[BLOCK_SIZE] ;
  int b_id ;
	int err;
  if((fileDescriptor<0) || (fileDescriptor>=NUM_INODO) || strlen(buffer)>MAX_FILE_SIZE){ //comprobamos que el parametro fileDescriptor se introduce correctamente y el tamaño del buffer no se pasa del tamaño maximo del fichero
    return -1;
  }
	if(inodos_x[fileDescriptor].posicion<0){ //comprobamos que el fichero esta abierto
		return -1;
	}

  int bytes_quedan_w=numBytes;
  int bytes_escritos=0;
  while(bytes_quedan_w>0){
    b_id = bmap(fileDescriptor, inodos_x[fileDescriptor].posicion);
    if(b_id==255){ //si el bloque no tiene un identificador asociado se le asocia un identificador
      b_id=bloque_alloc();
      if(-1==b_id){ //comprobar que el indentificador es valido
        return -1;
      }
      int bloque_logico=inodos_x[fileDescriptor].posicion/BLOCK_SIZE;
      inodos[fileDescriptor].bloqueDirecto[bloque_logico]=b_id;
    }
    err = bread(DEVICE_IMAGE, sbloques[0].primerBloqueDatos+b_id, b);
    if (err==-1)return -1;
    int bytes_a_empaquetar=BLOCK_SIZE-(inodos_x[fileDescriptor].posicion%BLOCK_SIZE);
    bytes_a_empaquetar=(bytes_a_empaquetar<bytes_quedan_w)?bytes_a_empaquetar:bytes_quedan_w;
    memmove(b+inodos_x[fileDescriptor].posicion%BLOCK_SIZE, buffer, bytes_a_empaquetar);
    buffer=buffer+bytes_a_empaquetar;
    err = bwrite(DEVICE_IMAGE, sbloques[0].primerBloqueDatos+b_id, b);
    if (err==-1)return -1;
    inodos_x[fileDescriptor].posicion += bytes_a_empaquetar;
    inodos[fileDescriptor].size = (inodos[fileDescriptor].size>inodos_x[fileDescriptor].posicion)?inodos[fileDescriptor].size:inodos_x[fileDescriptor].posicion;
    bytes_quedan_w=bytes_quedan_w-bytes_a_empaquetar;
    bytes_escritos=bytes_escritos+bytes_a_empaquetar;
  }
  	return bytes_escritos;
}

/*
 * @brief	Modifies the position of the seek pointer of a file.
 * @return	0 if succes, -1 otherwise.
 */
int lseekFile(int fileDescriptor, long offset, int whence)
{
	if(inodos_x[fileDescriptor].posicion<0){ //comprobamos que el fichero esta abierto
		return -1;
	}
	if(inodos_x[fileDescriptor].posicion>inodos[fileDescriptor].size){ //comprobamos que la posicion no se pasa del tamaño del fichero
		return -1;
	}else{
		if(whence== FS_SEEK_CUR){//se desplaza desde la posicion actual
			if((inodos_x[fileDescriptor].posicion+offset > inodos[fileDescriptor].size) || (inodos_x[fileDescriptor].posicion+offset < 0)) return -1;
			inodos_x[fileDescriptor].posicion=inodos_x[fileDescriptor].posicion+offset;
		}
		if(whence== FS_SEEK_BEGIN){//se desplaza al principio
			inodos_x[fileDescriptor].posicion=0;
		}
		if(whence== FS_SEEK_END){//se desplaza al final
			inodos_x[fileDescriptor].posicion=inodos[fileDescriptor].size-1;
		}
	}
	return 0;
}

/*
 * @brief	Checks the integrity of the file.
 * @return	0 if success, -1 if the file is corrupted, -2 in case of error.
 */

int checkFile (char * fileName)
{
	int inodo_id ;
	int err;
  int b_id;
	int igual = 1;//true
  char buffer[BLOCK_SIZE] ;
	inodo_id = namei(fileName) ;
  if (inodo_id<0) return -2; //comporbamos que existe el fichero
	if(inodos_x[inodo_id].posicion !=-1) return -2; //comprobamos que esta cerrado
	if(inodos[inodo_id].tipo==T_FICHERO){
	 for(int i = 0;i<5;i++){
    if(inodos[inodo_id].bloqueDirecto[i]!=255){ //comprobamos los bloques directos que tengan un identificador guardado
      b_id = inodos[inodo_id].bloqueDirecto[i];
       err=bread(DEVICE_IMAGE, sbloques[0].primerBloqueDatos+b_id, buffer);
        if(err==-1){ //comprobamos que el bread se hace correctamente
          return -2;
        }
        uint32_t val = CRC32((const unsigned char *)buffer, strlen(buffer)); //calculamos el crc de lo introducido por parametro
        uint32_t val2 = inodos[inodo_id].CRC[i]; //obtenemos el crc guardado
        if(val != val2) igual = 0;  //comprobamos si son distintos y se pone el valor igual a false(0)
        if(igual == 0) { //con solo un bloque que no cumpla la integridad, la funcion devuelve error
          return -1;
        }
      }
		}
		if (igual==1)	{  // si son iguales la funcion es correcta
      return 0;
    }
	}
    return -2;
}

/*
 * @brief	Include integrity on a file.
 * @return	0 if success, -1 if the file does not exists, -2 in case of error.
 */

int includeIntegrity (char * fileName)
{
	int inodo_id;
  char buffer[BLOCK_SIZE] ;
  int b_id;
	int err;
	inodo_id = namei(fileName) ;
	if(inodo_id==-1)return -1; //comporbamos que existe el fichero
	if(checkFile(fileName)==0) return -2; //comprobamos que el fichero tiene integridad
	if(inodos[inodo_id].tipo==T_FICHERO){ //esta funcion no se realiza para los enlaces
		for(int i = 0; i<5; i++){
      if(inodos[inodo_id].bloqueDirecto[i]!=255){ //comprobamos los bloques directos que tengan un identificador guardado
          b_id = inodos[inodo_id].bloqueDirecto[i];
			     err=bread(DEVICE_IMAGE, sbloques[0].primerBloqueDatos+b_id, buffer);
           if(err==-1)return -2; //comprobamos que el bread se hace correctamente
     				uint32_t val = CRC32((const unsigned char *)buffer, strlen(buffer)); //calculamos el crc de lo introducido por parametro
     				inodos[inodo_id].CRC[i] = val; //guardamos crc en el i-nodo
      }
		}
		return 0;
	}
    return -2;
}

/*
 * @brief	Opens an existing file and checks its integrity
 * @return	The file descriptor if possible, -1 if file does not exist, -2 if the file is corrupted, -3 in case of error
 */
int openFileIntegrity(char *fileName)
{
		int valor;
    if(sizeof(fileName)>33) return -1; //comprobamos que el nombre no tiene el tamaño correcto
		int integridad = checkFile(fileName); //comprobamos que el fichero tiene integridad
		if(integridad==0)valor = openFile(fileName); //si tiene integridad abre el fichero
    // en caso de que no haya integridad devulve sus respectivos errores
		if(integridad==-1) return -2;
		if(integridad==-2) return -3;
    //si el fichero se abre mal devuelve sus respectivos errores
		if (-1==valor) return -1;
		if (-2==valor) return -3;
	  return valor;
}

/*
 * @brief	Closes a file and updates its integrity.
 * @return	0 if success, -1 otherwise.
 */
int closeFileIntegrity(int fileDescriptor)
{
	int err;
  int b_id;
  char buffer[BLOCK_SIZE] ;
	for(int i = 0; i<5; i++){
    if(inodos[fileDescriptor].bloqueDirecto[i]!=255){ //comprobamos los bloques directos que tengan un identificador guardado
      b_id = inodos[fileDescriptor].bloqueDirecto[i];
       err=bread(DEVICE_IMAGE, sbloques[0].primerBloqueDatos+b_id, buffer);
  		if(err==-1)return -2; //comprobamos que el bread se hace correctamente
  			uint32_t val = CRC32((const unsigned char *)buffer, strlen(buffer)); //calculamos el crc de lo introducido por parametro
  			inodos[fileDescriptor].CRC[i] = val;  //guardamos crc en el i-nodo
        err = closeFile(fileDescriptor);
       if (err==-1)return -1;
    }
	}

    return 0;
}

/*
 * @brief	Creates a symbolic link to an existing file in the file system.
 * @return	0 if success, -1 if file does not exist, -2 in case of error.
 */
int createLn(char *fileName, char *linkName)
{
  int inodo_id;
  int inodo_id2;
  inodo_id = namei(linkName) ; //devuelve el descriptor del enlace
  inodo_id2 = namei(fileName) ; //devulve el descriptor del fichero
  if (inodo_id2 < 0) return -1; //comprobamos que existe el fichero

  if(inodo_id>=0) return -2; //comprobamos que existe el enlace

  inodo_id = ialloc();
  if(inodo_id <0)return -2;  //comprobamos si el identificador es valido

  inodos[inodo_id].tipo=T_ENLACE;
  inodos[inodo_id].apuntado=inodo_id2;
  strcpy(inodos[inodo_id].nombre,linkName);
  return 0;
}
/*
 * @brief 	Deletes an existing symbolic link
 * @return 	0 if the file is correct, -1 if the symbolic link does not exist, -2 in case of error.
 */
int removeLn(char *linkName)
{
    int inodo_id;
    inodo_id = namei(linkName) ;
    if (inodo_id ==-1 ) { //comprobamos que existe el enlace
    		return -2;
    	}
    if(inodos[inodo_id].nombre==NULL)return -1; //comprobamos que no existe el enlace usando el nombre
    memset(&(inodos[inodo_id]), 0, sizeof(TipoInodoDisco));
    ifree(inodo_id);
    metadata_fromMemoryToDisk();
    return 0;
}
