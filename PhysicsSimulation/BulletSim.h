#pragma once

#include "btBulletDynamicsCommon.h"
#include "Shape.h"

namespace bsim {
	
	class BulletSim {
	private:
		btDefaultCollisionConfiguration* collisionConfiguration;
		btCollisionDispatcher* dispatcher;
		btBroadphaseInterface* overlappingPairCache;
		btSequentialImpulseConstraintSolver* solver;
		btDiscreteDynamicsWorld* dynamicsWorld;
		std::vector<Shape*> shapes;

	public:
		BulletSim();
		~BulletSim();

		void draw(QPainter& painter);
		void init();
		void clear();
		void load(const QString& filename);
		void save(const QString& filename);
		btRigidBody* addBoxObject(btVector3 origin, btVector3 size, bool dynamic, btVector3 color);
		btRigidBody* addSphereObject(btVector3 origin, btScalar radius, bool dynamic, btVector3 color);
		btRigidBody* addRevolvingBarObject(btVector3 origin, btVector3 size, bool dynamic, btVector3 color);
		void stepSimulation(float timeStep);

	private:
		btRigidBody* addObject(btVector3 origin, btCollisionShape* shape, bool dynamic);
	};

}