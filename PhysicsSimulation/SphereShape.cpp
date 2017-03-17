#include "SphereShape.h"
#include "Utils.h"

namespace bsim {

	SphereShape::SphereShape(btRigidBody* body, bool dynamic, btVector3 color) {
		shape_type = Shape::SHAPE_SPHERE;
		this->body = body;
		this->dynamic = dynamic;
		this->color = color;
	}

	void SphereShape::draw(QPainter& painter) {
		btTransform trans;
		body->getMotionState()->getWorldTransform(trans);

		float x = trans.getOrigin().getX() * 100;
		float y = 800 - trans.getOrigin().getY() * 100;
		btQuaternion qt = trans.getRotation();
		float angle = -atan2f(2 * qt.z() * qt.w(), (1 - 2 * qt.z() * qt.z())) / 3.141592 * 180;

		painter.save();
		QColor color(color.x() * 255, color.y() * 255, color.z() * 255);
		painter.setBrush(QBrush(color));
		painter.setPen(QColor(0, 0, 0));
		painter.translate(x, y);
		painter.rotate(angle);

		btSphereShape* shape = static_cast<btSphereShape*>(body->getCollisionShape());
		float radius = shape->getRadius();
		painter.drawEllipse(QPointF(0, 0), radius * 100, radius * 100);

		painter.restore();
	}

	QDomElement SphereShape::toXml(QDomDocument& doc) {
		QDomElement node = doc.createElement("shape");
		node.setAttribute("type", "sphere");
		node.setAttribute("dynamic", dynamic ? "true" : "false");
		node.setAttribute("color_x", color.x());
		node.setAttribute("color_y", color.y());
		node.setAttribute("color_z", color.z());

		btTransform trans;
		body->getMotionState()->getWorldTransform(trans);

		float x = trans.getOrigin().getX() * 100;
		float y = 800 - trans.getOrigin().getY() * 100;
		node.setAttribute("origin_x", trans.getOrigin().getX());
		node.setAttribute("origin_y", trans.getOrigin().getY());

		btSphereShape* shape = static_cast<btSphereShape*>(body->getCollisionShape());
		float radius = shape->getRadius();
		node.setAttribute("radius", radius);

		return node;
	}

}