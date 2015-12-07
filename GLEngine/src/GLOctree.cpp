#include "GLOctree.h"
// Constructor del octree.
//@param	c1	Esquina inferior.
//@param	c2	Esquina superior.
//@param	d 	La profundidad del nodo.
GLOctree::GLOctree(vector3 c1, vector3 c2, int d) {
	corner1 = c1;
	corner2 = c2;
	center = (c1 + c2) / 2;
	depth = d;
	numBalls = 0;
	hasChildren = false;
		
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			for (int k = 0; k < 2; k++)
			{
				children[i][j][k] = NULL;
			}
		}
	}
}

// Destructor
GLOctree::~GLOctree() {
	if (hasChildren) {
		destroyChildren();
	}
}

// fileBall() se encarga de agregar o eliminar pelotas al nodo adecuado en el octree (se
// basa en la posicion de la pelota). Basicamente va a encontra a cual hijo esa pelota
// en particular pertenece y la va a remover o agregar dependiendo si @addBall es
// verdadera o falsa.
//@param    ball		    Pelotas.
//@param	pos				La posicion.
//@param	addBall			verdadero para agregar la pelota.

void GLOctree::fileBall(Ball* ball, vector3 pos, bool addBall) {
	// Determina en cual de sus hijos la pelota pertenece. 
	for (int x = 0; x < 2; x++) {
		if (x == 0) {
			if (pos.v[0] - ball->getRadius() > center.v[0]) {
				continue;
			}
		}
		else if (pos.v[0] + ball->getRadius() < center.v[0]) {
			continue;
		}

		for (int y = 0; y < 2; y++) {
			if (y == 0) {
				if (pos.v[1] - ball->getRadius() > center.v[1]) {
					continue;
				}
			}
			else if (pos.v[1] + ball->getRadius() < center.v[1]) {
				continue;
			}

			for (int z = 0; z < 2; z++) {
				if (z == 0) {
					if (pos.v[2] - ball->getRadius() > center.v[2]) {
						continue;
					}
				}
				else if (pos.v[2] + ball->getRadius() < center.v[2]) {
					continue;
				}

				// Agrega o elimina la pelota.
				if (addBall) {
					children[x][y][z]->add(ball);
				}
				else {
					children[x][y][z]->remove(ball, pos);
				}
			}
		}
	}
}

// Divide un cubo en particular a cubos mas pequenos. Es decir de un nodo crea a otros
// nodos mas. Toma todos los hijos del nodo en cuestion y los agrega a los nodos hijo de
// ese nodo.

void GLOctree::haveChildren() {
	for (int x = 0; x < 2; x++) {
		float minX;
		float maxX;
		if (x == 0) {
			minX = corner1.v[0];
			maxX = center.v[0];
		}
		else {
			minX = center.v[0];
			maxX = corner2.v[0];
		}

		for (int y = 0; y < 2; y++) {
			float minY;
			float maxY;
			if (y == 0) {
				minY = corner1.v[1];
				maxY = center.v[1];
			}
			else {
				minY = center.v[1];
				maxY = corner2.v[1];
			}

			for (int z = 0; z < 2; z++) {
				float minZ;
				float maxZ;
				if (z == 0) {
					minZ = corner1.v[2];
					maxZ = center.v[2];
				}
				else {
					minZ = center.v[2];
					maxZ = corner2.v[2];
				}
				//Crea un nuevo octree y la profundidad se incrementa en 1.
				children[x][y][z] = new GLOctree(vector3(minX, minY, minZ),
					vector3(maxX, maxY, maxZ),
					depth + 1);
			}
		}
	}

	// Itera sobre todas las bolas del antiguo nodo y las va colocando en los nuevos hijos con fileBall(). 
	for (set<Ball*>::iterator it = balls.begin(); it != balls.end();
		it++) {
		Ball* ball = *it;
		fileBall(ball, ball->getPositionV(), true);
	}
	// Elimina todas las bolas de ese nodo en particula ya que fueron colocadas en sus hijos.
	balls.clear();
	// Elimina todas las bolas de ese nodo en particula ya que fueron colocadas en sus hijos.
	hasChildren = true;
}

