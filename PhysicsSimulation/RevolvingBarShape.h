#pragma once

#include "Shape.h"

namespace bsim {

	class RevolvingBarShape : public Shape {
	public:
		float angular_speed;
		btVector3 origin;

	public:
		RevolvingBarShape(btRigidBody* body, bool dynamic, btVector3 color, float angular_speed);
		void draw(QPainter& painter);
		void customStepForward();
		QDomElement toXml(QDomDocument& doc);
	};

}
