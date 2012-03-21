#pragma once
#include "NxVehicle.h"
#include "NxKicker.h"
#include "NxDribbler.h"

class Simulation;

class NxRobot : public NxVehicle
{
private: 
	Simulation* simulation;
	int id;
	int idTeam; //0 (Yellow Team) - 1 (Blue Team)
public:
	int indexScene;
	NxKicker* kicker;
	NxDribbler* dribbler;
	float bodyRadius;
	float wheelsRadius;
public:
	NxRobot(Simulation* simulation);
	~NxRobot();
	void handleContactPair(NxContactPair& pair, NxU32 robotIndex);

	void cloneRobot(int indexNewScene, int indexNewRobot, NxVec3 newPosition, int indexNewTeam);
	void setGlobalPosition(NxVec3 position);
	void setGlobalOrientation(NxMat33 orientation);
	void putToSleep();
	void resetToInitialPose();
	void controlWheels( NxReal* wheelsSpeeds );
	void controlRobot(float speedX, float speedY, float speedAng, float dribblerSpeed, float kickerSpeed);
	void controlRobotByWheels(float speedWheel1, float speedWheel2, float speedWheel3, float speedWheel4, float dribblerSpeed, float kickerSpeed);
	NxReal* calcWheelSpeedFromRobotSpeed( NxReal speedAng, NxReal speedX, NxReal speedY);
	void goToThisPose( NxReal x, NxReal y, NxReal angle);
	void infinitePath();

	//getters and setters
	int getId();
	void setId(int id);
	int getIdTeam();
	void setIdTeam(int idTeam);
};