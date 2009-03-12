#ifndef WIDGET_H
#define WIDGET_H

#include <QtGui/QWidget>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();
private:
	virtual void drawItem(QPainter * painter, QPair<QPoint, double> param);
	QPair<QPoint, double> pointFromAngle(QPair<double, double> angle) const;
	void SetXYZRotationMatrix(const double angle_x,
							  const double angle_y,
							  const double angle_z);

	int radius;
	int myID;
	double rotXYZ_[3][3];
	QPoint center;
	QPoint mouseAt;

	QPair<double, double> angle; //radians

	QList<QPointF> items;

protected:
	void paintEvent ( QPaintEvent * event );
	void resizeEvent (QResizeEvent * event );
	void timerEvent ( QTimerEvent * event );
	void mouseMoveEvent ( QMouseEvent * event );
};

#endif // WIDGET_H
