#pragma once

#include "Shape.h"

namespace bsim {

	class BoxShape : public Shape {
	public:
		BoxShape(btRigidBody* body, bool dynamic, btVector3 color);
		void draw(QPainter& painter);
		void customStepForward();
		QDomElement toXml(QDomDocument& doc);
	};

}