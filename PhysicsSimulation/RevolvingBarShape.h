#pragma once

#include "Shape.h"

namespace bsim {

	class RevolvingBarShape : public Shape {
	public:
		RevolvingBarShape(btRigidBody* body, bool dynamic, btVector3 color);
		void draw(QPainter& painter);
		QDomElement toXml(QDomDocument& doc);
	};

}
