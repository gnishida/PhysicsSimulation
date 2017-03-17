#include "RevolvingBarShape.h"
#include "Utils.h"

namespace bsim {

	RevolvingBarShape::RevolvingBarShape(btRigidBody* body, bool dynamic, btVector3 color, float angular_speed) {
		shape_type = Shape::SHAPE_REVOLVING_BAR;
		this->body = body;
		this->dynamic = dynamic;
		this->color = color;
		this->angular_speed = angular_speed;

		btTransform trans;
		body->getMotionState()->getWorldTransform(trans);
		this->origin = trans.getOrigin();
	}

	void RevolvingBarShape::draw(QPainter& painter) {
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

		painter.setPen(QPen(QColor(0, 0, 0), 3));
		float axis_radius = std::min(size.x(), size.y()) * 0.7;
		painter.drawEllipse(QPointF(0, 0), axis_radius * 100, axis_radius * 100);

		painter.restore();
	}

	void RevolvingBarShape::customStepForward() {
		btTransform trans = body->getWorldTransform();
		btQuaternion qt = trans.getRotation();
		float angle = atan2f(2 * qt.z() * qt.w(), (1 - 2 * qt.z() * qt.z()));
		qt.setEuler(0, 0, angle + angular_speed);
		trans.setRotation(qt);
		trans.setOrigin(origin);
		body->setWorldTransform(trans);
		body->getMotionState()->setWorldTransform(trans);
	}

	QDomElement RevolvingBarShape::toXml(QDomDocument& doc) {
		QDomElement node = doc.createElement("shape");
		node.setAttribute("type", "revolving_bar");
		node.setAttribute("dynamic", dynamic ? "true" : "false");
		node.setAttribute("color_x", color.x());
		node.setAttribute("color_y", color.y());
		node.setAttribute("color_z", color.z());

		btTransform trans;
		body->getMotionState()->getWorldTransform(trans);

		node.setAttribute("origin_x", origin.getX());// trans.getOrigin().getX());
		node.setAttribute("origin_y", origin.getY());// trans.getOrigin().getY());

		btBoxShape* shape = static_cast<btBoxShape*>(body->getCollisionShape());
		btVector3 size = shape->getHalfExtentsWithMargin();
		node.setAttribute("size_x", size.x());
		node.setAttribute("size_y", size.y());
		node.setAttribute("size_z", size.z());

		node.setAttribute("angular_speed", angular_speed);

		return node;
	}

}