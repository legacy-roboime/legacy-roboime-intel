#ifndef ITEMROBOT_H
#define ITEMROBOT_H

#include <QGraphicsItem>
#include <Robot.h>

using LibIntelligence::Robot;

typedef enum {
	TeamBlue,
	TeamYellow,
} TeamType;

class ItemRobot : public QGraphicsItem
{

public:
	ItemRobot(QGraphicsItem *parent = NULL);
	~ItemRobot();
	
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);

	// Parenting
	void setRobot( Robot * r );

private:
	// Parent
	Robot * robot;
	qreal radius;

	// Angulo de corte
	qreal cutAngle;

	// Desenho cortado
	QPainterPath robotOutline;
	
};

#endif // ITEMROBOT_H
