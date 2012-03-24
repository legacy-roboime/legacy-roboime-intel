#include "Robot.h"
#include "Team.h"
#include "Stage.h"
#include <sstream>
//#include "Command.h"

using namespace LibIntelligence;

Robot::Robot(const Robot& r)
	: Component(r.parent()),
	Object(r.x_, r.y_, r.z_, r.speedX_, r.speedY_, r.speedZ_),
	id_(r.id_),
	patternId_(r.patternId_),
	color_(r.color_),
	dribbler_(r.dribbler_),
	kicker_(r.kicker_),
	body_(r.body_),
	battery_(r.battery_),
	wheels_(r.wheels_),
	command_(r.command_),
	orientation_(r.orientation_),
	stage_(r.stage_),
	team_(r.team_)/*,
	enemyTeam_(r.enemyTeam_),
	goal_(r.goal_),
	enemyGoal_(r.enemyGoal_)*/
{
	dribbler_.setParent(this);
	kicker_.setParent(this);
	body_.setParent(this);
	battery_.setParent(this);
	for(int i = 0; i < wheelsSize(); i++) wheels_[i].setParent(this);
}

Robot::Robot(QObject* parent, int id, int cc, TeamColor color)
	: Component(parent),
	Object(),
	id_(id),
	patternId_(cc),
	color_(color),
	dribbler_(this),
	kicker_(this),
	body_(this),
	battery_(this),
	wheels_(0),
	//body_(this, 150.0, 90.0),
	//wheels_(4, Wheel(this)),//TODO: think about this
	command_(id),//1,5707963267948966192313216916398
	orientation_(1.57079632)
{
	//try to identify the Team, Stage and Enemy Team
	team_ = qobject_cast<Team*>(parent);
	if(team_ != 0) {
		stage_ = team_->stage();
		/*color_ = team_->color();
		stage_ = qobject_cast<Stage*>(team_->parent());*/
		/*if(stage_ != 0) {
			if(color_ == BLUE) {
				goal_      = &stage_->blueGoal_;
				enemyGoal_ = &stage_->yellowGoal_;
				//enemyTeam_ = &stage_->yellowTeam_;
			} else {
				goal_      = &stage_->yellowGoal_;
				enemyGoal_ = &stage_->blueGoal_;
				enemyTeam_ = &stage_->blueTeam_;
			}
		} else {
			goal_ = 0;
			enemyGoal_ = 0;
			enemyTeam_ = 0;
		}*/
	} else {
		/*goal_ = 0;
		enemyGoal_ = 0;
		team_ = 0;
		enemyTeam_ = 0;*/
		stage_ = qobject_cast<Stage*>(parent);
	}
}

Robot::Robot(const Dribbler& d, const Kicker& k, const Body& b, const Battery& a, const QVector<Wheel>& w)
	: Component(),
	Object(),
	id_(0),
	patternId_(0),
	color_(BLUE),
	dribbler_(d),
	kicker_(k),
	body_(b),
	battery_(a),
	wheels_(w),
	command_(0),
	orientation_(0.0) {}

Robot::~Robot() {}

const Dribbler& Robot::dribbler() const
{
	return dribbler_;
}

const Kicker& Robot::kicker() const
{
	return kicker_;
}

const Body& Robot::body() const
{
	return body_;
}

const Battery& Robot::battery() const
{
	return battery_;
}

const QVector<Wheel>& Robot::wheels() const
{
	return wheels_;
}

const Command& Robot::command() const
{
	return command_;
}

Dribbler& Robot::dribbler()
{
	return dribbler_;
}

Kicker& Robot::kicker()
{
	return kicker_;
}

Body& Robot::body()
{
	return body_;
}

Battery& Robot::battery()
{
	return battery_;
}

QVector<Wheel>& Robot::wheels()
{
	return wheels_;
}

Stage* Robot::stage() const
{
	return stage_;
}

void Robot::setStage(Stage* stage)
{
	stage_ = stage;
}

Team* Robot::team() const
{
	return team_;
}

void Robot::setTeam(Team* team)
{
	team_ = team;
}

Team* Robot::enemyTeam() const
{
	return team_ == 0 ? 0 : team_->enemyTeam();
}

Goal* Robot::goal() const
{
	return team_ == 0 ? 0 : team_->goal();
}

Goal* Robot::enemyGoal() const
{
	return team_ == 0 ? 0 : team_->enemyGoal();
}
Command& Robot::command()
{
	return command_;
}

quint32 Robot::wheelsSize() const
{
	return wheels_.size();
}

void Robot::setColor(TeamColor c)
{
	color_ = c;
}

void Robot::setId(quint8 i)
{
	id_ = i;
	command_.setId(id_);
}

void Robot::setPatternId(quint8 i)
{
	patternId_ = i;
}

void Robot::setOrientation(qreal o)
{
	orientation_ = o;
}

string Robot::_summary() const
{
	stringstream out;
	out << "x: " << x() << endl;
	out << "y: " << y() << endl;
	out << "angle: " << orientation() << endl;
	return out.str();
}

bool Robot::canKick() const
{
	return kicker_.isActive() && kicker_.isWorking();
}

bool Robot::canDribble() const
{
	return dribbler_.isActive() && dribbler_.isWorking();
}

bool Robot::isWorking() const
{
	bool w = true;
	for(int i = 0; i < wheelsSize(); i++) w = w && wheels_[i].isWorking();
	return working && dribbler_.isWorking() && kicker_.isWorking() && body_.isWorking() && w;
}

bool Robot::isNotWorking() const
{
	bool w = false;
	for(int i = 0; i < wheelsSize(); i++) w = w || wheels_[i].isNotWorking();
	return !working || dribbler_.isNotWorking() || kicker_.isNotWorking() || body_.isNotWorking() || w;
}

void Robot::setAllWorking()
{
	dribbler_.setWorking();
	kicker_.setWorking();
	for(int i = 0; i < wheelsSize(); i++) wheels_[i].setWorking();
	body_.setWorking();
	setWorking();
}

void Robot::setAllNotWorking()
{
	dribbler_.setNotWorking();
	kicker_.setNotWorking();
	for(int i = 0; i < wheelsSize(); i++) wheels_[i].setNotWorking();
	body_.setNotWorking();
	setNotWorking();
}

string Robot::toString() const
{
	stringstream out;
	out << "Summary:" << endl;
	out << _summary() << endl;
	return out.str();
}

void Robot::kick(qreal speed)
{
	command_.setKickSpeed(speed);
}

void Robot::dribble(qreal speed)
{
	command_.setDribbleSpeed(speed);
}

TeamColor Robot::color() const
{
	return color_;
}

quint8 Robot::id() const
{
	return id_;
}

quint8 Robot::patternId() const
{
	return patternId_;
}

qreal Robot::orientation() const
{
	return orientation_;
}

void Robot::setCommand(const Command& c)
{
	//command = new Command(c);//TEMP
	command_ = c;
	command_.setId(id_);
}

void Robot::setCommandWheels(const QVector<qreal>& s)
{
	command_.setWheelsSpeed(s);
}

void Robot::newCommand()
{
	command_.clear();
	//command = new Command(this, getId());
}

void Robot::setParent(QObject* p)
{
	QObject::setParent(p);
	team_ = qobject_cast<Team*>(p);
	if(team_ != 0) {
		color_ = team_->color();
		stage_ = qobject_cast<Stage*>(team_->parent());
	} else {
		stage_ = qobject_cast<Stage*>(p);
	}
}