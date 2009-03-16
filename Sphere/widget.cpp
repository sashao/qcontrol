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
	angle.first = 1.0;
	angle.second = 1.0;
	myID = -1;
	accelerate_time = 2000;
	refresh_rate = 60;

	const int num = 41;
	for (int i = 0; i<num; ++i)
	{
		items.push_back(QPointF(pi2 * qrand()/RAND_MAX, M_PI * qrand()/RAND_MAX));
//		items.push_back(QPointF(pi2 * qrand()/RAND_MAX, pi2 * 0.63 ));
//		qDebug()<< pi2 * qrand()/RAND_MAX<< " / "<< pi2*0.1;
	}
	setMouseTracking(true);
	myID = startTimer(refresh_rate);
}

Widget::~Widget()
{

}

void Widget::paintEvent ( QPaintEvent * event )
{
	QWidget::paintEvent(event);
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);
//	painter.fillRect(event->rect(), Qt::green);

	painter.drawEllipse(center, radius, radius);

	QPointF iter;
	foreach(iter, items)
	{
		drawItem(&painter, pointFromAngle(QPair<double, double> (iter.x()   ,
																 iter.y()    )));
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
		static double scaler = 0;
		if (underMouse())
			scaler = 1;
		else
			scaler *= accelerate_time/(accelerate_time-refresh_rate);
		const double dx = ((mouseAt.x()-center.x()))*0.001*scaler;
		const double dy = ((mouseAt.y()-center.y()))*0.001*scaler;
//		qDebug()<< dy<< "--  "<< angle.first;
		angle.first  = angle.first - dy;
		angle.second = angle.second + dx;

//		setRotationMatrix(angle.first*0, angle.second, 0);
		setRotationMatrix(angle.first, angle.second*0, -angle.second);
		event->accept();
		repaint();
//		qDebug()<< angle.first;
	}
}

void Widget::drawItem(QPainter * painter, QPair<QPointF, double> param)
{
	const double rad = 12* param.second;
	painter->drawEllipse(param.first, rad, rad);
}

QPair<QPointF, double> Widget::pointFromAngle(QPair<double, double> angle) const
{
	// Many thanks to xplanet
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
	return QPair<QPointF, double>(pos+center, (rad+2)/2);
}


void
Widget::setRotationMatrix(const double angle_x,
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

//	rotXYZ_[0][0] =  cosy * cosz;
//	rotXYZ_[0][1] =  sinx * siny * cosz + cosx * sinz;
//	rotXYZ_[0][2] = -cosx * siny * cosz + sinx * sinz;
//	rotXYZ_[1][0] = -cosy * sinz;
//	rotXYZ_[1][1] = -sinx * siny * sinz + cosx * cosz;
//	rotXYZ_[1][2] =  cosx * siny * sinz + sinx * cosz;
//	rotXYZ_[2][0] =  siny;
//	rotXYZ_[2][1] = -sinx * cosy;
//	rotXYZ_[2][2] =  cosx * cosy;

#define rotZYX_ rotXYZ_
	rotZYX_[0][0] =  cosy * cosz;
	rotZYX_[0][1] =  cosy * sinz;
	rotZYX_[0][2] = -siny;
	rotZYX_[1][0] = -cosx * sinz + sinx * siny * cosz;
	rotZYX_[1][1] =  sinx * siny * sinz + cosx * cosz;
	rotZYX_[1][2] =  sinx * cosy;
	rotZYX_[2][0] =  cosx * siny * cosz + sinx * sinz;
	rotZYX_[2][1] = -sinx * cosz + cosx * siny * sinz;
	rotZYX_[2][2] =  cosx * cosy;


}

void Widget::mouseMoveEvent ( QMouseEvent * event )
{
	mouseAt = event->pos();
//	qDebug()<<"moved "<< mouseAt;
}







