#include <cstdio>
#include <map>

#if defined(__APPLE__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "NxVehicle.h"
#include "NxWheel.h"
#include "NxAllRobots.h"
#include "Simulation.h"

#include <NxActorDesc.h>
#include <NxBoxShape.h>
#include <NxConvexShape.h>
#include <NxConvexShapeDesc.h>
#include <NxConvexMesh.h>

#include "common.h"

#define TRAIL_FREQUENCY 0.025f

NxVehicle::NxVehicle() : /*_steeringWheelState(0),*/ _nxScene(NULL), _carMaterial(NULL) 
{ 
	//memset(_trailBuffer, 0, sizeof(NxVec3) * NUM_TRAIL_POINTS);
	_nextTrailSlot = 0;
	_lastTrailTime = 0.0f;
}

NxVehicle::~NxVehicle()
{
	if (_carMaterial)
		_nxScene->scene->releaseMaterial(*_carMaterial);

	if (_actor)
		_nxScene->scene->releaseActor(*_actor);
	for(NxU32 i = 0; i < _wheels.size(); i++)
	{
		if(_wheels[i])
		{
			delete _wheels[i];
			_wheels[i] = NULL;
		}
	}
}

NxVehicle* NxVehicle::_createVehicle(NxScene1* scene, NxVehicleDesc* vehicleDesc)
{
	//printf("Create Vehicle\n");

	//Criando veiculo
	if(vehicleDesc == NULL)
		return NULL;

	NxRobot* vehicle = new NxRobot(scene->getSimulation()); 
	vehicle->userData = vehicleDesc->userData;

	if(!vehicleDesc->isValid())
	{
		printf("Vehicle Desc not valid!!\n");
		return NULL;
	}

	vehicle->_nxScene = scene;

	if(vehicle->_carMaterial == NULL)
	{
		NxMaterialDesc robotMaterialDesc;
		robotMaterialDesc.dynamicFriction = 0.4f;
		robotMaterialDesc.staticFriction = 0.4f;
		robotMaterialDesc.restitution = 0;
		robotMaterialDesc.frictionCombineMode = NX_CM_MULTIPLY;
		vehicle->_carMaterial = scene->scene->createMaterial(robotMaterialDesc);
	}

	vehicle->_actor = vehicleDesc->actor;//scene->createActor(actorDesc);
	if(vehicle->_actor == NULL)
	{
		delete vehicle;
		return NULL;
	}
	vehicle->_actor->userData = vehicle;

	//vehicle->_transmissionEfficiency	= vehicleDesc->transmissionEfficiency;
	//vehicle->_differentialRatio			= vehicleDesc->differentialRatio;
	//vehicle->_maxVelocity				= vehicleDesc->maxVelocity;
	vehicle->_cameraDistance			= vehicleDesc->cameraDistance;
	vehicle->_actor->setCMassOffsetLocalPosition(vehicleDesc->centerOfMass);

	//Criando motores
	for(int i=0; i<vehicleDesc->motorsDesc.size(); i++)
	{
		if(vehicleDesc->motorsDesc[i] != NULL)
		{
			NxVehicleMotor* motor;
			motor = NxVehicleMotor::createMotor(*vehicleDesc->motorsDesc[i]);
			if(motor == NULL)
			{
				delete vehicle;
				return NULL;
			}
			else
				vehicle->_vehicleMotors.push_back(motor);
			//vehicle->_motorForce = 0;
		} else {
			vehicle->_vehicleMotors[i] = NULL;
			vehicle->_motorForce = vehicleDesc->motorForce;
		}
	}

	//Criando rodas
	int nbWheels = vehicleDesc->robotWheels.size();
	NxReal* torquesZero = new NxReal[nbWheels];
	for(NxU32 i = 0; i < nbWheels; i++)
	{
		torquesZero[i] = 0;
		NxWheel* wheel = NxWheel::createWheel2(vehicle->_actor, vehicleDesc->robotWheels[i]);

		if(wheel)
		{
			((NxWheel2*)wheel)->angWheelRelRobot = vehicleDesc->robotWheels[i]->angWheelRelRobot;
			((NxWheel2*)wheel)->id =vehicleDesc->robotWheels[i]->id;
			vehicle->_wheels.pushBack(wheel);
		} else {
			delete vehicle;
			return NULL;
		}
	}

	//don't go to sleep.
	//vehicle->_actor->wakeUp(1e10);
	vehicle->control(torquesZero);//0,0,0,0);//
	delete torquesZero;
	return vehicle;
}