// Se encarga de saber cuales bolas estan contenidas en un nodo particular incluyendo
// sus hijos y los va a colocar en el set @bs. Si este nodo en cuestion
// tiene hijos va a realizar una llamada recursiva hasta sus hijos que no tienen mas
// hijos y va a colocar las bolas de cada nodo hijo en el set bs. Este
// metodo se va a usar particularmente cuando queremos juntar los nodos.
void GLOctree::collectBalls(set<Ball*> &bs) {
	if (hasChildren) {
		for (int x = 0; x < 2; x++) {
			for (int y = 0; y < 2; y++) {
				for (int z = 0; z < 2; z++) {
					children[x][y][z]->collectBalls(bs);
				}
			}
		}
	}
	else {
		for (set<Ball*>::iterator it = balls.begin(); it != balls.end();
			it++) {
			Ball* ball = *it;
			bs.insert(ball);
		}
	}
}

// Destruye todos los hijos del nodo y mueve todas las pelotas al set de pelotas.
void GLOctree::destroyChildren() {
	// Destruye todos los hijos del nodo y mueve todas las pelotas al set de pelotas.
	collectBalls(balls);
	// Una vez que tenemos guardadas las pelotas procedemos a eliminar sus hijos.
	for (int x = 0; x < 2; x++) {
		for (int y = 0; y < 2; y++) {
			for (int z = 0; z < 2; z++) {
				delete children[x][y][z];
				children[x][y][z] = NULL;
			}
		}
	}
	// Una vez que tenemos guardadas las pelotas procedemos a eliminar sus hijos.
	hasChildren = false;
}

// Remueve la pelota del octree de la posicion particular @pos.
//@param    ball	Las pelotas.
//@param	pos		La posicion.
void GLOctree::remove(Ball* ball, vector3 pos) {
	// Decrementamos el numero de @numBalls.
	numBalls--;
	// Si el numero de @numBalls es menor al que establecimos en un principio que debe contener
	// cada nodo procedemos a destruir ese nodo. 
	if (hasChildren && numBalls < MIN_BALLS_PER_OCTREE) {
		destroyChildren();
	}
	// De lo contrario si todavia es mayor al numero menor de pelotas por caja, procedemos a
	// eliminar esa pelota con esa posicion del nodo. Por eso ponemos false en fileBall() 
	if (hasChildren) {
		fileBall(ball, pos, false);
	}
	// Si no tiene hijos entonces simplemente la borramos de este nodo en particular.
	else {
		balls.erase(ball);
	}
}

// Este metodo es llamado individualmente para cada una de las paredes de nuestra caja.
// Guarda las colisiones potenciales en un vector @cs. Recursivamente se manda a llamar
// en la mitad correcta del hijo. Una vez que ya no hay mas hijos se guarda en el vector
// @cs las colisiones potenciales entre la pared y la pelota.
//@param [in,out]	cs	Vector en donde guardamos la posibles colisiones.
//@param	w		  	Pared a procesar.
//@param	coord	  	Las coordenadas.
//@param	dir		  	The direccion.
void GLOctree::potentialBallWallCollisions(vector<BallWallPair> &cs,
	Wall w, char coord, int dir) {
	if (hasChildren) {
		for (int dir2 = 0; dir2 < 2; dir2++) {
			for (int dir3 = 0; dir3 < 2; dir3++) {
				GLOctree *child = NULL;
				switch (coord) {
				case 'x':
					child = children[dir][dir2][dir3];
					break;
				case 'y':
					child = children[dir2][dir][dir3];
					break;
				case 'z':
					child = children[dir2][dir3][dir];
					break;
				}

				child->potentialBallWallCollisions(cs, w, coord, dir);
			}
		}
	}
	else {
		// Agrega (pelota, pared) 
		for (set<Ball*>::iterator it = balls.begin(); it != balls.end();
			it++) {
			Ball* ball = *it;
			BallWallPair bwp;
			bwp.ball = ball;
			bwp.wall = w;
			cs.push_back(bwp);
		}
	}
}

