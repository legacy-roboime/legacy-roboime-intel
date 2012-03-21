#include "NxBall.h"
#include "Simulation.h"

NxBall::NxBall(Simulation* simulation)
{
	this->simulation = simulation;
}

NxBall::~NxBall(void)
{
}

void Simulation::buildModelBall(int indexScene){
	NxBall* ball = this->gScenes[indexScene]->ball;
	ball->ball = this->getActorBall(indexScene);
	//velocidade maxima permitida segundo as rule 2010 (10m/s)
	//A bola pode atingir velocidade maior pq tem o caso de esta sendo usada pelo driblador, mas a principio consideremos isso
	ball->ball->setMaxAngularVelocity(10000./21.5);
	//0.031 medido da bola do lab
	//46g a bola da rules 2010
	//estimativa da bola do laboratorio 31g
	ball->ball->setMass(0.046); //PLUGIN TAH COM PROBLEMA XML ERRADO 

	//TODO: LEVANTAR INERTIA TENSOR, CMASS, DAMPINGS
	//float teste = ball->ball->getAngularDamping();
	//ball->ball->setCMassOffsetGlobalPosition(NxVec3(0, 0, 0));
	ball->ball->setCMassOffsetLocalPose( NxMat34( NxMat33(NxVec3(8.37673, 0, 0), NxVec3(0, 8.37673, 0), NxVec3(0, 0, 8.37673)), NxVec3(0, 0, 0) ) );
	ball->ball->setAngularDamping(0.5);
	ball->ball->setLinearDamping(0.5);
	ball->ball->setMassSpaceInertiaTensor(/*ball->ball->getMassSpaceInertiaTensor()*100000.*/ NxVec3(8.37673, 8.37673, 8.37673) );

	ball->initialPose = ball->ball->getGlobalPose();
	ball->indexScene = indexScene;

	ball->ball->putToSleep();
}

void NxBall::cloneBall(int indexNewScene){
	NxBall* ball = simulation->gScenes[indexNewScene]->ball;
	ball->ball = simulation->cloneActor(this->ball, indexNewScene);
	ball->indexScene = indexNewScene;
	//ball->ball->putToSleep();
}

void NxBall::resetToInitialPose(){
	this->ball->setGlobalPose(this->initialPose);
}

void NxBall::putToSleep(){
	this->ball->putToSleep();
	this->ball->setLinearVelocity(NxVec3(0,0,0));
	this->ball->setAngularVelocity(NxVec3(0,0,0));
}