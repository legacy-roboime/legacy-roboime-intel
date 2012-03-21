#pragma once
#ifndef INTELLIGENCE_H
#define INTELLIGENCE_H

#include <QObject>
#include <QTimer>
#include "Team.h"
#include "Stage.h"
#include "Commander.h"
#include "Updater.h"
#include "Skill.h"
#include "Tactic.h"
#include "Play.h"
#include "KalmanFilters.h"

using namespace LibIntelligence;
using namespace LibIntelligence::Skills;
using namespace LibIntelligence::Tactics;
using namespace LibIntelligence::Filters;
using namespace LibIntelligence::Plays;

class Intelligence : public QObject
{
	Q_OBJECT

public:
	Intelligence(QObject *parent=0);
	~Intelligence();

private slots:
	void update();

private:
	Skill* test[10];
	QTimer* timer;
	Stage* sta;
	Commander* comB;
	Commander* comB2;
	Commander* comY;
	Updater* upd; //Vision
	Team* myTeam;
	Team* enemyTeam;
	Updater* updReferee;
	Ball* ball;
	KalmanFilters* filter;

	Play* halt;
	Play* stopReferee;
	Play* cbr2011;
	//Play* freeKickThem;
	Play* play;
	Tactic* controller;
	Skill* gotoold;
	Skill* skill1;
	Skill* skill2;
	Skill* skill3;
	Tactic* machine;
	Tactic* tactic;
};

#endif // INTELLIGENCE_H
