#include "Team.h"
#include "Robot.h"
#include "Stage.h"
#include <iostream>

using namespace LibIntelligence;

Team::Team(QObject* parent, TeamColor c)
	: QObject(parent),
	QList<Robot*>(),
	color_(c),
	goals_(0),
	yellowCards_(0),
	redCards_(0)
{
	stage_ = qobject_cast<Stage*>(this->parent());
}

Team::Team(Stage* stage, const Team& team)
	: QObject(stage),
	color_(team.color()),
	goals_(team.goals()),
	yellowCards_(team.yellowCards()),
	redCards_(team.redCards())
{
	stage_ = stage;
	for(int i=0; i<team.size(); i++){
		Robot* r = new Robot( *(team.at(i)) );
		r->setTeam(this);
		r->setStage(stage);
		this->push_back(r);
	}
	
}

void Team::setColor(TeamColor c)
{
	color_ = c;
}

void Team::setYellowCard(quint8 i)
{
	yellowCards_ = i;
}

void Team::setRedCard(quint8 i)
{
	redCards_ = i;
}

void Team::addYellowCard()
{
	yellowCards_++;
}

void Team::addRedCard()
{
	redCards_++;
}

quint8 Team::yellowCards() const
{
	return yellowCards_;
}

quint8 Team::redCards() const
{
	return redCards_;
}

TeamColor Team::color() const
{
	return color_;
}

void Team::setGoals(quint8 goals){
	goals_ = goals;
}

quint8 Team::goals() const{
	return goals_;
}

Team* Team::enemyTeam() const
{
	return stage_ == 0 ? 0 : color_ == BLUE ? stage_->yellowTeam() : stage_->blueTeam();
}

Stage* Team::stage() const
{
	return stage_;
}

Goal* Team::goal() const
{
	return stage_ == 0 ? 0 : color_ == BLUE ? stage_->blueGoal() : stage_->yellowGoal();
}

Goal* Team::enemyGoal() const
{
	return stage_ == 0 ? 0 : color_ == BLUE ? stage_->yellowGoal() : stage_->blueGoal();
}

Team& Team::operator=(const Team& team)
{
	((QList<Robot*>*)this)->operator=(team);
	this->color_ = team.color();
	this->goals_ = team.goals();
	this->yellowCards_ = team.yellowCards();
	this->redCards_ = team.redCards();
	return *this;
}

Robot* Team::getClosestPlayerToBall() const
{
	return this->stage()->getClosestPlayerToBall(this);
}

Robot* Team::getClosestPlayerToBallThatCanKick() const
{
	return this->stage()->getClosestPlayerToBallThatCanKick(this);
}

//void Team::add(Robot& r)
//{
//	_robot.push_back(&r);
//}
//
//void Team::add(const QVector<Robot*>& r)
//{
//	_robot += r;
//}

//used on sort
//bool comp(Robot* r1, Robot* r2) {return r1->id() < r2->id();}

//void Team::sort() {
	//_robot.sort(comp);
	//TODO: consider removing this method.
//}