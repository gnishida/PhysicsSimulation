#pragma once

#include "btBulletDynamicsCommon.h"

namespace bsim {

	class PinnedBody : public btRigidBody {
	private:
		//btTransform transform;

	public:
		PinnedBody(btScalar mass, btMotionState *motionState, btCollisionShape *collisionShape, const btVector3 &localInertia);
		~PinnedBody();

		void getWorldTransform(btTransform &worldTrans) const;
		void setWorldTransform(const btTransform &worldTrans);
	};

}