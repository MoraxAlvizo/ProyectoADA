#include "GLParallelOctree.h"

int getNumNodes(int level)
{
    int total = 0;
    int power = 1;

    for(int i = 0; i < level; i++)
    {
        power *= 8;
        total += power;
    }

    return total+1;
}

GLParallelOctree::GLParallelOctree(vector3 corner1, vector3 corner2, int depth)
{
    //ctor
    this->corner1 = corner1;
    this->corner2 = corner2;
    this->depth  = depth;
    this->tree = new Box[getNumNodes(depth-1)];

	createBoxes();
}

GLParallelOctree::~GLParallelOctree()
{
    //dtor
}

int getPower8(int level)
{
    int power = 1;
    for(int i = 0; i < level; i++)
        power *= 8;

    return power;
}

inline int getParent(int node)
{
    return ((node-1) >> 3);
}

/** CreateBoxes */
void GLParallelOctree::createBoxes()
{
    tree[0].corner1 = corner1;
    tree[0].corner2 = corner2;
    tree[0].centro = (corner1 + corner2) / 2;

    int offset=1;
    for(int level = 1; level < depth; level++)
    {
        int numNodes = getPower8(level);
		#pragma omp parallel for num_threads (4)
        for(int node = offset; node < getPower8(level) + offset; node++)
        {
            int parent = getParent(node);
            int box = (node-1) & MASK;
            vector3 corner1Parent = tree[parent].corner1;
            vector3 corner2Parent = tree[parent].corner2;
            vector3 centroParent  = tree[parent].centro;
            int minX, minY, minZ;
            int maxX, maxY, maxZ;

            if(!(box & X))
            {
                minX = corner1Parent.v[0];
                maxX = centroParent.v[0];
            }
            else
            {
                minX = centroParent.v[0];
                maxX = corner2Parent.v[0];
            }

            if(!(box & Y))
            {
                minY = corner1Parent.v[1];
                maxY = centroParent.v[1];
            }
            else
            {
                minY = centroParent.v[1];
                maxY = corner2Parent.v[1];
            }

            if(!(box & Z))
            {
                minZ = corner1Parent.v[2];
                maxZ = centroParent.v[2];
            }
            else
            {
                minZ = centroParent.v[2];
                maxZ = corner2Parent.v[2];
            }

			tree[node] = Box(vector3(minX, minY, minZ), vector3(maxX, maxY, maxZ),level == depth - 1);
        }
        offset += numNodes;
    }
}

void GLParallelOctree::printPOctree()
{
	int offset=1;
    for(int level = 1; level < depth; level++)
    {
        int numNodes = getPower8(level);
        for(int node = offset; node < getPower8(level) + offset; node++)
        {
			printf("nodo %i C1[%.2f,%.2f,%.2f] C2[%.2f,%.2f,%.2f]", node,
														tree[node].corner1.v[0],
														tree[node].corner1.v[1],
														tree[node].corner1.v[2],
														tree[node].corner2.v[0],
														tree[node].corner2.v[1],
														tree[node].corner2.v[2]);
			if(tree[node].balls != NULL)
				printf(" Num Balls: %i \n", tree[node].balls->size());
			else
				printf("\n");
        }
        offset += numNodes;
    }
}

void GLParallelOctree::insertBalls(vector<Ball*> &balls)
{
	#pragma omp parallel for num_threads(4)
	for(int i = 0; i < NUM_BALLS; i++)
	{
		stack<int> pila;
		pila.push(0);
		while(!pila.empty())
		{
			Ball* ball = balls[i];
			int node = pila.top();
			pila.pop();
			for (int x = 0; x < 2; x++) {
				if (x == 0) {
					if (ball->getPositionV().v[0] - ball->getRadius() > tree[node].centro.v[0]) 
						continue;	
				}
				else if (ball->getPositionV().v[0] + ball->getRadius() < tree[node].centro.v[0]) {
					continue;
				}

				for (int y = 0; y < 2; y++) {
					if (y == 0) {
						if (ball->getPositionV().v[1] - ball->getRadius() > tree[node].centro.v[1]) {
							continue;
						}
					}
					else if (ball->getPositionV().v[1] + ball->getRadius() < tree[node].centro.v[1]) {
						continue;
					}

					for (int z = 0; z < 2; z++) {
						if (z == 0) {
							if (ball->getPositionV().v[2] - ball->getRadius() > tree[node].centro.v[2]) {
								continue;
							}
						}
						else if (ball->getPositionV().v[2] + ball->getRadius() < tree[node].centro.v[2]) {
							continue;
						}
				
						int children = 0; 
						children |= (x)?X:0;
						children |= (y)?Y:0;
						children |= (z)?Z:0;
						children += (node << 3) +1; 
						// SI es un nodo hoja insertar
						if(tree[children].balls != NULL)
						{
							#pragma omp critical
							tree[children].balls->insert(ball);
						}
						else
						{
							pila.push(children);
						}
					}
				}
			}
		}

	}
}