NxVehicle* NxVehicle::createVehicle(NxScene1* scene, NxVehicleDesc* vehicleDesc)
{
	if (vehicleDesc == NULL)
		return NULL;
	NxVehicle* vehicle = NxVehicle::_createVehicle(scene, vehicleDesc);
	//NxAllRobots allRobots = Simulation::allRobots;
	scene->allRobots->AddRobot((NxRobot*)vehicle);
	//int nbRobots = allRobots.getNumberOfRobots();
	//if (allRobots.getActiveRobotNumber() != -1 || nbRobots == 1)
	//	allRobots.setActiveRobot(nbRobots-1);

	return vehicle;
}

/*void NxVehicle::handleContactPair(NxContactPair& pair, NxU32 carIndex)
{
	NxContactStreamIterator i(pair.stream);

	while(i.goNextPair())
	{
		NxShape * s = i.getShape(carIndex);

		while(i.goNextPatch())
		{
			const NxVec3& contactNormal = i.getPatchNormal();

			while(i.goNextPoint())
			{
				//user can also call getPoint() and getSeparation() here

				const NxVec3& contactPoint = i.getPoint();

				//add forces:

				//assuming front wheel drive we need to apply a force at the wheels.
				if (s->is(NX_SHAPE_CAPSULE) && s->userData != NULL) {
					//assuming only the wheels of the car are capsules, otherwise we need more checks.
					//this branch can't be pulled out of loops because we have to do a full iteration through the stream

					NxQuat local2global = s->getActor().getGlobalOrientationQuat();
					NxWheel* w = (NxWheel*)s->userData;
					if (!w->getWheelFlag(NX_WF_USE_WHEELSHAPE))
					{
						NxWheel1 * wheel = static_cast<NxWheel1*>(w);
						wheel->contactInfo.otherActor = pair.actors[1-carIndex];
						wheel->contactInfo.contactPosition = contactPoint;

						wheel->contactInfo.contactPositionLocal = contactPoint;
						wheel->contactInfo.contactPositionLocal -= _actor->getGlobalPosition();
						local2global.inverseRotate(wheel->contactInfo.contactPositionLocal);

						wheel->contactInfo.contactNormal = contactNormal;
						if (wheel->contactInfo.otherActor->isDynamic()) 
						{
							NxVec3 globalV = s->getActor().getLocalPointVelocity(wheel->getWheelPos());
							globalV -= wheel->contactInfo.otherActor->getLinearVelocity();
							local2global.inverseRotate(globalV);
							wheel->contactInfo.relativeVelocity = globalV.x;
							//printf("%2.3f (%2.3f %2.3f %2.3f)\n", wheel->contactInfo.relativeVelocity,
							//	globalV.x, globalV.y, globalV.z);
						} 
						else 
						{
							NxVec3 vel = s->getActor().getLocalPointVelocity(wheel->getWheelPos());
							local2global.inverseRotate(vel);
							wheel->contactInfo.relativeVelocity = vel.x;
							wheel->contactInfo.relativeVelocitySide = vel.z;
						}
						NX_ASSERT(wheel->hasGroundContact());
						//printf(" Wheel %x is touching\n", wheel);
					}
				}
			}
		}		
	}
	//printf("----\n");
}*/

void NxVehicle::updateVehicle(NxReal lastTimeStepSize)
{
	for(NxU32 i = 0; i < _wheels.size(); i++) 
	{
		NxWheel* wheel = _wheels[i];
		wheel->tick(false, _torqueAxleWheelControl[i], 0, lastTimeStepSize);
	}

	//delete _torqueAxleWheelControl;
	//printf("---\n");
}

//finds the actor that is touched by most wheels.
void NxVehicle::_computeMostTouchedActor()
{
	std::map<NxActor*, NxU32> actors;
	typedef std::map<NxActor*, NxU32> Map;
	for(NxU32 i = 0; i < _wheels.size(); i++)
	{
		NxActor* curActor = _wheels[i]->getTouchedActor();
		Map::iterator it = actors.find(curActor);
		if (it == actors.end())
		{
			actors[curActor] = 1;
		} else {
			it->second++;
		}
	}

	NxU32 count = 0;
	_mostTouchedActor = NULL;
	for(Map::iterator it = actors.begin(); it != actors.end(); ++it)
	{
		if(it->second > count)
		{
			count = it->second;
			_mostTouchedActor = it->first;
		}
	}
}

void NxVehicle::_computeLocalVelocity()
{
	_computeMostTouchedActor();
	NxVec3 relativeVelocity;
	if (_mostTouchedActor == NULL || !_mostTouchedActor->isDynamic())
	{
		relativeVelocity = _actor->getLinearVelocity();
	} else {
		relativeVelocity = _actor->getLinearVelocity() - _mostTouchedActor->getLinearVelocity();
	}
	NxQuat rotation = _actor->getGlobalOrientationQuat();
	NxQuat global2Local;
	_localVelocity = relativeVelocity;
	rotation.inverseRotate(_localVelocity);
	//printf("Velocity: %2.3f %2.3f %2.3f\n", _localVelocity.x, _localVelocity.y, _localVelocity.z);
}

