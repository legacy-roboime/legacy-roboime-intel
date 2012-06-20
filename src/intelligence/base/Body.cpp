#include "Body.h"

using namespace LibIntelligence;

Body::Body(const Body& b)
	: Component(b.parent()),
	height_(b.height()),
	radius_(b.radius()),
	cut_(b.cut())
{}

Body::Body(QObject* p, qreal h, qreal r, qreal c)
	: Component(p),
	height_(h),
	radius_(r),
	cut_(c)
{}

void Body::setHeight(qreal h)
{
	height_ = h;
}

qreal Body::height() const
{
	return height_;
}

void Body::setRadius(qreal r)
{
	radius_ = r;
}

qreal Body::radius() const
{
	return radius_;
}

void Body::setCut(qreal c)
{
	cut_ = c;
}

qreal Body::cut() const
{
	return cut_;
}