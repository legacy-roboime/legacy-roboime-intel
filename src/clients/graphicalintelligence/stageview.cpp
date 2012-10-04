#include "stageview.h"
#include "itemrobot.h"
#include "itemfield.h"
#include <QtGui>
#include "Ball.h"

using namespace LibIntelligence;
#define BALL_RADIUS		21.5

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
	field->setPos(0,0);

	// Desenho da bola
	QGraphicsEllipseItem* bola = new QGraphicsEllipseItem(stage->ball()->x()-BALL_RADIUS/2,stage->ball()->y()-BALL_RADIUS,BALL_RADIUS,BALL_RADIUS,NULL,scene());
	bola->setBrush(QBrush(orange));
	bola->setPen(QPen(orange));

	// Ajuste do Viewport
	//setSceneRect(-sta->fieldWidth()/2, -sta->fieldLength()/2, sta->fieldWidth(), sta->fieldLength());
	//scene->setSceneRect(-sta->fieldWidth()/2, -sta->fieldLength()/2, sta->fieldWidth(), sta->fieldLength());
	//QPoint r(width(), height());
	//QPointF mts = mapToScene(r);
	//qreal scaleWidth = width()/mapToScene(r).x();
	//scale(1/scaleWidth, 1);
	fitInView(-stage->fieldWidth()/2, -stage->fieldLength()/2, stage->fieldWidth(), stage->fieldLength(),Qt::KeepAspectRatio);
	//centerOn(0,0);
	
	LibIntelligence::Team* blueTeam = stage->blueTeam();
	LibIntelligence::Team* yellowTeam = stage->yellowTeam();
	LibIntelligence::Robot* robot;
	blueTeam->size();
	for(int i=0; i<blueTeam->size(); i++) {
		robot = blueTeam->at(i);
		if (robot->isActive()) {
			ItemRobot* grobot = new ItemRobot();
            grobot->setTeam(TeamBlue);
			grobot->setDirection(-robot->orientation()*180/M_PI);
			scene()->addItem(grobot);
			grobot->setPos(robot->x(), robot->y());
		}
	}
	for(int i=0; i<yellowTeam->size(); i++) {
		robot = yellowTeam->at(i);
		if (robot->isActive()) {
			ItemRobot* grobot = new ItemRobot();
            grobot->setTeam(TeamYellow);
			grobot->setDirection(-robot->orientation()*180/M_PI);
			scene()->addItem(grobot);
			grobot->setPos(robot->x(), robot->y());
		}
	}
}
