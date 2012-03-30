#include "UpdateRobot.h"
#include "Updater.h"
#include <iostream>
#include <sstream>
#include "messages_robocup_ssl_detection.pb.h"

namespace LibIntelligence
{
	struct UpdateRobotImpl
	{
		UpdateRobotImpl(TeamColor tc, quint8 i, qreal x, qreal y, qreal o)
			: teamColor(tc), pId(i), x(x), y(y), theta(o) {}

		TeamColor teamColor;
		quint8 pId;
		qreal x, y, theta;
		qreal vX, vY;
		qreal wZ;
	};
}

using namespace LibIntelligence;

UpdateRobot::UpdateRobot(TeamColor tc, const SSL_DetectionRobot& p, double t1, double t2, int camId)
	: Update(t1,t2,camId), pimpl(new UpdateRobotImpl(tc, p.robot_id(), p.x(), p.y(), p.orientation())) {}

UpdateRobot::UpdateRobot(TeamColor tc, quint8 i, qreal x, qreal y, qreal theta, double t1, double t2, int camId)
	: Update(t1,t2,camId), pimpl(new UpdateRobotImpl(tc, i, x, y, theta)) {}

UpdateRobot::~UpdateRobot()
{
	delete pimpl;
}

void UpdateRobot::apply(Updater* u)
{
	//cout << to_string() << endl;//test
	static qreal last_time_capture;
	for(quint32 k = u->robotsSize(); k > 0; k--) {
		if(u->robot(k-1)->patternId() == patternId() && u->robot(k-1)->color() == color()) {
			u->robot(k-1)->setX(x());
			u->robot(k-1)->setY(y());
			u->robot(k-1)->setOrientation(theta());
			//u->robot(k-1)->updateSpeed();
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

qreal UpdateRobot::x() const
{
	return pimpl->x;
}

qreal UpdateRobot::y() const
{
	return pimpl->y;
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
