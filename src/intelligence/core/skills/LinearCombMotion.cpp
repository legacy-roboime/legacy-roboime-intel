#include  "LinearCombMotion.h"

using namespace LibIntelligence;
using namespace LibIntelligence::Skills;


LinearCombMotion::LinearCombMotion(QObject* p, Robot* r, qreal *coefs ): Skill(p, r), 
  move_(p,r,0,0)

{ 
 obj_array_[0] =  (Object*) robot()->team()->goal();
 obj_array_[1] =  (Object*) robot()->enemyTeam()->goal();
 obj_array_[2] =  stage()->ball();

 obj_array_[3] =  robot()->enemyTeam()->at(0);
 obj_array_[4] =  robot()->enemyTeam()->at(1);
 obj_array_[5] =  robot()->enemyTeam()->at(2);
 obj_array_[6] =  robot()->enemyTeam()->at(3);
 obj_array_[7] =  robot()->enemyTeam()->at(4);

 obj_array_[8] =  robot()->team()->at(0);
 obj_array_[9] =  robot()->team()->at(1);
 obj_array_[10] =  robot()->team()->at(2);
 obj_array_[11] =  robot()->team()->at(3);
 obj_array_[12] =  robot()->team()->at(4); 

 this->setCoefs(coefs);
}


void LinearCombMotion::setCoefs( qreal *coefs )
{ 
 if( coefs ){
    for( int i=0; i < 15; i++ )	 
      coefs_[i] = coefs[i];
 }
}


void LinearCombMotion::step()
{
 qreal sx = 0;
 qreal sy = 0;

 sx += coefs_[0]*(obj_array_[0]->y() - robot()->x() );  
 sy += coefs_[0]*(obj_array_[0]->x() - robot()->y()  );   
 sx += coefs_[1]*(obj_array_[1]->y() - robot()->x() );  
 sy += coefs_[1]*(obj_array_[1]->x() - robot()->y()  );   

 for( int i = 2; i < 13; i++ ){
   sx += coefs_[i]*(obj_array_[i]->x() - robot()->x() );  
   sy += coefs_[i]*(obj_array_[i]->y() - robot()->y()  );   
 } 

 sx += coefs_[13];
 sy += coefs_[14];
 
 qreal l = sqrt(sx*sx + sy*sy);
 move_.setSpeeds(1000*sx/l , 1000*sy/l );
 move_.step();

}

bool LinearCombMotion::busy()
{
	return true;
}