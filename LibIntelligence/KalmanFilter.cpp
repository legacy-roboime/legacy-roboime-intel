#include "KalmanFilter.h"
#include "Update.h"
#include "UpdateBall.h"
#include "UpdateRobot.h"

using namespace LibIntelligence;
using namespace LibIntelligence::Filters;

KalmanFilter::KalmanFilter(filterobject type, qint8 id)
	:type_(type), id_(id)
{
	setDim(6, 0, 3, 3, 3);
	dt = 0.0166666667;
	const qreal _P0[] ={1, 0, 0, 0, 0, 0,
						0, 1, 0, 0, 0, 0,
						0, 0, 1, 0, 0, 0,
						0, 0, 0, 1, 0, 0,
						0, 0, 0, 0, 1, 0,
						0, 0, 0, 0, 0, 1};
	P0 = Matrix(6, 6, _P0);
	//delete(_P0);
}

void KalmanFilter::next_step()
{
	
	UpdateRobot* ur = dynamic_cast<UpdateRobot*>(current_);
	UpdateBall* ub = dynamic_cast<UpdateBall*>(current_);	
	
	if(ur!=NULL)
	{
		if(virgin)
		{	
			Vector x_ = Vector(6);
			x_(1) = ur->x();
			x_(2) = 0;
			x_(3) = ur->y();
			x_(4) = 0;
			x_(5) = ur->theta();
			x_(6) = 0;
			this->init(x_, P0);
		}
		else 
		{
			Vector x_ = Vector(6);
			Vector u_ = Vector(1);
			x_(1) = ur->x();
			x_(2) = 0;
			x_(3) = ur->y();
			x_(4) = 0;
			x_(5) = ur->theta();
			x_(6) = 0;
			this->step(u_, x_);
		}
		Vector _x = this->getX();
		estimate_ = new UpdateRobot(ur->color(), ur->patternId(), _x(1), _x(3), _x(5),  ur->time_sent(), ur->time_capture(), ur->camera_id());
	}
	else
	if(ub!=NULL)
	{
		if(virgin)
		{	
			Vector x_ = Vector(6);
			x_(1) = ub->x();
			x_(2) = 0;
			x_(3) = ub->y();
			x_(4) = 0;
			x_(5) = 0;
			x_(6) = 0;
			this->init(x_, P0);
		}
		else 
		{
			Vector x_ = Vector(6);
			Vector u_ = Vector(1);
			x_(1) = ub->x();
			x_(2) = 0;
			x_(3) = ub->y();
			x_(4) = 0;
			x_(5) = 0;
			x_(6) = 0;
			this->step(u_, x_);
		}
		Vector _x = this->getX();
		estimate_ = new UpdateBall(_x(1), _x(3), ub->time_sent(), ub->time_capture(), ub->camera_id());
		//dynamic_cast<UpdateBall*>(estimate_)->setSpeedX(_x(2));
		//dynamic_cast<UpdateBall*>(estimate_)->setSpeedY(_x(4));
	}
}

qint8 KalmanFilter::id()
{
	return id_;
}

void KalmanFilter::set_current(Update* p)
{
	current_ = p; return;
}

Update* KalmanFilter::current() 
{
	return current_;
}

Update* KalmanFilter::estimate()
{
	return estimate_;
}

filterobject KalmanFilter::type()
{
	return type_;
}

void KalmanFilter::makeProcess()
{
	Vector x_(x.size());
	x_(1) = x(1) + x(2)*dt;
	x_(2) = x(2);
	x_(3) = x(3) + x(4)*dt;
	x_(4) = x(4);
	x_(5) = x(5) + x(6)*dt;
	x_(6) = x(6);
	x.swap(x_);
}

void KalmanFilter::makeMeasure()
{
	z(1)=x(1);
	z(2)=x(3);
	z(3)=x(5);
}

void KalmanFilter::makeA()
{
	A(1,1) = 1;
	A(1,2) = dt;
	A(1,3) = 0;
	A(1,4) = 0;
	A(1,5) = 0;
	A(1,6) = 0;

	A(2,1) = 0;
	A(2,2) = 1;
	A(2,3) = 0;
	A(2,4) = 0;
	A(2,5) = 0;
	A(2,6) = 0;

	A(3,1) = 0;
	A(3,2) = 0;
	A(3,3) = 1;
	A(3,4) = dt;
	A(3,5) = 0;
	A(3,6) = 0;

	A(4,1) = 0;
	A(4,2) = 0;
	A(4,3) = 0;
	A(4,4) = 1;
	A(4,5) = 0;
	A(4,6) = 0;
	
	A(5,1) = 0;
	A(5,2) = 0;
	A(5,3) = 0;
	A(5,4) = 0;
	A(5,5) = 1;
	A(5,6) = dt;
	
	A(6,1) = 0;
	A(6,2) = 0;
	A(6,3) = 0;
	A(6,4) = 0;
	A(6,5) = 0;
	A(6,6) = 1;
}

void KalmanFilter::makeH()
{
	H(1,1) = 1;
	H(1,2) = 0;
	H(1,3) = 0;
	H(1,4) = 0;
	H(1,5) = 0;
	H(1,6) = 0;

	H(2,1) = 0;
	H(2,2) = 0;
	H(2,3) = 1;
	H(2,4) = 0;
	H(2,5) = 0;
	H(2,6) = 0;

	H(3,1) = 0;
	H(3,2) = 0;
	H(3,3) = 0;
	H(3,4) = 0;
	H(3,5) = 1;
	H(3,6) = 0;
}

void KalmanFilter::makeV()
{
	V(1,1) = 1;
	V(1,2) = 0;
	V(1,3) = 0;

	V(2,1) = 0;
	V(2,2) = 1;
	V(2,3) = 0;

	V(3,1) = 0;
	V(3,2) = 0;
	V(3,3) = 1;
}

void KalmanFilter::makeW()
{
	W(1,1) = dt*dt/2;
	W(1,2) = 0;
	W(1,3) = 0;
	
	W(2,1) = dt;
	W(2,2) = 0;
	W(2,3) = 0;

	W(3,1) = 0;
	W(3,2) = dt*dt/2;
	W(3,3) = 0;

	W(4,1) = 0;
	W(4,2) = dt;
	W(4,3) = 0;
	
	W(5,1) = 0;
	W(5,2) = 0;
	W(5,3) = dt*dt/2;
	
	W(6,1) = 0;
	W(6,2) = 0;
	W(6,3) = dt;
}

void KalmanFilter::makeQ()
{
	Q(1,1) = cov_q;
	Q(1,2) = 0;
	Q(1,3) = 0;

	Q(2,1) = 0;
	Q(2,2) = cov_q;
	Q(2,3) = 0;

	Q(3,1) = 0;
	Q(3,2) = 0;
	Q(3,3) = cov_q;	
}

void KalmanFilter::makeR()
{
	Q(1,1) = cov_r;
	Q(1,2) = 0;
	Q(1,3) = 0;

	Q(2,1) = 0;
	Q(2,2) = cov_r;
	Q(2,3) = 0;

	Q(3,1) = 0;
	Q(3,2) = 0;
	Q(3,3) = cov_r;	
}