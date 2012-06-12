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

	if(!init){
		sim->initSimulation(MAX_NUMBER_SCENES);
		init = true;

		NxAllRobots* robots = sim->gScenes[sceneNumber]->allRobots;
		int nbRobots = robots->getRobots().size();
		QVector<float> wheels = QVector<float>(robots->getRobotByIdByTeam(0, 0)->getNbWheels()); //supondo robos com quantidade de rodas identicas
		lastWheelSpeed = new QVector<QVector<float>>();
		lastDesiredWheelSpeed = new QVector<QVector<float>>();
		lastWheelTorque = new QVector<QVector<float>>();
		for(int i=0; i<nbRobots; i++){
			lastWheelSpeed->push_back(wheels);
			lastDesiredWheelSpeed->push_back(wheels);
			lastWheelTorque->push_back(wheels);
		}
	}
	else{
		const StageX& s = (const StageX&) stage;
		lastWheelSpeed = new QVector<QVector<float>>(*(s.lastWheelSpeed));
		lastDesiredWheelSpeed = new QVector<QVector<float>>(*(s.lastDesiredWheelSpeed));
		lastWheelTorque =  new QVector<QVector<float>>(*(s.lastWheelTorque));
	}
}

//StageX::StageX(const StageY& stage)
//	: built(false), 
//	StageY(stage)
//{
//	updater = new UpdaterSIMi(this);
//
//	if(!init){
//		sim->initSimulation(MAX_NUMBER_SCENES);
//		init = true;
//	}
//
//	Ball* ball = this->ball();
//	updater->add(ball);
//	updater->add(this);
//	Team* blueTeam = this->blueTeam();
//	Team* yellowTeam =this->yellowTeam();
//	for(int i=0; i<blueTeam->size(); i++){
//		updater->add(blueTeam->at(i));
//	}
//	for(int i=0; i<yellowTeam->size(); i++){
//		updater->add(yellowTeam->at(i));
//	}
//
//	sceneNumber = -1;
//	for(int i=0; i<MAX_NUMBER_SCENES; i++){
//		if(busyScenes[i] == false){
//			busyScenes[i] = true;
//			sceneNumber = i; 
//			break;
//		}
//	}
//	if(sceneNumber==-1){
//		printf("TODAS CENAS DO SIMULADOR ESTÃO OCUPADAS\n");
//		delete this;
//	}
//
//	NxAllRobots* robots = sim->gScenes[sceneNumber]->allRobots;
//	int nbRobots = robots->getRobots().size();
//	QVector<float> wheels = QVector<float>(robots->getRobotByIdByTeam(0, 0)->getNbWheels()); //supondo robos com quantidade de rodas identicas
//	for(int i=0; i<nbRobots; i++){
//		lastWheelSpeed.push_back(wheels);
//		lastDesiredWheelSpeed.push_back(wheels);
//		lastWheelTorque.push_back(wheels);
//	}
//}

