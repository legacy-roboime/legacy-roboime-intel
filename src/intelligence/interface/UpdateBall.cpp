#include "UpdateBall.h"
#include "Updater.h"
#include "Ball.h"
#include "messages_robocup_ssl_detection.pb.h"
#include "config.h"
#include "Stage.h"

namespace LibIntelligence
{
	struct UpdateBallImpl
	{
		UpdateBallImpl(qreal z)
			: z(z) {}
		qreal z;
	};
}

using namespace LibIntelligence;

UpdateBall::UpdateBall(const SSL_DetectionBall& p, double t1, double t2, int camId)
	: Update(t1, t2, camId),
	Point(p.x(), p.y()),
	pimpl(new UpdateBallImpl(p.has_z() ? p.z() : 0.0)) {}

UpdateBall::UpdateBall(qreal x, qreal y, double t1, double t2, int camId)
	: Update(t1, t2, camId),
	Point(x, y),
	pimpl(new UpdateBallImpl(0.0))
{}

UpdateBall::~UpdateBall()
{
	delete pimpl;
}

void UpdateBall::apply(Updater* u) {
	for(size_t k=u->ballsSize(); k>0; k--) {
		//TODO: identify which ball is which
		//if(u->ball(k-1)->i()==_i) {
			Ball* ball = u->ball(k-1);
			ball->updatePosition(*this);
			ball->updateSpeed(time_capture());

#ifdef TRANSFORMADA_CAMPO
			float A = 2*u->stage(0)->fieldWidth();
			float x = ball->x();
			float y = ball->y();
			ball->setX(y);
			ball->setY((-1.)*x + A/4.);
#endif
		//}
	}
}