#include "BoxShape.h"
#include "Utils.h"

namespace bsim {

	BoxShape::BoxShape(btRigidBody* body, bool dynamic, btVector3 color) {
		shape_type = Shape::SHAPE_BOX;
		this->body = body;
		this->dynamic = dynamic;
		this->color = color;
	}

	void BoxShape::draw(QPainter& painter) {
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

		btBoxShape* shape = static_cast<btBoxShape*>(body->getCollisionShape());
		btVector3 size = shape->getHalfExtentsWithMargin();
		painter.drawRect(-size.x() * 100, -size.y() * 100, size.x() * 200, size.y() * 200);

		painter.restore();
	}

	QDomElement BoxShape::toXml(QDomDocument& doc) {
		QDomElement node = doc.createElement("shape");
		node.setAttribute("type", "box");
		node.setAttribute("dynamic", dynamic ? "true" : "false");
		node.setAttribute("color_x", color.x());
		node.setAttribute("color_y", color.y());
		node.setAttribute("color_z", color.z());

		btTransform trans;
		body->getMotionState()->getWorldTransform(trans);

		node.setAttribute("origin_x", trans.getOrigin().getX());
		node.setAttribute("origin_y", trans.getOrigin().getY());
		node.setAttribute("quaternion_x", trans.getRotation().getX());
		node.setAttribute("quaternion_y", trans.getRotation().getY());
		node.setAttribute("quaternion_z", trans.getRotation().getZ());
		node.setAttribute("quaternion_w", trans.getRotation().getW());

		btBoxShape* shape = static_cast<btBoxShape*>(body->getCollisionShape());
		btVector3 size = shape->getHalfExtentsWithMargin();
		node.setAttribute("size_x", size.x());
		node.setAttribute("size_y", size.y());
		node.setAttribute("size_z", size.z());

		return node;
	}


}