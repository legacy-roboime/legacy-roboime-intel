#include "Sampler.h"
#include <QTime>

using namespace LibIntelligence;

Sampler::Sampler(QObject* parent)
{
}

Sampler::~Sampler(void)
{
}

qreal Sampler::randFloat()
{
	//QTime time = QTime::currentTime();
	//qsrand((uint)time.msec());
	return ((qreal)qrand())/((qreal)RAND_MAX);
}

long Sampler::randInt(int low, int high)
{
	//QTime time = QTime::currentTime();
	//qsrand((uint)time.msec());
	// Random number between low and high
	return qrand() % ((high + 1) - low) + low;
}

qreal Sampler::sampledPowerDribble(qreal minPower, qreal maxPower)
{
	return (randFloat() * (maxPower - minPower) + minPower);
}

qreal Sampler::sampledPowerKick(qreal minPower, qreal maxPower)
{
	return (randFloat() * (maxPower - minPower) + minPower);
}

Point Sampler::sampledPointKick(Line target)
{
	return target.pointAt(randFloat());
}

Point Sampler::sampledPointKickH(Line target)
{
	//TODO: Colocar uma heuristica para diminuir o espaço de busca do ponto melhor para chutar
	return target.pointAt(randFloat());
}

bool Sampler::sampledUniformDist(qreal min, qreal max)
{
	qreal rand = randFloat();
	return (rand <= max) && (rand >= min);
}