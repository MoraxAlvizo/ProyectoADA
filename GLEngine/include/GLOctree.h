#pragma once
#include "MathUtils.h"
#include <set>
#include <vector>
#include <GLMesh.h>
using namespace std;

// MAX_OCTREE_DEPTH es el numero maximo de niveles en el arbol.Puede perjudicar si
// existen muchos niveles en el arbol ya que la caja puede llegar a ser tan pequena que
// el radio de las pelotas esten contenidas en muchas cajas a la vez.
const int MAX_OCTREE_DEPTH = 3;
// Si removemos pelotas de los nodos y queda por debajo de este numero entonces lo que
// vamos hacer es juntarlo ese nodo.
const int MIN_BALLS_PER_OCTREE = 2;	 
// Queremos tener un maximo de pelotas por nodo en el octree.Si existen mas pelotas
// entonces lo que vamos hacer es dividirlo mas.
const int MAX_BALLS_PER_OCTREE = 6;
// Es el tamaño de la caja que contine a las pelotas.
const float BOX_SIZE = 30.0f;

// Guarda informacion referente a una pelota.
typedef GLMesh Ball;

// Guarda todos los tipos de paredes.
enum Wall { WALL_LEFT, WALL_RIGHT, WALL_FAR, WALL_NEAR, WALL_TOP, WALL_BOTTOM };

// Guarda un par de apuntadores a las pelotas. Esto es usado para despues indicar
// colisiones potenciales entre dos pelotas.
struct BallPair {
	Ball* ball1;
	Ball* ball2;
};

// Guarda un par de apuntadores de una pelota y una pared. Esto es usado para despues
// indicar colisiones potenciales entre una pelota y una pared.
struct BallWallPair {
	Ball* ball;
	Wall wall;
};

class GLOctree {
private:
	// Estos 3 vectores representan las esquinas del cubo en el octree y tambien el centro.
	vector3 corner1; //(minX, minY, minZ)
	vector3 corner2; //(maxX, maxY, maxZ)
	vector3 center;//((minX + maxX) / 2, (minY + maxY) / 2, (minZ + maxZ) / 2)

	// Guarda los hijos si es que hay alguno.Basicamente usamos un arreglo para guardar los
	// 8 hijos del octree.children[0][*][*] son los hijos con cordenada x que van desde
	// minX hasta centerX.children[1][*][*] son los hijos con cordenada x que van desde
	// centerX hasta maxX.Esto aplica para las otras dos dimensiones.
	GLOctree *children[2][2][2];
	// Nos indica si un nodo en particular tiene algun hijo.
	bool hasChildren;
	// Guarda todas las pelotas en un nodo en particular si este nodo no tiene ningun hijo.
	set<Ball*> balls;
	// El nivel por el que nos encontramos en el Octree.
	int depth;

public:
	// El numero total de pelotas que contiene el nodo.Tambien incluye aquellas guardadas
	// en sus hijos.Necesitamos tener muy encuenta este numero ya que nos va a indicar
	// cuando dividir o juntar los nodos.
	int numBalls;
private:
	// fileBall() se encarga de agregar o eliminar pelotas al nodo adecuado en el octree (se
	// basa en la posicion de la pelota). Basicamente va a encontra a cual hijo esa pelota
	// en particular pertenece y la va a remover o agregar dependiendo si @addBall es
	// verdadera o falsa.
	//@param    ball		    Pelotas.
	//@param	pos				La posicion.
	//@param	addBall			verdadero para agregar la pelota.
	void fileBall(Ball* ball, vector3 pos, bool addBall);

	// Divide un cubo en particular a cubos mas pequenos. Es decir de un nodo crea a otros
	// nodos mas. Toma todos los hijos del nodo en cuestion y los agrega a los nodos hijo de
	// ese nodo.
	void haveChildren() ;

	// Se encarga de saber cuales bolas estan contenidas en un nodo particular incluyendo
	// sus hijos y los va a colocar en el set @bs. Si este nodo en cuestion
	// tiene hijos va a realizar una llamada recursiva hasta sus hijos que no tienen mas
	// hijos y va a colocar las bolas de cada nodo hijo en el set bs. Este
	// metodo se va a usar particularmente cuando queremos juntar los nodos.
	void collectBalls(set<Ball*> &bs) ;

	// Destruye todos los hijos del nodo y mueve todas las pelotas al set de pelotas.
	void destroyChildren() ;

	// Remueve la pelota del octree de la posicion particular @pos.
	//@param    ball	Las pelotas.
	//@param	pos		La posicion.
	void remove(Ball* ball, vector3 pos) ;