StageX::~StageX()
{
	if(sceneNumber!=-1)
		busyScenes[sceneNumber] = false; 
	//if(updater)
	//	delete updater;
	delete lastWheelSpeed;
	delete lastDesiredWheelSpeed;
	delete lastWheelTorque;
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
		//XXX: ignorando posição z
		robot->setGlobalPosition(NxVec3(r->x(), r->y(), initPose.t.z));//));
		NxMat33 newOri = initPose.M;
		newOri.rotZ(r->orientation());
		robot->setGlobalOrientation(newOri);
		//robot->putToSleep();
		NxActor* actor = robot->getActor();
		//XXX: ignorando velocidade em z
		actor->setLinearVelocity(NxVec3(r->speed().x(), r->speed().y(), 0.0));
		//cout << r->speed().x() << " " << r->speed().y() << " " << r->speedZ() << endl;
		actor->setAngularVelocity(NxVec3(0, 0, r->angSpeed()));
		robot->dribbler->speedToExecute = r->dribbler().speed();
		robot->kicker->controlKicker(r->kicker().speed(), robot);
		for(int j=0; j<robot->getNbWheels(); j++){
			NxWheel* wheel = robot->getWheel(j);
			wheel->lastDesiredWheelSpeed = lastDesiredWheelSpeed->at(i).at(j);
			wheel->lastWheelSpeed = lastWheelSpeed->at(i).at(j);
			wheel->lastWheelTorque = lastWheelTorque->at(i).at(j);
			//cout << "ATUALIZANDO " << wheel->lastDesiredWheelSpeed << " " << wheel->lastWheelSpeed << " " << wheel->lastWheelTorque << endl;
		}
	}

	//ball
	NxBall* ball = scene->ball;
	NxMat34 initPose = ball->initialPose;
	NxActor* actor = ball->ball;
	//XXX: ignorando posição z
	actor->setGlobalPosition(NxVec3(b->x(), b->y(), initPose.t.z));
	//ball->putToSleep();
	//XXX: ignorando velocidade em z
	actor->setLinearVelocity(NxVec3(b->speed().x(), b->speed().y(), 0.0));
	actor->setAngularVelocity(NxVec3(0, 0, b->angSpeed()));

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

	//cout << "ANTES " << robotsA[4]->getActor()->getLinearVelocity().x << " " << robotsA[4]->getActor()->getLinearVelocity().y << " " << robotsA[4]->getActor()->getAngularVelocity().z << endl;

	// simular
	sim->simulate(sceneNumber, t);

	//cout << "DEPOIS " <<  robotsA[4]->getActor()->getLinearVelocity().x << " " << robotsA[4]->getActor()->getLinearVelocity().y << " " << robotsA[4]->getActor()->getAngularVelocity().z << endl;

	// atualizar StageX pelos dados da cena (pelo SSL_WrapperPacket)
	SSL_WrapperPacket packet = sim->getSSLWrapper(sceneNumber, t);
	updater->addPacket(&packet);
	updater->step();
	updater->apply(); //TODO: NAO EXECUTAR O UPDATESPEED DAS CLASSES UPDATEROBOT E UPDATEBALL
	for(int n = 0; n < robotsA.size(); n++) {
		NxRobot* robot = robotsA[n];
		int idTeam = robot->getIdTeam();
		Robot* r = idTeam == TeamColor::YELLOW ? yellowTeam->at(robot->getId()) : blueTeam->at(robot->getId());
		NxActor* actor = robot->getActor();
		NxVec3 rSpeed = actor->getLinearVelocity();
		// atualizar velocidades que poderiam ser calculadas pelos dados da visao
		//XXX: ignorando dados em z
		r->setSpeed(rSpeed.x, rSpeed.y);
		r->setAngSpeed(actor->getAngularVelocity().z);
		// atualizar controle
		for(int j=0; j<robot->getNbWheels(); j++){
			NxWheel* wheel = robot->getWheel(j);
			(*lastDesiredWheelSpeed)[n][j] = wheel->lastDesiredWheelSpeed;
			(*lastWheelSpeed)[n][j] = wheel->lastWheelSpeed;
			(*lastWheelTorque)[n][j] = wheel->lastWheelTorque;
			//cout << "COPIANDO " << (*lastDesiredWheelSpeed)[n][j] << " " << (*lastWheelSpeed)[n][j] << " " << (*lastWheelTorque)[n][j] << endl;
		}
	}
	Ball* ball = this->ball();
	NxActor* b = scene->ball->ball;
	NxVec3 ballSpeed = b->getLinearVelocity();
	// atualizar velocidades que poderiam ser calculadas pelos dados da visao
	//XXX: ignorando dados em z
	ball->setSpeed(ballSpeed.x, ballSpeed.y);
	ball->setAngSpeed(b->getAngularVelocity().z);
}

uint StageX::getSceneNumber()
{
	return sceneNumber;
}