#ifndef GEOMUTILS_H
#define GEOMUTILS_H
#include <QPointF>
#include <QLineF>
#include <QVector2D>

namespace LibIntelligence
{
	typedef ::QPointF Point;

	typedef ::QVector2D Vector;

	class Line : public ::QLineF
	{
	public:
		Line() : QLineF() {}
		Line(const Point & p1, const Point & p2) : QLineF(p1, p2) {}
		Line(qreal x1, qreal y1, qreal x2, qreal y2) : QLineF(x1, y1, x2, y2) {}
		Line(const Line & line ) : QLineF(line) {}
		Line(const QLineF & line ) : QLineF(line) {}

		static Line fromPolar(qreal length, qreal angle) {
			return QLineF::fromPolar(length, 360 - length);
		}

		qreal angle() const {
			return 360 - QLineF::angle();
		}

		void setAngle(qreal angle) {
			QLineF::setAngle(360 - angle);
		}

		qreal angleTo(const Line &line) const {
			return 360 - QLineF::angleTo(line);
		}

		qreal distanceTo(const Point &point) const {
			//the following line starts at point and is normal to line
			Line normalLine(Line::fromPolar(1.0, angle() + 90).translated(point));
			//get the intersection point
			Point intersection;
			normalLine.intersect(*this, &intersection);
			//now we go from point to the intersection point of line
			normalLine.setP2(intersection);
			//voilà the distance is the length now
			return normalLine.length();
		}
	};
}

#endif // GEOMUTILS_H