// Agrega una pelota al nodo.
//@param ball Las pelotas.
void GLOctree::add(Ball* ball) {
	numBalls++;
	// Si el numero de @numBalls es mayor que el numero maximo permitido por MAX_BALLS_PER_OCTREE y
	// la profundidad es menor que el permitido por MAX_OCTREE_DEPTH y ese nodo no tiene hijos
	// entonces se divide el nodo con haveChildren() 
	if (!hasChildren && depth < MAX_OCTREE_DEPTH &&
		numBalls > MAX_BALLS_PER_OCTREE) {
		haveChildren();
	}
	// Si el nodo tiene hijos simplemente se agrega la pelota y su posicion. De lo contrario
	// solamente se agrega la pelota al set. 
	if (hasChildren) {
		fileBall(ball, ball->getPositionV(), true);
	}
	else {
		balls.insert(ball);
	}
}

// Remueve la pelota del nodo.Hace una llamada al otro metodo remove.
//@param	ball	La pelota a remover.
void GLOctree::remove(Ball* ball) {
	remove(ball, ball->getPositionV());
}

// Se manda a llamar cada vez que la pelota cambia de posicion.Esto sucede en
// GLScene.cpp.Para hacer esto solamente eliminamos la pelota de la posicion antigua y
// la agregamos en su nueva posicion.
//@param	ball	Pelota para actualizar.
//@param	oldPos			Posicion Antigua.
void GLOctree::ballMoved(Ball* ball, vector3 oldPos) {
	remove(ball, oldPos);
	add(ball);
}
//Agrega colisiones potenciales entre pelotas al vector @collisions Si
//el nodo tiene hijos entonces se navega a sus nodos hijos y se manda a llamar
//recursivamente la funcion hasta que no tenga mas hijos y en ese caso iteramos
//sobre el set de pelotas para agregarlas al vector de @collisions.
void GLOctree::potentialBallBallCollisions(vector<BallPair> &collisions) {
	if (hasChildren) {
		for (int x = 0; x < 2; x++) {
			for (int y = 0; y < 2; y++) {
				for (int z = 0; z < 2; z++) {
					children[x][y][z]->
						potentialBallBallCollisions(collisions);
				}
			}
		}
	}
	else {
		// Agregamos todos los pares de pelotas. 
		for (set<Ball*>::iterator it = balls.begin(); it != balls.end();it++) {
			Ball* ball1 = *it;
			for (set<Ball*>::iterator it2 = balls.begin();it2 != balls.end(); it2++) {
				Ball* ball2 = *it2;
				// Nos aseguramos que agregamos cada par solamente una vez. 
				if (ball1 < ball2) {
					BallPair bp;
					bp.ball1 = ball1;
					bp.ball2 = ball2;
					collisions.push_back(bp);
				}
			}
		}
	}
}

// Agrega las colisiones potenciales entre cada pared y las pelotas. 
void GLOctree::potentialBallWallCollisions(vector<BallWallPair> &collisions) {
	potentialBallWallCollisions(collisions, WALL_LEFT, 'x', 0);
	potentialBallWallCollisions(collisions, WALL_RIGHT, 'x', 1);
	potentialBallWallCollisions(collisions, WALL_BOTTOM, 'y', 0);
	potentialBallWallCollisions(collisions, WALL_TOP, 'y', 1);
	potentialBallWallCollisions(collisions, WALL_FAR, 'z', 0);
	potentialBallWallCollisions(collisions, WALL_NEAR, 'z', 1);
}

