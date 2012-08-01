#pragma once

///////////#include <QThread>
#include <QMutex>
#include <QMutexLocker>
#include <QObject>
#include <stdio.h>
#include <sstream>
#include <string>
#include <iostream>
//#include <time.h>
#include <vector>
#include <deque>

// Physics code
#undef random
#include "NxPhysics.h"
#include "UserAllocator.h"
#include "ErrorStream.h"
#include "NxCooking.h"
#include "Stream.h"

#include "Timing.h"
#include "MediaPath.h"
#include "cooking.h"
//#include "Utilities.h"
//#include "SamplesVRDSettings.h"

#include "NxScene1.h"
#include "NxMath1.h"
#include "MyUserNotify.h"
#include "MyContactReport.h"
#include "NxAllRobots.h"
#include "NxAllBalls.h"
#include "NxAllFields.h"
//#include "TimePosix.h"
#include <QtAlgorithms>
#include <QMap>
#include <QQueue>
#include <QString>

#include "messages_robocup_ssl_refbox_log.pb.h"
#include "messages_robocup_ssl_wrapper.pb.h"

using namespace std;

class Simulation : public QObject
{
	Q_OBJECT

private:
	QQueue<QString> qCommand;

	bool gPause;
	bool gSave;
	int	gLoad;
	bool gClear;

	QMutex mutex;
	NxU32 gBaseScene;
	NxVec3 gDefaultGravity;
	static UserAllocator *gMyAllocator;
	ErrorStream gErrorStream;
	MyUserNotify* gUserNotify;
	//extern NxUserContactReport * robotContactReport;
	NxArray<NxUserContactReport *> robotContactReport;
	
	friend class MyUserNotify;
	friend class UDPServerSimInt;
	friend class SimulationView;
	friend class LogPlayer;
	friend class Simulator;
	friend class NxRobot;
	friend class NxScene1;
	friend class UDPMulticastSenderSSLVision;
	friend class NxWheel;
	friend class NxWheel1;
	friend class NxWheel2;
	friend class NxVehicle;
	friend class NxField;
	friend class NxBall;
	friend class NxKicker;
	friend class NxDribbler;
	friend class MyContactReport;
private:
	void releaseScene(NxScene &scene);
	void CreateCube(const NxVec3 &pos, int size, const NxVec3 *initial_velocity = NULL, int indexRenderScene=0);
	void CreateStack(int size);
	void CreateTower(int size);
	bool InitNx();
	bool InitBlankScene();
	static void ReleaseNx();
	NX_BOOL LoadScene(const char *pFilename,NXU::NXU_FileType type);
	void SaveScene(const char *pFilename);
	bool FileExistTestSimple(const char *fname);
	void CSL_Scene();

	NxActor* cloneActor(NxActor* actorSource, int indexDestScene);
	NxShapeDesc* copyShapeDesc(NxShape* shapeSource);

	NxActor* getActorBall(int indexScene);
	NxActor* getActorRobot(int indexScene, int indexRobot);
	NxActor* getActorByLabel(int indexScene, string label);
	NxActor* getActorWheel(int indexScene, int indexRobot, int indexWheel);
	int getNumberWheels(int indexScene, int indexRobot);

	/**
	* Atributos para inteligência
	*/
	float timeStep;
	
	static NxPhysicsSDK *gPhysicsSDK;
	/**
	* Metodos para Inteligencia
	*/
	//build simulation
	void buildModelField(int indexScene);
	void buildModelBall(int indexScene);
	void buildModelRobot(int indexRobot, int indexScene, int indexTeam);
	void cloneScene(int indexSceneSource, int indexCloneScene);
	void deleteScene(int indexScene);
	//advance simulation
	void simulateReal(float dt = 1./60., int maxStepIter = 4);
	void simulate();
	void sleepComponents();
	bool isInRectangle(NxRobot* robot, NxVec3 p1, NxVec3 p2);
public:
	float sideBlue;

	Simulation(QObject* parent=0);
	~Simulation(void);

	// pode ser usado em vez do std::cout
	static ostringstream cout;

	string parseLegacyString(string);
	void append(const QString& data);
	SSL_WrapperPacket getSSLWrapper(int sceneNumber, float deltaTime = -1);

	//interface para inteligencia (direta)
	bool initSimulation( int nbScenes ); 
	void simulate( int indexScene, float dt = 1./60., int maxStepIter = 4 ); //timestep de 1/60 e 4 intervalos (baseado na tese do stefan zickler
	static QMap<int, NxScene1*> gScenes;
};