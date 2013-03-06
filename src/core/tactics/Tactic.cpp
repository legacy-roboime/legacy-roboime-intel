#include "Tactic.h"
#include "Skill.h"
#include "Robot.h"
#include "Skill.h"

using namespace LibIntelligence;
using namespace LibIntelligence::Tactics;
using namespace LibIntelligence::Skills;

Tactic::Tactic(QObject* p, Robot* s, bool deterministic)
	: //QObject(p),
	StateMachine(p, deterministic), 
	robot_(s),
	stage_(s->stage())//,
	//skills(0) 
	{}

Tactic::Tactic(QObject* p, Robot* s, const Tactic& tactic) : StateMachine(p ,tactic)
{
	this->setRobot(s);
	this->setStage(s->stage());
}

Tactic::~Tactic()
{
	for(int i=0; i<states_.size(); i++)
		delete ((Skill*)states_.at(i));
}

void Tactic::setRobot(Robot* r)
{
	robot_ = r;
	stage_ = r->stage();
	for(int i = 0; i < states_.size(); i++) {
		((Skills::Skill*)(states_.at(i)))->setRobot(r);
	}
}

void Tactic::setStage(Stage* s)
{
	stage_ = s;
}

const Robot* Tactic::robot() const
{
	return robot_;
}

const Stage* Tactic::stage() const
{
	return stage_;
}

Robot* Tactic::robot()
{
	return robot_;
}

Stage* Tactic::stage()
{
	return stage_;
}

void Tactic::step()
{
	Skill* current = (Skill*)this->getCurrentState();
	current->step();
	this->execute();
	//const Robot* r = this->robot();
	//cout << r->id() << endl;
	//cout << "SKILL: " << current->objectName().toStdString() << endl;
}

void Tactic::setSpeed(qreal speed)
{
	for(int i = 0; i < states_.size(); i++) {
		((Skills::Skill*)(states_.at(i)))->setSpeed(speed);
	}
}