// Nos ayuda a debuguear la creacion del arbol
void GLOctree::printCHildren(int tab, GLOctree* raiz)
{
		
	if (raiz == NULL)
	{
		return;
	}
	for (int i = 0; i < tab; i++)printf(" ");

		
	printf("%i, E1: [%.2f][%.2f][%.2f], E2: [%.2f][%.2f][%.2f]\n", raiz->numBalls, raiz->corner1.v[0], raiz->corner1.v[1], raiz->corner1.v[2], raiz->corner2.v[0], raiz->corner2.v[1], raiz->corner2.v[2]);
	for (auto ball:raiz->balls)
	{
		for (int i = 0; i < tab; i++)printf(" ");
		printf("PosBall: [%.2f][%.2f][%.2f]\n", ((Ball*)ball)->getPositionV(), ball->getPositionV().v[1], ball->getPositionV().v[2]);
	}
	if (hasChildren)
		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				for (int k = 0; k < 2; k++)
				{
					printCHildren(tab + 2, raiz->children[i][j][k]);
				}
			}
		}

}

//Va a encontrar todas las colisiones posibles entre pares de pelotas. Va a
//guardar los resultados en el vector @potentialCollisions. Realiza esto al 
//llamar uno de los metodos del Octree potentialBallBallCollisions()
void potentialBallBallCollisions(vector<BallPair> &potentialCollisions,
	vector<Ball*> &balls, GLOctree* octree) {
	//Metodo Rapido
	octree->potentialBallBallCollisions(potentialCollisions);

	//Metodo Lento
	//iterativeMethodBallBallCollision(potentialCollisions, balls);
}

//Busca todas las posibles colisiones entre una pelota y una pared. Va a guardar
//los resultados en el vector @potentialCollisions. Realiza esto al llamar uno
//de los metodos del Octree potentialBallWallCollsions()
void potentialBallWallCollisions(vector<BallWallPair> &potentialCollisions,
	vector<Ball*> &balls, GLOctree* octree) {
	//Metodo Rapido
	octree->potentialBallWallCollisions(potentialCollisions);

	//Metodo Lento
	//iterativeMethodBallWallCollision(potentialCollisions, balls);
}

// Prueba si dos pelotas estan colisionando una contra la otra. Hace esto dependiendo
// si la distancia de cualquiera de las dos pelotas es menor que la suma de los radios.
// Si estan muy cerca queremos hacer una revisada extra de que si estan colisionando.
// Si las pelotas se estan moviendo en sentidos opuestos una de otra entonces decimos que
// no estan colisinando, porque significa que probablemente acaban de rebotar.
bool testBallBallCollision(Ball* b1, Ball* b2) {
	//Revisa si las pelotas estan lo suficientemente cerca.
	float r = b1->getRadius() + b2->getRadius();
	if ((b1->getPositionV() - b2->getPositionV()).magnitudeSquared() < r * r) {
		//Revisa si las pelotas se estan acercando hacia ellas.
		vector3 netVelocity = b1->getVelocity() - b2->getVelocity();
		vector3 displacement = b1->getPositionV() - b2->getPositionV();
		return dot(netVelocity, displacement) < 0;
	}
	else
		return false;
}

// Va a encontrar todas las colisiones potenciales entre pelotas usando 
// potentialBallBallCollsions(). Luego va a recorrer todo el vector @bsp que 
// contiene las colisiones potenciales y revisar si si hay colisiones con
// testBallBallCollision(). En el caso que si exista va a realizar las 
// matematicas para cambiar la direccion de ambas pelotas y que reboten sin que
// pierdan su velocidad.
void handleBallBallCollisions(vector<Ball*> &balls, GLOctree* octree) {
	vector<BallPair> bps;
	potentialBallBallCollisions(bps, balls, octree);

	for (unsigned int i = 0; i < bps.size(); i++) {
		BallPair bp = bps[i];

		Ball* b1 = bp.ball1;
		Ball* b2 = bp.ball2;
		if (testBallBallCollision(b1, b2)) {
			// Hacemos que las pelotas parezcan que rebotan.
			vector3 displacement = (b1->getPositionV() - b2->getPositionV()).normalise() ;
			b1->setVelocity(b1->getVelocity() - ((displacement * 2) * dot(b1->getVelocity(), displacement)) );
			b2->setVelocity(b2->getVelocity() - ((displacement * 2) * dot(b2->getVelocity(), displacement)) );
		}
	}
}

