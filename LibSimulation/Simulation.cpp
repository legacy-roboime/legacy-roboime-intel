#include "Simulation.h"
#include <QDateTime>

QMap<int, NxScene1*> Simulation::gScenes = QMap<int, NxScene1*>();
UserAllocator* Simulation::gMyAllocator = NULL;
NxPhysicsSDK* Simulation::gPhysicsSDK = 0;

/**
* Método do PhysX
*/
/*const char* getNxSDKCreateError(const NxSDKCreateError& errorCode) 
{
switch(errorCode) 
{
case NXCE_NO_ERROR: return "NXCE_NO_ERROR";
case NXCE_PHYSX_NOT_FOUND: return "NXCE_PHYSX_NOT_FOUND";
case NXCE_WRONG_VERSION: return "NXCE_WRONG_VERSION";
case NXCE_DESCRIPTOR_INVALID: return "NXCE_DESCRIPTOR_INVALID";
case NXCE_CONNECTION_ERROR: return "NXCE_CONNECTION_ERROR";
case NXCE_RESET_ERROR: return "NXCE_RESET_ERROR";
case NXCE_IN_USE_ERROR: return "NXCE_IN_USE_ERROR";
default: return "Unknown error";
}
};*/

/**
* Método do PhysX
*/
NX_INLINE bool isProcessorBigEndian()
{
	union EndianUnion
	{
		NxU8 bytes[4];
		NxU32 dword;
	};

	volatile EndianUnion val;

	val.dword = 1;

	return val.bytes[3] != 0;

}

Simulation::Simulation(QObject* parent) : QObject(parent)
{

	gPause = false;
	gSave = false;
	gLoad = 2;//0;
	gClear = false;
	
	gBaseScene = 0;
	gDefaultGravity = NxVec3(0.0f, 0.0f, - 9.81f);
	
	gErrorStream = ErrorStream();
	gUserNotify = new MyUserNotify(this);
	timeStep = 1./60.; //timestep de 1/60 e 4 intervalos (baseado na tese do stefan zickler
	robotContactReport = NxArray<NxUserContactReport *>();
}

Simulation::~Simulation(void)
{
	//~ dos ponteiros do PhsyX em ReleaseNX
	//delete gPhysicsSDK;
	//delete gScenes;
	//delete gMyAllocator;

	//ReleaseNx executa a liberacao de memoria

	//delete outputfile;
}

//==================================================================================
void Simulation::ReleaseNx()
{
	CloseCooking();
	NXU::releasePersistentMemory();
	if (gPhysicsSDK)
	{
		QMapIterator<int, NxScene1*> i(gScenes);
		while (i.hasNext()) 
		{
			i.next();
			if (i.value()->scene)
			{
				gPhysicsSDK->releaseScene(*(i.value()->scene));
				i.value()->scene = NULL;
			}
		}
		NxReleasePhysicsSDK(gPhysicsSDK);
		gPhysicsSDK = NULL;
	}

	if (gMyAllocator != NULL)
	{
		delete gMyAllocator;
		gMyAllocator = NULL;
	}
}

void Simulation::releaseScene(NxScene &scene)
{
	gPhysicsSDK->releaseScene(scene);
}
//==================================================================================
void Simulation::CreateCube(const NxVec3 &pos, int size, const NxVec3 *initial_velocity, int indexRenderScene)
{
	if (gScenes[indexRenderScene]->scene)
	{
		// Create body
		NxBodyDesc BodyDesc;
		BodyDesc.angularDamping = 0.5f;
		//	BodyDesc.maxAngularVelocity	= 10.0f;
		if (initial_velocity)
		{
			BodyDesc.linearVelocity =  *initial_velocity;
		}

		NxBoxShapeDesc BoxDesc;
		BoxDesc.dimensions = NxVec3(float(size), float(size), float(size));

		NxActorDesc ActorDesc;
		ActorDesc.shapes.pushBack(&BoxDesc);
		ActorDesc.body = &BodyDesc;
		ActorDesc.density = 10.0f;
		ActorDesc.globalPose.t = pos;
		gScenes[indexRenderScene]->scene->createActor(ActorDesc)->userData = (void*)size;
	}
}

//==================================================================================
void Simulation::CreateStack(int size)
{
	float CubeSize = 2.0f;
	//	float Spacing = 0.01f;
	float Spacing = 0.0001f;
	NxVec3 Pos(0.0f, CubeSize, 0.0f);
	float Offset =  - size *(CubeSize *2.0f + Spacing) *0.5f;
	while (size)
	{
		for (int i = 0; i < size; i++)
		{
			Pos.x = Offset + float(i)*(CubeSize *2.0f + Spacing);
			CreateCube(Pos, (int)CubeSize);
		}
		Offset += CubeSize;
		Pos.y += (CubeSize *2.0f + Spacing);
		size--;
	}
}

//==================================================================================
void Simulation::CreateTower(int size)
{
	float CubeSize = 2.0f;
	float Spacing = 0.01f;
	NxVec3 Pos(0.0f, CubeSize, 0.0f);
	while (size)
	{
		CreateCube(Pos, (int)CubeSize);
		Pos.y += (CubeSize *2.0f + Spacing);
		size--;
	}
}

//==================================================================================
bool Simulation::InitNx()
{
	if (!gMyAllocator)
	{
		gMyAllocator = new UserAllocator();
	}

	NXU::setErrorReport(gUserNotify);
	NXU::setEndianMode(isProcessorBigEndian());

	// Initialize PhysicsSDK
	NxPhysicsSDKDesc desc;
	NxSDKCreateError errorCode = NXCE_NO_ERROR;
	gPhysicsSDK = NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION, gMyAllocator, &gErrorStream, desc, &errorCode);
	if(gPhysicsSDK == NULL) 
	{
		char* errorString;
		switch(errorCode) 
		{
		case NXCE_NO_ERROR: errorString = "NXCE_NO_ERROR";
		case NXCE_PHYSX_NOT_FOUND: errorString = "NXCE_PHYSX_NOT_FOUND";
		case NXCE_WRONG_VERSION: errorString = "NXCE_WRONG_VERSION";
		case NXCE_DESCRIPTOR_INVALID: errorString = "NXCE_DESCRIPTOR_INVALID";
		case NXCE_CONNECTION_ERROR: errorString = "NXCE_CONNECTION_ERROR";
		case NXCE_RESET_ERROR: errorString = "NXCE_RESET_ERROR";
		case NXCE_IN_USE_ERROR: errorString = "NXCE_IN_USE_ERROR";
		default: errorString = "Unknown error";
		}
		printf("\nSDK create error (%d - %s).\nUnable to initialize the PhysX SDK, exiting the sample.\n\n", errorCode, errorString);
		return false;
	}
#if SAMPLES_USE_VRD
	// The settings for the VRD host and port are found in SampleCommonCode/SamplesVRDSettings.h
	if (gPhysicsSDK->getFoundationSDK().getRemoteDebugger() && !gPhysicsSDK->getFoundationSDK().getRemoteDebugger()->isConnected())
		gPhysicsSDK->getFoundationSDK().getRemoteDebugger()->connect(SAMPLES_VRD_HOST, SAMPLES_VRD_PORT, SAMPLES_VRD_EVENTMASK);
