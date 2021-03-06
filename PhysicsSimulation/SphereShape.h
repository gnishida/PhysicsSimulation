#pragma once

#include "Shape.h"

namespace bsim {
	
	class SphereShape : public Shape {
	public:
		SphereShape(btRigidBody* body, bool dynamic, btVector3 color);
		void draw(QPainter& painter);
		QDomElement toXml(QDomDocument& doc);
	};

}
