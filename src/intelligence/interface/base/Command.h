#pragma once
#ifndef COMMAND_H
#define COMMAND_H

#include "LibIntelligence.h"
#include <QObject>
#include <QVector>
#include <QDatetime>

class CommandRobot;

namespace LibIntelligence
{
	struct CommandImpl;

	class Command
	{
	public:
		Command(const Command&);
		Command(quint8 id=0, const QVector<qreal>& wheels=QVector<qreal>(4), qreal dribble=0.0, qreal kick=0.0);
		~Command();

		void setId(quint8);
		int id() const;

		void setKickSpeed(qreal);
		qreal kickSpeed() const;

		void setDribbleSpeed(qreal);
		qreal dribbleSpeed() const;

		void setWheelsSpeed(const QVector<qreal>&);
		const QVector<qreal>& wheelsSpeed() const;
		qreal wheelSpeedAt(quint32) const;
		quint32 wheelsSize() const;

		void setTimestamp(double);
		void setTimestampNow();
		double timestamp() const;

		void setForcedKick(qreal);
		bool isForcedKick() const;

		void setForcedDribble(qreal);
		bool isForcedDribble() const;

		void setOld();
		bool isOld() const;

		void clear();
		const CommandRobot& raw() const;

	private:
		CommandImpl* pimpl;
	};
}

#endif // COMMAND_H
