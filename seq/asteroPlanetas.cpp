#include <iostream>
#include <cstdlib>
#include <random>
#include <string>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <limits>
#include "asteroPlanetas.h"

using namespace std;

const float g= (6.674e-5); /* gravitación */
const float at= 0.1; /* Intervalo */
const int dmin= 5; /* Distancia */
const double width= 200.0; /* Anchura */
const double height= 200.0; /* Altura  */
const int m= 1000; /* distribución normal de las masas */
const int sdm= 50; /* Desviación estándar para el cálculo de la dist. normal de las masas */


/* generar las distribuciones aleatorias con la semilla */
uniform_real_distribution<double> xdist{0.0, std::nextafter(width,std :: numeric_limits<double>::max())};
uniform_real_distribution<double> ydist{0.0, std::nextafter(height,std :: numeric_limits<double>::max())};
normal_distribution<double> mdist{m, sdm};

void creacion(vector<asteroides> &vectorAsteroidesNew, vector<planetas> &vectorPlanetasNew, default_random_engine reNew, ofstream &fileNew){
    	fileNew.open("init_conf.txt", ios::app);
	for(int i = 0; i<static_cast<int>(vectorAsteroidesNew.size()); i++){
		vectorAsteroidesNew[i].setXDist(xdist(reNew));
		fileNew<<fixed<<setprecision(3)<<(vectorAsteroidesNew[i].getXDist());
		fileNew<<" ";
		vectorAsteroidesNew[i].setYDist(ydist(reNew));
		fileNew<<fixed<<setprecision(3)<<(vectorAsteroidesNew[i].getYDist());
		fileNew<<" ";
		vectorAsteroidesNew[i].setMasa(mdist(reNew));
		fileNew<<fixed<<setprecision(3)<<(vectorAsteroidesNew[i].getMasa());
		fileNew<<endl;
    	}
	int j=0;
	for(int i = 0; i<static_cast<int>(vectorPlanetasNew.size()); i++){
		if(j==0){
			vectorPlanetasNew[i].setXDist(0.0);
			fileNew<<fixed<<setprecision(3)<<(vectorPlanetasNew[i].getXDist());
			fileNew<<" ";
			vectorPlanetasNew[i].setYDist(ydist(reNew));
			fileNew<<fixed<<setprecision(3)<<(vectorPlanetasNew[i].getYDist());
			fileNew<<" ";
		}
		if(j==1){
			vectorPlanetasNew[i].setXDist(xdist(reNew));
			fileNew<<fixed<<setprecision(3)<<(vectorPlanetasNew[i].getXDist());
			fileNew<<" ";
			vectorPlanetasNew[i].setYDist(0.0);
			fileNew<<fixed<<setprecision(3)<<(vectorPlanetasNew[i].getYDist());
			fileNew<<" ";
		}
		if(j==2){
			vectorPlanetasNew[i].setXDist(200.0);
			fileNew<<fixed<<setprecision(3)<<(vectorPlanetasNew[i].getXDist());
			fileNew<<" ";
			vectorPlanetasNew[i].setYDist(ydist(reNew));
			fileNew<<fixed<<setprecision(3)<<(vectorPlanetasNew[i].getYDist());
			fileNew<<" ";
		}
		if(j==3){
			vectorPlanetasNew[i].setXDist(xdist(reNew));
			fileNew<<fixed<<setprecision(3)<<(vectorPlanetasNew[i].getXDist());
			fileNew<<" ";
			vectorPlanetasNew[i].setYDist(200.0);
			fileNew<<fixed<<setprecision(3)<<(vectorPlanetasNew[i].getYDist());
			fileNew<<" ";
			j=-1;
		}
		j++;
		vectorPlanetasNew[i].setMasa(mdist(reNew)*10);
		fileNew<<fixed<<setprecision(3)<<(vectorPlanetasNew[i].getMasa());
		fileNew<<endl;
    	}
        fileNew.close();
}

