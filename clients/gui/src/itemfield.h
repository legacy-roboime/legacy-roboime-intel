#ifndef ITEMFIELD_H
#define ITEMFIELD_H

#include <QGraphicsItem>
#include "Stage.h"

using namespace LibIntelligence;

class ItemField : public QGraphicsItem
{

public:
    //enum { Type = UserType + 2 };
	ItemField(Stage* stage, QGraphicsItem *parent=NULL);
	~ItemField();
    int type() const;
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);

private:
	Stage* stage_;
};

#endif // ITEMFIELD_H
