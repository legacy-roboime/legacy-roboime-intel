#include "Ball.h"

using namespace LibIntelligence;

Ball::Ball(QObject* p, qreal r)
	: QObject(p),
	Object(),
	radius_(r) {}

Ball::Ball(const Ball& ball):
	QObject(ball.parent()),
	Object(ball)
{
	this->radius_ = ball.radius();
}

void Ball::setRadius(qreal d)
{
	radius_ = d;
}

qreal Ball::radius() const
{
	return radius_;
}

Ball& Ball::operator=(const Ball& ball)
{
	((Object*)this)->operator=(ball);
	radius_ = ball.radius();
	return *this;
}