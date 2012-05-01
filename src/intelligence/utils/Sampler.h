#pragma once
#ifndef SAMPLER_H
#define SAMPLER_H

#include "LibIntelligence.h"
#include <QPointF>
#include <QLineF>
#include <QObject>

namespace LibIntelligence
{
	class Sampler : public QObject
	{
		Q_OBJECT

	public:
		Sampler(QObject* parent=0);
		~Sampler(void);
		static qreal sampledPowerDribble(qreal minPower, qreal maxPower);
		static qreal sampledPowerKick(qreal minPower, qreal maxPower);
		static QPointF sampledPointKick(QLineF target);
		static QPointF sampledPointKickH(QLineF target);
		static bool sampledUniformDist(qreal min, qreal max);
		static qreal randFloat(); //entre 0 e 1
		static long randInt(int low, int high);
	private:
		
	};
}

#endif // SAMPLER_H