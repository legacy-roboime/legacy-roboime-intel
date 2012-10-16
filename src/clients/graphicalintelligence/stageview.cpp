#include "stageview.h"
#include "itemrobot.h"
#include "itemfield.h"
#include <QtGui>
#include "Ball.h"

using namespace LibIntelligence;
#define BALL_RADIUS		21.5
#define BORDER          200
StageView::StageView(QWidget *parent)
	: QGraphicsView(parent)
{
    //setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
	setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
	setBackgroundBrush(QBrush(Qt::darkGreen));
	setCacheMode(QGraphicsView::CacheNone);
}

StageView::~StageView()
{

}

void StageView::setStage(Stage* sta)
{
	stage = sta;

	QGraphicsScene *scene = new QGraphicsScene(-sta->fieldWidth()/2, -sta->fieldLength()/2, sta->fieldWidth(), sta->fieldLength());
	setScene(scene);
	scale(1,-1);

}
#include <iostream>
void StageView::redraw()
{
	QColor orange(0xff,0xbb,0x00);

	scene()->clear();
	

	// Desenho do campo
    ItemField* field = new ItemField(stage);
    scene()->addItem(field);
    field->setPos(-stage->fieldLength()/2,-stage->fieldWidth()/2);
    //field->setPos(0,0);
	// Desenho da bola
	if(			stage->ball()->x() > -stage->fieldLength()/2 - BORDER &&
                stage->ball()->x() < stage->fieldLength()/2 + BORDER &&
                stage->ball()->y() > -stage->fieldWidth()/2 - BORDER &&
                stage->ball()->y() < stage->fieldWidth()/2 + BORDER)
	{
		QGraphicsEllipseItem* bola = new QGraphicsEllipseItem(
					field->pos().x() + (stage->ball()->x() - BALL_RADIUS/2),
					field->pos().y() + (stage->ball()->y() - BALL_RADIUS),
					BALL_RADIUS,BALL_RADIUS,NULL,scene());

		bola->setBrush(QBrush(orange));
		bola->setPen(QPen(orange));
	}

	// Ajuste do Viewport
	//setSceneRect(-sta->fieldWidth()/2, -sta->fieldLength()/2, sta->fieldWidth(), sta->fieldLength());
	//scene->setSceneRect(-sta->fieldWidth()/2, -sta->fieldLength()/2, sta->fieldWidth(), sta->fieldLength());
	//QPoint r(width(), height());
	//QPointF mts = mapToScene(r);
	//qreal scaleWidth = width()/mapToScene(r).x();
	//scale(1/scaleWidth, 1);

	//centerOn(0,0);
	
	LibIntelligence::Team* blueTeam = stage->blueTeam();
	LibIntelligence::Team* yellowTeam = stage->yellowTeam();
	LibIntelligence::Robot* robot;
	blueTeam->size();
	for(int i=0; i<blueTeam->size(); i++) {
		robot = blueTeam->at(i);
        if (robot->isActive() &&
                robot->x() > -stage->fieldLength()/2 - BORDER &&
                robot->x() < stage->fieldLength()/2 + BORDER &&
                robot->y() > -stage->fieldWidth()/2 - BORDER &&
                robot->y() < stage->fieldWidth()/2 + BORDER) {
			ItemRobot* grobot = new ItemRobot();
            grobot->setTeam(TeamBlue);
			grobot->setDirection(-robot->orientation()*180/M_PI);
			scene()->addItem(grobot);
            grobot->setPos(field->pos().x() + robot->x(), field->pos().y() + robot->y());
		}
	}
	for(int i=0; i<yellowTeam->size(); i++) {
		robot = yellowTeam->at(i);
        if (robot->isActive() &&
                robot->x() > -stage->fieldLength()/2 - BORDER &&
                robot->x() < stage->fieldLength()/2 + BORDER &&
                robot->y() > -stage->fieldWidth()/2 - BORDER &&
                robot->y() < stage->fieldWidth()/2 + BORDER) {
			ItemRobot* grobot = new ItemRobot();
            grobot->setTeam(TeamYellow);
			grobot->setDirection(-robot->orientation()*180/M_PI);
			scene()->addItem(grobot);
            grobot->setPos(field->pos().x() + robot->x(), field->pos().y() + robot->y());
		}
	}
    fitInView(-stage->fieldLength()/2-5*BORDER, -stage->fieldWidth()/2-5*BORDER, stage->fieldLength()+10*BORDER, stage->fieldWidth()+10*BORDER,Qt::KeepAspectRatio);
}