#endif

	//if (!InitCooking(gMyAllocator, &gErrorStream)) {
	//	printf("\nError: Unable to initialize the cooking library, exiting the sample.\n\n");
	//	return false;
	//}

	gPhysicsSDK->setParameter(NX_SKIN_WIDTH, 0.025f);
	//enable visualisation
	gPhysicsSDK->setParameter(NX_VISUALIZATION_SCALE, 1.0f);

	gPhysicsSDK->setParameter(NX_VISUALIZE_BODY_AXES, 1.0f);
	gPhysicsSDK->setParameter(NX_VISUALIZE_COLLISION_SHAPES, 1.0f);
	gPhysicsSDK->setParameter(NX_VISUALIZE_COLLISION_AXES, 1.0f);
	gPhysicsSDK->setParameter(NX_VISUALIZE_CONTACT_POINT, 1.0f);
	gPhysicsSDK->setParameter(NX_VISUALIZE_CONTACT_NORMAL, 1.0f);

	gPhysicsSDK->setParameter(NX_VISUALIZE_JOINT_LOCAL_AXES, 1.0f);
	gPhysicsSDK->setParameter(NX_VISUALIZE_JOINT_WORLD_AXES, 1.0f);
	gPhysicsSDK->setParameter(NX_VISUALIZE_JOINT_LIMITS, 1.0f);

	// Don't slow down jointed objects
	gPhysicsSDK->setParameter(NX_ADAPTIVE_FORCE, 0.0f);

	//Quanto coloquei esse trecho de codigo o programa ficou lento
	//for (NxU32 i = 0; i < 1; ++i) //Just one base scene
	//{
		//if (!InitBlankScene()) return false;
	//}

	NxScene1* scene1 = new NxScene1(this);
	scene1->indexScene = gBaseScene;
	gScenes = QMap<int, NxScene1*>();
	gScenes.insert(gBaseScene, scene1);

	return true;
}

//==================================================================================
bool Simulation::InitBlankScene()
{
	if (gPhysicsSDK)
	{
		// Create a blank scene
		NxSceneDesc sceneDesc;
		//sceneDesc.gravity = gDefaultGravity;

		gScenes[gBaseScene]->scene = gPhysicsSDK->createScene(sceneDesc);
		if(gScenes[gBaseScene]->scene == NULL) 
		{
			printf("\nError: Unable to create a PhysX scene, exiting the sample.\n\n");
			return false;
		}
		
		//NxMaterial *defaultMaterial = gScenes[gBaseScene]->getMaterialFromIndex(0);
		//defaultMaterial->setRestitution(0.0f);
		//defaultMaterial->setStaticFriction(0.5f);
		//defaultMaterial->setDynamicFriction(0.5f);
		
		//// Create ground plane
		NxPlaneShapeDesc planeDesc;
		planeDesc.setToDefault();
		
		//NxActorDesc ActorDesc;
		//ActorDesc.shapes.pushBack(&planeDesc);
		//gScenes[gBaseScene]->createActor(ActorDesc);
	}
	return true;
}

void Simulation::append(const QString& d)
{
	qCommand.enqueue(d);
}

void Simulation::simulateReal(float dt, int maxStepIter)
{
	//QTime time = QTime::currentTime();
	//static double lTime = 0;
	//double mSec = time.minute() * 60 * 1000 + time.second() * 1000 + time.msec() - lTime;
	//printf("SIM %f\n",mSec);
	//lTime = time.minute() * 60 * 1000 + time.second() * 1000 + time.msec();
	
	/*while(!qCommand.empty()) {
		QString& string = qCommand.dequeue();
		parseLegacyString(string.toStdString());
	}*/

	static int count=0; //coloca para executar o ultimo comando um periodo de tempo assim como o nosso robo real TODO: perguntar pro renault qual é o tempo
	static QString& stringO = QString("");
	if(!qCommand.isEmpty()) {
		stringO = qCommand.last();
		parseLegacyString(stringO.toStdString());
		qCommand.clear();
		count = 0;
	}
	else if(!stringO.isEmpty() && count<60){ //acho q era um segundo
		parseLegacyString(stringO.toStdString()); 
		count++;
	}
	//parseLegacyString("15 0 1 74.0603 74.0603 -55.8787 -55.8787 0 0 74.0603 74.0603 -55.8787 -55.8787 0 0 74.0603 74.0603 -55.8787 -55.8787 0 0 74.0603 74.0603 -55.8787 -55.8787 0 0 74.0603 74.0603 -55.8787 -55.8787 0 0\n");

	//cout << "a " << this->gScenes[0]->allRobots->getRobotByIdByTeam(0,1)->getGlobalPose().t.x << endl;
	simulate(this->gBaseScene, dt, maxStepIter);
	//cout << "d " << this->gScenes[0]->allRobots->getRobotByIdByTeam(0,1)->getGlobalPose().t.x << endl;
}

void Simulation::simulate()
{	
	// Start simulation
	// Physics code
	QMapIterator<int, NxScene1*> i(gScenes);
	while (i.hasNext()) 
	{
		i.next();
		if (i.value()->scene && !gPause)
		{
			//printf("RODA: %f\n", ((NxWheel2*)(i.value()->allRobots->getRobotByIdByTeam(0, 1)->getWheel(0)))->getWheelShape()->getAxleSpeed());
			//timestep de 1/60 e 4 intervalos (baseado na tese do stefan zickler
			i.value()->scene->setTiming(timeStep, 4, NX_TIMESTEP_FIXED);
			i.value()->scene->simulate(timeStep);
		}	
	}

	i.toFront();
	while (i.hasNext()) 
	{
		i.next();
		if (i.value()->scene && !gPause)
		{
			i.value()->scene->flushStream();
			i.value()->scene->fetchResults(NxSimulationStatus::NX_RIGID_BODY_FINISHED, true);
		}
	}
	// ~Physics code
}

void Simulation::simulate(int indexScene, float dt, int maxStepIter )
{
	//parseLegacyString("15 0 1 74.0603 74.0603 -55.8787 -55.8787 0 0 74.0603 74.0603 -55.8787 -55.8787 0 0 74.0603 74.0603 -55.8787 -55.8787 0 0 74.0603 74.0603 -55.8787 -55.8787 0 0 74.0603 74.0603 -55.8787 -55.8787 0 0\n");
	// Start simulation
	// Physics code
	//cout << "a " << this->gScenes[indexScene]->allRobots->getRobotByIdByTeam(4,1)->getGlobalPose().t.x << endl;
	//if (gScenes[indexScene]->scene && !gPause)
	//{
		 gScenes[indexScene]->scene->setTiming(dt, maxStepIter, NX_TIMESTEP_FIXED);
		 gScenes[indexScene]->scene->simulate(dt);
		 gScenes[indexScene]->scene->flushStream();
		 gScenes[indexScene]->scene->fetchResults(NX_RIGID_BODY_FINISHED, true);
	//}
	//cout << "d " << this->gScenes[indexScene]->allRobots->getRobotByIdByTeam(4,1)->getGlobalPose().t.x << endl;
	// ~Physics code
}

NxActor* Simulation::getActorBall(int indexScene)
{
	NxActor* actor = NULL;
	const char* actorName = NULL;
	if( gScenes[indexScene]->scene!=NULL)
	{
		for(unsigned int j=0;j< gScenes[indexScene]->scene->getNbActors();j++)
		{
			actor =  gScenes[indexScene]->scene->getActors()[j];
			if(actor != NULL)
			{
				actorName = actor->getName();
				if(actorName != NULL)
				{
					if(strcmp(actorName,"Bola")==0) break;
					else actor = NULL;
				}
			}
			else continue;
		}
	}
	return actor;
}

