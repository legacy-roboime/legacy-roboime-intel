#ifndef GRAPHICALINTELLIGENCE_H
#define GRAPHICALINTELLIGENCE_H

#include <QtGui/QMainWindow>
#include "ui_GraphicalIntelligence.h"
#include <QTimer>
#include "Team.h"
#include "Stage.h"
#include "Commander.h"
#include "Updater.h"
#include "Skill.h"

using namespace LibIntelligence;
using namespace LibIntelligence::Skills;

class GraphicalIntelligence : public QMainWindow
{
	Q_OBJECT

public:
	GraphicalIntelligence(QWidget *parent = 0, Qt::WFlags flags = 0);
	~GraphicalIntelligence();

private slots:
	void update();

private:
	Ui::GraphicalIntelligenceClass ui;
	QTimer* timer;
	Stage* sta;
	Commander* comB;
	Commander* comB2;
	Commander* comY;
	Updater* upd;
	Team blueTeam;
	Team yellowTeam;
	//Updater* updReferee;
	Skill* test[10];
	Ball* ball;
};

#endif // GRAPHICALINTELLIGENCE_H
