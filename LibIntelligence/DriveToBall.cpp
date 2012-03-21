#include "DriveToBall.h"
#include "Robot.h"
#include "Stage.h"
#include "Goal.h"
#include "Ball.h"
#include <QLineF>

#define M_PI	3.1415926535897932

//valor carteado para a distancia central do robo para o corte da frente
#define CART	82.6

using namespace LibIntelligence;
using namespace LibIntelligence::Skills;

DriveToBall::DriveToBall(QObject* parent, Robot* slave, const Object* refLookPoint, qreal speed)
	: DriveTo(parent, slave),
	refLookPoint_(refLookPoint)
{
	this->speed = speed;
	this->threshold = CART;
	//lookPoint = Object(*refLookPoint_);
}


DriveToBall::~DriveToBall(void)
{
}

//Goto em coordenadas polares
void DriveToBall::step()
{
	Stage* stage = this->stage();
	Ball* ball = stage->ball();
	lookPoint = Object(*refLookPoint_);

	QLineF target = QLineF(lookPoint.x(), -lookPoint.y(), ball->x(), -ball->y()); //O referencial do y na reta é invertido do comum (x positivo direita, y positivo cima)

	this->bPoint.setX(ball->x());
	this->bPoint.setY(ball->y());
	this->bAngle = target.angle() * M_PI / 180.;
	DriveTo::step();
}

void DriveToBall::setRefLookPoint(const Object* refLookPoint)
{
	refLookPoint_ = refLookPoint;
}

bool DriveToBall::busy()
{
	//TODO: adicionar a condicao do sensor da bola estar ativado
	return DriveTo::busy();
}