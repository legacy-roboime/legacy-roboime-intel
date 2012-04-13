#include "StageX.h"
#include "Simulation.h"
#include "Ball.h"

using namespace LibIntelligence;

#define M_2PI	6.2831853071795865

Simulation* /*const*/ StageX::sim = new Simulation();
bool StageX::init = false;
qreal StageX::defaultTimeStep = 1./60;
bool StageX::busyScenes[StageX::MAX_NUMBER_SCENES] = {false};

StageX::StageX(const Stage& stage)
	: built(false), 
	StageY(stage)
{
	updater = new UpdaterSIMi(this);
	if(!init){
		sim->initSimulation(MAX_NUMBER_SCENES);
		init = true;
	}

	Ball* ball = this->ball();
	updater->add(ball);
	updater->add(this);
	Team* blueTeam = this->blueTeam();
	Team* yellowTeam =this->yellowTeam();
	for(int i=0; i<blueTeam->size(); i++){
		updater->add(blueTeam->at(i));
	}
	for(int i=0; i<yellowTeam->size(); i++){
		updater->add(yellowTeam->at(i));
	}

	sceneNumber = -1;
	for(int i=0; i<MAX_NUMBER_SCENES; i++){
		if(busyScenes[i] == false){
			busyScenes[i] = true;
			sceneNumber = i; 
			break;
		}
	}
	if(sceneNumber==-1){
		printf("TODAS CENAS DO SIMULADOR ESTÃO OCUPADAS\n");
		delete this;
	}
}

StageX::StageX(const StageY& stage)
	: built(false), 
	StageY(stage)
{
	updater = new UpdaterSIMi(this);
	if(!init){
		sim->initSimulation(MAX_NUMBER_SCENES);
		init = true;
	}

	Ball* ball = this->ball();
	updater->add(ball);
	updater->add(this);
	Team* blueTeam = this->blueTeam();
	Team* yellowTeam =this->yellowTeam();
	for(int i=0; i<blueTeam->size(); i++){
		updater->add(blueTeam->at(i));
	}
	for(int i=0; i<yellowTeam->size(); i++){
		updater->add(yellowTeam->at(i));
	}

	sceneNumber = -1;
	for(int i=0; i<MAX_NUMBER_SCENES; i++){
		if(busyScenes[i] == false){
			busyScenes[i] = true;
			sceneNumber = i; 
			break;
		}
	}
	if(sceneNumber==-1){
		printf("TODAS CENAS DO SIMULADOR ESTÃO OCUPADAS\n");
		delete this;
	}
}

StageX::~StageX()
{
	if(sceneNumber!=-1)
		busyScenes[sceneNumber] = false; 
	//if(updater)
	//	delete updater;
}

void StageX::releaseScene()
{
	if(sceneNumber!=-1)
		busyScenes[sceneNumber] = false; 
	if(updater)
		delete updater;
	built = false;
}

//Constroi a cena respectiva do StageX no simulador
void StageX::build()
{
	Team* yellowTeam = this->yellowTeam();
	Team* blueTeam = this->blueTeam();
	Ball* b = this->ball();

	NxScene1* scene = sim->gScenes[sceneNumber];
	NxAllRobots* robots = scene->allRobots;
	NxArray<NxRobot*> robotsA = robots->getRobots();

	for(int i=0; i<robotsA.size(); i++){
		NxRobot* robot = robotsA[i];
		int idTeam = robot->getIdTeam();
		const Robot* r = idTeam == TeamColor::YELLOW ? yellowTeam->at(robot->getId()) : blueTeam->at(robot->getId());
		NxMat34 initPose = robot->getInitialPose();
		robot->setGlobalPosition(NxVec3(r->x(), r->y(), r->z()));//initPose.t.z));
		NxMat33 newOri = initPose.M;
		newOri.rotZ(r->orientation());
		robot->setGlobalOrientation(newOri);
		//robot->putToSleep();
		NxActor* actor = robot->getActor();
		actor->setLinearVelocity(NxVec3(r->speedX(), r->speedY(), r->speedZ()));
		actor->setAngularVelocity(NxVec3(0, 0, r->angSpeedZ()));
		robot->dribbler->speedToExecute = r->dribbler().speed();
		robot->kicker->controlKicker(r->kicker().speed(), robot);
	}

	//ball
	NxBall* ball = scene->ball;
	NxMat34 initPose = ball->initialPose;
	NxActor* actor = ball->ball;
	actor->setGlobalPosition(NxVec3(b->x(), b->y(), b->z()));//initPose.t.z));
	//ball->putToSleep();
	actor->setLinearVelocity(NxVec3(b->speedX(), b->speedY(), b->speedZ()));
	actor->setAngularVelocity(NxVec3(0, 0, b->angSpeedZ()));

	built = true;
}

void StageX::simulate(const qreal t)
{
	if(!built) 
		build();
	Team* yellowTeam = this->yellowTeam();
	Team* blueTeam = this->blueTeam();

	NxScene1* scene = sim->gScenes[sceneNumber];
	NxAllRobots* robots = scene->allRobots;
	NxArray<NxRobot*> robotsA = robots->getRobots();

	// enviar comandos dos robos do StageX pra simulação
	for(int n = 0; n < robotsA.size(); n++) {
		NxRobot* robot = robotsA[n];
		int idTeam = robot->getIdTeam();
		Robot* r = idTeam == TeamColor::YELLOW ? yellowTeam->at(robot->getId()) : blueTeam->at(robot->getId());
		Command& c = r->command();
		//TODO: implement logging
		//QVector<qreal> w = c.wheelsSpeed();
		if(c.wheelsSize() != 4) {
			qCritical("prepare: Wrong number of wheels!");
			break;
		}
		robot->controlRobotByWheels(c.wheelSpeedAt(0) * M_2PI, c.wheelSpeedAt(1) * M_2PI, c.wheelSpeedAt(2) * M_2PI, c.wheelSpeedAt(3) * M_2PI, c.dribbleSpeed(), c.kickSpeed());
		r->newCommand();
	}

	// simular
	sim->simulate(sceneNumber, t);

	// atualizar StageX pelos dados da cena (pelo SSL_WrapperPacket)
	SSL_WrapperPacket packet = sim->getSSLWrapper(sceneNumber);
	updater->addPacket(&packet);
	updater->step();
	updater->apply();
	// atualizar StageX com dados 3D
	for(int n = 0; n < robotsA.size(); n++) {
		NxRobot* robot = robotsA[n];
		int idTeam = robot->getIdTeam();
		Robot* r = idTeam == TeamColor::YELLOW ? yellowTeam->at(robot->getId()) : blueTeam->at(robot->getId());
		NxActor* actor = robot->getActor();
		r->setZ(actor->getGlobalPosition().z);
		r->setSpeedZ(actor->getLinearVelocity().z);
	}
	Ball* ball = this->ball();
	NxActor* b = scene->ball->ball;
	ball->setZ(b->getGlobalPosition().z);
	ball->setSpeedZ(b->getLinearVelocity().z);
}

uint StageX::getSceneNumber()
{
	return sceneNumber;
}