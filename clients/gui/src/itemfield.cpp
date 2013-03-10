#include "itemfield.h"
#include "Goal.h"
#include <QPainter>

ItemField::ItemField(Stage* stage, QGraphicsItem *parent)
	: QGraphicsItem(parent),
	stage_(stage)
{

}

ItemField::~ItemField()
{

}


QRectF ItemField::boundingRect() const
{
	
	int width_, height_;

	width_ = stage_->fieldLength();
	height_ = stage_->fieldWidth();

    return QRectF(-width_/2, -height_/2, width_, height_);
}

void ItemField::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                    QWidget *widget)
{

	int temp, tempB;
	int width_, height_;

	width_ = stage_->fieldLength();
	height_ = stage_->fieldWidth();

	// TODO: Mudar exibição de linhas por exibição de retângulos!!!

    painter->setPen(Qt::white);
    //painter->drawEllipse(-radius(), -radius(), 2*radius(), 2*radius());

	// Laterais
    painter->drawRect(boundingRect());

	// Linhas centrais
	temp = stage_->centerCircleRadius();
	painter->drawLine(0, -height_/2, 0, height_/2);
	painter->drawEllipse(-temp, -temp, temp*2, temp*2);

	// Linhas de penalti
	temp = stage_->defenseRadius();
	tempB = stage_->defenseStretch();
	painter->drawLine(-width_/2+temp, -tempB/2, -width_/2+temp, tempB/2);
	painter->drawArc(-width_/2-temp, -temp-tempB/2, temp*2, temp*2, 0, 90*16);
	painter->drawArc(-width_/2-temp, tempB/2-temp, temp*2, temp*2, 0, -90*16);
	painter->drawLine(width_/2-temp, -tempB/2, width_/2-temp, tempB/2);
	painter->drawArc(width_/2-temp, -temp-tempB/2, temp*2, temp*2, -180*16, -90*16);
	painter->drawArc(width_/2-temp, tempB/2-temp, temp*2, temp*2, 180*16, 90*16);

	// Gols
	temp = stage_->blueGoal()->width();
	tempB = stage_->blueGoal()->depth();
	painter->drawRect(-width_/2-tempB, -temp/2, tempB, temp);
	painter->drawRect(width_/2, -temp/2, tempB, temp);
}

int ItemField::type() const
{
    //! Permite o uso de qgraphicsitem_cast com este item.
    return Type;
}