NxActor* Simulation::getActorRobot(int indexScene, int indexRobot)
{
	NxActor* actor = NULL;
	const char* actorName = NULL;
	if( gScenes[indexScene]->scene!=NULL)
	{
		for(unsigned int j=0;j< gScenes[indexScene]->scene->getNbActors();j++)
		{
			actor =  gScenes[indexScene]->scene->getActors()[j];
			// gScenes[indexScene]->scene->get
			if(actor != NULL)
			{
				actorName = actor->getName();
				if(actorName != NULL)
				{
					string label;
					string plabel = "Robo";
					stringstream out;
					out << indexRobot;
					label.append(plabel);
					label.append(out.str());
					char* arrayLabel = new char[label.size()+1];
					arrayLabel[label.size()]=0;
					memcpy(arrayLabel, label.c_str(), label.size());

					if(strcmp(actorName,arrayLabel)==0) 
					{
						delete arrayLabel;
						break;
					}
					else 
					{
						actor = NULL;
						delete arrayLabel;
					}
				}
			}
			else continue;
		}
	}
	return actor;
}

NxActor* Simulation::getActorByLabel(int indexScene, string label)
{
	NxActor* actor = NULL;
	const char* actorName = NULL;
	if( gScenes[indexScene]->scene!=NULL)
	{
		for(unsigned int j=0;j< gScenes[indexScene]->scene->getNbActors();j++)
		{
			actor =  gScenes[indexScene]->scene->getActors()[j];
			// gScenes[indexScene]->scene->get
			if(actor != NULL)
			{
				actorName = actor->getName();
				if(actorName != NULL)
				{
					if(strcmp(actorName,label.c_str())==0) 
					{
						break;
					}
					else 
					{
						actor = NULL;
					}
				}
			}
			else continue;
		}
	}
	return actor;
}

NxActor* Simulation::getActorWheel(int indexScene, int indexRobot, int indexWheel)
{
	NxActor* actor = NULL;
	const char* actorName = NULL;
	if( gScenes[indexScene]->scene!=NULL)
	{
		for(unsigned int j=0;j< gScenes[indexScene]->scene->getNbActors();j++)
		{
			actor =  gScenes[indexScene]->scene->getActors()[j];
			// gScenes[indexScene]->scene->get
			if(actor != NULL)
			{
				actorName = actor->getName();
				if(actorName != NULL)
				{
					string label;
					string plabel = "Roda-";
					stringstream out;
					stringstream out1;
					out << indexRobot;
					out1 << indexWheel;
					label.append(plabel);
					label.append(out.str());
					label.append("-");
					label.append(out1.str());
					char* arrayLabel = new char[label.size()+1];
					arrayLabel[label.size()]=0;
					memcpy(arrayLabel, label.c_str(), label.size());

					if(strcmp(actorName,arrayLabel)==0) 
					{
						delete arrayLabel;
						break;
					}
					else 
					{
						actor = NULL;
						delete arrayLabel;
					}
				}
			}
			else continue;
		}
	}
	return actor;
}

int Simulation::getNumberWheels(int indexScene, int indexRobot)
{
	NxActor* actor = NULL;
	int numberOfWheels = 0;
	const char* actorName = NULL;
	if( gScenes[indexScene]->scene!=NULL)
	{
		for(unsigned int j=0;j< gScenes[indexScene]->scene->getNbActors();j++)
		{
			actor =  gScenes[indexScene]->scene->getActors()[j];
			if(actor != NULL)
			{
				actorName = actor->getName();
				if(actorName != NULL)
				{
					string label;
					string plabel = "Roda-";
					stringstream out;
					out << indexRobot;
					label.append(plabel);
					label.append(out.str());
					char* arrayLabel = new char[label.size()+1];
					arrayLabel[label.size()]=0;
					memcpy(arrayLabel, label.c_str(), label.size());

					char* actorNameAux = new char[strlen(actorName)-1];
					actorNameAux[strlen(actorName)-2]=0;
					memcpy(actorNameAux, actorName, strlen(actorName)-2);
					if(strcmp(actorNameAux,arrayLabel)==0) 
					{
						delete arrayLabel;
						delete actorNameAux;
						numberOfWheels++;
					}
					else 
					{
						actor = NULL;
						delete arrayLabel;
						delete actorNameAux;
					}
				}
			}
			else continue;
		}
	}
	return numberOfWheels;
}

NxActor* Simulation::cloneActor(NxActor* actorSource, int indexDestScene)
{
	NxActorDesc actorDesc;
	actorSource->saveToDesc(actorDesc);

	// Create body
	if(actorSource->isDynamic()){
		NxBodyDesc bodyDesc;
		actorSource->saveBodyToDesc(bodyDesc);
		actorDesc.body = &bodyDesc;
	}

	//TODO: Saber pq tive que fazer diferente dos exemplos do PhysX, pois lá os ShapeDesc eram instancias locais, tive que mudar para ponteiros
	for(int i=0; i<actorSource->getNbShapes(); i++)
	{
		NxShapeType type = (actorSource->getShapes())[i]->getType();
		if(type==NxShapeType::NX_SHAPE_BOX){
			NxBoxShapeDesc* boxDesc = new NxBoxShapeDesc();
			(actorSource->getShapes())[i]->isBox()->saveToDesc(*boxDesc);
			actorDesc.shapes.push_back(boxDesc);
		}
		else if(type==NxShapeType::NX_SHAPE_CONVEX){
			NxConvexShapeDesc* convexShapeDesc = new NxConvexShapeDesc();
			//static NxConvexMeshDesc convexMeshDesc;
			(actorSource->getShapes())[i]->isConvexMesh()->saveToDesc(*convexShapeDesc);
					
			//MemoryWriteBuffer buf;
			//bool status = CookConvexMesh(convexMeshDesc, buf);
			//if(status){
			//	convexShapeDesc.meshData = gPhysicsSDK->createConvexMesh(MemoryReadBuffer(buf.data));
				actorDesc.shapes.push_back(convexShapeDesc);
			//}

			/*	NxConvexShapeDesc robotShape[1];
			if(robotShapes)
			{
			static NxConvexMeshDesc convexMesh;
			robotShapes[0]->isConvexMesh()->getConvexMesh().saveToDesc(convexMesh);

			MemoryWriteBuffer buf;
			bool status = CookConvexMesh(convexMesh, buf);
			if(status)
			{
			robotShape[0].meshData = Simulation::gPhysicsSDK->createConvexMesh(MemoryReadBuffer(buf.data));
			vehicleDesc.robotShapes.pushBack(&robotShape[0]);

			}
			}*/
		}
		else if(type==NxShapeType::NX_SHAPE_SPHERE){
			NxSphereShapeDesc* shapeDesc = new NxSphereShapeDesc();
			(actorSource->getShapes())[i]->isSphere()->saveToDesc(*shapeDesc);	
			actorDesc.shapes.push_back(shapeDesc);
		}
		else if(type==NxShapeType::NX_SHAPE_WHEEL){ //Tirei pra num clonar 2 vezes as shapes das rodas na rotina cloneRobot 
			//NxWheelShapeDesc* shapeDesc = new NxWheelShapeDesc();
			//(actorSource->getShapes())[i]->isWheel()->saveToDesc(*shapeDesc);	
			//actorDesc.shapes.push_back(shapeDesc);
		}
		else if(type==NxShapeType::NX_SHAPE_PLANE){
			NxPlaneShapeDesc* shapeDesc = new NxPlaneShapeDesc();
			(actorSource->getShapes())[i]->isPlane()->saveToDesc(*shapeDesc);	
			actorDesc.shapes.push_back(shapeDesc);
		}
		else if(type==NxShapeType::NX_SHAPE_CAPSULE){
			NxCapsuleShapeDesc* shapeDesc = new NxCapsuleShapeDesc();
			(actorSource->getShapes())[i]->isCapsule()->saveToDesc(*shapeDesc);	
			actorDesc.shapes.push_back(shapeDesc);
		}
		else{
			printf("SHAPE NAO CLONADA\n");
		}
	}
	return gScenes[indexDestScene]->scene->createActor(actorDesc);
}

