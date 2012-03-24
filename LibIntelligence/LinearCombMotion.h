#pragma once
#ifndef LINEAR_COMB_MOTION_H
#define LINEAR_COMB_MOTION_H

#include "LibIntelligence.h"
#include "Robot.h"
#include "Stage.h"
#include "Team.h"
#include "Ball.h"
#include "Skill.h"
#include "Move.h"
#include "SteerToBall.h" 



namespace LibIntelligence
{ 
	namespace Skills
	{
      class LinearCombMotion : public Skill
	  {
        Q_OBJECT

        public:
			LinearCombMotion(QObject* parent, Robot* slave, qreal* coefs = 0);
			void setCoefs( qreal* coefs );
		 	void step();
			bool busy();

	    private:
			qreal coefs_[15];
			SteerToBall  move_;
			Object *obj_array_[13];
	  };		 


	}

}
#endif
