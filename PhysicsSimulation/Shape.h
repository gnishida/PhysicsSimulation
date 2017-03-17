#pragma once

#include "btBulletDynamicsCommon.h"
#include <QDomDocument>
#include <QPainter>

namespace bsim {

	class Shape {
	public:
		static enum { SHAPE_BOX = 0, SHAPE_SPHERE, SHAPE_REVOLVING_BAR };

	public:
		int shape_type;
		bool dynamic;
		btRigidBody* body;
		btVector3 color;

	public:
		virtual void draw(QPainter& painter) = 0;
		virtual QDomElement toXml(QDomDocument& doc) = 0;
	};

}