NxShapeDesc* Simulation::copyShapeDesc(NxShape* shapeSource){
	NxShapeType type = shapeSource->getType();
	if(type==NxShapeType::NX_SHAPE_BOX){
		NxBoxShapeDesc* boxDesc = new NxBoxShapeDesc();
		NxBoxShape* boxShape = shapeSource->isBox();
		boxShape->saveToDesc(*boxDesc);
		return boxDesc;
	}
	else if(type==NxShapeType::NX_SHAPE_CONVEX){
		NxConvexShapeDesc* convexShapeDesc = new NxConvexShapeDesc();
		//static NxConvexMeshDesc convexMeshDesc;
		shapeSource->isConvexMesh()->saveToDesc(*convexShapeDesc);
		return convexShapeDesc;

		//MemoryWriteBuffer buf;
		//bool status = CookConvexMesh(convexMeshDesc, buf);
		//if(status){
		//	convexShapeDesc.meshData = gPhysicsSDK->createConvexMesh(MemoryReadBuffer(buf.data);
		//}

		/*	NxConvexShapeDesc robotShape[1];
		if(robotShapes)
		{
		static NxConvexMeshDesc convexMesh;
		robotShapes[0]->isConvexMesh()->getConvexMesh().saveToDesc(convexMesh);

		MemoryWriteBuffer buf;
		bool status = CookConvexMesh(convexMesh, buf);
		if(status)
		{
		robotShape[0].meshData = Simulation::gPhysicsSDK->createConvexMesh(MemoryReadBuffer(buf.data));
		vehicleDesc.robotShapes.pushBack(&robotShape[0]);

		}
		}*/
	}
	else if(type==NxShapeType::NX_SHAPE_SPHERE){
		NxSphereShapeDesc* shapeDesc = new NxSphereShapeDesc();
		shapeSource->isSphere()->saveToDesc(*shapeDesc);	
		return shapeDesc;
	}
	else if(type==NxShapeType::NX_SHAPE_WHEEL){
		NxWheelShapeDesc* shapeDesc = new NxWheelShapeDesc();
		shapeSource->isWheel()->saveToDesc(*shapeDesc);	
		return shapeDesc;		
	}
	else if(type==NxShapeType::NX_SHAPE_PLANE){
		NxPlaneShapeDesc* shapeDesc = new NxPlaneShapeDesc();
		shapeSource->isPlane()->saveToDesc(*shapeDesc);	
		return shapeDesc;
	}
	else if(type==NxShapeType::NX_SHAPE_CAPSULE){
		NxCapsuleShapeDesc* shapeDesc = new NxCapsuleShapeDesc();
		shapeSource->isCapsule()->saveToDesc(*shapeDesc);	
		return shapeDesc;
	}
	else
		return NULL;
}

void Simulation::cloneScene(int indexSceneSource, int indexCloneScene){
	NxScene1* sceneSource = gScenes[indexSceneSource];

	NxSceneDesc sceneDesc;
	sceneSource->scene->saveToDesc(sceneDesc);

	NxScene1* scene1 = new NxScene1(this);
	gScenes.insert(indexCloneScene, scene1);
	scene1->scene = gPhysicsSDK->createScene(sceneDesc);
	scene1->indexScene = indexCloneScene;

	//Clone Robots
	NxArray<NxRobot*> robots = sceneSource->allRobots->getRobots();
	for(int i=0; i<robots.size(); i++){
		robots[i]->cloneRobot(indexCloneScene, robots[i]->getId(), robots[i]->getPos(), robots[i]->getIdTeam());
	}

	//Clone Field
	sceneSource->field->cloneField(indexCloneScene);

	//Clone Ball
	sceneSource->ball->cloneBall(indexCloneScene);

	//Contact Report (List of Collisions)
	NxUserContactReport * contactReport = new MyContactReport(this);
	robotContactReport.push_back(contactReport);
	scene1->scene->setUserContactReport(contactReport);
	NxActor** actors = scene1->scene->getActors();
	//TODO: Executar o setActorPairFlags quando incluir um actor na scene (cloneBall, cloneRobot)
	for(int i=0; i<scene1->scene->getNbActors()-1; i++){
		scene1->scene->setActorPairFlags(*(actors[i]), *(actors[i+1]), NX_NOTIFY_ON_TOUCH);
	}

	//NxActor** actors = gScenes[indexSource]->getActors();
	//for(int i=0; i<gScenes[indexSource]->getNbActors(); i++)
	//	cloneActor(actors[i], gScenes.size()-1);

	//gScenes[indexSource]->resetJointIterator();
	//for(int i=0; i<gScenes[indexSource]->getNbJoints(); i++){
	//	NxJoint* joint = gScenes[indexSource]->getNextJoint();
	//	if (joint!=NULL)
	//		cloneJoint(joint, gScenes.size()-1);
	//}
}

void Simulation::deleteScene(int indexScene){
	NxScene1* sceneSource = gScenes[indexScene];
	gScenes.remove(indexScene);

	if(sceneSource)
		delete sceneSource;
}

//==================================================================================
//This is the function that calls the importer to load the state of the currently selected scene from a file
NX_BOOL Simulation::LoadScene(const char *pFilename,NXU::NXU_FileType type)
{
	NX_BOOL success = false;

	if (Simulation::gPhysicsSDK)
	{
		char LoadFilename[512];
		FindMediaFile(pFilename, LoadFilename);

		NXU::NxuPhysicsCollection *c = NXU::loadCollection(LoadFilename, type );

		if ( c )
		{
			if(Simulation::gScenes[Simulation::gBaseScene]->scene){
				if (Simulation::gScenes[Simulation::gBaseScene]->scene!=NULL){
					// Release old scene
					Simulation::releaseScene(*(Simulation::gScenes[Simulation::gBaseScene]->scene));
					Simulation::gScenes[Simulation::gBaseScene]->scene = 0;
				}
			}

			if (Simulation::gPhysicsSDK)
			{
				success = NXU::instantiateCollection( c, *this->gPhysicsSDK, 0, 0, this->gUserNotify );
			}
			NXU::releaseCollection(c);
		}
		else
		{
		}
	}

	Simulation::gLoad = 0;

	if ( success ){
		printf("Cena %d carregada do arquivo %s.\n", Simulation::gBaseScene, pFilename);
	}

	return success;
}
//==================================================================================
//This is the function that calls the exporter to save out the state of the currently selected scene to a file
void Simulation::SaveScene(const char *pFilename)
{
	char SaveFilename[512];
	GetTempFilePath(SaveFilename);
	strcat(SaveFilename, pFilename);

	NXU::setUseClothActiveState(true);
	NXU::setUseSoftBodyActiveState(true);

	NXU::NxuPhysicsCollection *c = NXU::extractCollectionScene(Simulation::gScenes[Simulation::gBaseScene]->scene);

	if (c)
	{
		char scratch[512];
		sprintf(scratch, "%s.xml", SaveFilename);
		printf("Saving NxuStream XML file to '%s'\r\n", scratch);
		NXU::saveCollection(c, scratch, NXU::FT_XML, false, false);

		sprintf(scratch, "%s.nxb", SaveFilename);
		printf("Saving NxuStream BINARY file to '%s'\r\n", scratch);
		NXU::saveCollection(c, scratch, NXU::FT_BINARY, true, true);

		sprintf(scratch, "%s.dae", SaveFilename);
		printf("Saving COLLADA Physics 1.4.1 file to '%s'\r\n", scratch);
		NXU::saveCollection(c, scratch, NXU::FT_COLLADA, false, false);

		NXU::releaseCollection(c);

	}

}
//==================================================================================
bool Simulation::FileExistTestSimple(const char *fname)
{
	char buff[512];
	FindMediaFile(fname, buff);

	FILE *fp=fopen(buff,"rb");
	if(fp == NULL)
	{
		return false;
	}
	else
	{
		fclose(fp);
		return true;
	}
}

