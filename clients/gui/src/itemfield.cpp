#define _USE_MATH_DEFINES
#include <cmath>

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

    return QRectF(-1.5*width_, -1.5*height_, 3*width_, 3*height_);
}

void ItemField::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                    QWidget *widget)
{
	qreal temp, tempB, tempC,
		width_, height_,
		line;

	width_ = stage_->fieldLength();
	height_ = stage_->fieldWidth();

	line = stage_->lineWidth();

	// Save transformation:
    QTransform oldTransformation = painter->worldTransform();
	
	// Change position
	painter->translate(-stage_->fieldLength()/2, -stage_->fieldWidth()/2 );

	painter->setBrush(Qt::white); 
    painter->setPen(Qt::white);

	// Centro
	drawArc(width_/2, height_/2,0,1.5*line,0,360,painter);

	// Laterais
	painter->drawRect(0,0,line,height_);
	painter->drawRect(width_-line,0,line,height_);
	painter->drawRect(0,0,width_,line);
	painter->drawRect(0,height_-line,width_,line);

	// Linhas centrais
	temp = stage_->centerCircleRadius();
	painter->fillRect( (width_-line)/2, 0, line, height_, Qt::white );

	drawArc(width_/2,height_/2,temp-line,temp,0,360,painter);

	// Linhas de defesa
	temp = stage_->defenseRadius();
	tempB = stage_->defenseStretch();
	
	painter->drawRect( temp-line, (height_-tempB)/2, line, tempB );
	drawArc( 0, (height_-tempB)/2,temp-line,temp,0,90,painter);
	drawArc( 0, (height_+tempB)/2,temp-line,temp,270,360,painter);

	painter->drawRect( width_ - temp, (height_-tempB)/2, line, tempB );
	drawArc( width_, (height_-tempB)/2,temp-line,temp,90,180,painter);
	drawArc( width_, (height_+tempB)/2,temp-line,temp,180,270,painter);

	// Penalti
	drawArc(stage_->penaltyLineDistance(), height_/2,0,line,0,360,painter);
	drawArc(width_ - stage_->penaltyLineDistance(), height_/2,0,line,0,360,painter);

	// Gols
	temp = stage_->blueGoal()->width();
	tempB = stage_->blueGoal()->depth();
	tempC = stage_->blueGoal()->wallWidth();

	painter->drawRect(-tempB-tempC	,(height_-temp)/2-tempC	,tempB+tempC	,tempC);
	painter->drawRect(-tempB-tempC	,(height_-temp)/2		,tempC			,temp);
	painter->drawRect(-tempB-tempC	,(height_+temp)/2		,tempB+tempC	,tempC);

	painter->drawRect(width_			,(height_-temp)/2-tempC	,tempB+tempC	,tempC);
	painter->drawRect(width_+tempB		,(height_-temp)/2		,tempC			,temp);
	painter->drawRect(width_			,(height_+temp)/2		,tempB+tempC	,tempC);


	// Reset transformation
	painter->setTransform(oldTransformation);
}

int ItemField::type() const
{
    //! Permite o uso de qgraphicsitem_cast com este item.
    return Type;
}


// Desenha um arco
void drawArc( qreal x, qreal y, qreal radiusIn, qreal radiusOut, qreal angleInit, qreal angleEnd, QPainter *painter)
{
	// Cria path
	QPainterPath path; 

	// Desenha
	path.moveTo( x + radiusIn*cos(angleInit*M_PI/180), y + radiusIn*sin(angleInit*M_PI/180) );
	path.arcTo( x-radiusOut, y-radiusOut, 2*radiusOut,2*radiusOut,angleInit,angleEnd-angleInit);
	path.arcTo( x-radiusIn, y-radiusIn, 2*radiusIn, 2*radiusIn, angleEnd, angleInit-angleEnd );
	path.closeSubpath();

	painter->drawPath(path);
}