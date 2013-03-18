//========================================================================
//  This software is free: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License Version 3,
//  as published by the Free Software Foundation.
//
//  This software is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  Version 3 in the file COPYING that came with this distribution.
//  If not, see <http://www.gnu.org/licenses/>.
//========================================================================
/*!
  \file    sslField.cpp
  \brief   Class for drawing the current field, robots, and the ball; reusing parts of graphicalClient
  \author  Tim Laue (C) 2010
*/
//========================================================================

#include "sslfield.h"
#include <QPainter>
#include <Ball.h>
#include <itemfield.h>
#include <itemrobot.h>

#define _USE_MATH_DEFINES
#include <cmath>


using LibIntelligence::Stage;
using LibIntelligence::Ball;

#define BORDER          200

SSLField::SSLField(QWidget * parent) : QGraphicsView(parent)
{
	// Seta a forma de renderização básica, a cor de fundo e o modo do cache
	setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
	setBackgroundBrush(QBrush(Qt::darkGreen));
	setCacheMode(QGraphicsView::CacheNone);

	viewScale = 0.1f;
	viewXOffset = 0;
	viewYOffset = 0;

	scale(viewScale,viewScale);
}

// 
void SSLField::setStage(Stage * sta)
{
    stage = sta;

	QGraphicsScene *scene = new QGraphicsScene(-sta->fieldWidth()/2, -sta->fieldLength()/2, sta->fieldWidth(), sta->fieldLength());
	setScene(scene);
}


QRectF SSLField::boundingRect() const
{
    return QRectF(0,0,0,0);
}

void SSLField::redraw()
{
	// Cria cor laranja
	QColor orange(0xff,0xbb,0x00);

	// Apaga a cena
	scene()->clear();

	// Campo
	// Desenho do campo
    ItemField* field = new ItemField(stage);
	scene()->addItem(field);
    field->setPos(-stage->fieldLength()/2,-stage->fieldWidth()/2);

	// Desenho da bola
	/*
	if(			stage->ball()->x() > -stage->fieldLength()/2 - BORDER &&
                stage->ball()->x() < stage->fieldLength()/2 + BORDER &&
                stage->ball()->y() > -stage->fieldWidth()/2 - BORDER &&
                stage->ball()->y() < stage->fieldWidth()/2 + BORDER)
	*/
	{
		scene()->addEllipse(
		field->pos().x() + stage->ball()->x() - stage->ball()->radius(),
		field->pos().y() - stage->ball()->y() - stage->ball()->radius(),
		stage->ball()->radius()*2,
		stage->ball()->radius()*2,
		QPen(orange),
		QBrush(orange) );
	}

	// Desenho dos robos
	LibIntelligence::Team* blueTeam = stage->blueTeam();
	LibIntelligence::Team* yellowTeam = stage->yellowTeam();
	LibIntelligence::Robot* robot;


	for(int i=0; i<blueTeam->size(); i++)
	{
		robot = blueTeam->at(i);

        if (robot->isActive() )
		/*	&&
                robot->x() > -stage->fieldLength()/2 - BORDER &&
                robot->x() < stage->fieldLength()/2 + BORDER &&
                robot->y() > -stage->fieldWidth()/2 - BORDER &&
                robot->y() < stage->fieldWidth()/2 + BORDER)
		*/
		{
			ItemRobot* grobot = new ItemRobot();
			grobot->setRobot(robot);
			scene()->addItem(grobot);
		}
	}

	for(int i=0; i<yellowTeam->size(); i++)
	{
		robot = yellowTeam->at(i);
		
        if (robot->isActive() )
		/*
		&&
                robot->x() > -stage->fieldLength()/2 - BORDER &&
                robot->x() < stage->fieldLength()/2 + BORDER &&
                robot->y() > -stage->fieldWidth()/2 - BORDER &&
                robot->y() < stage->fieldWidth()/2 + BORDER)
		*/
		{
			ItemRobot* grobot = new ItemRobot();
			grobot->setRobot(robot);
			scene()->addItem(grobot);
		}
	}

	centerOn(-stage->fieldLength()/2-viewXOffset,-stage->fieldWidth()/2-viewYOffset);
}

void SSLField::wheelEvent ( QWheelEvent * event )
{
  double zoomRatio = -double(event->delta())/1000.0;
  double oldScale = viewScale;
  viewScale = viewScale*(1.0+zoomRatio);

  scale(oldScale/viewScale,oldScale/viewScale);
}

void SSLField::mouseMoveEvent(QMouseEvent* event)
{
  bool leftButton = event->buttons().testFlag(Qt::LeftButton);
  bool midButton = event->buttons().testFlag(Qt::MidButton);
  bool rightButton = event->buttons().testFlag(Qt::RightButton);
  
  if(leftButton)
  {
    //Pan
    viewXOffset += (event->x() - mouseStartX);//*viewScale;
    viewYOffset += (event->y() - mouseStartY);//*viewScale
    mouseStartX = event->x();
    mouseStartY = event->y();
  }
}

void SSLField::mousePressEvent(QMouseEvent* event)
{
  bool leftButton = event->buttons().testFlag(Qt::LeftButton);
  bool midButton = event->buttons().testFlag(Qt::MidButton);
  bool rightButton = event->buttons().testFlag(Qt::RightButton);
  bool shiftKey = event->modifiers().testFlag(Qt::ShiftModifier);
  bool ctrlKey = event->modifiers().testFlag(Qt::ControlModifier);
  
  if(leftButton)
    setCursor(Qt::ClosedHandCursor);
  if(midButton)
  {
	  viewXOffset = 0;
	  viewYOffset = 0;
  }
  if(leftButton){
    // Start Pan / Zoom
    mouseStartX = event->x();
    mouseStartY = event->y();
  }

  // Arrastar com o meio do mouse
  /*
  else if(midButton)
  {
    //Zoom
    double zoomRatio = double(event->y() - mouseStartY)/500.0;
    double oldScale = viewScale;
    viewScale = viewScale*(1.0+zoomRatio);

    mouseStartX = event->x();
    mouseStartY = event->y();
  }
  */
}

void SSLField::mouseReleaseEvent(QMouseEvent* event)
{
  bool shiftKey = event->modifiers().testFlag(Qt::ShiftModifier);
  bool ctrlKey = event->modifiers().testFlag(Qt::ControlModifier);
  setCursor(Qt::ArrowCursor);
}