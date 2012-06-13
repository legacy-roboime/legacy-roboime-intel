#include "Goal.h"

using namespace LibIntelligence;

Goal::Goal(qreal x, qreal y, qreal w, qreal d, qreal ww)
	: QLineF(),
	Object(x, y),
	width_(w),
	depth_(d),
	wallWidth_(ww)
{
	updatePoints();
}

Goal::Goal(const Goal& goal)
	: QLineF(goal),
	Object(goal),
	width_(goal.width()),
	depth_(goal.depth()),
	wallWidth_(goal.wallWidth()),
	penaltyMark_(goal.penaltyMark()),
	penaltyLine_(goal.penaltyLine()) {}

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
	setP1(QPointF(x(), y() - width_ / 2));
	setP2(QPointF(x(), y() + width_ / 2));
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

QPointF Goal::penaltyMark() const{
	return penaltyMark_;
}

void Goal::setPenaltyLine(qreal x){
	penaltyLine_ = x;
}

qreal Goal::penaltyLine() const{
	return penaltyLine_;
}
