#include "GLOctree.h"

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

//Destructor
GLOctree::~GLOctree() {
	if (hasChildren) {
		destroyChildren();
	}
}

void GLOctree::fileBall(Ball* ball, vector3 pos, bool addBall) {
	//Figure out in which child(ren) the ball belongs
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

				//Add or remove the ball
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

				children[x][y][z] = new GLOctree(vector3(minX, minY, minZ),
					vector3(maxX, maxY, maxZ),
					depth + 1);
			}
		}
	}

	//Remove all balls from "balls" and add them to the new children
	for (set<Ball*>::iterator it = balls.begin(); it != balls.end();
		it++) {
		Ball* ball = *it;
		fileBall(ball, ball->getPositionV(), true);
	}
	balls.clear();

	hasChildren = true;
}

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

void GLOctree::destroyChildren() {
	//Move all balls in descendants of this to the "balls" set
	collectBalls(balls);

	for (int x = 0; x < 2; x++) {
		for (int y = 0; y < 2; y++) {
			for (int z = 0; z < 2; z++) {
				delete children[x][y][z];
				children[x][y][z] = NULL;
			}
		}
	}

	hasChildren = false;
}

void GLOctree::remove(Ball* ball, vector3 pos) {
	numBalls--;

	if (hasChildren && numBalls < MIN_BALLS_PER_OCTREE) {
		destroyChildren();
	}

	if (hasChildren) {
		fileBall(ball, pos, false);
	}
	else {
		balls.erase(ball);
	}
}

