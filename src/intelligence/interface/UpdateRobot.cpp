#include "UpdateRobot.h"
#include "Updater.h"
#include <iostream>
#include <sstream>
#include "messages_robocup_ssl_detection.pb.h"
#include "config.h"
#define PI 3.14159265
#include "Stage.h"

struct LibIntelligence::UpdateRobotImpl
{
	UpdateRobotImpl(TeamColor tc, quint8 i, qreal o)
		: teamColor(tc), pId(i), theta(o) {}

	TeamColor teamColor;
	quint8 pId;
	qreal theta;
};

using namespace LibIntelligence;

UpdateRobot::UpdateRobot(TeamColor tc, const SSL_DetectionRobot& p, double t1, double t2, int camId)
	: Update(t1, t2, camId),
	Point(p.x(), p.y()),
	pimpl(new UpdateRobotImpl(tc, p.robot_id(), p.orientation())),
	last_time_capture(0)
{}

UpdateRobot::UpdateRobot(TeamColor tc, quint8 i, qreal x, qreal y, qreal theta, double t1, double t2, int camId)
	: Update(t1,t2,camId),
	Point(x, y),
	pimpl(new UpdateRobotImpl(tc, i, theta)),
	last_time_capture(0)
{}

UpdateRobot::~UpdateRobot()
{
	delete pimpl;
}

void UpdateRobot::apply(Updater* u)
{
	//cout << to_string() << endl;//test
	//qreal time = time_capture();//unused
	for(quint32 k = u->robotsSize(); k > 0; k--) {
		Robot* robot = u->robot(k-1);
		if(robot->patternId() == patternId() && robot->color() == color()) {
			robot->updatePosition(*this);
			robot->setOrientation(theta());
			robot->updateSpeed(time_capture());

#ifdef TRANSFORMADA_CAMPO
			float A = 2*u->stage(0)->fieldWidth(); //length antigo
			float x = robot->x();
			float y = robot->y();
			robot->setX(y);
			robot->setY((-1.)*x + A/4.);
			robot->setOrientation(robot->orientation() - PI/2.);
			if(robot->orientation()>PI) 
				robot->setOrientation(robot->orientation() - 2.*PI);
			else if(robot->orientation()<-PI) 
				robot->setOrientation(robot->orientation() + 2.*PI);
#endif
		}
	}
}

TeamColor UpdateRobot::color() const
{
	return pimpl->teamColor;
}

quint8 UpdateRobot::patternId() const
{
	return pimpl->pId;
}

qreal UpdateRobot::theta() const
{
	return pimpl->theta;
}

string UpdateRobot::to_string() const
{
	stringstream out;
	out << "UpdateRobot: "// << patternId()
		<< " " << x() 
		<< " " << y() 
		<< " " << theta();
	return out.str();
}
