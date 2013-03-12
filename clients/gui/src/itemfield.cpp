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
	int line;

	width_ = stage_->fieldLength();
	height_ = stage_->fieldWidth();

	line = stage_->lineWidth();

	// TODO: Mudar exibição de linhas por exibição de retângulos!!!

	// Save transformation:
    QTransform oldTransformation = painter->worldTransform();
	
	// Change position
	painter->translate(-stage_->fieldLength()/2, -stage_->fieldWidth()/2 );


    painter->setPen(Qt::white);

	// Laterais
	painter->fillRect(0,0,line,height_,Qt::white);
	painter->fillRect(width_-line,0,line,height_,Qt::white);
	painter->fillRect(0,0,width_,line,Qt::white);
	painter->fillRect(0,height_,width_,line,Qt::white);

	// Linhas centrais
	temp = stage_->centerCircleRadius();
	painter->fillRect( width_/2, 0, line, height_, Qt::white );
	//painter->drawEllipse(-temp, -temp, temp*2, temp*2);

	// Linhas de penalti
	temp = stage_->defenseRadius();
	tempB = stage_->defenseStretch();
	painter->drawLine(-width_/2+temp, -tempB/2, -width_/2+temp, tempB/2);
	//painter->drawArc(-width_/2-temp, -temp-tempB/2, temp*2, temp*2, 0, 90*16);
	//painter->drawArc(-width_/2-temp, tempB/2-temp, temp*2, temp*2, 0, -90*16);
	painter->drawLine(width_/2-temp, -tempB/2, width_/2-temp, tempB/2);
	//painter->drawArc(width_/2-temp, -temp-tempB/2, temp*2, temp*2, -180*16, -90*16);
	//painter->drawArc(width_/2-temp, tempB/2-temp, temp*2, temp*2, 180*16, 90*16);

	// Gols
	temp = stage_->blueGoal()->width();
	tempB = stage_->blueGoal()->depth();
	painter->drawRect(-width_/2-tempB, -temp/2, tempB, temp);
	painter->drawRect(width_/2, -temp/2, tempB, temp);

	// Reset transformation
	painter->setTransform(oldTransformation);
}

int ItemField::type() const
{
    //! Permite o uso de qgraphicsitem_cast com este item.
    return Type;
}