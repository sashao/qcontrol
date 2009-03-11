#include "widget.h"

#include <math.h>

#include <QPainter>
#include <QPaintEvent>
#include <QDebug>
#include <QList>

static const double pi2 = 2.0*M_PI;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
	angle.first = 0;
	angle.second = 0;
	myID = -1;

	const int num = 131;
	for (int i = 0; i<num; ++i)
	{
		items.push_back(QPointF(pi2 * qrand()/RAND_MAX, M_PI * qrand()/RAND_MAX));
//		items.push_back(QPointF(pi2 * qrand()/RAND_MAX, pi2 * 0.63 ));
//		qDebug()<< pi2 * qrand()/RAND_MAX<< " / "<< pi2*0.1;
	}

	myID = startTimer(40);
}

Widget::~Widget()
{

}

void Widget::paintEvent ( QPaintEvent * event )
{
	QWidget::paintEvent(event);
	QPainter painter(this);
//	painter.fillRect(event->rect(), Qt::green);

	painter.drawEllipse(center, radius, radius);

	QPointF iter;
	foreach(iter, items)
	{
		drawItem(&painter, pointFromAngle(QPair<double, double> (iter.x()+angle.first*0       ,
																 iter.y()+angle.second*0    )));
	}
}

void Widget::resizeEvent (QResizeEvent * event )
{
	radius = qMin(event->size().height(), event->size().width())/2;
//	const int delta = event->size().width() - event->size().height();
	center.setX(event->size().width()/2);
	center.setY(event->size().height()/2);


	return QWidget::resizeEvent(event);
}

void Widget::timerEvent ( QTimerEvent * event )
{
	if (event->timerId() == myID)
	{
		angle.first = angle.first + M_PI*0.03;
		angle.second = angle.second + M_PI*.009;

		SetXYZRotationMatrix(angle.first, angle.second, 0);
		event->accept();
		repaint();
//		qDebug()<< angle.first;
	}
}

void Widget::drawItem(QPainter * painter, QPair<QPoint, double> param)
{
	const int rad = qRound(12* param.second);
	painter->drawEllipse(param.first, rad, rad);
}

QPair<QPoint, double> Widget::pointFromAngle(QPair<double, double> angle) const
{
//	QPoint pos(sin(angle.second)*radius*sin(angle.first), cos(angle.first)*radius);
	const double x = sin(angle.first)*sin(angle.second);
	const double y = cos(angle.first);
	const double z = sin(angle.first)*cos(angle.second);

	const double x_ = x;
	const double y_ = y*cos(angle.first) + z*sin(angle.first);
	const double z_ = z*cos(angle.first) - y*sin(angle.first);

	const double X0 = cos(angle.first) * cos(angle.second);
	const double Y0 = cos(angle.first) * sin(angle.second);
	const double Z0 = sin(angle.first);


	const double X1 = (rotXYZ_[0][0] * X0
					   + rotXYZ_[0][1] * Y0
					   + rotXYZ_[0][2] * Z0);
	const double Y1 = (rotXYZ_[1][0] * X0
					   + rotXYZ_[1][1] * Y0
					   + rotXYZ_[1][2] * Z0);
	const double Z1 = (rotXYZ_[2][0] * X0
					   + rotXYZ_[2][1] * Y0
					   + rotXYZ_[2][2] * Z0);

//	QPoint pos(	 x_  *radius	, y_ *  radius  );
	QPoint pos(	 X1  *radius	, Y1 *  radius  );
	const double rad = Z1;//z_;
//	if (rad > 0) qDebug(">>>>");
//	else qDebug("<<<<");
	return QPair<QPoint, double>(pos+center, (rad+1)/2);
}


void
Widget::SetXYZRotationMatrix(const double angle_x,
									 const double angle_y,
									 const double angle_z)
{
	if (angle_x == 0 && angle_y == 0 && angle_z == 0)
	{
		for (int j = 0; j < 3; j++)
			for (int i = 0; i < 3; i++)
				rotXYZ_[j][i] = (i == j ? 1 : 0 );
		return;
	}

	const double cosx = cos(angle_x);
	const double cosy = cos(angle_y);
	const double cosz = cos(angle_z);
	const double sinx = sin(angle_x);
	const double siny = sin(angle_y);
	const double sinz = sin(angle_z);

	rotXYZ_[0][0] =  cosy * cosz;
	rotXYZ_[0][1] =  sinx * siny * cosz + cosx * sinz;
	rotXYZ_[0][2] = -cosx * siny * cosz + sinx * sinz;
	rotXYZ_[1][0] = -cosy * sinz;
	rotXYZ_[1][1] = -sinx * siny * sinz + cosx * cosz;
	rotXYZ_[1][2] =  cosx * siny * sinz + sinx * cosz;
	rotXYZ_[2][0] =  siny;
	rotXYZ_[2][1] = -sinx * cosy;
	rotXYZ_[2][2] =  cosx * cosy;

}









