#ifndef ITEMROBOT_H
#define ITEMROBOT_H

#include <QGraphicsItem>

typedef enum {
	TeamBlue,
	TeamYellow,
} TeamType;

class ItemRobot : public QGraphicsItem
{

public:
    enum { Type = UserType + 1 };
	ItemRobot(QGraphicsItem *parent = NULL);
	~ItemRobot();
    int type() const;
	void setTeam(TeamType newTeam);
    void setRadius(qreal radius);
    qreal radius() const;
    void setColor(QColor color);
	void setColor(enum Qt::GlobalColor color);
    QColor color();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
    qreal direction();
    void setDirection(qreal dir);

private:
	TeamType team;
    qreal p_direction;
    qreal p_radius;
    QColor p_color;
	
};

#endif // ITEMROBOT_H
