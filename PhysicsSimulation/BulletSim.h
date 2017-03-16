#pragma once

#include "btBulletDynamicsCommon.h"
#include <QPainter>

namespace bsim {

	class Shape {
	public:
		static enum { SHAPE_BOX = 0, SHAPE_SPHERE };

	public:
		int shape_type;
		btRigidBody* body;
		btVector3 color;

	public:
		Shape(int shape_type, btRigidBody* body) : shape_type(shape_type), body(body) {}
	};

	class BulletSim {
	private:
		btDefaultCollisionConfiguration* collisionConfiguration;
		btCollisionDispatcher* dispatcher;
		btBroadphaseInterface* overlappingPairCache;
		btSequentialImpulseConstraintSolver* solver;
		btDiscreteDynamicsWorld* dynamicsWorld;
		btAlignedObjectArray<btCollisionShape*> collisionShapes;
		//btDefaultMotionState* barMotionState;
		btRigidBody* barBody;
		std::vector<Shape> shapes;

	public:
		BulletSim();
		~BulletSim();

		void draw(QPainter& painter);
		void init();
		void clear();
		btRigidBody* addBoxObject(btVector3 origin, btVector3 size, bool dynamic);
		btRigidBody* addSphereObject(btVector3 origin, btScalar radius, bool dynamic);
		void stepSimulation(float timeStep);

	private:
		btRigidBody* addObject(btVector3 origin, btCollisionShape* shape, bool dynamic);
	};

}