#pragma once
#ifndef NX_VEHICLE
#define NX_VEHICLE

#include "NxVehicleDesc.h"
#include "NxWheel.h"
#include "NxVehicleMotor.h"
//#include "NxVehicleGears.h"
//class NxVehicleGears;

//#include <NxScene.h>
#include <NxArray.h>
#include <NxUserContactReport.h>

#define NUM_TRAIL_POINTS 1600

class NxScene1;

class NxVehicle {
private:
	NxArray<NxWheel*>			_wheels;
	NxArray<NxVehicle*>			_children;
	NxActor*					_actor;
	NxScene1*					_nxScene;
	NxMat34						initialPose;

	NxArray<NxVehicleMotor*>	_vehicleMotors;
	//NxArray<NxVehicleGears*>	_vehicleGears;
	NxF32						_gearRatio;

	//NxReal					_steeringWheelState;
	NxReal						_accelerationPedal;
	//NxReal					_brakePedal;
	//bool						_brakePedalChanged;
	//bool						_handBrake;
	//NxReal					_acceleration;

	//NxReal					_digitalSteeringDelta;
	//NxVec3					_steeringTurnPoint;
	//NxVec3					_steeringSteerPoint;
	//NxReal					_steeringMaxAngleRad;
	NxReal						_motorForce;
	NxReal						_transmissionEfficiency;
	NxReal						_differentialRatio;

	NxVec3						_localVelocity;
	//bool						_braking;
	//bool						_releaseBraking;
	NxReal						_maxVelocity;
	NxMaterial*					_carMaterial;
	NxReal						_cameraDistance;

	NxVec3						_trailBuffer[NUM_TRAIL_POINTS];
	NxU32						_nextTrailSlot;
	NxReal						_lastTrailTime;

	NxActor*					_mostTouchedActor;
	void						_computeMostTouchedActor();
	void						_computeLocalVelocity();
	//NxReal					_computeAxisTorque(NxU32 indexMotor);
	NxReal						_computeRpmFromWheel(NxU32 indexWheel);
	//NxReal					_computeMotorRpm(NxReal rpm);

	//void						_updateRpms();

	NxF32						_getGearRatio();

	//void						_controlSteering(NxReal steering, bool analogSteering);
	void						_controlAcceleration(NxReal acceleration, bool analogAcceleration);
	static NxVehicle*			_createVehicle(NxScene1* scene/*int indexScene*/, NxVehicleDesc* vehicleDesc);
public:
	//int							indexScene;
	NxMat34						getInitialPose();
	void						setInitialPose(NxMat34 initialBodyPose);
	void*						userData;

	NxReal*						_torqueAxleWheelControl;
	//NxArray<NxReal>				_torqueAxleWheelControl;
	//NxReal						_torqueAxleWheelControl[4];

	NxVehicle();
	~NxVehicle();

	//void						handleContactPair(NxContactPair& pair, NxU32 carIndex);
	void						updateVehicle(NxReal lastTimeStepSize = 0); //Para rodas da classe Wheel2 o lastTimeStepSize nao eh util
	void						control (NxReal* torqueWheels);//NxArray<NxReal> torqueWheels);//NxReal t1, NxReal t2, NxReal t3, NxReal t4);//
	void						control1 ();
	//void						gearUp();
	//void						gearDown();

	void						draw(bool debug = false);

	void						applyRandomForce();

	NxReal						getDriveVelocity() { return NxMath::abs(_localVelocity.x); }

	NxReal						getMaxVelocity() { return _maxVelocity; }
	NxU32						getNbMotors() { return _vehicleMotors.size(); }
	const NxVehicleMotor*		getMotor(NxU32 i) const { return _vehicleMotors[i]; }
	//const NxVehicleGears*		getGear(NxU32 i) const { return _vehicleGears[i]; }
	NxActor*					getActor() { return _actor; }
	NxVehicle*					getChild(NxU32 i);
	void						addChild(NxVehicle* child);
	NxU32						nbChildren() { return _children.size(); }

	NxU32						getNbWheels() { return _wheels.size(); }
	const NxWheel*				getWheel(NxU32 i) { NX_ASSERT(i < _wheels.size()); return _wheels[i]; }
	NxReal						getCameraDistance() { return _cameraDistance; }

	NxMat34						getGlobalPose() { return _actor->getGlobalPose(); }

	static NxVehicle*			createVehicle(NxScene1* scene/*int indexScene*/, NxVehicleDesc* vehicleDesc);

	NxVec3						getPos();
	NxReal						getAngle2DFromVehicle();
	NxVec3						getAngle3DFromVehicle();
};

NX_INLINE NxVehicle* NxVehicle::getChild(NxU32 i) {
	if (i < _children.size())
		return _children[i];
	return NULL;
}

NX_INLINE void NxVehicle::addChild(NxVehicle* child) {
	_children.pushBack(child);
}
#endif
