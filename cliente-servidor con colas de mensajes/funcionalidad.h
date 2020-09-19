//Acciones complementarias a la lista y el servidor
void tratar_mensaje(struct mensaje *mes);
int init_local(char *nombre, int N);
int set_local(char* nombre, int i, int valor);
int get_local(char *nombre, int i, int *valor);
int destroy_local(char  *nombre);
