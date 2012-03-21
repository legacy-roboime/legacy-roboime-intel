#include "KalmanFilter.h"
#include "KalmanFilters.h"
#include "Filter.h"
#include "Update.h"
#include "UpdateBall.h"
#include "UpdateRobot.h"
#include "Updates.h"

using namespace LibIntelligence;
using namespace LibIntelligence::Filters;


KalmanFilters::KalmanFilters(QObject *p, Updates* u) 
	: Filter(p, u){}

KalmanFilters::~KalmanFilters()
{

}

//Chama cada um dos filtros(um pra cada robo/bola)
void KalmanFilters::step()
{
	while(!(updates_from->isEmpty()))
	{
		//cast pra ver se é bola ou robô
		Update* u = updates_from->dequeue();
		UpdateBall* ub = dynamic_cast<UpdateBall*>(u);
		UpdateRobot* ur = dynamic_cast<UpdateRobot*>(u);
		bool found = false;

		if(ub!=NULL)
		{
			for(int i=0; i<filters.length(); i++)
			{	
				if(filters[i]->type() == BALL)
				{
					found = true;
					filters[i]->set_current(u);
					filters[i]->next_step();
					updates_.enqueue(filters[i]->estimate());
					emit filteredUpdate();
				}
			}
			if(!found)
				filters.append(new KalmanFilter(BALL, -1));		

		}
		else if(ur!=NULL)
		{
			for(int i=0; i<filters.length(); i++)
			{	
				if(((ur->color() == BLUE && filters[i]->type() == BLUE) || (ur->color() == YELLOW && filters[i]->type() == YELLOW)) && ur->patternId() == filters[i]->id() )
				{
					found = true;
					filters[i]->set_current(u);
					filters[i]->next_step();
					updates_.enqueue(filters[i]->estimate());
					emit filteredUpdate();
				}
			}
			if(!found)
				if(ur->color() == BLUE)
					filters.append(new KalmanFilter(BLUE, ur->patternId()));
				else
					filters.append(new KalmanFilter(YELLOW, ur->patternId()));
		}
		else
			updates_.enqueue(u);
	}
	
}

