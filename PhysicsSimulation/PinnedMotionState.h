#pragma once

#include "btBulletDynamicsCommon.h"

namespace bsim {

	class PinnedMotionState : public btDefaultMotionState {
	private:
		btTransform transform;

	public:
		PinnedMotionState(const btTransform& transform);
		~PinnedMotionState();

		void getWorldTransform(btTransform &worldTrans) const;
		void setWorldTransform(const btTransform &worldTrans);
	};

}