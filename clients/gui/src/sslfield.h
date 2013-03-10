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
  \file    sslField.h
  \brief   Class for drawing the current field, robots, and the ball
  \author  Tim Laue (C) 2010
*/
//========================================================================

#ifndef SSL_FIELD_H
#define SSL_FIELD_H

#include <QGraphicsView>
#include <Stage.h>
#include <Robot.h>

using LibIntelligence::Stage;
using LibIntelligence::Robot;

class SSLField : public QGraphicsView
{
public:
	SSLField(QWidget * parent);
	QRectF boundingRect() const;

    void setStage(Stage *sta);

	void redraw();

protected:
    void paintRobot(const Robot& robot, const QColor& teamColor);

private:
    Stage *stage;
};

#endif