// Regresa la direccion del centro del cubo a una pared en particular.
vector3 wallDirection(Wall wall) {
	switch (wall) {
	case WALL_LEFT:
		return vector3(-1, 0, 0);
	case WALL_RIGHT:
		return vector3(1, 0, 0);
	case WALL_FAR:
		return vector3(0, 0, -1);
	case WALL_NEAR:
		return vector3(0, 0, 1);
	case WALL_TOP:
		return vector3(0, 1, 0);
	case WALL_BOTTOM:
		return vector3(0, -1, 0);
	default:
		return vector3(0, 0, 0);
	}
}

// Prueba si una pelota y una pared estan realmente colisionando. Revisa esto al
// sumar la posicion actual de la pelota mas su radio es mayor que el centro de
// de la caja. Tambien hacemos una revision etra al ver si la pelota acaba o no
// de rebotar sobre la pared.
bool testBallWallCollision(Ball* ball, Wall wall) {
	vector3 dir = wallDirection(wall);
	return dot(ball->getPositionV(), dir) + ball->getRadius() > BOX_SIZE / 2 &&
		dot(ball->getVelocity(), dir) > 0;

}

//Va a encontrar todas las colisiones potenciales entre pelotas y paredes usando
//potentialBallWallCollisions(). Despues va a iterar sobre ese vector llamando
//testBallWallCollision() para saber si es en verdad una colision. De ser asi,
//refleja la pelota fuera de la pared sin perder su velocidad.
void handleBallWallCollisions(vector<Ball*> &balls, GLOctree* octree) {
	vector<BallWallPair> bwps;
	potentialBallWallCollisions(bwps, balls, octree);
	for (unsigned int i = 0; i < bwps.size(); i++) {
		BallWallPair bwp = bwps[i];

		Ball* b = bwp.ball;
		Wall w = bwp.wall;
		if (testBallWallCollision(b, w)) {
			//Hace que la pelota rebote de la pared.
			vector3 dir = (wallDirection(w)).normalise();
			b->setVelocity(b->getVelocity() - ((dir * 2) * dot(b->getVelocity(), dir))); 
		}
	}

}

//Prueba todos los escenarios posibles. Todas las colisiones entre pelotas.
void iterativeMethodBallBallCollision(vector<BallPair> &potentialCollisions, vector<Ball*> &balls)
{
	for (unsigned int i = 0; i < balls.size(); i++) {
		for (unsigned int j = i + 1; j < balls.size(); j++) {
			BallPair bp;
			bp.ball1 = balls[i];
			bp.ball2 = balls[j];
			potentialCollisions.push_back(bp);
		}
	}
}

//Prueba todos los escenarios posibles. Todas las colisiones entre pelotas y paredes.
void iterativeMethodBallWallCollision(vector<BallWallPair> &potentialCollisions, vector<Ball*> &balls)
{
	Wall walls[] = { WALL_LEFT,
		WALL_RIGHT,
		WALL_FAR,
		WALL_NEAR,
		WALL_TOP,
		WALL_BOTTOM };
	for (unsigned int i = 0; i < balls.size(); i++) {
		for (int j = 0; j < 6; j++) {
			BallWallPair bwp;
			bwp.ball = balls[i];
			bwp.wall = walls[j];
			potentialCollisions.push_back(bwp);
		}
	}
}