//Funcion matriz distancias
double** distanciasFinales(vector<asteroides> vectorAsteroidesNew, vector<planetas> vectorPlanetasNew){
	double **distancesMatrix=0;
	distancesMatrix= new double *[vectorAsteroidesNew.size()];
	for(int i = 0; i<static_cast<int>(vectorAsteroidesNew.size()); i++){
		distancesMatrix[i]= new double [vectorAsteroidesNew.size()+vectorPlanetasNew.size()];
		for(int j = 0; j<static_cast<int>((vectorAsteroidesNew.size()+vectorPlanetasNew.size())); j++){
			if(j<static_cast<int>(vectorAsteroidesNew.size())){
				distancesMatrix[i][j] = distanciaFormula(vectorAsteroidesNew[i].getXDist(), vectorAsteroidesNew[i].getYDist(), vectorAsteroidesNew[j].getXDist(), vectorAsteroidesNew[j].getYDist());
			}else{
				distancesMatrix[i][j] = distanciaFormula(vectorAsteroidesNew[i].getXDist(), vectorAsteroidesNew[i].getYDist(), vectorPlanetasNew[j-vectorAsteroidesNew.size()].getXDist(), vectorPlanetasNew[j-vectorAsteroidesNew.size()].getYDist());
			}
		}
	}
	return distancesMatrix;
}

//Funcion distancia
double distanciaFormula(double xPrimero, double yPrimero, double xSegundo, double ySegundo){
	double distancia = sqrt(pow((xPrimero-xSegundo),2)+pow((yPrimero-ySegundo),2));
	return distancia;
}

//Funcion calculo del angulo influencia
double** anguloInfluencia(vector<asteroides> vectorAsteroidesNew, vector<planetas> vectorPlanetasNew, double **matrizDistancia){
	double **pendingMatrix=0;
	pendingMatrix= new double *[vectorAsteroidesNew.size()];
	for(int i = 0; i<static_cast<int>(vectorAsteroidesNew.size()); i++){
		pendingMatrix[i]= new double [vectorAsteroidesNew.size()+vectorPlanetasNew.size()];
		for(int j = 0; j<static_cast<int>((vectorAsteroidesNew.size()+vectorPlanetasNew.size())); j++){
			if(matrizDistancia[i][j]>dmin&&j<static_cast<int>(vectorAsteroidesNew.size())){
			     if(i!=j){
                pendingMatrix[i][j] = pendienteFormula(vectorAsteroidesNew[i].getXDist(), vectorAsteroidesNew[i].getYDist(), vectorAsteroidesNew[j].getXDist(), vectorAsteroidesNew[j].getYDist());
           }
		  }else if (j>=static_cast<int>(vectorAsteroidesNew.size())){
            pendingMatrix[i][j] = pendienteFormula(vectorAsteroidesNew[i].getXDist(), vectorAsteroidesNew[i].getYDist(), vectorPlanetasNew[j-vectorAsteroidesNew.size()].getXDist(), vectorPlanetasNew[j-vectorAsteroidesNew.size()].getYDist());
      }
  	}
	}
	anguloFormula(pendingMatrix, vectorAsteroidesNew.size(), vectorAsteroidesNew.size()+vectorPlanetasNew.size());
	return pendingMatrix;
}

//Funcion pendiente
double pendienteFormula(double xPrimero, double yPrimero, double xSegundo, double ySegundo){
	double pendiente = (yPrimero-ySegundo)/(xPrimero-xSegundo);
	if (pendiente<-1){
		pendiente = -1.0;
	}
	if (pendiente>1){
		pendiente = 1.0;
	}
	return pendiente;
}

//Funcion calculo angulo de pendiente
void anguloFormula(double **matrizPendiente, int rows, int columns){
	for (int i = 0; i<rows; i++){
		for (int j = 0; j<columns; j++){
			matrizPendiente[i][j]= atan(matrizPendiente[i][j]);
		}
	}
}

