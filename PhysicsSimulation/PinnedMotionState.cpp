#include "PinnedMotionState.h"
#include <iostream>

namespace bsim {

	PinnedMotionState::PinnedMotionState(const btTransform& transform) : btDefaultMotionState(transform) {
		this->transform = transform;
	}


	PinnedMotionState::~PinnedMotionState() {
	}

	void PinnedMotionState::getWorldTransform(btTransform &worldTrans) const {
		worldTrans = transform;
		std::cout << transform.getOrigin().y() << std::endl;
		/*
		super->getWorldTransform(orientatino);

		orientation = this->WorldOrientation;
		worldTrans.setRotation(orientation);

		//Ogre::Vector3 position = m_pThing->WorldPosition;
		worldTrans.setOrigin(pined_origin);
		*/
	}

	void PinnedMotionState::setWorldTransform(const btTransform &worldTrans) {
		std::cout << transform.getOrigin().y() << "--> " << worldTrans.getOrigin().y() << std::endl;

		btTransform transform = m_graphicsWorldTrans * m_centerOfMassOffset.inverse();
		transform.setRotation(worldTrans.getRotation());
		m_graphicsWorldTrans = transform * m_centerOfMassOffset;

		//transform.setRotation(worldTrans.getRotation());
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