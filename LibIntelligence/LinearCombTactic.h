#pragma once
#ifndef LINEAR_COMB_TATIC_H
#define LINEAR_COMB_TATIC_H

#include "LibIntelligence.h"
#include "Tactic.h"
#include "LinearCombMotion.h"


namespace LibIntelligence
{
	namespace Tactics
	{
		class LinearCombTactic : public Tactic
		{
			Q_OBJECT

		public:
			LinearCombTactic(QObject* parent, Robot* slave, qreal *coefs=0);
			void setCoefs( qreal *coefs );
			void step();

		protected:
			Skills::LinearCombMotion* lin_comb_motion_;
		};
	}	
}

#endif // LINEAR_COMB_TATIC_H