//Funcion  para calcular la matriz de fuerzas de atraccion
void fuerzasAtraccion(vector<asteroides> &vectorAsteroides, vector<planetas> &vectorPlanetas, double **matrizDistancias, double **matrizAnguloInfluencia){
  for(int i = 0; i<static_cast<int>(vectorAsteroides.size()); i++){
		for(int j = 0; j<static_cast<int>((vectorAsteroides.size()+vectorPlanetas.size())); j++){
      if(matrizDistancias[i][j]>dmin&&j<static_cast<int>(vectorAsteroides.size())){
        if(i<j){
				  vectorAsteroides[i].setXForce(vectorAsteroides[i].getXForce()+fuerzaXFormula(vectorAsteroides[i].getMasa(), vectorAsteroides[j].getMasa(), matrizAnguloInfluencia[i][j], matrizDistancias[i][j]));
				  vectorAsteroides[i].setYForce(vectorAsteroides[i].getYForce()+fuerzaYFormula(vectorAsteroides[i].getMasa(), vectorAsteroides[j].getMasa(), matrizAnguloInfluencia[i][j], matrizDistancias[i][j]));
			  }else if(i>j){
				  vectorAsteroides[i].setXForce(vectorAsteroides[i].getXForce()+(-1*fuerzaXFormula(vectorAsteroides[i].getMasa(), vectorAsteroides[j].getMasa(), matrizAnguloInfluencia[i][j], matrizDistancias[i][j])));
				  vectorAsteroides[i].setYForce(vectorAsteroides[i].getYForce()+(-1*fuerzaXFormula(vectorAsteroides[i].getMasa(), vectorAsteroides[j].getMasa(), matrizAnguloInfluencia[i][j], matrizDistancias[i][j])));
			  }
		  }else if(j>=static_cast<int>(vectorAsteroides.size())){
        vectorAsteroides[i].setXForce(vectorAsteroides[i].getXForce()+fuerzaXFormula(vectorAsteroides[i].getMasa(), vectorPlanetas[j-vectorAsteroides.size()].getMasa(), matrizAnguloInfluencia[i][j], matrizDistancias[i][j]));
        vectorAsteroides[i].setYForce(vectorAsteroides[i].getYForce()+fuerzaYFormula(vectorAsteroides[i].getMasa(), vectorPlanetas[j-vectorAsteroides.size()].getMasa(), matrizAnguloInfluencia[i][j], matrizDistancias[i][j]));
      }
    }
  }
}

//Formula de fuerza en la X
double fuerzaXFormula(double masaPrimero, double masaSegundo, double angulo, double distanciaF){
	double fuerzaX = ((g*masaPrimero*masaSegundo)/(distanciaF*distanciaF))*cos(angulo);
	if(fuerzaX>100)fuerzaX=100;
	return fuerzaX;
}

//Formula de fuerza en el Y
double fuerzaYFormula(double masaPrimero, double masaSegundo, double angulo, double distanciaF){
	double fuerzaY = ((g*masaPrimero*masaSegundo)/(distanciaF*distanciaF))*sin(angulo);
	if(fuerzaY>100)fuerzaY=100;
	return fuerzaY;
}

//Formula de la aceleracion
aceleracion* aceleracionFormula(vector<asteroides> &vectorAsteroides){
  aceleracion *acceleration= new aceleracion[vectorAsteroides.size()];

	for (int i=0; i<static_cast<int>(vectorAsteroides.size()); i++){
		acceleration[i].ax= (1/(vectorAsteroides[i].getMasa())) * vectorAsteroides[i].getXForce();
		acceleration[i].ay= (1/(vectorAsteroides[i].getMasa())) * vectorAsteroides[i].getYForce();
	}

	return acceleration;
}

//Formula de la velocidad
velocidad* velocidadFormula(vector<asteroides> &vectorAsteroidesNew, aceleracion* aceleracionNew){
	velocidad *speed= new velocidad[vectorAsteroidesNew.size()];

	for (int i=0; i<static_cast<int>(vectorAsteroidesNew.size()); i++){
		speed[i].vx= (vectorAsteroidesNew[i].getXVel()) + (aceleracionNew[i].ax * at);
		speed[i].vy= (vectorAsteroidesNew[i].getYVel()) + (aceleracionNew[i].ay * at);
	}

	return speed;
}

//Formula de la velocidad
posicion* posicionFormula(vector<asteroides> &vectorAsteroidesNew, velocidad* velocidadNew){
	posicion *speed= new posicion[vectorAsteroidesNew.size()];

	for (int i=0; i<static_cast<int>(vectorAsteroidesNew.size()); i++){
		speed[i].px= (vectorAsteroidesNew[i].getXDist()) + (velocidadNew[i].vx * at);
		speed[i].py= (vectorAsteroidesNew[i].getYDist()) + (velocidadNew[i].vy * at);
	}

	return speed;
}

