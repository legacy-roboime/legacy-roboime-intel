#pragma once
#ifndef GOAL_H
#define GOAL_H

#include "LibIntelligence.h"
#include <QObject>
#include <QString>
#include "Object.h"
#include <QPointF>
#include <QlineF>

namespace LibIntelligence
{
	class Goal : public QObject, public Object
	{
		Q_OBJECT

	public:
		Goal(QObject* parent=0, qreal x=0.0, qreal y=0.0, qreal width=0.0, qreal depth=0.0, qreal wallWidth=0.0);
		Goal(const Goal& goal);

		void setWidth(qreal);
		qreal width() const;

		void setDepth(qreal);
		qreal depth() const;

		void setWallWidth(qreal);
		qreal wallWidth() const;

		void setPenaltyMark(qreal , qreal);
		QPointF penaltyMark() const;

		void setPenaltyLine(qreal);
		qreal penaltyLine() const;

		Goal& operator=(const Goal& goal);
	private:
		qreal width_, depth_, wallWidth_;
		QPointF penaltyMark_;
		qreal penaltyLine_; //coordenada x da linha de penalty
	};
}

#endif // GOAL_H
