#pragma once

#include "NxActor.h"

class Simulation;

class NxField
{
private:
	Simulation* simulation;
public:
	int indexScene;
	
	float fieldLength;
	float fieldWidth;
	float goalHeigth;

	int line_width;
	float linesLength;//int field_length;
	float linesWidth;//int field_width;
	int boundary_width;
	int referee_width;
	float goalWidth;//int goal_width;
	float goalLength;//int goal_depth;
	int goal_wall_width;
	int center_circle_radius;
	int defense_radius;
	int defense_stretch;
	int free_kick_from_defense_dist;
	int penalty_spot_from_field_line_dist;
	int penalty_line_from_spot_dist;

	NxActor* actorParedeLateralPositivo;
	NxActor* actorParedeLateralNegativo;
	NxActor* actorParedeFundoNegativo;
	NxActor* actorParedeFundoPositivo;
	NxActor* actorCampo;
	NxActor* actorGolPositivoParedeFundo;
	NxActor* actorGolPositivoParedeNegativo;
	NxActor* actorGolPositivoParedePositivo;
	NxActor* actorGolNegativoParedeFundo;
	NxActor* actorGolNegativoParedePositivo;
	NxActor* actorGolNegativoParedeNegativo;
public:
	NxField(Simulation* simulation);
	~NxField(void);

	void cloneField(int indexNewScene);
	void setDimensions(float fieldLength, float fieldWidth, float linesLength, float linesWidth, float goalLength = 200., float goalWidth = 700., float goalHeigth = 160.);
	NxVec3 getLeftGoalPos();
	NxVec3 getRightGoalPos();
	NxVec3 getCenterPos();
};
