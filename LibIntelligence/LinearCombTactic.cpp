#include "LinearCombTactic.h"


using namespace LibIntelligence;
using namespace Tactics;
using namespace Skills;

LinearCombTactic::LinearCombTactic(QObject* p, Robot* r, qreal *coefs):Tactic(p,r)
	,lin_comb_motion_(new LinearCombMotion(this,r,coefs))
{}


void LinearCombTactic::setCoefs( qreal *coefs )
{
 lin_comb_motion_->setCoefs(coefs);
}


void LinearCombTactic::step()
{
 lin_comb_motion_->step();
}