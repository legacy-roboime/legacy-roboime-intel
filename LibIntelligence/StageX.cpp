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

	for(int indexRobot=0; indexRobot<5; indexRobot++){
		//yellow
		const Robot* r = yellowTeam->at(indexRobot);
		NxRobot* robot = robots->getRobotByIdByTeam(indexRobot, TeamColor::YELLOW);
		NxMat34 initPose = robot->getInitialPose();
		robot->setGlobalPosition(NxVec3(r->x(), r->y(), initPose.t.z));
		NxMat33 newOri = initPose.M;
		newOri.rotZ(r->orientation());
		robot->setGlobalOrientation(newOri);
		//robot->putToSleep();
		NxActor* actor = robot->getActor();
		actor->setLinearVelocity(NxVec3(/*0, 0*/r->speedX(), r->speedY(), 0)); //TODO: implementar velocidade na inteligencia (a implementacao na classe object nao tah legal)
		actor->setAngularVelocity(NxVec3(0, 0, 0)); //TODO: implementar vAng eixo z
		robot->dribbler->speedToExecute = r->dribbler().speed();
		robot->kicker->controlKicker(r->kicker().speed(), robot);

		//blue
		const Robot* rr = blueTeam->at(indexRobot);
		NxRobot* robott = robots->getRobotByIdByTeam(indexRobot, TeamColor::BLUE);
		NxMat34 initPosee = robott->getInitialPose();
		robott->setGlobalPosition(NxVec3(rr->x(), rr->y(), initPosee.t.z));
		NxMat33 newOrii = initPosee.M;
		newOrii.rotZ(rr->orientation());
		robott->setGlobalOrientation(newOrii);
		//robott->putToSleep();
		NxActor* actorr = robott->getActor();
		actorr->setLinearVelocity(NxVec3(/*0, 0*/r->speedX(), r->speedY(), 0)); //TODO: implementar velocidade na inteligencia (a implementacao na classe object nao tah legal)
		actorr->setAngularVelocity(NxVec3(0, 0, 0)); // TODO: implementar vAng eixo z
		robott->dribbler->speedToExecute = rr->dribbler().speed();
		robott->kicker->controlKicker(rr->kicker().speed(), robott);
	}

	//ball
	qreal x = b->x();
	qreal y = b->y();
	qreal vX = b->speedX(); //0;//TODO: implementar velocidade na inteligencia (a implementacao na classe object nao tah legal)
	qreal vY = b->speedY(); //0;//TODO: implementar velocidade na inteligencia (a implementacao na classe object nao tah legal)
	qreal vAng = 0;
	NxBall* ball = scene->ball;
	NxMat34 initPose = ball->initialPose;
	NxActor* actor = ball->ball;
	actor->setGlobalPosition(NxVec3(x, y, initPose.t.z));
	//ball->putToSleep();
	actor->setLinearVelocity(NxVec3(vX, vY, 0));
	actor->setAngularVelocity(NxVec3(0, 0, vAng));

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

	// enviar comandos dos robos do StageX pra simulação
	for(int n = 0; n < 5; n++) {
		if(yellowTeam->at(n)!=0) {
			Robot* r = yellowTeam->at(n);
			Command& c = r->command();
			//TODO: implement logging
			//QVector<qreal> w = c.wheelsSpeed();
			if(c.wheelsSize() != 4) {
				qCritical("prepare: Wrong number of wheels!");
				break;
			}

			robots->getRobotByIdByTeam(n, TeamColor::YELLOW)->controlRobotByWheels(c.wheelSpeedAt(0) * M_2PI, c.wheelSpeedAt(1) * M_2PI, c.wheelSpeedAt(2) * M_2PI, c.wheelSpeedAt(3) * M_2PI, c.dribbleSpeed(), c.kickSpeed());

			r->newCommand();
		} else {
			robots->getRobotByIdByTeam(n, TeamColor::YELLOW)->controlRobotByWheels(0, 0, 0, 0, 0, 0);
		}

		if(blueTeam->at(n)!=0) {
			Robot* rr = blueTeam->at(n);
			Command& cc = rr->command();
			//TODO: implement logging
			//QVector<qreal> w = c.wheelsSpeed();
			if(cc.wheelsSize() != 4) {
				qCritical("prepare: Wrong number of wheels!");
				break;
			}

			//cout << cc.wheelSpeedAt(0) << " " << cc.wheelSpeedAt(1) << " " << cc.wheelSpeedAt(2) << " " << cc.wheelSpeedAt(3) << " " << n << endl;

			robots->getRobotByIdByTeam(n, TeamColor::BLUE)->controlRobotByWheels(cc.wheelSpeedAt(0) * M_2PI, cc.wheelSpeedAt(1) * M_2PI, cc.wheelSpeedAt(2) * M_2PI, cc.wheelSpeedAt(3) * M_2PI, cc.dribbleSpeed(), cc.kickSpeed());

			rr->newCommand();
		} else {
			robots->getRobotByIdByTeam(n, TeamColor::BLUE)->controlRobotByWheels(0, 0, 0, 0, 0, 0);
		}
	}

	// simular
	if(t>0)
		sim->simulate(sceneNumber, t);
	else
		sim->simulate(sceneNumber);

	// atualizar StageX pelos dados da cena (pelo SSL_WrapperPacket)
	SSL_WrapperPacket packet = sim->getSSLWrapper(sceneNumber);
	updater->addPacket(&packet);
	updater->step();
	updater->apply();
}

uint StageX::getSceneNumber()
{
	return sceneNumber;
}