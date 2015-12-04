#ifndef GLPARALLELOCTREE_H
#define GLPARALLELOCTREE_H

#include <iostream>
#include <MathUtils.h>
#include <set>
#include <GLMesh.h>
#include <GLOctree.h>
#include <stack>


#define MASK 0x7
#define X 0x1
#define Y 0x2
#define Z 0x4

#define DEPTH 3
#define NUM_BALLS 500
#define START_POS 9
#define TOTAL_NODES 73

typedef GLMesh Ball;

using namespace std;

/**
This parallel octree is diveded in four parts:

    1: Create boxes
    2: Insert Balls
    3: Calculate Colissions
    4: Clean tree

**/


struct Box{
    vector3 corner1;
    vector3 corner2;
    vector3 centro;
	set<Ball*> *balls;

	Box(vector3 c1, vector3 c2, bool isLeaf)
	{
		this->corner1 = c1;
		this->corner2 = c2;
		this->centro = (c1 + c2) /2;
		if(isLeaf)
			balls = new set<Ball*>();
		else
			balls = NULL;
	}

	Box(){balls = NULL;}
};


class GLParallelOctree
{
    public:
        GLParallelOctree(vector3 corner1, vector3 corner2, int depth);
        virtual ~GLParallelOctree();

        /** CreateBoxes */
        void createBoxes();

        /** InsertBalls*/
		void insertBalls(vector<Ball*> &balls);

        /** Colissions */
        void Colissions();
		

        /** Clean tree */
        void cleanTree();

		/** Print Octree */
		void printPOctree();
    protected:
    private:
        Box *tree;
        int depth;
        vector3 corner1;
        vector3 corner2;

		void WallCollisions();
		void potentialBallWallCollisions(Wall w, char coord, int dir, int node);
		 
};

#endif // GLPARALLELOCTREE_H
