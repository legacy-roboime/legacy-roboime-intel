#pragma once
#ifndef INTELLIGENCE_H
#define INTELLIGENCE_H

#include <QObject>
#include <QTimer>
#include <QMutex>
#include <QThread>
#include <map>

#include "Team.h"
#include "Stage.h"
#include "Commander.h"
#include "Updater.h"
#include "Skill.h"
#include "Tactic.h"
#include "Play.h"

using namespace LibIntelligence;

class Intelligence : public QObject
{
	friend class IntelligenceCli;
	Q_OBJECT

public slots:
	void update();

public:
	Intelligence(QObject *parent=0);
	~Intelligence();

private:
	// Basic objects
	map<string, Team*> team;
	map<string, Robot*> robot;
	map<string, Stage*> stage;

	// STP utils
	map<string, Plays::Play*> play;
	map<string, Tactics::Tactic*> tactic;
	map<string, Skills::Skill*> skill;

	// Interface utils
	map<string, Commander*> commander;
	map<string, Updater*> updater;

	QTimer* timer;
	QThread* cli;
	QMutex mutex;
};

#endif // INTELLIGENCE_H