void Simulation::CSL_Scene()
{
	//Check if a request was made to clear, save or load a scene
	if (Simulation::gClear)
	{
		if (Simulation::gScenes[Simulation::gBaseScene]->scene)
		{
			Simulation::gPhysicsSDK->releaseScene(*(Simulation::gScenes[Simulation::gBaseScene]->scene));
		}
		//InitBlankScene();
		Simulation::gClear = false;
	}
	else if (Simulation::gSave)
	{
		char scratch[512];
		sprintf(scratch,"SceneSave%d", Simulation::gBaseScene );
		SaveScene(scratch);
		Simulation::gSave = false;
	}
	else if (Simulation::gLoad)
	{
		switch ( Simulation::gLoad )
		{
		case 1:
			if ( 1 )
			{
				char scratch[512];
				sprintf(scratch,"SceneSave%d.nxb", Simulation::gBaseScene ); // load the binary saved version.
				LoadScene(scratch, NXU::FT_BINARY);
			}
			break;
		case 2:
			LoadScene("test.xml", NXU::FT_XML);
			break;
		case 3:
			LoadScene("test.dae", NXU::FT_COLLADA);
			break;
		case 4:
			if(FileExistTestSimple("test.nxb"))
			{
				LoadScene("test.nxb", NXU::FT_BINARY);
			}
			else
			{
				LoadScene("test.xml", NXU::FT_XML);
				SaveScene("testbin");
			}
			break;
		}
	}
}

bool Simulation::initSimulation( int nbScenes )
{
	bool init = Simulation::InitNx();
	CSL_Scene();

	if(gScenes[gBaseScene]->scene != NULL)
	{
		buildModelRobot( 4, Simulation::gBaseScene, 1 );
		buildModelField( Simulation::gBaseScene );
		buildModelBall( Simulation::gBaseScene );
	}

	//Build Scene
	//	Contact Report (List of Collisions)
	NxUserContactReport * contactReport = new MyContactReport(this);
	robotContactReport.push_back(contactReport);
	gScenes[gBaseScene]->scene->setUserContactReport(contactReport);
	NxActor** actors = gScenes[gBaseScene]->scene->getActors();
	//TODO: Executar o setActorPairFlags quando incluir um actor na scene (cloneBall, cloneRobot)
	for(int i=0; i<gScenes[gBaseScene]->scene->getNbActors()-1; i++){
		gScenes[gBaseScene]->scene->setActorPairFlags(*(actors[i]), *(actors[i+1]), NX_NOTIFY_ON_TOUCH);
	}

	/*NxMaterial *defaultMaterial0 = Simulation::gScenes[Simulation::gBaseScene]->getMaterialFromIndex(0);
	defaultMaterial0->setRestitution(0.5f);
	defaultMaterial0->setStaticFriction(0.3f);
	defaultMaterial0->setDynamicFriction(0.3f);

	NxMaterial *defaultMaterial1 = Simulation::gScenes[Simulation::gBaseScene]->getMaterialFromIndex(1);
	//NxReal real2 = defaultMaterial1->getRestitution();
	//NxReal real = defaultMaterial1->getStaticFriction();
	//NxReal real1 = defaultMaterial1->getDynamicFriction();
	defaultMaterial1->setRestitution(0.5f);
	defaultMaterial1->setStaticFriction(0.3f);
	defaultMaterial1->setDynamicFriction(0.3f);

	NxMaterial *defaultMaterial2 = Simulation::gScenes[Simulation::gBaseScene]->getMaterialFromIndex(2);
	defaultMaterial2->setRestitution(0.5f);
	defaultMaterial2->setStaticFriction(0.3f);
	defaultMaterial2->setDynamicFriction(0.3f);*/
	
	for(int i=0; i<Simulation::gScenes[Simulation::gBaseScene]->scene->getNbMaterials(); i++){
		NxMaterial *defaultMaterial = Simulation::gScenes[Simulation::gBaseScene]->scene->getMaterialFromIndex(i);
		defaultMaterial->setRestitution(0.5);//0.5f);//     //TODO: LEVANTAR PARAMETROS
		defaultMaterial->setStaticFriction(0.5);//0.3f);//  //TODO: LEVANTAR PARAMETROS
		defaultMaterial->setDynamicFriction(0.4);//0.3f);//	//TODO: LEVANTAR PARAMETROS
		//TODO: USAR NxCombineMode
	}
	
	//Simulation::allFields.fields[Simulation::gBaseScene].setDimensions(7400, 5400, 6000, 0, 200., 700., 160.);
	
	NxAllRobots* allRobots = Simulation::gScenes[Simulation::gBaseScene]->allRobots;
	NxReal z = allRobots->getRobotByIdByTeam(4, 1)->getInitialPose().t.z;
	z=0;
	allRobots->getRobotByIdByTeam(4, 1)->setGlobalPosition(NxVec3(-3000, 0, z));
	allRobots->getRobotByIdByTeam(4, 1)->cloneRobot(Simulation::gBaseScene, 3, NxVec3(-2000, 1000, z), 1);
	allRobots->getRobotByIdByTeam(4, 1)->cloneRobot(Simulation::gBaseScene, 2, NxVec3(-2000, -1000, z), 1);
	allRobots->getRobotByIdByTeam(4, 1)->cloneRobot(Simulation::gBaseScene, 1, NxVec3(-1000, 1000, z), 1);
	allRobots->getRobotByIdByTeam(4, 1)->cloneRobot(Simulation::gBaseScene, 0, NxVec3(-1000, -1000, z), 1);
	allRobots->getRobotByIdByTeam(4, 1)->cloneRobot(Simulation::gBaseScene, 4, NxVec3(3000, 0, 0), z);
	allRobots->getRobotByIdByTeam(4, 1)->cloneRobot(Simulation::gBaseScene, 3, NxVec3(2000, -1000, z), 0);
	allRobots->getRobotByIdByTeam(4, 1)->cloneRobot(Simulation::gBaseScene, 2, NxVec3(2000, 1000, z), 0);
	allRobots->getRobotByIdByTeam(4, 1)->cloneRobot(Simulation::gBaseScene, 1, NxVec3(1000, -1000, z), 0);
	allRobots->getRobotByIdByTeam(4, 1)->cloneRobot(Simulation::gBaseScene, 0, NxVec3(1000, 1000, z), 0);


	for(int i=1; i<nbScenes; i++)
		cloneScene(gBaseScene, i);
	//deleteScene(5);

	// Initialize physics scene and start the application main loop if scene was created
	return init; 
}

