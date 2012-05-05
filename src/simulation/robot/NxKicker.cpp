#include "NxKicker.h"
#include "Simulation.h"

NxKicker::NxKicker(Simulation* simulation)
{
	this->simulation = simulation;
}

NxKicker::~NxKicker(void)
{
	if(kickerShapeDesc)
		delete kickerShapeDesc;
}

void NxKicker::controlKicker( float kickerSpeed, NxRobot* robot )
{
	//TODO: implementar o controlador
	if(robot && kickerSpeed>0){
		NxBall* ball = simulation->gScenes[robot->indexScene]->ball;
		NxShape* const* ballShapes = ball->ball->getShapes();

		NxBoxShapeDesc* kickerShapeDesc = (NxBoxShapeDesc*)this->kickerShapeDesc;

		NxReal angle = robot->getAngle2DFromVehicle();
		NxVec3 dir = NxVec3(cos(angle), sin(angle), 0); 
		
		kickerShapeDesc->localPose.t.x *= dir.x;
		kickerShapeDesc->localPose.t.y *= dir.y;

		NxBox box = NxBox(kickerShapeDesc->localPose.t + robot->getActor()->getGlobalPosition(), kickerShapeDesc->dimensions,  robot->getActor()->getGlobalOrientation());
		
		NxSphere sphere = NxSphere(ball->ball->getGlobalPosition(), ballShapes[0]->isSphere()->getRadius());

		float min_dist;

		NxVec3 normal;

		NxUtilLib* gUtilLib = NxGetUtilLib();
		if(gUtilLib->NxSweepBoxSphere(box, sphere, -dir, 10+25.2, min_dist, normal)){ // Nao sei pq tem q ser o vetor -dir pra da certo pela logica eh NxVec3(cos(angle), sin(angle), 0)
			ball->ball->addForce(/*NxVec3(kickerSpeed*cos(angle)*150., kickerSpeed*sin(angle)*150., 0)*/(-normal)*kickerSpeed*300., NX_IMPULSE); //TODO: VERIFICAR A FORÇA OU IMPULSO
		}
	}
}
