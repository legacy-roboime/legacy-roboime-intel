#include "Skill.h"
#include "Robot.h"

using namespace LibIntelligence;
using namespace LibIntelligence::Skills;

Skill::Skill(QObject* p, Robot* r, bool deterministic)
	: //QObject(p),
	robot_(r),
	State(p, deterministic)
{}

Skill::Skill(const Skill& skill) : State(skill)
{}

void Skill::setRobot(Robot* r) {
	robot_ = r;
}

const Robot* Skill::robot() const
{
	return robot_;
}

const Stage* Skill::stage() const
{
	return robot_->stage();
}

Robot* Skill::robot()
{
	return robot_;
}

Stage* Skill::stage()
{
	return robot_->stage();
}

//bool Skill::busy()
//{
//	return false;
//}