void NxVehicle::control(NxReal* torqueWheels)//NxArray<NxReal> torqueWheels)//NxReal t1, NxReal t2, NxReal t3, NxReal t4)//
{
	_actor->wakeUp(0.05);

	_torqueAxleWheelControl = torqueWheels;
}

void NxVehicle::draw(bool debug) 
{
	glPushMatrix();

	float glmat[16];
	_actor->getGlobalPose().getColumnMajor44(glmat);
	glMultMatrixf(glmat);

	if(debug) glDisable(GL_LIGHTING);

	for(unsigned int i=0;i<_wheels.size(); i++) 
	{
		_wheels[i]->drawWheel(5, debug);
	}

	if(debug) glEnable(GL_LIGHTING);

	glPopMatrix();

	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glPointSize(10.0f);

	const float alphaStart = 0.3f;
	float alpha = alphaStart;

	static NxVec3* pBufferTrailPos = NULL;
	static float* pBufferTrailColor = NULL;
	if(pBufferTrailPos == NULL)
	{
		pBufferTrailPos = new NxVec3[NUM_TRAIL_POINTS];
		pBufferTrailColor = new float[NUM_TRAIL_POINTS*4];
	}

	for(int x = NUM_TRAIL_POINTS-1; x >= 0; x--)
	{
		alpha-=alphaStart/NUM_TRAIL_POINTS;
		pBufferTrailColor[x*4+0] = 0.1f;
		pBufferTrailColor[x*4+1] = 0.1f;
		pBufferTrailColor[x*4+2] = 0.1f;
		pBufferTrailColor[x*4+3] = alpha;
		pBufferTrailPos[x]=_trailBuffer[(_nextTrailSlot+x)%NUM_TRAIL_POINTS];
	}

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glVertexPointer(3, GL_FLOAT, sizeof(NxVec3), NUM_TRAIL_POINTS, pBufferTrailPos);
	glColorPointer(4, GL_FLOAT, 0, NUM_TRAIL_POINTS, pBufferTrailColor);

	glDrawArrays(GL_POINTS, 0, NUM_TRAIL_POINTS);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	glDisable(GL_BLEND);
	glEnable(GL_LIGHTING);

}

NxF32 NxVehicle::_getGearRatio()
{
	return _gearRatio;
}

void NxVehicle::applyRandomForce()
{
	NxVec3 pos(NxMath::rand(-4.f,4.f),NxMath::rand(-4.f,4.f),NxMath::rand(-4.f,4.f));
	NxReal force = NxMath::rand(_actor->getMass()*0.5f, _actor->getMass() * 2.f);
	_actor->addForceAtLocalPos(NxVec3(0, force*100.f, 0), pos);
}

NxVec3 NxVehicle::getPos()
{
	return _actor->getGlobalPosition();
}

NxReal NxVehicle::getAngle2DFromVehicle()
{
	NxMat33 rotMatrix = _actor->getGlobalOrientation();
	NxMat33 rotMatrixInv;
	rotMatrix.getInverse(rotMatrixInv);
	NxVec3 vecY = rotMatrixInv.getColumn(1);
	NxReal value = vecY.dot(NxVec3(0,1,0));
	value /= vecY.magnitude();

	NxReal teta = NxMath::acos(value);
	if( teta < NxPi*0.5 )
	{
		if( vecY.x * vecY.y > 0 )
		{
			teta = NxPi*2 - teta;
		}
	}
	else
	{
		if( vecY.x * vecY.y < 0 )
		{
			teta = NxPi*2 - teta;
		}
	}
	teta = NxPi*2 - teta;
	return teta;
}

NxVec3 NxVehicle::getAngle3DFromVehicle()
{
	NxMat33 rotMatrix = _actor->getGlobalOrientation();
	NxMat33 rotMatrixInv;
	rotMatrix.getInverse(rotMatrixInv);
	NxVec3 vecY = rotMatrixInv.getColumn(1);
	NxReal value = vecY.dot(NxVec3(0,1,0));
	value /= vecY.magnitude();

	NxReal teta = NxMath::acos(value);
	if( teta < NxPi*0.5 )
	{
		if( vecY.x * vecY.y > 0 )
		{
			teta = NxPi*2 - teta;
		}
	}
	else
	{
		if( vecY.x * vecY.y < 0 )
		{
			teta = NxPi*2 - teta;
		}
	}
	teta = NxPi*2 - teta;
	return vecY;
}

NxMat34 NxVehicle::getInitialPose(){
	return this->initialPose;
}

void NxVehicle::setInitialPose(NxMat34 initialPose){
	this->initialPose = initialPose;
}