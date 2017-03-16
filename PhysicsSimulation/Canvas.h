#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <QKeyEvent>
#include <QTimer>
#include "BulletSim.h"

class Canvas : public QWidget {
Q_OBJECT

private:
	bool ctrlPressed;
	bool shiftPressed;

	QTimer* animation_timer;
	bsim::BulletSim sim;

public:
	Canvas(QWidget *parent = NULL);
    ~Canvas();

	void init();
	void run();
	void stop();

public slots:
	void animation_update();

protected:
	void paintEvent(QPaintEvent* e);
	void mousePressEvent(QMouseEvent* e);
	void mouseMoveEvent(QMouseEvent* e);
	void mouseReleaseEvent(QMouseEvent* e);
	void resizeEvent(QResizeEvent *e);

public:
	void keyPressEvent(QKeyEvent* e);
	void keyReleaseEvent(QKeyEvent* e);
};

#endif // CANVAS_H
