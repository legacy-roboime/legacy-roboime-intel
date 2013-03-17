#include "itemrobot.h"
#include <QtCore>
#include <QtGui>

#include <Stage.h>
#include <Body.h>

using LibIntelligence::Stage;
using LibIntelligence::Body;

ItemRobot::ItemRobot(QGraphicsItem *parent)
	: QGraphicsItem(parent)
{
	setFlag(ItemIsMovable);
}

ItemRobot::~ItemRobot()
{

}

void ItemRobot::setRobot( Robot * r )
{
	robot = r;

	qreal cut = robot->body().cut(),
		radius = robot->body().radius();

	cutAngle = acos( cut/radius )*180.0/M_PI;

	// Cria a forma do robo
	robotOutline.moveTo ( radius,0 );
    robotOutline.arcTo ( -radius,-radius,2*radius,2*radius,0,360-2*cutAngle );
	robotOutline.closeSubpath();
}

QRectF ItemRobot::boundingRect() const
{
    return QRectF(-robot->body().radius(), -robot->body().radius(), 2*robot->body().radius(), 2*robot->body().radius());
}

void ItemRobot::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                    QWidget *widget)
{
	// Verifica a cor do time, amarelo ou azul
	QColor p_color = ( robot->color() == YELLOW ) ? QColor(255,255,0) : QColor(0,0,255);

    // Save transformation:
    QTransform oldTransformation = painter->worldTransform();
    
	// Change position
	painter->translate(-robot->stage()->fieldLength()/2, -robot->stage()->fieldWidth()/2 );
    painter->translate(robot->x(),-robot->y());
    painter->setBrush(p_color);
    painter->setPen(p_color);
    double robotRotation = robot->orientation()*180.0/M_PI;
    
	// Draw robot shape
	painter->rotate(-cutAngle-robotRotation);
	painter->drawPath(robotOutline);
    painter->rotate( cutAngle+robotRotation);

	// Draw id
    QString robotId("?");
    robotId.setNum(robot->id());
    painter->setBrush(QColor(0,0,0));
    painter->setPen(QColor(0,0,0)); 
    painter->setFont(QFont ( "Courier",80,2,false ));
    painter->drawText(-90,-210,1000,1000,0,robotId);

	// Reset transformation
	painter->setTransform(oldTransformation);

	/*
    // Draw team marker
    painter->setBrush(p_color);    
    painter->setPen(p_color); 
    painter->drawEllipse(QPoint(0,0),25,25);
     // Draw confidence
     painter->setBrush(color);    
     painter->setPen(color); 
     painter->drawRect ( -90,-190, ( int ) ( ( ( double ) 180 ) * robot.confidence() ),80 );
     
     
	 */
}
