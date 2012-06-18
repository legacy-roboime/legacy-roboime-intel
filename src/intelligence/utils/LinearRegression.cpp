#include "LinearRegression.h"
#include <QVector>
#include "geomutils.h"

using namespace LibIntelligence;

LinearRegression::LinearRegression(Point *p, long size, unsigned char max_size_points)
	: xPoints(new QVector<float>()), yPoints(new QVector<float>())
{
	MAX_SIZE_POINTS = max_size_points;

	for(int i = 0; i < MAX_SIZE_POINTS; i++){
		xPoints->push_back(0.);
		yPoints->push_back(0.);
	}
	long i;
	a = b = sumX = sumY = sumXsquared = sumYsquared = sumXY = 0.0;
	n = 0L;

	if (size > 0L) // if size greater than zero there are data arrays
		for (n = 0, i = 0L; i < size; i++)
			addPoint(p[i]);
}

LinearRegression::LinearRegression(const LinearRegression& linearRegression)
{
	n = linearRegression.n;
	sumX = linearRegression.sumX;
	sumY = linearRegression.sumY;
	sumXsquared = linearRegression.sumXsquared;
	sumYsquared = linearRegression.sumYsquared;
	sumXY = linearRegression.sumXY;
	a = linearRegression.a;
	b = linearRegression.b;
	coefC = linearRegression.coefC;
	coefD = linearRegression.coefD;
	stdError = linearRegression.stdError;
	MAX_SIZE_POINTS = linearRegression.MAX_SIZE_POINTS;
	xPoints = new QVector<float>(*linearRegression.xPoints);
	yPoints = new QVector<float>(*linearRegression.yPoints);
}

LinearRegression::~LinearRegression()
{
	if(yPoints)
		delete yPoints;
	if(xPoints)
		delete xPoints;
}
//modificado
void LinearRegression::addXY(const double& x, const double& y)
{
	n++;
	int i = n % MAX_SIZE_POINTS;
	double xOld = xPoints->at(i);
	double yOld = yPoints->at(i);
	xPoints->replace(n % MAX_SIZE_POINTS,x);
	yPoints->replace(n % MAX_SIZE_POINTS,y);
	sumX += x - xOld;
	sumY += y - yOld;
	sumXsquared += x * x - xOld*xOld;
	sumYsquared += y * y - yOld*yOld;
	sumXY += x * y - xOld*yOld;
	Calculate();

	//for(int i = 0; i < MAX_SIZE_POINTS; i++ )	cout << xPoints->at(i) << ";"  << yPoints->at(i) << endl;

	//cout << "-----------------------------------------------------" << endl;
}

void LinearRegression::Calculate()
{
	if (haveData())
	{
		if (fabs( double(n) * sumXsquared - sumX * sumX) > DBL_EPSILON)
		{
			b = ( double(n) * sumXY - sumY * sumX) /
				( double(n) * sumXsquared - sumX * sumX);
			a = (sumY - b * sumX) / double(n);

			double sx = b * ( sumXY - sumX * sumY / double(n) );
			double sy2 = sumYsquared - sumY * sumY / double(n);
			double sy = sy2 - sx;

			coefD = sx / sy2;
			coefC = sqrt(coefD);
			stdError = sqrt(sy / double(n - 2));
		}
		else
		{
			a = b = coefD = coefC = stdError = 0.0;
		}
	}
}

Point* LinearRegression::getLastPointInserted()
{
	if(n > 0) 
		return new Point(xPoints->at(n % MAX_SIZE_POINTS), yPoints->at(n % MAX_SIZE_POINTS));
	else 
		return new Point(0,0);
}