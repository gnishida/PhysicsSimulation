#include "BulletSim.h"
#include "Utils.h"
#include <iostream>

namespace bsim {

	BulletSim::BulletSim() {
		///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
		collisionConfiguration = new btDefaultCollisionConfiguration();

		///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
		dispatcher = new	btCollisionDispatcher(collisionConfiguration);

		///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
		overlappingPairCache = new btDbvtBroadphase();

		///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
		solver = new btSequentialImpulseConstraintSolver;

		dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
		dynamicsWorld->setGravity(btVector3(0, -9.8, 0));

		barBody = NULL;

		init();
	}

	BulletSim::~BulletSim() {
		clear();

		delete dynamicsWorld;
		delete solver;
		delete overlappingPairCache;
		delete dispatcher;
		delete collisionConfiguration;
	}

	void BulletSim::draw(QPainter& painter) {
		//for (int i = 0; i < dynamicsWorld->getNumCollisionObjects(); i++) {
		for (int i = 0; i < shapes.size(); i++) {
			//btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
			//btRigidBody* body = btRigidBody::upcast(obj);
			btRigidBody* body = shapes[i].body;
			btTransform trans;
			if (body && body->getMotionState()) {
				body->getMotionState()->getWorldTransform(trans);

				float x = trans.getOrigin().getX() * 100;
				float y = 800 - trans.getOrigin().getY() * 100;
				btQuaternion qt = trans.getRotation();
				float angle = -atan2f(2 * qt.z() * qt.w(), (1 - 2 * qt.z() * qt.z())) / 3.141592 * 180;

				painter.save();
				QColor color((i * 30 + 255) % 256, (i * 79 + 255) % 256, (i * 83 + 255) % 256);
				painter.setBrush(QBrush(color));
				painter.setPen(QColor(0, 0, 0));
				painter.translate(x, y);
				painter.rotate(angle);

				//if (obj->getCollisionShape()->getShapeType() == BOX_SHAPE_PROXYTYPE) {
				if (shapes[i].shape_type == Shape::SHAPE_BOX) {
					//btBoxShape* shape = static_cast<btBoxShape*>(obj->getCollisionShape());
					btBoxShape* shape = static_cast<btBoxShape*>(shapes[i].body->getCollisionShape());
					btVector3 size = shape->getHalfExtentsWithMargin();
					painter.drawRect(-size.x() * 100, -size.y() * 100, size.x() * 200, size.y() * 200);
				}

				painter.restore();
			}
		}
	}

	void BulletSim::init() {
		clear();

		addBoxObject(btVector3(4, -4, 0), btVector3(5, 5, 5), false);
		barBody = addBoxObject(btVector3(4, 4, 0), btVector3(1, 0.1, 5), false);

		for (int i = 0; i < 100; i++) {
			addBoxObject(btVector3(utils::genRand(1, 7), utils::genRand(2, 7), 0), btVector3(0.2, 0.2, 0.2), true);
		}
	}

	void BulletSim::clear() {
		for (int i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--) {
			btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
			btRigidBody* body = btRigidBody::upcast(obj);
			btCollisionShape* shape = NULL;
			if (body && body->getMotionState()) {
				delete body->getCollisionShape();
				delete body->getMotionState();
			}
			dynamicsWorld->removeCollisionObject(obj);
			delete obj;
		}

		shapes.clear();
	}

	btRigidBody* BulletSim::addBoxObject(btVector3 origin, btVector3 size, bool dynamic) {
		btRigidBody* body = addObject(origin, new btBoxShape(size), dynamic);
		shapes.push_back(Shape(Shape::SHAPE_BOX, body));
		return body;
	}

	btRigidBody* BulletSim::addSphereObject(btVector3 origin, btScalar radius, bool dynamic) {
		btRigidBody* body = addObject(origin, new btSphereShape(radius), dynamic);
		shapes.push_back(Shape(Shape::SHAPE_SPHERE, body));
		return body;
	}

	void BulletSim::stepSimulation(float timeStep) {
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[1];
		btRigidBody* body = btRigidBody::upcast(obj);
		btTransform trans = body->getWorldTransform();
		btQuaternion qt = trans.getRotation();
		float angle = atan2f(2 * qt.z() * qt.w(), (1 - 2 * qt.z() * qt.z()));
		//std::cout << "angle: " << angle << std::endl;
		//std::cout << qt.x() << "," << qt.y() << "," << qt.z() << "," << qt.w() << std::endl;
		qt.setEuler(0, 0, angle + 0.01);
		trans.setRotation(qt);
		trans.setOrigin(btVector3(4, 4, 0));
		body->setWorldTransform(trans);
		barBody->getMotionState()->setWorldTransform(trans);

		dynamicsWorld->stepSimulation(timeStep, 10);
	}

	btRigidBody* BulletSim::addObject(btVector3 origin, btCollisionShape* shape, bool dynamic) {
		//collisionShapes.push_back(shape);

		// set transform
		btTransform transform;
		transform.setIdentity();
		transform.setOrigin(origin);

		btScalar mass(0.f);
		btVector3 localInertia(0, 0, 0);
		if (dynamic) {
			mass = btScalar(1.f);
			shape->calculateLocalInertia(mass, localInertia);
		}

		//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(transform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, shape, localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);

		// move/rotate only in X-Y plane
		body->setLinearFactor(btVector3(1, 1, 0));
		body->setAngularFactor(btVector3(0, 0, 1));

		dynamicsWorld->addRigidBody(body);

		return body;
	}

}