string Simulation::parseLegacyString(string s) {
	string temp;
	std::stringstream os(s);
	os >> temp;
	string sendString;

	if(temp.compare("15") == 0) //pacote 15 CONTROL ALL ROBOTS FROM TEAM(CONTROL WHEELS)
	{
		//Lendo argumentos
		os >> temp;
		int indexScene = atoi(temp.c_str());

		NxScene1* scene = gScenes[indexScene];

		//Definindo string para enviar
		stringstream out;

		os >> temp;
		int indexTeam = atoi(temp.c_str());

		//Lendo argumentos do robo e controlando
		NxAllRobots* robots = scene->allRobots;
		for(int indexRobot=0; indexRobot<5; indexRobot++)
			//for(int indexRobot=1; indexRobot<=5; indexRobot++)
		{
			os >> temp;
			float speedWheel1 = atof(temp.c_str());
			os >> temp;
			float speedWheel2 = atof(temp.c_str());
			os >> temp;
			float speedWheel3 = atof(temp.c_str());
			os >> temp;
			float speedWheel4 = atof(temp.c_str());
			os >> temp;
			float dribblerSpeed = atof(temp.c_str());
			os >> temp;
			float kickerSpeed = atof(temp.c_str());

			//printf("%f %f %f %f\n",speedWheel1, speedWheel2, speedWheel3, speedWheel4);

			/*if(indexRobot==1){
			printf("ROBO1 DOIDO: %f %f %f %f\n", speedWheel1, speedWheel2, speedWheel3, speedWheel4);
			}
			if(indexRobot==2){
			printf("ROBO2 NORMAL: %f %f %f %f\n", speedWheel1, speedWheel2, speedWheel3, speedWheel4);
			}*/

			robots->getRobotByIdByTeam(indexRobot, indexTeam)->controlRobotByWheels(speedWheel1, speedWheel2, speedWheel3, speedWheel4, dribblerSpeed, kickerSpeed);
		}

		out << "ACK 15\n"; // confirmando pacote 15

		//Construindo string para enviar
		sendString = out.str();
	}
	else if(temp.compare("5") == 0) //pacote 5 SIMULATE
	{
		//Lendo argumentos
		os >> temp;
		int indexScene = atoi(temp.c_str());

		os >> temp;
		qreal timeStep = atof(temp.c_str());

		//Definindo string para enviar
		stringstream out;

		this->simulate(indexScene, timeStep);

		out << "ACK 5\n"; // confirmando pacote 5

		//Construindo string para enviar
		sendString = out.str();
	}
	else if(temp.compare("18") == 0){//INICIALIZAR A SIMULACAO
		os >> temp;
		int nbScenes = atoi(temp.c_str());

		//Definindo string para enviar
		stringstream out;

		this->initSimulation(nbScenes);

		out << "ACK 18\n"; // confirmando pacote 18

		//Construindo string para enviar
		sendString = out.str();
	}
	else if(temp.compare("3") == 0) //pacote 3 SET DYNAMIC VALUES
	{
		//Lendo argumentos
		os >> temp;
		int indexScene = atoi(temp.c_str());
		//os >> temp;
		//int indexTeam = atoi(temp.c_str());

		NxScene1* scene = gScenes[indexScene];

		NxAllRobots* robots = scene->allRobots;
		for(int indexTeam=0; indexTeam<2; indexTeam++){
			for(int indexRobot=0; indexRobot<5; indexRobot++)
				//for(int indexRobot=1; indexRobot<=5; indexRobot++)
			{
				os >> temp;
				float x = atof(temp.c_str());
				os >> temp;
				float y = atof(temp.c_str());
				os >> temp;
				float ang = atof(temp.c_str());
				os >> temp;
				float speedX = atof(temp.c_str());
				os >> temp;
				float speedY = atof(temp.c_str());
				os >> temp;
				float speedAng = atof(temp.c_str());
				os >> temp;
				float kickerSpeed = atof(temp.c_str());
				os >> temp;
				float dribblerSpeed = atof(temp.c_str());


				/*if(indexRobot==1){
				printf("ROBO1 DOIDO: %f %f %f %f\n", speedWheel1, speedWheel2, speedWheel3, speedWheel4);
				}
				if(indexRobot==2){
				printf("ROBO2 NORMAL: %f %f %f %f\n", speedWheel1, speedWheel2, speedWheel3, speedWheel4);
				}*/

				NxRobot* robot = robots->getRobotByIdByTeam(indexRobot, indexTeam);
				NxMat34 initPose = robot->getInitialPose();
				robot->setGlobalPosition(NxVec3(x, y, initPose.t.z));
				NxMat33 newOri = initPose.M;
				newOri.rotZ(ang);
				robot->setGlobalOrientation(newOri);
				//robot->putToSleep();
				NxActor* actor = robot->getActor();
				actor->setLinearVelocity(NxVec3(speedX, speedY, 0));
				actor->setAngularVelocity(NxVec3(0, 0, speedAng));
				robot->dribbler->speedToExecute = dribblerSpeed;
				robot->kicker->controlKicker(kickerSpeed, robot);
			}
		}

		//bola
		os >> temp;
		float x = atof(temp.c_str());
		os >> temp;
		float y = atof(temp.c_str());
		os >> temp;
		float speedX = atof(temp.c_str());
		os >> temp;
		float speedY = atof(temp.c_str());
		os >> temp;
		float speedAng = atof(temp.c_str());
		NxBall* ball = scene->ball;
		NxMat34 initPose = ball->initialPose;
		NxActor* actor = ball->ball;
		actor->setGlobalPosition(NxVec3(x, y, initPose.t.z));
		//ball->putToSleep();
		actor->setLinearVelocity(NxVec3(speedX, speedY, 0));
		actor->setAngularVelocity(NxVec3(0, 0, speedAng));

		//Definindo string para enviar
		stringstream out;
		out << "ACK 3\n"; // confirmando pacote 3

		//Construindo string para enviar
		sendString = out.str();
	}
	else if(temp.compare("16") == 0){ //CLONAR CENA
		//Lendo argumentos
		os >> temp;
		int indexSourceScene = atoi(temp.c_str());
		os >> temp;
		int indexCloneScene = atoi(temp.c_str());

		//Definindo string para enviar
		stringstream out;

		this->cloneScene(indexSourceScene, indexCloneScene);

		out << "ACK 16\n"; // confirmando pacote 16

		//Construindo string para enviar
		sendString = out.str();
	}
	else if(temp.compare("17") == 0){//DELETAR CENA
		//Lendo argumentos
		os >> temp;
		int indexScene = atoi(temp.c_str());

		//Definindo string para enviar
		stringstream out;

		this->deleteScene(indexScene);

		out << "ACK 17\n"; // confirmando pacote 17

		//Construindo string para enviar
		sendString = out.str();
	}
	else if(temp.compare("0") == 0) //pacote 0 GET STATIC VALUES (WORLDMODEL TB PASSA ISSO)
	{
		//Lendo argumentos
		os >> temp;
		int indexScene = atoi(temp.c_str());

		//Definindo string para enviar
		stringstream out;

		//Parametros de Simulacao
		out << this->timeStep;
		//out << " ";

		out << "\n";

		//Construindo string para enviar
		sendString = out.str();
	}
	else if(temp.compare("1") == 0) //pacote 1 GET DYNAMIC VALUES
	{
		//Lendo argumentos
		os >> temp;
		int indexScene = atoi(temp.c_str());

		os >> temp;
		int indexTeam = atoi(temp.c_str());

		//Definindo string para enviar
		stringstream out;

		////Bola
		//NxVec3 ballPos = this->gScenes[indexScene]->ball->ball->getGlobalPosition();
		//out << ballPos.x;
		//out << " ";
		//out << ballPos.y;
		////Robo
		//NxAllRobots* robots = this->gScenes[indexScene]->allRobots;
		//for(int indexRobot=0; indexRobot<5; indexRobot++)
		//	//for(int indexRobot=1; indexRobot<=5; indexRobot++)
		//{
		//	NxRobot* robot = robots->getRobotByIdByTeam(indexRobot, indexTeam);
		//	NxVec3 robotPos = robot->getPos();
		//	NxReal robotAngle = robot->getAngle2DFromVehicle();
		//	out << " ";
		//	out << robotPos.x;
		//	out << " ";
		//	out << robotPos.y;
		//	out << " ";
		//	out << robotAngle;
		//}
		////Motor
		////Driblador
		////Chutador


		out << "\n";

		//Construindo string para enviar
		sendString = out.str();
	}
	else if(temp.compare("2") == 0) //pacote 2 SET STATIC VALUES
	{

	}
	else if(temp.compare("4") == 0) //pacote 4 CONTROL ALL ROBOTS
	{
		//Lendo argumentos
		os >> temp;
		int indexScene = atoi(temp.c_str());
		os >> temp;
		int indexTeam = atoi(temp.c_str());

		//Definindo string para enviar
		stringstream out;

		//Lendo argumentos do robo e controlando
		NxAllRobots* robots = this->gScenes[indexScene]->allRobots;
		for(int indexRobot=0; indexRobot<5; indexRobot++)
			//for(int indexRobot=1; indexRobot<=5; indexRobot++)
		{
			os >> temp;
			float speedX = atof(temp.c_str());
			os >> temp;
			float speedY = atof(temp.c_str());
			os >> temp;
			float speedAng = atof(temp.c_str());
			os >> temp;
			float dribblerSpeed = atof(temp.c_str());
			os >> temp;
			float kickerSpeed = atof(temp.c_str());

			NxRobot* robot = robots->getRobotByIdByTeam(indexRobot, indexTeam);
			robot->controlRobot(speedX, speedY, speedAng, dribblerSpeed, kickerSpeed);
		}

		//this->simulate();

		out << "ACK 4\n"; // confirmando pacote 4

		//Construindo string para enviar
		sendString = out.str();
	}
	else if(temp.compare("6") == 0) //pacote 6 GO TO THIS POSE FOR ALL ROBOTS
	{
		//Lendo argumentos
		os >> temp;
		int indexScene = atoi(temp.c_str());
		os >> temp;
		int indexTeam = atoi(temp.c_str());

		//Definindo string para enviar
		stringstream out;

		//Lendo argumentos e executando goToThisPose
		NxAllRobots* robots = this->gScenes[indexScene]->allRobots;
		for(int indexRobot=0; indexRobot<5; indexRobot++)
			//for(int indexRobot=1; indexRobot<=5; indexRobot++)
		{
			os >> temp;
			float x = atof(temp.c_str());
			os >> temp;
			float y = atof(temp.c_str());
			os >> temp;
			float angle = atof(temp.c_str());
			robots->getRobotByIdByTeam(indexRobot, indexTeam)->goToThisPose(x, y, angle);
		}

		out << "ACK 6\n"; // confirmando pacote 6

		//Construindo string para enviar
		sendString = out.str();
	}
	else if(temp.compare("7") == 0) //pacote 7 INFINITE PATH
	{
		//Lendo argumentos
		os >> temp;
		int indexScene = atoi(temp.c_str());
		os >> temp;
		int indexTeam = atoi(temp.c_str());

		//Definindo string para enviar
		stringstream out;

		//Lendo argumentos e executando InfinePath
		NxAllRobots* robots = this->gScenes[indexScene]->allRobots;
		for(int indexRobot=0; indexRobot<5; indexRobot++)
			//for(int indexRobot=1; indexRobot<=5; indexRobot++)
		{
			robots->getRobotByIdByTeam(indexRobot, indexTeam)->infinitePath();
		}

		out << "ACK 7\n"; // confirmando pacote 7

		//Construindo string para enviar
		sendString = out.str();
	}
	else if(temp.compare("8") == 0) //pacote 8 CONTROL DRIBBLER AND KICKER ALL ROBOTS
	{
		//Lendo argumentos
		os >> temp;
		int indexScene = atoi(temp.c_str());
		os >> temp;
		int indexTeam = atoi(temp.c_str());

		//Definindo string para enviar
		stringstream out;

		//Lendo argumentos do robo e controlando
		NxAllRobots* robots = this->gScenes[indexScene]->allRobots;
		for(int indexRobot=0; indexRobot<5; indexRobot++)
			//for(int indexRobot=1; indexRobot<=5; indexRobot++)
		{
			os >> temp;
			float dribblerSpeed = atof(temp.c_str());
			os >> temp;
			float kickerSpeed = atof(temp.c_str());

			NxRobot* robot = robots->getRobotByIdByTeam(indexRobot, indexTeam);
			robot->dribbler->controlDribbler( dribblerSpeed );
			robot->kicker->controlKicker( kickerSpeed, robot );
		}

		out << "ACK 8\n"; // confirmando pacote 8

		//Construindo string para enviar
		sendString = out.str();
	}
	else if(temp.compare("9") == 0) //pacote 9 GO TO THIS POSE ONE ROBOT
	{
		//Lendo argumentos
		os >> temp;
		int indexScene = atoi(temp.c_str());
		os >> temp;
		int indexTeam = atoi(temp.c_str());
		os >> temp;
		int indexRobot = atoi(temp.c_str());

		//Definindo string para enviar
		stringstream out;

		//Lendo argumentos e executando goToThisPose
		os >> temp;
		float x = atof(temp.c_str());
		os >> temp;
		float y = atof(temp.c_str());
		os >> temp;
		float angle = atof(temp.c_str());

		this->gScenes[indexScene]->allRobots->getRobotByIdByTeam(indexRobot, indexTeam)->goToThisPose(x, y, angle);

		out << "ACK 9\n"; // confirmando pacote 9

		//Construindo string para enviar
		sendString = out.str();
	}
	else if(temp.compare("10") == 0) //pacote 10 SIMULATE THE INDEXSCENE WITH DT TIME
	{
		//Lendo argumentos
		os >> temp;
		int indexScene = atoi(temp.c_str());
		os >> temp;
		float dt = atof(temp.c_str());

		//Definindo string para enviar
		stringstream out;

		this->simulate(indexScene, dt);

		out << "ACK 10\n"; // confirmando pacote 10

		//Construindo string para enviar
		sendString = out.str();
	}
	else if(temp.compare("11") == 0) //pacote 11 CONTROL ONE ROBOT
	{
		//Lendo argumentos
		os >> temp;
		int indexScene = atoi(temp.c_str());

		//Definindo string para enviar
		stringstream out;

		//Lendo argumentos do robo e controlando
		os >> temp;
		int indexTeam = atoi(temp.c_str());
		os >> temp;
		int indexRobot = atoi(temp.c_str());

		os >> temp;
		float speedX = atof(temp.c_str());
		os >> temp;
		float speedY = atof(temp.c_str());
		os >> temp;
		float speedAng = atof(temp.c_str());
		os >> temp;
		float dribblerSpeed = atof(temp.c_str());
		os >> temp;
		float kickerSpeed = atof(temp.c_str());

		this->gScenes[indexScene]->allRobots->getRobotByIdByTeam(indexRobot, indexTeam)->controlRobot(speedX, speedY, speedAng, dribblerSpeed, kickerSpeed);

		out << "ACK 11\n"; // confirmando pacote 11

		//Construindo string para enviar
		sendString = out.str();
	}
	else if(temp.compare("12") == 0) //pacote 12 CONTROL DRIBBLER AND KICKER ONE ROBOT
	{
		//Lendo argumentos
		os >> temp;
		int indexScene = atoi(temp.c_str());

		//Definindo string para enviar
		stringstream out;

		//Lendo argumentos do robo e controlando
		os >> temp;
		int indexTeam = atoi(temp.c_str());
		os >> temp;
		int indexRobot = atoi(temp.c_str());
		os >> temp;
		float dribblerSpeed = atof(temp.c_str());
		os >> temp;
		float kickerSpeed = atof(temp.c_str());

		NxRobot* robot = this->gScenes[indexScene]->allRobots->getRobotByIdByTeam(indexRobot, indexTeam);
		robot->dribbler->controlDribbler( dribblerSpeed);
		robot->kicker->controlKicker( kickerSpeed, robot );

		out << "ACK 12\n"; // confirmando pacote 12

		//Construindo string para enviar
		sendString = out.str();
	}
	else if(temp.compare("13") == 0) //pacote 13 CONTROL ONE ROBOT (CONTROL WHEELS)
	{
		//Lendo argumentos
		os >> temp;
		int indexScene = atoi(temp.c_str());

		//Definindo string para enviar
		stringstream out;

		//Lendo argumentos do robo e controlando
		os >> temp;
		int indexTeam = atoi(temp.c_str());
		os >> temp;
		int indexRobot = atoi(temp.c_str());

		os >> temp;
		float speedWheel1 = atof(temp.c_str());
		os >> temp;
		float speedWheel2 = atof(temp.c_str());
		os >> temp;
		float speedWheel3 = atof(temp.c_str());
		os >> temp;
		float speedWheel4 = atof(temp.c_str());
		os >> temp;
		float dribblerSpeed = atof(temp.c_str());
		os >> temp;
		float kickerSpeed = atof(temp.c_str());

		this->gScenes[indexScene]->allRobots->getRobotByIdByTeam(indexRobot, indexTeam)->controlRobotByWheels(speedWheel1, speedWheel2, speedWheel3, speedWheel4, dribblerSpeed, kickerSpeed);

		out << "ACK 13\n"; // confirmando pacote 13

		//Construindo string para enviar
		sendString = out.str();
	}
	else if(temp.compare("14") == 0) //pacote 14 CONTROL ALL ROBOTS (CONTROL WHEELS)
	{
		//Lendo argumentos
		os >> temp;
		int indexScene = atoi(temp.c_str());
		os >> temp;
		int indexTeam = atoi(temp.c_str());

		//Definindo string para enviar
		stringstream out;

		//Lendo argumentos do robo e controlando
		NxAllRobots* robots = this->gScenes[indexScene]->allRobots;
		for(int indexRobot=0; indexRobot<5; indexRobot++)
			//for(int indexRobot=1; indexRobot<=5; indexRobot++)
		{
			os >> temp;
			float speedWheel1 = atof(temp.c_str());
			os >> temp;
			float speedWheel2 = atof(temp.c_str());
			os >> temp;
			float speedWheel3 = atof(temp.c_str());
			os >> temp;
			float speedWheel4 = atof(temp.c_str());
			os >> temp;
			float dribblerSpeed = atof(temp.c_str());
			os >> temp;
			float kickerSpeed = atof(temp.c_str());

			robots->getRobotByIdByTeam(indexRobot, indexTeam)->controlRobotByWheels(speedWheel1, speedWheel2, speedWheel3, speedWheel4, dribblerSpeed, kickerSpeed);
		}

		out << "ACK 14\n"; // confirmando pacote 14

		//Construindo string para enviar
		sendString = out.str();
	}
	else
	{
		sendString = "NACK";
	}

	return sendString;
}

