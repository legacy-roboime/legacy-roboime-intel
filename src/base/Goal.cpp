#include "Goal.h"
#include "Sampler.h"

using namespace LibIntelligence;

Goal::Goal(qreal x, qreal y, qreal w, qreal d, qreal ww)
	: Line(),
	Object(x, y),
	width_(w),
	depth_(d),
	wallWidth_(ww),
	penaltyLine_(0.0)
{
	updatePoints();
}

Goal::Goal(const Goal& goal)
	: Line(goal),
	Object(goal),
	width_(goal.width()),
	depth_(goal.depth()),
	wallWidth_(goal.wallWidth()),
	penaltyMark_(goal.penaltyMark()),
	penaltyLine_(goal.penaltyLine()) {}

Goal::Goal(const Goal* goal)
	: Line(*goal),
	Object(*goal),
	width_(goal->width()),
	depth_(goal->depth()),
	wallWidth_(goal->wallWidth()),
	penaltyMark_(goal->penaltyMark()),
	penaltyLine_(goal->penaltyLine()) {}

Point Goal::randomPoint() const
{
	return pointAt(Sampler::randFloat());
}

void Goal::setX(qreal x)
{
	Object::setX(x);
	updatePoints();
}

void Goal::setY(qreal y)
{
	Object::setY(y);
	updatePoints();
}

void Goal::updatePoints()
{
	setP1(Point(x(), y() - width_ / 2));
	setP2(Point(x(), y() + width_ / 2));
}

void Goal::setWidth(qreal w)
{
	width_ = w;
}

void Goal::setDepth(qreal d)
{
	depth_ = d;
}

void Goal::setWallWidth(qreal w) {
	wallWidth_ = w;
}

qreal Goal::width() const
{
	return width_;
}

qreal Goal::depth() const
{
	return depth_;
}

qreal Goal::wallWidth() const
{
	return wallWidth_;
}

void Goal::setPenaltyMark(qreal penaltyMarkX, qreal penaltyMarkY){
	this->penaltyMark_.setX(penaltyMarkX);
	this->penaltyMark_.setY(penaltyMarkY);
}

Point Goal::penaltyMark() const{
	return penaltyMark_;
}

void Goal::setPenaltyLine(qreal x){
	penaltyLine_ = x;
}

qreal Goal::penaltyLine() const{
	return penaltyLine_;
}
