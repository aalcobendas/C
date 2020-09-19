
/*
 *
 * Operating System Design / Diseño de Sistemas Operativos
 * (c) ARCOS.INF.UC3M.ES
 *
 * @file 	auxiliary.h
 * @brief 	Headers for the auxiliary functions required by filesystem.c.
 * @date	Last revision 01/04/2020
 *
 */

int ialloc( void );
int bloque_alloc(void);
int ifree(int inodo_id);
int bloque_free(int block_id);
int namei(char *fname);
int bmap ( int inodo_id, int offset );
int metadata_fromMemoryToDisk(void);
int  metadata_fromDiskToMemory(void);