	// Este metodo es llamado individualmente para cada una de las paredes de nuestra caja.
	// Guarda las colisiones potenciales en un vector @cs. Recursivamente se manda a llamar
	// en la mitad correcta del hijo. Una vez que ya no hay mas hijos se guarda en el vector
	// @cs las colisiones potenciales entre la pared y la pelota.
	//@param [in,out]	cs	Vector en donde guardamos la posibles colisiones.
	//@param	w		  	Pared a procesar.
	//@param	coord	  	Las coordenadas.
	//@param	dir		  	The direccion.
	void potentialBallWallCollisions(vector<BallWallPair> &cs,
		Wall w, char coord, int dir);
public:
	// Constructor
	GLOctree(vector3 c1, vector3 c2, int d);

	// Destructor
	~GLOctree();

	// Agrega una pelota al nodo.
	//@param ball Las pelotas.
	void add(Ball* ball);

	// Remueve la pelota del nodo.
	//@param	ball	La pelota a remover.
	void remove(Ball* ball);

	// Se manda a llamar cada vez que la pelota cambia de posicion.Esto sucede en
	// GLScene.cpp.Para hacer esto solamente eliminamos la pelota de la posicion antigua y
	// la agregamos en su nueva posicion.
	//@param	ball	Pelota para actualizar.
	//@param	oldPos			Posicion Antigua.
	void ballMoved(Ball* ball, vector3 oldPos);

	// Agrega colisiones potenciales entre pelotas al vector @collisions Si
	// el nodo tiene hijos entonces se navega a sus nodos hijos y se manda a llamar
	// recursivamente la funcion hasta que no tenga mas hijos y en ese caso iteramos
	// sobre el set de pelotas para agregarlas al vector de @collisions.
	void potentialBallBallCollisions(vector<BallPair> &collisions) ;

	// Agrega las colisiones potenciales entre cada pared y las pelotas. 
	void potentialBallWallCollisions(vector<BallWallPair> &collisions);

	// Nos ayuda a debuguear la creacion del arbol
	void printCHildren(int tab, GLOctree* raiz);
	
};

//Va a encontrar todas las colisiones posibles entre pares de pelotas. Va a
//guardar los resultados en el vector @potentialCollisions. Realiza esto al 
//llamar uno de los metodos del Octree potentialBallBallCollisions()
void potentialBallBallCollisions(vector<BallPair> &potentialCollisions,
	vector<Ball*> &balls, GLOctree* octree);

//Busca todas las posibles colisiones entre una pelota y una pared. Va a guardar
//los resultados en el vector @potentialCollisions. Realiza esto al llamar uno
//de los metodos del Octree potentialBallWallCollsions()
void potentialBallWallCollisions(vector<BallWallPair> &potentialCollisions,
	vector<Ball*> &balls, GLOctree* octree);

// Prueba si dos pelotas estan colisionando una contra la otra. Hace esto dependiendo
// si la distancia de cualquiera de las dos pelotas es menor que la suma de los radios.
// Si estan muy cerca queremos hacer una revisada extra de que si estan colisionando.
// Si las pelotas se estan moviendo en sentidos opuestos una de otra entonces decimos que
// no estan colisinando, porque significa que probablemente acaban de rebotar.
bool testBallBallCollision(Ball* b1, Ball* b2);

// Va a encontrar todas las colisiones potenciales entre pelotas usando 
// potentialBallBallCollsions(). Luego va a recorrer todo el vector @bsp que 
// contiene las colisiones potenciales y revisar si si hay colisiones con
// testBallBallCollision(). En el caso que si exista va a realizar las 
// matematicas para cambiar la direccion de ambas pelotas y que reboten sin que
// pierdan su velocidad.
void handleBallBallCollisions(vector<Ball*> &balls, GLOctree* octree);


// Regresa la direccion del centro del cubo a una pared en particular.
vector3 wallDirection(Wall wall);

// Prueba si una pelota y una pared estan realmente colisionando. Revisa esto al
// sumar la posicion actual de la pelota mas su radio es mayor que el centro de
// de la caja. Tambien hacemos una revision etra al ver si la pelota acaba o no
// de rebotar sobre la pared.
bool testBallWallCollision(Ball* ball, Wall wall);

//Va a encontrar todas las colisiones potenciales entre pelotas y paredes usando
//potentialBallWallCollisions(). Despues va a iterar sobre ese vector llamando
//testBallWallCollision() para saber si es en verdad una colision. De ser asi,
//refleja la pelota fuera de la pared sin perder su velocidad.
void handleBallWallCollisions(vector<Ball*> &balls, GLOctree* octree);

// Prueba todos los escenarios posibles. Todas las colisiones entre pelotas. 
void iterativeMethodBallBallCollision(vector<BallPair> &potentialCollisions, vector<Ball*> &balls);

// Prueba todos los escenarios posibles. Todas las colisiones entre pelotas y paredes. 
void iterativeMethodBallWallCollision(vector<BallWallPair> &potentialCollisions, vector<Ball*> &balls);
