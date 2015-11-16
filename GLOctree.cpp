#include "GLOctree.h"

//Puts potential ball-ball collisions in potentialCollisions.  It must return
//all actual collisions, but it need not return only actual collisions.
void potentialBallBallCollisions(vector<BallPair> &potentialCollisions,
								 vector<Ball*> &balls, Octree* octree) {
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
	*/
}

//Puts potential ball-wall collisions in potentialCollisions.  It must return
//all actual collisions, but it need not return only actual collisions.
void potentialBallWallCollisions(vector<BallWallPair> &potentialCollisions,
								 vector<Ball*> &balls, Octree* octree) {
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
	*/
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
void handleBallBallCollisions(vector<Ball*> &balls, Octree* octree) {
	vector<BallPair> bps;
	potentialBallBallCollisions(bps, balls, octree);
	for(unsigned int i = 0; i < bps.size(); i++) {
		BallPair bp = bps[i];
		
		Ball* b1 = bp.ball1;
		Ball* b2 = bp.ball2;
		if (testBallBallCollision(b1, b2)) {
			//Make the balls reflect off of each other
			vector3 displacement = (b1->getPositionV() - b2->getPositionV()).normalise();
			b1->setVelocity(b1->getVelocity() -  ((displacement * 2) * dot(b1->getVelocity(), displacement)));
			b2->setVelocity(b2->getVelocity() -  ((displacement * 2) * dot(b2->getVelocity(), displacement)));
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
	cout << endl;
	cout << dot(ball->getPositionV(), dir) << endl;
	cout <<  ball->getRadius() << endl;
	cout << (BOX_SIZE / 2) << endl;
	cout << dot(ball->getVelocity(), dir) << endl;
	cout << dir.v[0] << ":" << dir.v[1] << ":" << dir.v[2] <<endl;
	cout << ball->getPositionV().v[0] << ":" << ball->getPositionV().v[1] << ":" << ball->getPositionV().v[2] <<endl;
	cout << ball->getVelocity().v[0] << ":" << ball->getVelocity().v[1] << ":" << ball->getVelocity().v[2] << endl;
	return dot(ball->getPositionV(), dir) + ball->getRadius() > BOX_SIZE / 2 &&
			dot(ball->getVelocity(), dir) > 0;
}

//Handles all ball-wall collisions
void handleBallWallCollisions(vector<Ball*> &balls, Octree* octree) {
	vector<BallWallPair> bwps;
	potentialBallWallCollisions(bwps, balls, octree);
	for(unsigned int i = 0; i < bwps.size(); i++) {
		BallWallPair bwp = bwps[i];
		
		Ball* b = bwp.ball;
		Wall w = bwp.wall;
		if (testBallWallCollision(b, w)) {
			//Make the ball reflect off of the wall
			vector3 dir = (wallDirection(w)).normalise();
			b->setVelocity(b->getVelocity() -  ((dir * 2) * dot(b->getVelocity(),dir))); // Trucha con esto
		}
	}
}
