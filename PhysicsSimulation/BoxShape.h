#pragma once

#include "Shape.h"

namespace bsim {

	class BoxShape : public Shape {
	public:
		BoxShape(btRigidBody* body, bool dynamic, btVector3 color);
		void draw(QPainter& painter);
		QDomElement toXml(QDomDocument& doc);
	};

}