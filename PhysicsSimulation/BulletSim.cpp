#include "BulletSim.h"
#include "Utils.h"
#include <iostream>
#include <QFile>
#include <QDate>
#include <QTextStream>
#include "BoxShape.h"
#include "SphereShape.h"
#include "RevolvingBarShape.h"

namespace bsim {

	BulletSim::BulletSim() {
		///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
		collisionConfiguration = new btDefaultCollisionConfiguration();

		///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
		dispatcher = new btCollisionDispatcher(collisionConfiguration);

		///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
		overlappingPairCache = new btDbvtBroadphase();

		///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
		solver = new btSequentialImpulseConstraintSolver;

		dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
		dynamicsWorld->setGravity(btVector3(0, -9.8, 0));

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
		for (int i = 0; i < shapes.size(); i++) {
			shapes[i]->draw(painter);
		}
	}

	void BulletSim::init() {
		clear();

		addBoxObject(btVector3(4, -4, 0), btVector3(5, 5, 5), false, btVector3(1, 1, 1));
		addRevolvingBarObject(btVector3(4, 4, 0), btVector3(1, 0.1, 5), false, btVector3(0.5, 0.5, 0.5));
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

	void BulletSim::load(const QString& filename) {
		QFile file(filename);
		if (!file.open(QFile::ReadOnly | QFile::Text)) throw "Fild cannot open.";

		QDomDocument doc;
		doc.setContent(&file);

		QDomElement root = doc.documentElement();
		if (root.tagName() != "world")	throw "Invalid file format.";

		// clear the data
		clear();

		QDomNode node = root.firstChild();
		while (!node.isNull()) {
			if (node.toElement().tagName() == "shape") {
				QString type = node.toElement().attribute("type");
				float origin_x = node.toElement().attribute("origin_x").toFloat();
				float origin_y = node.toElement().attribute("origin_y").toFloat();
				bool dynamic = node.toElement().attribute("dynamic").toLower() == "true";
				float color_x = node.toElement().attribute("color_x").toFloat();
				float color_y = node.toElement().attribute("color_y").toFloat();
				float color_z = node.toElement().attribute("color_z").toFloat();
				if (type == "box") {
					float size_x = node.toElement().attribute("size_x").toFloat();
					float size_y = node.toElement().attribute("size_y").toFloat();
					float size_z = node.toElement().attribute("size_z").toFloat();
					addBoxObject(btVector3(origin_x, origin_y, 0), btVector3(size_x, size_y, size_z), dynamic, btVector3(color_x, color_y, color_z));
				}
				else if (type == "sphere") {
					float radius = node.toElement().attribute("radius").toFloat();
					addSphereObject(btVector3(origin_x, origin_y, 0), radius, dynamic, btVector3(color_x, color_y, color_z));
				}
				else if (type == "revolving_bar") {
					float size_x = node.toElement().attribute("size_x").toFloat();
					float size_y = node.toElement().attribute("size_y").toFloat();
					float size_z = node.toElement().attribute("size_z").toFloat();
					addRevolvingBarObject(btVector3(origin_x, origin_y, 0), btVector3(size_x, size_y, size_z), dynamic, btVector3(color_x, color_y, color_z));
				}
			}

			node = node.nextSibling();
		}
	}

	void BulletSim::save(const QString& filename) {
		QFile file(filename);
		if (!file.open(QFile::WriteOnly)) throw "File cannot open.";

		QDomDocument doc;

		// set root node
		QDomElement root = doc.createElement("world");
		root.setAttribute("author", "Gen Nishida");
		root.setAttribute("version", "1.0");
		root.setAttribute("date", QDate::currentDate().toString("MM/dd/yyyy"));
		doc.appendChild(root);

		// write shapes
		for (int i = 0; i < shapes.size(); ++i) {
			QDomElement shape_node = shapes[i]->toXml(doc);
			root.appendChild(shape_node);
		}

		QTextStream out(&file);
		doc.save(out, 4);

	}

	btRigidBody* BulletSim::addBoxObject(btVector3 origin, btVector3 size, bool dynamic, btVector3 color) {
		btRigidBody* body = addObject(origin, new btBoxShape(size), dynamic);
		shapes.push_back(new BoxShape(body, dynamic, color));
		return body;
	}

	btRigidBody* BulletSim::addSphereObject(btVector3 origin, btScalar radius, bool dynamic, btVector3 color) {
		btRigidBody* body = addObject(origin, new btSphereShape(radius), dynamic);
		shapes.push_back(new SphereShape(body, dynamic, color));
		return body;
	}

	btRigidBody* BulletSim::addRevolvingBarObject(btVector3 origin, btVector3 size, bool dynamic, btVector3 color) {
		btRigidBody* body = addObject(origin, new btBoxShape(size), dynamic);
		shapes.push_back(new RevolvingBarShape(body, dynamic, color));
		return body;
	}

	void BulletSim::stepSimulation(float timeStep) {
		for (int i = 0; i < shapes.size(); ++i) {
			if (shapes[i]->shape_type == Shape::SHAPE_REVOLVING_BAR) {
				btTransform trans = shapes[i]->body->getWorldTransform();
				btQuaternion qt = trans.getRotation();
				float angle = atan2f(2 * qt.z() * qt.w(), (1 - 2 * qt.z() * qt.z()));
				qt.setEuler(0, 0, angle + 0.01);
				trans.setRotation(qt);
				trans.setOrigin(btVector3(4, 4, 0));
				shapes[i]->body->setWorldTransform(trans);
				shapes[i]->body->getMotionState()->setWorldTransform(trans);
			}
		}

		dynamicsWorld->stepSimulation(timeStep, 10);
	}

	btRigidBody* BulletSim::addObject(btVector3 origin, btCollisionShape* shape, bool dynamic) {
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

		// using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
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