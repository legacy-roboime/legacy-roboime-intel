#pragma once
#ifndef UPDATER_H
#define UPDATER_H

#include "LibIntelligence.h"
#include "Updates.h"
#include <QObject>
#include <QList>
#include <QQueue>

namespace LibIntelligence
{
	class Robot;
	class Ball;
	class Update;
	class Stage;

	class Updater : public QObject
	{
		Q_OBJECT

	public:
		Updater(QObject* parent=0);
		~Updater();

		virtual void receive();
		virtual void prepare();
		virtual void step();
		void apply();

		void add(Robot*);
		void del(Robot*);
		quint32 robotsSize() const;
		Robot* robot(quint32) const;

		void add(Ball*);
		void del(Ball*);
		quint32 ballsSize() const;
		Ball* ball(quint32) const;

		void add(Stage*);
		void del(Stage*);
		quint32 stagesSize() const;
		Stage* stage(quint32) const;

		Updates*const updates();

	signals:
		void receivedUpdate();

	protected:
		void enqueue(Update*);
		Update* dequeue();

	private:
		Updates updates_;
		Updates logs;
		QList<Ball*> balls;
		QList<Robot*> robots;
		QList<Stage*> stages;

	};
}

#endif // UPDATER_H