//Se calcula la posicion nueva que va a tener cada asteroide al rebotar con el grid
posicion* posicionReboteGrid(int size, posicion* posicionesPrimeras){
  for (int i= 0; i<size; i++){
    //Si x<= 0
    if(posicionesPrimeras[i].px<=0){
      posicionesPrimeras[i].px=5.0;
    }
    if(posicionesPrimeras[i].py<=0){
      posicionesPrimeras[i].py=5.0;
    }
    if(posicionesPrimeras[i].px>=width){
      posicionesPrimeras[i].px=width-5;
    }
    if(posicionesPrimeras[i].py>=height){
      posicionesPrimeras[i].py=height-5;
    }
  }
  return posicionesPrimeras;
}

//Se calcula la velocidad nueva que va a tener cada asteroide al rebotar con el grid
velocidad* velocidadReboteGrid(int size, velocidad* velocidadesPrimeras, posicion* posicionesPrimeras){
  for (int i= 0; i<size; i++){
    if(posicionesPrimeras[i].px<=0||posicionesPrimeras[i].px>=width){
      velocidadesPrimeras[i].vx=-1*velocidadesPrimeras[i].vx;
    }
    if(posicionesPrimeras[i].py<=0||posicionesPrimeras[i].py>=height){
      velocidadesPrimeras[i].vy=-1*velocidadesPrimeras[i].vy;
    }
  }
  return velocidadesPrimeras;
}

//REBOTE ASTEROIDES.
//Se calcula la nueva velocidad que va a tener un asteroide al chocar con otros.
velocidad* velocidadReboteAsteroides(int sizeAst, velocidad* velocidadesPrimeras, double** matrizDistancias){
  double velocidadCambiada =0.0;
	for(int i = 0; i<sizeAst; i++){
	 for(int j = i; j<sizeAst; j++){
	      if(matrizDistancias[i][j]<=dmin&&i!=j){
		        velocidadCambiada= velocidadesPrimeras[i].vx;
		        velocidadesPrimeras[i].vx= velocidadesPrimeras[j].vx;
		        velocidadesPrimeras[j].vx= velocidadCambiada;
		        velocidadCambiada= velocidadesPrimeras[i].vy;
		        velocidadesPrimeras[i].vy= velocidadesPrimeras[j].vy;
		        velocidadesPrimeras[j].vy= velocidadCambiada;
	      }
	   }
	 }
  return velocidadesPrimeras;
}

//Funcion que actualiza los datos para la siguiten iteración
void actualizacionAsteroides(vector<asteroides> &vectorAsteroides, posicion* posicionesNuevas, velocidad* velocidadesNuevas){
  for(int i = 0; i<static_cast<int>(vectorAsteroides.size()); i++){
    vectorAsteroides[i].setXDist(posicionesNuevas[i].px);
    vectorAsteroides[i].setYDist(posicionesNuevas[i].py);
    vectorAsteroides[i].setXVel(velocidadesNuevas[i].vx);
    vectorAsteroides[i].setYVel(velocidadesNuevas[i].vy);
    vectorAsteroides[i].setXForce(0.0);
    vectorAsteroides[i].setYForce(0.0);
  }
}

//Impresion para el out.txt
void impresionFinal(vector<asteroides> &vectorAsteroides, ofstream &salidaIteraciones){
  salidaIteraciones.open("out.txt", ios::app);
  for(int i = 0; i<static_cast<int>(vectorAsteroides.size()); i++){
    salidaIteraciones<<fixed<<setprecision(3)<<(vectorAsteroides[i].getXDist())<<" ";
    salidaIteraciones<<fixed<<setprecision(3)<<(vectorAsteroides[i].getYDist())<<" ";
    salidaIteraciones<<fixed<<setprecision(3)<<(vectorAsteroides[i].getXVel())<<" ";
    salidaIteraciones<<fixed<<setprecision(3)<<(vectorAsteroides[i].getYVel())<<" ";
    salidaIteraciones<<fixed<<setprecision(3)<<(vectorAsteroides[i].getMasa())<<" ";
    salidaIteraciones<<endl;
  }
  salidaIteraciones.close();
}
