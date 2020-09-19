#include <iostream>
#include <cstdlib>
#include <random>
#include <string>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <chrono>
#include "asteroPlanetas.h"

using namespace std;
using namespace std::chrono;

int main(int argc, char *argv[]){
	//Calculo de tiempo
	using clk = chrono::high_resolution_clock;
	auto t1 = clk::now();

	//Imprimimos error en los argumentos
	if(argc!=5) {
		cout<<"nasteroids-par: Wrong arguments."<<endl;
		cout<<"Correct use:"<<endl;
		cout<<"nasteroids-par num_asteroides num_iteraciones num_planetas semilla"<<endl;
		return -1;
	}

	//Fichero inicial
	ofstream file("init_conf.txt", ios::out);
	//fichero de salida
	ofstream salidaIteraciones("out.txt", ios::out);

	//Cogemos los parametros
	int num_asteroides = atoi(argv[1]);
	int num_iteraciones = atoi(argv[2]);
	int num_planetas = atoi(argv[3]);
	int seed = atoi(argv[4]);

  //Vectores con valores actuales de asteroides y planetas
  vector<asteroides> vectorAsteroides(num_asteroides);
  vector<planetas> vectorPlanetas(num_planetas);

	//generar las distribuciones aleatorias con la semilla
	default_random_engine re{seed};

	//Print valores dados
	file<<num_asteroides<<" "<<num_iteraciones<<" "<<num_planetas<<" "<<seed<<endl;
	file.close();
	salidaIteraciones.close();

	//Creamos los valores de inicio
	creacion(vectorAsteroides, vectorPlanetas, re, file);

	//Cambios para comprobar cambios de velocidades
	velocidad* velocidadesCambiadas = new velocidad[num_asteroides];
	posicion* posicionGrid =new posicion[num_asteroides];


	//INTRDUCCION DE UN BUCLE WHILE PARA LAS ITERACIONES
	while(num_iteraciones>0){
		//CALCULO DE FUERZAS DE ATTRACCION 2.2.3
		//
		//Distancias
		//Guardamos las distancias de los asteroides con respecto de otros cuerpos.
		double **matrizDistancias = distanciasFinales(vectorAsteroides, vectorPlanetas);

		//MOVIMIENTO NORMAL
		//Calculo del angulo
		double **matrizAnguloInfluencia = anguloInfluencia(vectorAsteroides, vectorPlanetas, matrizDistancias);
		#pragma omp parallel num_threads(4)
		{
		//Calculo de fuerzas
		fuerzasAtraccion(vectorAsteroides, vectorPlanetas, matrizDistancias, matrizAnguloInfluencia);
		}
		//Calculo de la nueva aceleracion de los asteroides
		aceleracion* Vaceleracion= aceleracionFormula(vectorAsteroides);
		//Calculo de la nueva velocidad de los asteroides
		velocidad* Vvelocidad= velocidadFormula(vectorAsteroides, Vaceleracion);
		//Calculo de la nueva posicicion de los asteroides
		posicion* Vposicion= posicionFormula(vectorAsteroides, Vvelocidad);

		//EFECTO REBOTE
		//Recalculo de posiciones con respecto al grid
		posicionGrid = posicionReboteGrid(vectorAsteroides.size(), Vposicion);
		//Recalculo de velocidades con respecto al grid

		//Antes estaba puesto esto pero creo que esta mal, velocidadGrid no existe en realidad.
		Vvelocidad = velocidadReboteGrid(vectorAsteroides.size(), Vvelocidad, Vposicion);

		//REBOTE ENTRE ASTEROIDES 2.2.4
		//
		velocidadesCambiadas = velocidadReboteAsteroides(vectorAsteroides.size(), Vvelocidad, matrizDistancias);
		//Actualizamos los valores en el vectorAsteroides
		actualizacionAsteroides(vectorAsteroides, posicionGrid, velocidadesCambiadas);

		num_iteraciones--;
	}
	impresionFinal(vectorAsteroides, salidaIteraciones);
	//calculo de tiempo
	auto t2 = clk :: now();
	auto diff = duration_cast<microseconds>(t2-t1);
	cout<<diff.count();
}
