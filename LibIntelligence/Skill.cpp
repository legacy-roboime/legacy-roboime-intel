#include "Skill.h"
#include "Robot.h"

using namespace LibIntelligence;
using namespace LibIntelligence::Skills;

Skill::Skill(QObject* p, Robot* r, bool deterministic)
	: //QObject(p),
	robot_(r),
	stage_(r->stage()),
	State(p)
{
}

Skill::~Skill(){
	//delete robot_;
	//delete stage_;
}

Skill::Skill(const Skill& skill) : State(skill)
{

}

void Skill::setRobot(Robot* r) {
	robot_ = r;
	stage_ = r->stage();
}

const Robot* Skill::robot() const
{
	return robot_;
}

const Stage* Skill::stage() const
{
	return stage_;
}

Robot* Skill::robot()
{
	return robot_;
}

Stage* Skill::stage()
{
	return stage_;
}

//bool Skill::busy()
//{
//	return false;
//}