/** Colissions */
void GLParallelOctree::Colissions()
{
	int startPos = getNumNodes(depth - 2);
	int total = startPos + getPower8(depth - 1);

	WallCollisions() ;

	#pragma omp parallel for num_threads (4)
	for(int i = START_POS; i < TOTAL_NODES; i++)
	{
		for (set<Ball*>::iterator it = tree[i].balls->begin(); it != tree[i].balls->end();it++) {
			Ball* ball1 = *it;
			for (set<Ball*>::iterator it2 = it ; it2 != tree[i].balls->end(); it2++) {
				Ball* ball2 = *it2;
				//This test makes sure that we only add each pair once
				if (ball1 < ball2 && testBallBallCollision(ball1, ball2)) 
				{
					//Make the balls reflect off of each other
					#pragma omp critical
					{
						vector3 displacement = (ball1->getPositionV() - ball2->getPositionV()).normalise() ;
						ball1->setVelocity(ball1->getVelocity() - ((displacement * 2) * dot(ball1->getVelocity(), displacement)) );
						ball2->setVelocity(ball2->getVelocity() - ((displacement * 2) * dot(ball2->getVelocity(), displacement)) );
					}
				}
				
			}	
		}
		// Limpiar Caja
		tree[i].balls->clear();
	}
	
}

void GLParallelOctree::potentialBallWallCollisions(Wall w, char coord, int dir, int node) {
	if (tree[node].balls == NULL) {
		//Recursively call potentialBallWallCollisions on the correct
		//half of the children (e.g. if w is WALL_TOP, call it on
		//children above centerY)
		for (int dir2 = 0; dir2 < 2; dir2++) {
			for (int dir3 = 0; dir3 < 2; dir3++) {
				int children = 0; 
				
				switch (coord) {
				case 'x':
					children |= (dir)?X:0;
					children |= (dir2)?Y:0;
					children |= (dir3)?Z:0;
					break;
				case 'y':
					children |= (dir2)?X:0;
					children |= (dir)?Y:0;
					children |= (dir3)?Z:0;
					break;
				case 'z':
					children |= (dir2)?X:0;
					children |= (dir3)?Y:0;
					children |= (dir)?Z:0;
					break;
				}
				children += (node << 3) +1; 
				potentialBallWallCollisions(w, coord, dir,children);
			}
		}
	}
	else {
		//Add (ball, w) for all balls in this
		for (set<Ball*>::iterator it = tree[node].balls->begin(); it != tree[node].balls->end();
			it++) 
		{
				Ball* ball = *it;
				if (testBallWallCollision(ball, w)) {
					//Make the ball reflect off of the wall
					vector3 dir = (wallDirection(w)).normalise();
					#pragma omp critical
					ball->setVelocity(ball->getVelocity() - ((dir * 2) * dot(ball->getVelocity(), dir))); 
				}
		}
	}
}

void GLParallelOctree::WallCollisions() {
	#pragma omp parallel sections
	{
		#pragma omp section
		{
			potentialBallWallCollisions(WALL_LEFT, 'x', 0, 0);
		}
	
		#pragma omp section
		{
			potentialBallWallCollisions(WALL_RIGHT, 'x', 1,0);
		}
	
		#pragma omp section
		{
			potentialBallWallCollisions(WALL_BOTTOM, 'y', 0,0);
		}
	
		#pragma omp section
		{
			potentialBallWallCollisions(WALL_TOP, 'y', 1,0);
		}
	
		#pragma omp section
		{
			potentialBallWallCollisions(WALL_FAR, 'z', 0,0);
		}
	
		#pragma omp section
		{
			potentialBallWallCollisions(WALL_NEAR, 'z', 1,0);
		}
	}
}