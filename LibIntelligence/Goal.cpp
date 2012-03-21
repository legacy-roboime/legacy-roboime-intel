#include "Goal.h"

using namespace LibIntelligence;

Goal::Goal(QObject* p, qreal x, qreal y, qreal w, qreal d, qreal ww)
	: QObject(p),
	Object(x, y),
	width_(w),
	depth_(d),
	wallWidth_(ww) {}

void Goal::setWidth(qreal w)
{
	width_ = w;
}

Goal::Goal(const Goal& goal)
	: QObject(goal.parent()),
	Object(goal),
	width_(goal.width()),
	depth_(goal.depth()),
	wallWidth_(goal.wallWidth()),
	penaltyMark_(goal.penaltyMark()),
	penaltyLine_(goal.penaltyLine()) {}

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

Goal& Goal::operator=(const Goal& goal)
{
	((Object*)this)->operator=(goal);
	this->width_ = goal.width();
	this->depth_ = goal.depth();
	this->wallWidth_ = goal.wallWidth();
	this->penaltyLine_ = goal.penaltyLine();
	this->penaltyMark_ = goal.penaltyMark();
	return *this;
}