void GLOctree::potentialBallWallCollisions(vector<BallWallPair> &cs,
	Wall w, char coord, int dir) {
	if (hasChildren) {
		//Recursively call potentialBallWallCollisions on the correct
		//half of the children (e.g. if w is WALL_TOP, call it on
		//children above centerY)
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
		//Add (ball, w) for all balls in this
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

void GLOctree::add(Ball* ball) {
	numBalls++;
	if (!hasChildren && depth < MAX_OCTREE_DEPTH &&
		numBalls > MAX_BALLS_PER_OCTREE) {
		haveChildren();
	}

	if (hasChildren) {
		fileBall(ball, ball->getPositionV(), true);
	}
	else {
		balls.insert(ball);
	}
}

void GLOctree::remove(Ball* ball) {
	remove(ball, ball->getPositionV());
}

void GLOctree::ballMoved(Ball* ball, vector3 oldPos) {
	remove(ball, oldPos);
	add(ball);
}

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
		//Add all pairs (ball1, ball2) from balls
		for (set<Ball*>::iterator it = balls.begin(); it != balls.end();it++) {
			Ball* ball1 = *it;
			for (set<Ball*>::iterator it2 = balls.begin();it2 != balls.end(); it2++) {
				Ball* ball2 = *it2;
				//This test makes sure that we only add each pair once
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

void GLOctree::potentialBallWallCollisions(vector<BallWallPair> &collisions) {
	potentialBallWallCollisions(collisions, WALL_LEFT, 'x', 0);
	potentialBallWallCollisions(collisions, WALL_RIGHT, 'x', 1);
	potentialBallWallCollisions(collisions, WALL_BOTTOM, 'y', 0);
	potentialBallWallCollisions(collisions, WALL_TOP, 'y', 1);
	potentialBallWallCollisions(collisions, WALL_FAR, 'z', 0);
	potentialBallWallCollisions(collisions, WALL_NEAR, 'z', 1);
}

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

void potentialBallBallCollisions(vector<BallPair> &potentialCollisions,
	vector<Ball*> &balls, GLOctree* octree) {
	//Fast method
	octree->potentialBallBallCollisions(potentialCollisions);

	/*
	//Slow method
	for(unsigned int i = 0; i < balls.size(); i++) {
	for(unsigned int j = i + 1; j < balls.size(); j++) {
	BallPair bp;
	bp.ball1 = balls[i];
	bp.ball2 = balls[j];
	potentialCollisions.push_back(bp);
	}
	}
	//*/
}

//Puts potential ball-wall collisions in potentialCollisions.  It must return
//all actual collisions, but it need not return only actual collisions.
void potentialBallWallCollisions(vector<BallWallPair> &potentialCollisions,
	vector<Ball*> &balls, GLOctree* octree) {
	//Fast method
	octree->potentialBallWallCollisions(potentialCollisions);

	/*
	//Slow method
	Wall walls[] =
	{WALL_LEFT, WALL_RIGHT, WALL_FAR, WALL_NEAR, WALL_TOP, WALL_BOTTOM};
	for(unsigned int i = 0; i < balls.size(); i++) {
	for(int j = 0; j < 6; j++) {
	BallWallPair bwp;
	bwp.ball = balls[i];
	bwp.wall = walls[j];
	potentialCollisions.push_back(bwp);
	}
	}
	//*/
}

//Returns whether two balls are colliding
bool testBallBallCollision(Ball* b1, Ball* b2) {
	//Check whether the balls are close enough
	float r = b1->getRadius() + b2->getRadius();
	if ((b1->getPositionV() - b2->getPositionV()).magnitudeSquared() < r * r) {
		//Check whether the balls are moving toward each other
		vector3 netVelocity = b1->getVelocity() - b2->getVelocity();
		vector3 displacement = b1->getPositionV() - b2->getPositionV();
		return dot(netVelocity, displacement) < 0;
	}
	else
		return false;
}

//Handles all ball-ball collisions
void handleBallBallCollisions(vector<Ball*> &balls, GLOctree* octree) {
	vector<BallPair> bps;
	potentialBallBallCollisions(bps, balls, octree);

	for (unsigned int i = 0; i < bps.size(); i++) {
		BallPair bp = bps[i];

		Ball* b1 = bp.ball1;
		Ball* b2 = bp.ball2;
		if (testBallBallCollision(b1, b2)) {
			//Make the balls reflect off of each other
			vector3 displacement = (b1->getPositionV() - b2->getPositionV()).normalise() ;
			b1->setVelocity(b1->getVelocity() - ((displacement * 2) * dot(b1->getVelocity(), displacement)) );
			b2->setVelocity(b2->getVelocity() - ((displacement * 2) * dot(b2->getVelocity(), displacement)) );
		}
	}
}

//Returns the direction from the origin to the wall
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

//Returns whether a ball and a wall are colliding
bool testBallWallCollision(Ball* ball, Wall wall) {
	vector3 dir = wallDirection(wall);
	//Check whether the ball is far enough in the "dir" direction, and whether
	//it is moving toward the wall
/*	cout << endl;
	cout << dot(ball->getPositionV(), dir) << endl;
	cout << ball->getRadius() << endl;
	cout << (BOX_SIZE / 2) << endl;
	cout << dot(ball->getVelocity(), dir) << endl;
	cout << dir.v[0] << ":" << dir.v[1] << ":" << dir.v[2] << endl;
	cout << ball->getPositionV().v[0] << ":" << ball->getPositionV().v[1] << ":" << ball->getPositionV().v[2] << endl;
	cout << ball->getVelocity().v[0] << ":" << ball->getVelocity().v[1] << ":" << ball->getVelocity().v[2] << endl;*/
	return dot(ball->getPositionV(), dir) + ball->getRadius() > BOX_SIZE / 2 &&
		dot(ball->getVelocity(), dir) > 0;

}

//Handles all ball-wall collisions
void handleBallWallCollisions(vector<Ball*> &balls, GLOctree* octree) {
	vector<BallWallPair> bwps;
	potentialBallWallCollisions(bwps, balls, octree);
	for (unsigned int i = 0; i < bwps.size(); i++) {
		BallWallPair bwp = bwps[i];

		Ball* b = bwp.ball;
		Wall w = bwp.wall;
		if (testBallWallCollision(b, w)) {
			//Make the ball reflect off of the wall
			vector3 dir = (wallDirection(w)).normalise();
			b->setVelocity(b->getVelocity() - ((dir * 2) * dot(b->getVelocity(), dir))); 
		}
	}
}
