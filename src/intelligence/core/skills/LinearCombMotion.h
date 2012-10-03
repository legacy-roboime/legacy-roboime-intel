#pragma once
#ifndef LINEAR_COMB_MOTION_H
#define LINEAR_COMB_MOTION_H

#include "LibIntelligence.h"
#include "SteerToPoint.h" 

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
			SteerToBall  move_; //ARQUITETURA NAO BOA, POIS AO DAR SETROBOT EM LINEARCOMBMOTION TERA QUE DAR SET ROBOT NESSA SKILL
			Object *obj_array_[13];
	  };		 


	}

}
#endif
