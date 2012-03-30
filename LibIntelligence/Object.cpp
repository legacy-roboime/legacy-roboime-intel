#include "Object.h"
#include <qvector.h>
#include <qlist.h>

using namespace LibIntelligence;

Object::Object(qreal x, qreal y, qreal z, qreal sx, qreal sy, qreal sz)
	: x_(x),
	y_(y),
	z_(z),
	speedX_(sx),
	speedY_(sy),
	speedZ_(sz),
    speedQueue(QQueue<QPointF>()) 
{}

Object::Object(const Object& object)
{
	this->x_ = object.x();
	this->y_ = object.y();
	this->z_ = object.z();
	this->speedX_ = object.speedX();
	this->speedY_ = object.speedY();
	this->speedZ_ = object.speedZ();
}

void Object::setX(qreal x)
{
	x_ = x;
}

qreal Object::x() const
{
	return x_;
}

void Object::setY(qreal y)
{
	y_ = y;
}

void Object::setY(qreal y, qreal time_capture)
{
	static qreal last_time_capture;

	//TODO: tirar daqui e colocar o estimador de velocidade no tracker
	qreal diffTime = time_capture - last_time_capture;
	qreal speed = (y - y_)/(diffTime);
	
	if(abs(speed) < 10000 && diffTime>0){ //margem limiar para velocidade maxima dos objetos
		speedY_ = (speedY_ + 2.*speed)/3.;
		//printf("SPEEDY: %f\n",speed);
	}
	
	y_ = y;
	last_time_capture = time_capture;
}

qreal Object::y() const
{
	return y_;
}

void Object::setZ(qreal z)
{
	z_ = z;
}

qreal Object::z() const
{
	return z_;
}

void Object::setSpeedX(qreal sx)
{
	speedX_ = sx;
}

qreal Object::speedX() const
{
	return speedX_;
}

void Object::setSpeedY(qreal sy)
{
	speedY_ = sy;
}

qreal Object::speedY() const
{
	return speedY_;
}

void Object::setSpeedZ(qreal sz)
{
	speedZ_ = sz;
}

qreal Object::speedZ() const
{
	return speedZ_;
}

void Object::setXY(qreal x, qreal y)
{
	setX(x);
	setY(y);
}

void Object::setXYZ(qreal x, qreal y, qreal z)
{
	setX(x);
	setY(y);
	setZ(z);
}

void Object::setSpeedXY(qreal sx, qreal sy)
{
	setSpeedX(sx);
	setSpeedY(sy);
}

void Object::setSpeedXYZ(qreal sx, qreal sy, qreal sz)
{
	setSpeedX(sx);
	setSpeedY(sy);
	setSpeedZ(sz);
}

Object& Object::operator=(const Object& object)
{
	this->x_ = object.x();
	this->y_ = object.y();
	this->z_ = object.z();
	this->speedX_ = object.speedX();
	this->speedY_ = object.speedY();
	this->speedZ_ = object.speedZ();
	return *this;
}


qreal Object::getLinearSpeed() {
	return linearSpeed;
}

QLineF Object::getSpeedVector() {
	return regression();
}

QLineF Object::regression() {
	
	double r[2],div,*iden;
	int n = speedQueue.count();
	double *pt = new double[20];

	for(int i=0; i<n; i++) pt[i] = speedQueue.at(i).x();
	for(int i=0; i<n; i++) pt[i+n] = speedQueue.at(i).y();

	regressao(pt,r,n);

	int x1 = this->x();
	int y1 = this->y();
	int x2 = 0;
	int y2 = r[1] * x2 + r[0];

	QLineF reta(x2,y2,x1,1);

	return reta;
	//return QLineF(0,0,1,1);
}

void Object::updateSpeed() {
	
	QPointF point(this->x(),this->y());
	speedQueue.pop_front();
	speedQueue.push_back(point);
	speedVector = regression();
	calculateLinearSpeed();
}

qreal Object::calculateLinearSpeed() {

	int n = speedQueue.count();

	qreal deltaX = speedQueue.first().x() - speedQueue.last().x();
	qreal deltaY = speedQueue.first().y() - speedQueue.last().y();

	qreal deltaT = n * 3/100; //Intervalo de tempo entre as duas medicoes

	this->setSpeedX(deltaX/deltaT);
	this->setSpeedY(deltaY/deltaT);
	linearSpeed = sqrt( deltaX*deltaX + deltaY*deltaY) / deltaT;
	return linearSpeed;
}

double Object::InnerProduct(double *x,double *y,int n)
{
	int i;
	double r=0;
	
	for (i=0; i<n; i++)		
		r=r+(x[i]*y[i]);
	
	return r;
}

void Object::regressao(double *pt,double *a,int n)//pt=[x1,...,xn,y1,...,yn] e a=[independente, termo de primeiro grau]
{
	double div,*iden;
	
	//alocando memoria
	iden = new double[n];//=malloc(n*sizeof(double));
	
	//vetor unitario
	for (int i=0; i<n; i++) iden[i]=1;
	
	//reta y= a*x+b

	div=(n*(InnerProduct(pt,pt,n))-(InnerProduct(pt,iden,n))*(InnerProduct(pt,iden,n)));

	a[0]=((InnerProduct(pt,pt,n))*(InnerProduct(&pt[n],iden,n))-(InnerProduct(&pt[n],pt,n))*(InnerProduct(pt,iden,n)))/div;
	
	a[1]=((InnerProduct(&pt[n],pt,n)*n)-(InnerProduct(pt,iden,n))*(InnerProduct(&pt[n],iden,n)))/div;
	
	//printf("\nreta: y = (%f)*x + (%f)\n",a[1],a[0]);
}
