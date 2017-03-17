#include "PinnedBody.h"
#include <iostream>

namespace bsim {

	PinnedBody::PinnedBody(btScalar mass, btMotionState *motionState, btCollisionShape *collisionShape, const btVector3 &localInertia) : btRigidBody(mass, motionState, collisionShape, localInertia) {
	}


	PinnedBody::~PinnedBody() {
	}

	void PinnedBody::getWorldTransform(btTransform &worldTrans) const {
		worldTrans = m_worldTransform;
		/*
		super->getWorldTransform(orientatino);

		orientation = this->WorldOrientation;
		worldTrans.setRotation(orientation);

		//Ogre::Vector3 position = m_pThing->WorldPosition;
		worldTrans.setOrigin(pined_origin);
		*/
	}

	void PinnedBody::setWorldTransform(const btTransform &worldTrans) {
		std::cout << m_worldTransform.getOrigin().y() << "--> " << worldTrans.getOrigin().y() << std::endl;
		m_worldTransform.setRotation(worldTrans.getRotation());
		getMotionState()->setWorldTransform(m_worldTransform);
		//transform = worldTrans;
		//transform.setOrigin(transform.getOrigin());

		/*
		btQuaternion orientation = worldTrans.getRotation();
		m_pThing->WorldOrientation = bullet2ogre(orientation);
		*/
		//btVector3 position = worldTrans.getOrigin();
		//m_pThing->WorldPosition = bullet2ogre(position);
	}

}