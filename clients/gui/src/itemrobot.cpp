#include "itemrobot.h"
#include <QtCore>
#include <QtGui>

#define DEFAULT_RADIUS  90

ItemRobot::ItemRobot(QGraphicsItem *parent)
	: QGraphicsItem(parent)
{
	setRadius(DEFAULT_RADIUS);
	setFlag(ItemIsMovable);
}

ItemRobot::~ItemRobot()
{

}

void ItemRobot::setTeam(TeamType newTeam)
{
	team = newTeam;
    if (team==TeamBlue) {
		setColor(Qt::cyan);
	}
	else {
		setColor(Qt::yellow);
	}
}

void ItemRobot::setRadius(qreal radius)
{
    //! Configura novo raio
    prepareGeometryChange();
    p_radius = radius;
}

qreal ItemRobot::radius() const
{
    //! Retorna o raio
    return p_radius;
}

void ItemRobot::setColor(QColor color)
{
    //! Configura nova cor
    p_color = color;
}

void ItemRobot::setColor(enum Qt::GlobalColor color)
{
    //! Configura nova cor
    p_color = QColor(color);
}

QColor ItemRobot::color()
{
    //! Retorna a cor do ItemRoboto
    return p_color;
}

QRectF ItemRobot::boundingRect() const
{
    return QRectF(-radius(), -radius(), 2*radius(), 2*radius());
}

void ItemRobot::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                    QWidget *widget)
{
    painter->setBrush(isSelected() ? Qt::green : color());
    painter->drawEllipse(-radius(), -radius(), 2*radius(), 2*radius());
    painter->drawLine(0,0,radius()*cos(p_direction*M_PI/-180),radius()*sin(p_direction*M_PI/-180));
	painter->drawText(0,0,"Teste");
}

int ItemRobot::type() const
{
    //! Permite o uso de qgraphicsitem_cast com este item.
    return Type;
}

qreal ItemRobot::direction()
{
    return p_direction;
}

void ItemRobot::setDirection(qreal dir)
{
    if (dir>180) dir-=360;
    else if (dir<-180) dir+=360;
    p_direction = dir;
}
