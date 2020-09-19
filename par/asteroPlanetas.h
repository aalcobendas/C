using namespace std;
//CLASE ASTEROIDES
//

struct fuerza{
	double fx;
	double fy;
};

struct aceleracion{
	double ax;
	double ay;
};

struct velocidad{
	double vx;
	double vy;
};

struct posicion{
	double px;
	double py;
};

class asteroides{
	private:
		double xdist, ydist, masa, xvel, yvel, xforce, yforce;
	public:
	//Funciones
	void setXDist (double distx){
		xdist = distx;
	}
	double getXDist (void){
		return xdist;
	}

	void setYDist (double disty){
		ydist = disty;
	}
	double getYDist (void){
		return ydist;
	}

	void setMasa (double mass){
		masa = mass;
	}
	double getMasa (void){
		return masa;
	}

	void setXVel (double velx){
		xvel = velx;
	}
	double getXVel (void){
		return xvel;
	}

	void setYVel (double vely){
		yvel = vely;
	}
	double getYVel (void){
		return yvel;
	}

	void setXForce (double forcex){
		xforce = forcex;
	}
	double getXForce (void){
		return xforce;
	}

	void setYForce (double forcey){
		yforce = forcey;
	}
	double getYForce (void){
		return yforce;
	}
	asteroides(void){
	}
};




//CLASE PLANETAS
//
class planetas{
	private:
		double xdist, ydist, masa, xvel, yvel;
	public:
	//Funciones
	void setXDist (double distx){
		xdist = distx;
	}
	double getXDist (void){
		return xdist;
	}

	void setYDist (double disty){
		ydist = disty;
	}
	double getYDist (void){
		return ydist;
	}

	void setMasa (double mass){
		masa = mass;
	}
	double getMasa (void){
		return masa;
	}

	void setXVel (double velx){
		xvel = velx;
	}
	double getXVel (void){
		return xvel;
	}

	void setYVel (double vely){
		yvel = vely;
	}
	double getYVel (void){
		return yvel;
	}
	planetas(void){
	}
};



//FUNCIONES ASTEROPLANETAS
//
//Funcion de creacion de planetas y asteroides
void creacion(vector<asteroides> &vectorAsteroidesNew, vector<planetas> &vectorPlanetasNew, default_random_engine reNew, ofstream &fileNew);

//Funcion de matriz de distancias
double** distanciasFinales(vector<asteroides> vectorAsteroidesNew, vector<planetas> vectorPlanetasNew);

//Funcion de distancias
double distanciaFormula(double xPrimero, double yPrimero, double xSegundo, double ySegundo);

//Funcion de matriz de angulos de influencias
double** anguloInfluencia(vector<asteroides> vectorAsteroidesNew, vector<planetas> vectorPlanetasNew, double **matrizDistancia);

//Funcion de pendiente
double pendienteFormula(double xPrimero, double yPrimero, double xSegundo, double ySegundo);

//Funcion de angulo
void anguloFormula(double **matrizPendiente, int rows, int columns);

//Funcion de matriz de fuerzas de atraccion
void fuerzasAtraccion(vector<asteroides> &vectorAsteroides, vector<planetas> &vectorPlanetas, double **matrizDistancias, double **matrizAnguloInfluencia);

//Funcion de fuerzaX
double fuerzaXFormula(double masaPrimero, double masaSegundo, double angulo, double distanciaF);

//Funcion de fuerzaY
double fuerzaYFormula(double masaPrimero, double masaSegundo, double angulo, double distanciaF);

//Funcion aceleracion
aceleracion* aceleracionFormula(vector<asteroides> &vectorAsteroidesNew);

//Funcion velocidad
velocidad* velocidadFormula(vector<asteroides> &vectorAsteroidesNew, aceleracion* aceleracionNew);

//Funcion posicion
posicion* posicionFormula(vector<asteroides> &vectorAsteroidesNew, velocidad* posicionNew);

//Recalculo de posiciones con respecto al grid
posicion* posicionReboteGrid(int size, posicion* posicionesPrimeras);

//Recalculo de velocidad con respecto al grid
velocidad* velocidadReboteGrid(int size, velocidad* velocidadesPrimeras, posicion* posicionesPrimeras);

//Actualizamos velocidad y posicion de asteroides
velocidad* velocidadReboteAsteroides(int sizeAst, velocidad* velocidadesPrimeras, double** matrizDistancias);

//Modificacion valores al final de una iteracion
void actualizacionAsteroides(vector<asteroides> &vectorAsteroides, posicion* posicionesNuevas, velocidad* velocidadesNuevas);

//Impresion de valores finales
void impresionFinal(vector<asteroides> &vectorAsteroides, ofstream &salidaIteraciones);
