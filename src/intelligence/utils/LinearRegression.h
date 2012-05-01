#pragma once

//http://howtobyexamples.blogspot.com.br/2011/10/c-linear-regression-source-code.html

#ifndef LINEARREGRESSION_H
#define LINEARREGRESSION_H

#include <qpoint.h>
#include "qvector.h"
#include <iostream>

using namespace std;

class LinearRegression
{
public:
	// Constructor using an array of QPoint objects
	// This is also the default constructor
	LinearRegression(QPointF *p = 0, long size = 0, unsigned char max_size_points = 50); 
	LinearRegression(const LinearRegression& linearRegression); 
	~LinearRegression();

	virtual void addXY(const double& x, const double& y);
	//void clear() { sumX = sumY = sumXsquared = sumYsquared = sumXY = n = 0; }
	void addPoint(const QPointF& p) { addXY(p.x(), p.y()); }

	// Must have at least 3 points to calculate
	// standard error of estimate. Do we have enough data?
	int haveData() const { return (n > 2 ? 1 : 0); }
	long items() const { return n; }

	virtual double getA() const { return a; }
	virtual double getB() const { return b; }

	double getCoefDeterm() const { return coefD; }
	double getCoefCorrel() const { return coefC; }
	double getStdErrorEst() const { return stdError; }
	virtual double estimateY(double x) const { return (a + b * x); }
	QPointF* getLastPointInserted();


protected:
	long n;       // number of data points input so far
	double sumX, sumY; // sums of x and y
	QVector<float>* xPoints;
	QVector<float>* yPoints;
	double sumXsquared, // sum of x squares
		sumYsquared; // sum y squares
	double sumXY;    // sum of x*y

	double a, b;    // coefficients of f(x) = a + b*x
	double coefD,    // coefficient of determination
		coefC,    // coefficient of correlation
		stdError;  // standard error of estimate
	void Calculate();  // calculate coefficients

private:
	unsigned char MAX_SIZE_POINTS;
};

#endif LINEARREGRESSION_H