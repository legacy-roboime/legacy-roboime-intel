#include "GotoOld.h"
#include "Robot.h"
#include "PID.h"
#include "geomutils.h"

#define M_2PI	6.2831853071795865
#define M_PI	3.1415926535897932
#define __q(x)	((x) > M_PI ? (x) - M_2PI : -(x) > M_PI ? (x) + M_2PI : (x))

using namespace LibIntelligence;
using namespace Skills;

GotoOld::GotoOld(QObject *p, Robot* s, qreal x, qreal y)
	: Skill(p, s), x(x), y(y) {}

void GotoOld::step()
{
	static QVector<qreal> speed(2);
	//diferença entre angulo destino-robo e orientação do robo
	Line robodest(robot()->x(), robot()->y(), x, y);
	qreal angdif = __q(robodest.angle() * M_PI / 180 - robot()->orientation());
	//controle PID do angulo
	static CONTROLLER_S cona(10.0, 0.0, 0.0, 127.0, 10.0);
	cona.entrada = 0.0;
	cona.realimentacao = angdif;
	pidService(cona);
	qreal outa = cona.saida;
	//controle PID da velocidade
	static CONTROLLER_S conv(10.0, 0.0, 0.0, 127.0, 10.0);
	conv.entrada = 0.0;
	conv.saida = robodest.length();
	pidService(conv);
	qreal outv = conv.saida;
	//saida
	speed[0] = outv + outa;
	speed[1] = outv - outa;
	
	robot()->setCommandWheels(speed);
}