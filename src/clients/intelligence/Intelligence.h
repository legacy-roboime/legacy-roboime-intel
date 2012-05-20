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
#include "Plays.h"
#include "KalmanFilters.h"
#include "Tactics.h"

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

#ifndef SOCCER_DEBUG
private slots:
	void update();
#else
public:
	void update();
#endif

public:
	Minmax2* play;

signals:
	void readySend();

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

	Goalkeeper* player0;
	Defender* player1;
	Defender* player2;
	Defender* player3;
	Attacker* player4;

	Play* halt;
	Play* stopReferee;
	Plays::CBR2011* cbr2011;
	//Play* freeKickThem;
	
	Tactic* controller;
	Skill* gotoold;
	Skill* skill1;
	Skill* skill2;
	Skill* skill3;
	Tactic* machine;
	Tactics::Attacker* tactic;
	Tactics::AttackerMinMax2* attacker;
};

#endif // INTELLIGENCE_H