SSL_WrapperPacket Simulation::getSSLWrapper(int sceneNumber){
	// Verify that the version of the library that we linked against is
	// compatible with the version of the headers we compiled against.
	GOOGLE_PROTOBUF_VERIFY_VERSION;

	SSL_WrapperPacket wrapperPacket;
	static unsigned long int frameNumber = 0;
	
	//timeval tv;
	//TimePosix::gettimeofday(&tv,NULL);
	//double time = (double)tv.tv_sec + tv.tv_usec*(1.0E-6);
	double time = QDateTime::currentMSecsSinceEpoch() * 1.0E-3;

	if(true)//withDetection)
	{

		NxScene1* scene1 = this->gScenes[sceneNumber];

		SSL_DetectionFrame detectionFrame = SSL_DetectionFrame();

		detectionFrame.set_frame_number(frameNumber);
		detectionFrame.set_camera_id(0);
		detectionFrame.set_t_capture(time);
		detectionFrame.set_t_sent(time);

		NxVec3 ballGlobalPos = scene1->ball->ball->getGlobalPosition();
		SSL_DetectionBall* detectionBall = detectionFrame.add_balls();
		detectionBall->set_area(1);
		detectionBall->set_confidence(1);
		detectionBall->set_pixel_x(0);
		detectionBall->set_pixel_y(0);
		detectionBall->set_x(ballGlobalPos.x);
		detectionBall->set_y(ballGlobalPos.y);
		detectionBall->set_z(1);

		NxArray<NxRobot*> allRobots = scene1->allRobots->getRobots();
		for(int i=0; i<allRobots.size(); i++)
		{
			NxRobot* robot = allRobots[i];
			NxVec3 robotPos = robot->getPos();
			SSL_DetectionRobot* detectionRobot;
			
			if (robot->getIdTeam()==1)
				detectionRobot = detectionFrame.add_robots_blue();
			else //if (robot->getIdTeam()==0)
				detectionRobot = detectionFrame.add_robots_yellow();
			
			detectionRobot->set_confidence(1);
			detectionRobot->set_height(150);
			detectionRobot->set_orientation(robot->getAngle2DFromVehicle());
			detectionRobot->set_pixel_x(0);
			detectionRobot->set_pixel_y(0);
			detectionRobot->set_robot_id(robot->getId()/*-1*/); //FOI SUBTRAIDO 1 PQ NO TEAMBOTS OS INDICES DOS ROBOS VAI DE 0 A 4
			detectionRobot->set_x(robotPos.x);
			detectionRobot->set_y(robotPos.y);
		}

		SSL_DetectionFrame * nframe = wrapperPacket.mutable_detection();
		nframe->CopyFrom(detectionFrame);
	}
	if(true)//withGeometry)
	{
		NxField* field = this->gScenes[sceneNumber]->field;

		SSL_GeometryData geometryData = SSL_GeometryData();

		SSL_GeometryFieldSize* geometryFieldSize = geometryData.mutable_field();
		geometryFieldSize->set_boundary_width(field->boundary_width);
		geometryFieldSize->set_center_circle_radius(field->center_circle_radius);
		geometryFieldSize->set_defense_radius(field->defense_radius);
		geometryFieldSize->set_defense_stretch(field->defense_stretch);
		geometryFieldSize->set_field_length(field->linesLength);
		geometryFieldSize->set_field_width(field->linesWidth);
		geometryFieldSize->set_free_kick_from_defense_dist(field->free_kick_from_defense_dist);
		geometryFieldSize->set_goal_depth(field->goalLength);
		geometryFieldSize->set_goal_wall_width(field->goal_wall_width);
		geometryFieldSize->set_goal_width(field->goalWidth);
		geometryFieldSize->set_line_width(field->line_width);
		geometryFieldSize->set_penalty_line_from_spot_dist(field->penalty_line_from_spot_dist);
		geometryFieldSize->set_penalty_spot_from_field_line_dist(field->penalty_spot_from_field_line_dist);
		geometryFieldSize->set_referee_width(field->referee_width);

		SSL_GeometryCameraCalibration* geometryCameraCalibration = geometryData.add_calib();
		geometryCameraCalibration->set_camera_id(0);
		geometryCameraCalibration->set_derived_camera_world_tx(0);
		geometryCameraCalibration->set_derived_camera_world_ty(0);
		geometryCameraCalibration->set_distortion(0);
		geometryCameraCalibration->set_focal_length(0);
		geometryCameraCalibration->set_principal_point_x(0);
		geometryCameraCalibration->set_principal_point_y(0);
		geometryCameraCalibration->set_q0(0);
		geometryCameraCalibration->set_q1(0);
		geometryCameraCalibration->set_q2(0);
		geometryCameraCalibration->set_q3(0);
		geometryCameraCalibration->set_tx(0);
		geometryCameraCalibration->set_ty(0);
		geometryCameraCalibration->set_tz(0);

		SSL_GeometryData * gdata = wrapperPacket.mutable_geometry();
		gdata->CopyFrom(geometryData);
	}

	frameNumber++;

	// Optional:  Delete all global objects allocated by libprotobuf.
	//google::protobuf::ShutdownProtobufLibrary();

	return wrapperPacket;
}