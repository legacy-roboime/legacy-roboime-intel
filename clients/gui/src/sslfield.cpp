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
	if(			stage->ball()->x() > -stage->fieldLength()/2 - BORDER &&
                stage->ball()->x() < stage->fieldLength()/2 + BORDER &&
                stage->ball()->y() > -stage->fieldWidth()/2 - BORDER &&
                stage->ball()->y() < stage->fieldWidth()/2 + BORDER)
	{
		scene()->addEllipse(
			field->pos().x() + (stage->ball()->x() - stage->ball()->radius()/2),
			field->pos().y() - (stage->ball()->y() - stage->ball()->radius()),
			stage->ball()->radius(),
			stage->ball()->radius(),
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

        if (robot->isActive() &&
                robot->x() > -stage->fieldLength()/2 - BORDER &&
                robot->x() < stage->fieldLength()/2 + BORDER &&
                robot->y() > -stage->fieldWidth()/2 - BORDER &&
                robot->y() < stage->fieldWidth()/2 + BORDER)
		{
			ItemRobot* grobot = new ItemRobot();
			grobot->setRobot(robot);
			scene()->addItem(grobot);
		}
	}

	for(int i=0; i<yellowTeam->size(); i++)
	{
		robot = yellowTeam->at(i);
        if (robot->isActive() &&
                robot->x() > -stage->fieldLength()/2 - BORDER &&
                robot->x() < stage->fieldLength()/2 + BORDER &&
                robot->y() > -stage->fieldWidth()/2 - BORDER &&
                robot->y() < stage->fieldWidth()/2 + BORDER)
		{
			ItemRobot* grobot = new ItemRobot();
			grobot->setRobot(robot);
			scene()->addItem(grobot);
		}
	}
	
    fitInView(-stage->fieldLength()/2-5*BORDER, -stage->fieldWidth()/2-5*BORDER, stage->fieldLength()+10*BORDER, stage->fieldWidth()+10*BORDER,Qt::KeepAspectRatio);
}