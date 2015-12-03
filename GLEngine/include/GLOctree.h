#pragma once
#include "MathUtils.h"
#include <set>
#include <vector>
#include <GLMesh.h>
using namespace std;

const int MAX_OCTREE_DEPTH = 15;
const int MIN_BALLS_PER_OCTREE = 1;
const int MAX_BALLS_PER_OCTREE = 5;
const float BOX_SIZE = 30.0f; //The length of one side of the box

//Stores information regarding a ball
typedef GLMesh Ball;

enum Wall { WALL_LEFT, WALL_RIGHT, WALL_FAR, WALL_NEAR, WALL_TOP, WALL_BOTTOM };

//Stores a pair of balls
struct BallPair {
	Ball* ball1;
	Ball* ball2;
};

//Stores a ball and a wall
struct BallWallPair {
	Ball* ball;
	Wall wall;
};

//Our data structure for making collision detection faster
class GLOctree {
private:
	vector3 corner1; //(minX, minY, minZ)
	vector3 corner2; //(maxX, maxY, maxZ)
	vector3 center;//((minX + maxX) / 2, (minY + maxY) / 2, (minZ + maxZ) / 2)

	/* The children of this, if this has any.  children[0][*][*] are the
	* children with x coordinates ranging from minX to centerX.
	* children[1][*][*] are the children with x coordinates ranging from
	* centerX to maxX.  Similarly for the other two dimensions of the
	* children array.
	*/
	GLOctree *children[2][2][2];
	//Whether this has children
	bool hasChildren;
	//The balls in this, if this doesn't have any children
	set<Ball*> balls;
	//The depth of this in the tree
	int depth;
	//The number of balls in this, including those stored in its children
public:
	int numBalls;
private:
	//Adds a ball to or removes one from the children of this
	void fileBall(Ball* ball, vector3 pos, bool addBall);

	//Creates children of this, and moves the balls in this to the children
	void haveChildren() ;

	//Adds all balls in this or one of its descendants to the specified set
	void collectBalls(set<Ball*> &bs) ;

	//Destroys the children of this, and moves all balls in its descendants
	//to the "balls" set
	void destroyChildren() ;

	//Removes the specified ball at the indicated position
	void remove(Ball* ball, vector3 pos) ;

	/* Helper fuction for potentialBallWallCollisions(vector).  Adds
	* potential ball-wall collisions to cs, where w is the type of wall,
	* coord is the relevant coordinate of the wall ('x', 'y', or 'z'), and
	* dir is 0 if the wall is in the negative direction and 1 if it is in
	* the positive direction.  Assumes that this is in the extreme
	* direction of the coordinate, e.g. if w is WALL_TOP, the function
	* assumes that this is in the far upward direction.
	*/
	void potentialBallWallCollisions(vector<BallWallPair> &cs,
		Wall w, char coord, int dir);
public:
	//Constructs a new Octree.  c1 is (minX, minY, minZ), c2 is (maxX, maxY,
	//maxZ), and d is the depth, which starts at 1.
	GLOctree(vector3 c1, vector3 c2, int d);
	//Destructor
	~GLOctree();
	//Adds a ball to this
	void add(Ball* ball);
	//Removes a ball from this
	void remove(Ball* ball);
	//Changes the position of a ball in this from oldPos to ball->pos
	void ballMoved(Ball* ball, vector3 oldPos);
	//Adds potential ball-ball collisions to the specified set
	void potentialBallBallCollisions(vector<BallPair> &collisions) ;
	//Adds potential ball-wall collisions to the specified set
	void potentialBallWallCollisions(vector<BallWallPair> &collisions);
	void printCHildren(int tab, GLOctree* raiz);
	
};

//Puts potential ball-ball collisions in potentialCollisions.  It must return
//all actual collisions, but it need not return only actual collisions.
void potentialBallBallCollisions(vector<BallPair> &potentialCollisions,
	vector<Ball*> &balls, GLOctree* octree);

//Puts potential ball-wall collisions in potentialCollisions.  It must return
//all actual collisions, but it need not return only actual collisions.
void potentialBallWallCollisions(vector<BallWallPair> &potentialCollisions,
	vector<Ball*> &balls, GLOctree* octree);
//Returns whether two balls are colliding
bool testBallBallCollision(Ball* b1, Ball* b2);

//Handles all ball-ball collisions
void handleBallBallCollisions(vector<Ball*> &balls, GLOctree* octree);


//Returns the direction from the origin to the wall
vector3 wallDirection(Wall wall);
//Returns whether a ball and a wall are colliding
bool testBallWallCollision(Ball* ball, Wall wall);

//Handles all ball-wall collisions
void handleBallWallCollisions(vector<Ball*> &balls, GLOctree* octree);
