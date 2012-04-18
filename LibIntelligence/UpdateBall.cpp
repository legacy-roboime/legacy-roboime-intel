#include "UpdateBall.h"
#include "Updater.h"
#include "Ball.h"
#include "messages_robocup_ssl_detection.pb.h"

namespace LibIntelligence
{
	struct UpdateBallImpl
	{
		UpdateBallImpl(qreal x, qreal y, qreal z)
			: x(x), y(y), z(z) {}

		qreal x, y, z;
	};
}

using namespace LibIntelligence;

UpdateBall::UpdateBall(const SSL_DetectionBall& p, double t1, double t2, int camId)
	: Update(t1,t2,camId), pimpl(new UpdateBallImpl(p.x(), p.y(), p.has_z() ? p.z() : 0.0)) {}

UpdateBall::UpdateBall(qreal x, qreal y, double t1, double t2, int camId)
	: Update(t1,t2,camId), pimpl(new UpdateBallImpl(x, y, 0.0)) {}

qreal UpdateBall::x() const
{
	return pimpl->x;
}

qreal UpdateBall::y() const
{
	return pimpl->y;
}

void UpdateBall::apply(Updater* u) {
	for(size_t k=u->ballsSize(); k>0; k--) {
		//TODO: identify which ball is which
		//if(u->ball(k-1)->i()==_i) {
			u->ball(k-1)->setX(pimpl->x);
			u->ball(k-1)->setY(pimpl->y);
			u->ball(k-1)->updateSpeed(time_capture());
		//}
	}
}