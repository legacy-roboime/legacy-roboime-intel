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
#include "config.h"

using namespace LibIntelligence;

class Intelligence : public QObject
{
	friend struct IntelligenceCli;
	Q_OBJECT

public slots:
	void update();

public:
	Intelligence(QObject *parent=0);
	~Intelligence();

	// STP utils
	map<string, Plays::Play*> play;
	map<string, Tactics::Tactic*> tactic;
	map<string, Skills::Skill*> skill;
	enum {NONE, SKILL, TACTIC, PLAY, CONTROLLER} mode;
	int controlled;//tirar quando o controller for para o time inteiro

private:
	// Basic objects
	map<string, Team*> team;
	map<string, Robot*> robot;
	map<string, Stage*> stage;

	// Interface utils
	map<string, Commander*> commander;
	map<string, Updater*> updater;
	bool useSimulation;
	void resetPatterns();

	QTimer* timer;
	QThread* cli;
	QMutex mutex;
};

#endif // INTELLIGENCE_H
