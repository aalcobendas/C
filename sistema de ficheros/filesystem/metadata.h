
/*
 *
 * Operating System Design / Diseño de Sistemas Operativos
 * (c) ARCOS.INF.UC3M.ES
 *
 * @file 	metadata.h
 * @brief 	Definition of the structures and data types of the file system.
 * @date	Last revision 01/04/2020
 *
 */
 #include <stdlib.h>
 #include <stdio.h>
 #include <string.h>
 #include <stdint.h>

#define bitmap_getbit(bitmap_, i_) (bitmap_[i_ >> 3] & (1 << (i_ & 0x07)))
static inline void bitmap_setbit(char *bitmap_, int i_, int val_) {
  if (val_)
    bitmap_[(i_ >> 3)] |= (1 << (i_ & 0x07));
  else
    bitmap_[(i_ >> 3)] &= ~(1 << (i_ & 0x07));
}

#define NUM_INODO 48
#define T_FICHERO    1
#define T_ENLACE     2
#define NUM_MAX_BLOQUES 300



typedef struct {
     int numMagico;	                        /* Número mágico del superbloque: 0x12345 */
      int numBloquesDatos;                  /* Número de bloques de datos en el disp. */
      int primerBloqueDatos;                /* Número de bloque del 1º bloque de datos */
      int tamDispositivo;	                  /* Tamaño total del disp. (en bytes) */
      char estadoBloques[NUM_MAX_BLOQUES];  /*mapa de bloques. 0:libre 1:ocupado*/
} TipoSuperbloque;

typedef struct {
    int8_t tipo;	                  /* T_FICHERO o T_DIRECTORIO */
    char nombre[33];	              /* Nombre del fichero/ enlace simbolico asociado */
    int8_t apuntado;                /* Referencia al fichero desde enlace simbolico*/
    int16_t size;	                  /* Tamaño actual del fichero en bytes */
    uint8_t bloqueDirecto[5];	      /* Número del bloque directo */
    int32_t CRC[5];                 /* Guarda los crc de los bloques directos*/
} TipoInodoDisco;
 TipoInodoDisco inodos[NUM_INODO];
#define INODOS_BLOQUE (BLOCK_SIZE / sizeof(TipoInodoDisco)) //numero de i-nodos que hay en un bloque

typedef char  inodo_map[NUM_INODO]; //definimos un tipo de dato mapa i-nodo 0:libre 1:ocupado
TipoSuperbloque sbloques[1];
inodo_map  i_map;


// Metadatos extra de apoyo (que no van a disco)
struct {
    int posicion; // posición de lectura/escritura. -1: cerrado
} inodos_x [NUM_INODO];


int8_t montado = 0 ; // 0: falso, 1: verdadero
