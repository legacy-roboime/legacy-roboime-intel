#include "Ball.h"

using namespace LibIntelligence;

Ball::Ball(qreal r)
	: Object(),
	radius_(r)
{}

Ball::Ball(const Ball& ball)
	: Object(ball),
	radius_(ball.radius_)
{}

void Ball::setRadius(qreal d)
{
	radius_ = d;
}

qreal Ball::radius() const
{
	return radius_;
}
