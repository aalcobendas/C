all:  servidor cliente libarray.so

servidor: servidor.o funcionalidad.o lista.o
	gcc -Wall -o servidor servidor.o funcionalidad.o lista.o -lrt -pthread

servidor.o:
	gcc -Wall -c servidor.c -lrt -pthread

cliente: cliente.o libarray.so
	gcc -Wall -o cliente cliente.o libarray.so -lrt

cliente.o:
	gcc -Wall -c cliente.c

funcionalidad.o:
	gcc -Wall -c funcionalidad.c

lista.o:
	gcc -Wall -c lista.c

array.o: array.c
	gcc -Wall -fPIC -c array.c

libarray.so: array.o
	gcc -shared -o libarray.so array.o

clean:
	@echo "Eliminando elementos..."
	@rm -rfv *.o
	@rm -rfv cliente
	@rm -rfv servidor
	@rm -rfv libarray.so
