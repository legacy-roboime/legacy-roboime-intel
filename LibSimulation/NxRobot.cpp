#include "NxRobot.h"
#include "Simulation.h"
#include "NxAllRobots.h"
#include "Stream.h"
#include "cooking.h"

NxRobot::NxRobot(Simulation* simulation) : NxVehicle()
{
	this->simulation = simulation;
	idTeam = 1;
	bodyRadius = 87.5;
	wheelsRadius = 80.6;
	kicker = new NxKicker(simulation);
	dribbler = new NxDribbler();
}

NxRobot::~NxRobot()
{
	delete kicker;
	delete dribbler;
}

void NxRobot::handleContactPair(NxContactPair& pair, NxU32 robotIndex)
{
	NxContactStreamIterator i(pair.stream);

	while(i.goNextPair())
	{
		NxShape * s = i.getShape(robotIndex); //umas das partes do robo (o robo eh um unico actor com as MESHs: rodas, driblador, corpo)
		NxShape * s1 = i.getShape(1-robotIndex); //corpo que o robo esta colidindo

		while(i.goNextPatch())
		{
			const NxVec3& contactNormal = i.getPatchNormal();

			while(i.goNextPoint())
			{
				//user can also call getPoint() and getSeparation() here

				const NxVec3& contactPoint = i.getPoint();

				//add forces:

				/*//assuming front wheel drive we need to apply a force at the wheels.
				if (s->is(NX_SHAPE_CAPSULE) && s->userData != NULL) {
					//assuming only the wheels of the car are capsules, otherwise we need more checks.
					//this branch can't be pulled out of loops because we have to do a full iteration through the stream

					NxQuat local2global = s->getActor().getGlobalOrientationQuat();
					NxWheel* w = (NxWheel*)s->userData;
					if (!w->getWheelFlag(NX_WF_USE_WHEELSHAPE))
					{
						NxWheel1 * wheel = static_cast<NxWheel1*>(w);
						wheel->contactInfo.otherActor = pair.actors[1-robotIndex];
						wheel->contactInfo.contactPosition = contactPoint;

						wheel->contactInfo.contactPositionLocal = contactPoint;
						wheel->contactInfo.contactPositionLocal -= this->getActor()->getGlobalPosition();
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
				}*/
				const char* name = s->getName();
				if(name){
					const char* name1 = s1->getName();
					if(name1){
						char* dribblerName = new char[10]; //"Driblador\0"
						dribblerName[9] = 0;
						memcpy(dribblerName, name, strlen(dribblerName));

						char* ballName = new char[5]; //"Bola\0"
						ballName[4] = 0;
						memcpy(ballName, name1, strlen(ballName));

						if(strcmp(dribblerName, "Driblador")==0 && strcmp(ballName, "Bola")==0){
							float angle = this->getAngle2DFromVehicle();
							angle += NxPi/2.;
							NxActor& ball = s1->getActor();
							NxVec3 dribblerGlobalPosition = this->dribbler->getLocalPosition() + this->getPos();
							NxVec3 r = contactPoint - dribblerGlobalPosition;
							NxVec3 w = NxVec3(cos(angle), sin(angle), 0); 
							NxVec3 force = w.cross(r);
							force.setMagnitude(1);
							NxReal coefKinect = 0.5;
							NxReal normalMagnitude = contactNormal.magnitude();
							NxVec3 resultForce = this->dribbler->speedToExecute * 20. * coefKinect * normalMagnitude * force;
							//NxReal teste =  resultForce.magnitude();
							//SimulationView::DrawForce(&ball, resultForce, NxVec3(1,1,1));
							ball.addForceAtPos(/*NxVec3(sin(angle)*this->dribbler->speedToExecute*1000000.,cos(angle)*this->dribbler->speedToExecute*1000000.,0)*/resultForce, contactPoint, NX_IMPULSE); 
							this->dribbler->speedToExecute = 0;
						}

						delete dribblerName;
						delete ballName;
					}
				}
			}
		}		
	}
	//printf("----\n");
}

int NxRobot::getId()
{
	return id;
}

void NxRobot::setId(int id)
{
	this->id = id;
}

int NxRobot::getIdTeam()
{
	return idTeam;
}

void NxRobot::setIdTeam(int idTeam)
{
	this->idTeam = idTeam;
}

void Simulation::buildModelRobot(int indexRobot, int indexScene, int indexTeam)
{
	//Veiculo descricao
	//Body Descricao
	NxActor* robotActor = Simulation::getActorRobot(indexScene, indexRobot);
	//NxBounds3 bodyBounds;
	//robotShapes[0]->getWorldBounds(bodyBounds);
	NxVehicleDesc vehicleDesc;
	NxReal wheelRadius = 27.6;

	vehicleDesc.position				= NxVec3(robotActor->getGlobalPosition());
	float mass = 3.;
	vehicleDesc.mass					= mass;//robotActor->getMass(); //PLUGIN TAH COM PROBLEMA XML ERRADO
	//vehicleDesc.motorForce				= 70000;
	//vehicleDesc.maxVelocity				= 300.f;
	//vehicleDesc.cameraDistance			= 8.0f;
	vehicleDesc.centerOfMass.set(NxVec3(0,0,0));//robotActor->getCMassLocalPosition());
	//vehicleDesc.differentialRatio = 3.42f;
	//vehicleDesc.transmissionEfficiency
	vehicleDesc.actor = robotActor;
	vehicleDesc.actor->setMaxAngularVelocity(6.2);
	vehicleDesc.actor->setMass(mass);

	//TODO: LEVANTAR DAMPING
	//float t = vehicleDesc.actor->getLinearDamping();
	//float b = vehicleDesc.actor->getAngularDamping();
	vehicleDesc.actor->setAngularDamping(0.5);
	vehicleDesc.actor->setLinearDamping(0.5);

	//TODO: LEVANTAR CMASS E INERTIA TENSOR
	
	//vehicleDesc.actor->setCMassOffsetGlobalPosition(NxVec3(0, 0, 0));
	NxMat33 inertiaTensor = NxMat33(NxVec3(1294.4362, 3.14502, -66.954), NxVec3(3.14502, 1094.42351, -0.24279), NxVec3(-66.954, -0.24279, 1754.80511));
	vehicleDesc.actor->setCMassOffsetLocalPose( NxMat34( inertiaTensor, NxVec3(0,0,0) ) );
	//TODO: Diagonalizar inertiaTensor e passar para setMassSpaceInertiaTensor
	vehicleDesc.actor->setMassSpaceInertiaTensor(/*vehicleDesc.actor->getMassSpaceInertiaTensor()*1000.*/NxVec3(1764.3, 1284.9, 1094.4) );

	//Motor descricao
	//NxVehicleMotorDesc motorsDesc[4];
	//for(NxU32 i=0;i<4;i++)
	//{
		//motorsDesc[i].setToCorvette();
		//vehicleDesc.motorsDesc.push_back(&motorsDesc[i]);
	//}

	//Roda (Wheel) descricao
	int numberWheels = Simulation::getNumberWheels(indexScene, indexRobot);
	NxWheelDesc* wheelDesc = new NxWheelDesc[numberWheels];
	for(NxU32 i=0;i<numberWheels;i++)
	{
		wheelDesc[i].setToDefault();
		//NxActor* wheelModel = Simulation::getActorWheel(indexScene,indexRobot,i);
		//NxActorDesc wheelActorDesc;
		//wheelModel->saveToDesc(wheelActorDesc);
		//Simulation::gScenes[0]->releaseActor(*wheelModel);
		NxActor* actorWheel = Simulation::getActorWheel(indexScene,indexRobot,i);//wheelModel;//Simulation::gScenes[0]->createActor(wheelActorDesc);
		//NxShape*const* wheelShapes = actorWheel->getShapes();
		//NxBounds3 wheelBounds;
		//wheelShapes[0]->getWorldBounds(wheelBounds);

		//Para exportar modelo da roda do 3ds Max
		//	NxWhee
		//wheelDesc[i]
		//robot1Shapes[0]->isConvexMesh()->getConvexMesh().saveToDesc(convexMesh);
		//NxWheelShape* wheelShape = (NxWheelShape*)wheel;
		//NxTriangleMeshDesc meshDesc = *((NxTriangleMeshDesc*)(mesh->userData));
		//robot1Shapes[0]->isWheel()->

		//wheelDesc[i].wheelApproximation = 10;

		wheelDesc[i].wheelOrientation = actorWheel->getGlobalOrientation();
		wheelDesc[i].position.set(actorWheel->getGlobalPosition()-robotActor->getGlobalPosition());
		//wheelDesc[i].position.z = 0;
		wheelDesc[i].id = i;
		wheelDesc[i].wheelRadius = wheelRadius;
		//wheelDesc[i].wheelWidth = 100;
		wheelDesc[i].wheelSuspension = 0;
		wheelDesc[i].springRestitution = 0;
		wheelDesc[i].springDamping = 0;
		wheelDesc[i].springBias = 0.0f;
		wheelDesc[i].maxBrakeForce = 100;
		wheelDesc[i].frictionToFront = 0.1f;//0.1f;	//TODO: CONFIGURAR PARÂMETRO
		wheelDesc[i].frictionToSide = 0;//0.02f;	//TODO: CONFIGURAR PARÂMETRO
		wheelDesc[i].inverseWheelMass = 0.1;		//TODO: CONFIGURAR PARÂMETRO

		//Angulo das Rodas (Omni)
		NxVec3 wheelPosRel = actorWheel->getGlobalPosition() - robotActor->getGlobalPosition();
		wheelDesc[i].angWheelRelRobot = NxMath::atan2( wheelPosRel.y, wheelPosRel.x );

		vehicleDesc.robotWheels.pushBack(&wheelDesc[i]);
		Simulation::gScenes[indexScene]->scene->releaseActor(*actorWheel);

		//NxU32 flags = NX_WF_BUILD_LOWER_HALF;
		
		wheelDesc[i].wheelFlags = NX_WF_ACCELERATED | NX_WF_AFFECTED_BY_HANDBRAKE | NX_WF_USE_WHEELSHAPE | NX_WF_BUILD_LOWER_HALF ;//| NxWheelFlags::NX_WF_STEERABLE_AUTO;// |/*NX_WF_STEERABLE_INPUT |*/ flags;
	}

	//NxBall* teste = Simulation::gScenes[indexScene]->ball;

	//Criar robot, vehicle base
	NxRobot* robot = (NxRobot*)NxRobot::createVehicle(Simulation::gScenes[indexScene], &vehicleDesc);
	if(robot){
		robot->setId(indexRobot);
		robot->setIdTeam(indexTeam);
		robot->indexScene = indexScene;

		//Dribbler and Kicker
		for(int i=0; i<robotActor->getNbShapes(); i++){
			NxShape*const* robotShapes = robotActor->getShapes();
			const char* shapeName = robotShapes[i]->getName();
			if(shapeName){
				char* dribblerName = new char[10];//"Driblador\0"
				dribblerName[9] = 0;
				memcpy(dribblerName, shapeName, strlen(dribblerName));
				
				char* kickerName = new char[9];//"Chutador\0"
				kickerName[8] = 0;
				memcpy(kickerName, shapeName, strlen(kickerName));
				
				if(strcmp(dribblerName, "Driblador") == 0){
					robot->dribbler->dribblerShapes.push_back(robotShapes[i]);
				}
				else if(strcmp(kickerName, "Chutador") == 0){
					robot->kicker->kickerShapeDesc = Simulation::copyShapeDesc(robotShapes[i]);
					robotActor->releaseShape(*(robotShapes[i]));
				}
				delete dribblerName;
				delete kickerName;
			}
		}

		//Initial Pose
		robot->setInitialPose(NxMat34(robotActor->getGlobalPose().M, NxVec3(robotActor->getGlobalPose().t.x, robotActor->getGlobalPose().t.y, 0) ));
		//robot->setInitialPose(robotActor->getGlobalPose());

		robotActor->putToSleep();

		//Mudar pose do robo
		//NxQuat q;
		//q.
		//q.fromAngleAxis(180.0f, NxVec3(0.0f, 1.0f, 0.0f));
		//robot->getActor()->setGlobalPose(pose);

		//Release no actor importado do 3ds Max
		//gScenes[0]->releaseActor(*robotActor);

		string label;
		string plabel = "Robo";
		stringstream out;
		out << indexRobot;
		out << "-";
		out << indexTeam;
		//out << "-";
		//out << indexScene;
		label.append(plabel);
		label.append(out.str());
		char* arrayLabel = new char[label.size()+1];
		arrayLabel[label.size()]=0;
		memcpy(arrayLabel, label.c_str(), label.size());
		robotActor->setName(arrayLabel);
		//delete arrayLabel;
	}
}

void NxRobot::cloneRobot(int indexNewScene, int indexNewRobot, NxVec3 newPosition, int indexNewTeam)
{
	NxRobot* nxRobotSource = simulation->gScenes[this->indexScene]->allRobots->getRobotByIdByTeam(this->id, this->idTeam);

	NxActor* robotActor = simulation->cloneActor(nxRobotSource->getActor(),indexNewScene);
	//NxBounds3 bodyBounds;
	//robotShapes[0]->getWorldBounds(bodyBounds);
	
	NxVehicleDesc vehicleDesc;
	
	vehicleDesc.position				= NxVec3(robotActor->getGlobalPosition());
	vehicleDesc.mass					= robotActor->getMass();
	//vehicleDesc.motorForce				= 70000;
	//vehicleDesc.maxVelocity				= 300.f;
	//vehicleDesc.cameraDistance			= 8.0f;
	vehicleDesc.centerOfMass.set(robotActor->getCMassLocalPosition());
	//vehicleDesc.differentialRatio = 3.42f;
	//vehicleDesc.transmissionEfficiency
	vehicleDesc.actor = robotActor;

	//Motor descricao
	//NxVehicleMotorDesc motorsDesc[4];
	//for(NxU32 i=0;i<4;i++)
	//{
		//motorsDesc[i].setToCorvette();
		//vehicleDesc.motorsDesc.push_back(&motorsDesc[i]);
	//}

	//Roda (Wheel) descricao
	int numberWheels = nxRobotSource->getNbWheels();
	NxWheelDesc* wheelDesc = new NxWheelDesc[numberWheels];
	for(NxU32 i=0;i<numberWheels;i++)
	{
		//NxActor* wheelModel = Simulation::getActorWheel(indexSourceScene,indexNewRobot,i);
		//NxActorDesc wheelActorDesc;
		//wheelModel->saveToDesc(wheelActorDesc);
		//Simulation::gScenes[0]->releaseActor(*wheelModel);
		
		//NxShape*const* wheelShapes = actorWheel->getShapes();
		//NxBounds3 wheelBounds;
		//wheelShapes[0]->getWorldBounds(wheelBounds);

		const NxWheel* wheel = nxRobotSource->getWheel(i);
		NxWheelShape* wheelShape = ((NxWheel2*)wheel)->getWheelShape();
		//wheelDesc[i].wheelApproximation = 10;
		wheelDesc[i].wheelOrientation = wheelShape->getGlobalOrientation();
		wheelDesc[i].position.set(wheelShape->getGlobalPosition()-robotActor->getGlobalPosition());
		//wheelDesc[i].position.z = 0;
		wheelDesc[i].id = i;
		wheelDesc[i].wheelFlags = ((NxWheel*)wheel)->getWheelFlags();
		wheelDesc[i].wheelRadius = wheel->getRadius();
		//wheelDesc[i].wheelWidth = 100;
		wheelDesc[i].wheelSuspension = wheelShape->getSuspensionTravel();
		wheelDesc[i].springRestitution = 0;
		wheelDesc[i].springDamping = 0;
		wheelDesc[i].springBias = 0.0f;
		wheelDesc[i].maxBrakeForce = 100;
		wheelDesc[i].frictionToFront = 0.1f;//0.1f;
		wheelDesc[i].frictionToSide = 0;//0.02f;//
		wheelDesc[i].inverseWheelMass = wheelShape->getInverseWheelMass(); //TODO: CONFIGURAR PARÂMETRO

		//Angulo das Rodas (Omni)
		wheelDesc[i].angWheelRelRobot = ((NxWheel2*)wheel)->angWheelRelRobot;
		
		vehicleDesc.robotWheels.pushBack(&wheelDesc[i]);
	}

	//Criar robot, vehicle base
	NxRobot* robot = (NxRobot*)NxRobot::createVehicle(simulation->gScenes[indexNewScene], &vehicleDesc);
	//NxRobot* robot = (NxRobot*)NxRobot::createVehicle(gScenes[indexSourceScene], &vehicleDesc);
	robot->setId(indexNewRobot);
	robot->setIdTeam(indexNewTeam);
	robot->indexScene = indexNewScene;
	robot->bodyRadius = nxRobotSource->bodyRadius;

	//Dribbler and Kicker
	NxShape*const* robotShapes = robotActor->getShapes();
	for(int i=0; i<robotActor->getNbShapes(); i++){
		const char* shapeName = robotShapes[i]->getName();
		if(shapeName){
			char* dribblerName = new char[10];//"Driblador\0"
			dribblerName[9] = 0;
			memcpy(dribblerName, shapeName, strlen(dribblerName));

			if(strcmp(dribblerName, "Driblador") == 0){
				robot->dribbler->dribblerShapes.push_back(robotShapes[i]);
			}
			delete dribblerName;
		}
	}
	robot->kicker->kickerShapeDesc = new NxBoxShapeDesc();
	NxShapeDesc* shapeDesc = nxRobotSource->kicker->kickerShapeDesc;
	robot->kicker->kickerShapeDesc->localPose = shapeDesc->localPose;
	((NxBoxShapeDesc*)(robot->kicker->kickerShapeDesc))->dimensions = ((NxBoxShapeDesc*)shapeDesc)->dimensions;

	//Initial Pose
	robot->setInitialPose(robotActor->getGlobalPose());

	robotActor->putToSleep();

	//Transladando o robo
	robot->getActor()->setGlobalPosition(newPosition);

	string label;
	string plabel = "Robo";
	stringstream out;
	out << indexNewRobot;
	out << "-";
	out << indexNewTeam;
	//out << "-";
	//out << indexNewScene;
	label.append(plabel);
	label.append(out.str());
	char* arrayLabel = new char[label.size()+1];
	arrayLabel[label.size()]=0;
	memcpy(arrayLabel, label.c_str(), label.size());
	robotActor->setName(arrayLabel);
	//delete arrayLabel;
}

void NxRobot::setGlobalPosition(NxVec3 position){
	//get relative distances from body
	//NxVec3 bodyPos = this->getBodyPos();
	//NxVec3 dribblerPos = this->dribbler.dribbler->getGlobalPosition();
	//NxVec3 kickerPos = this->kicker.kicker->getGlobalPosition();

	this->getActor()->setGlobalPosition(position);
}

void NxRobot::setGlobalOrientation(NxMat33 orientation){
	this->getActor()->setGlobalOrientation(orientation);
}

void NxRobot::putToSleep(){
	//for(int i=0; i<this->getNbWheels(); i++){
	//	((NxWheel2*)(this->getWheel(i)))->getWheelShape()->setMo
	//}
	this->getActor()->putToSleep();
	this->getActor()->setLinearVelocity(NxVec3(0,0,0));
	this->getActor()->setAngularVelocity(NxVec3(0,0,0));
}

void NxRobot::resetToInitialPose(){
	this->getActor()->setGlobalPose(this->getInitialPose());
}

void NxRobot::goToThisPose( NxReal x, NxReal y, NxReal angle )
{
	//Controle proporcional
	//Velocidades Maximas do nosso robô: 17,5 x  pi x 0,057m = 3,13 m/s (o da Skuba, atual campeã mundial: 3,5m/s)  
	//Rotacao maxima: 6,180582777638119118568314880347
	NxReal maxSpeedAng = 6.2;
	NxReal maxLinearSpeed = 3130;

	//Controle de angulo
	NxReal distanceAngle = angle - this->getAngle2DFromVehicle(); //TODO: remover redundância de getAngle2DFromRobot tanto em goToThisPose quanto em calcWheelSpeedFromRobotSpeed
	NxReal speedAng;
	if(NxMath::abs(distanceAngle) > NxPi / 72.) speedAng = distanceAngle / NxPi * maxSpeedAng;
	else speedAng = 0;

	//Controle de posicao
	NxVec3 position = this->getPos();
	NxReal distanceX = x - position.x;
	NxReal distanceY = y - position.y;
	NxReal distance = NxMath1::calcDistanceVec2D( position.x, position.y, x, y);
	NxReal distanceThreshold = 2000.;
	NxReal speedX;
	NxReal speedY;
	NxReal speed;
	if(NxMath::abs(distance) > 10.)
	{
		if( distance > distanceThreshold ) 
			if( distance > 0 ) speed = maxLinearSpeed;
			else speed = - maxLinearSpeed;
		else 
			speed = distance / distanceThreshold * maxLinearSpeed;

		speedX = speed * NxMath::cos( NxMath::atan2( distanceY, distanceX ) );
		speedY = speed * NxMath::sin( NxMath::atan2( distanceY, distanceX ) );
	}
	else
	{
		speedX = 0;
		speedY = 0;
	}

	controlRobot(speedX, speedY, speedAng, 0, 0);
}

/**
* Calcula a velocidade das 4 rodas de um determinado robo tendo como entrada as velocidades angular e linear desejadas para o robo.
*/
NxReal* NxRobot::calcWheelSpeedFromRobotSpeed( NxReal speedAng, NxReal speedX, NxReal speedY)
{
	//Matriz de omnidirecionalidade
	//Leva em consideracao os angulos das rodas
	//-0.5446    0.8387    1.0000
	//-0.5446   -0.8387    1.0000
	//0.7071   -0.7071    1.0000
	//0.7071    0.7071    1.0000

	NxReal angRobo = this->getAngle2DFromVehicle();
	int nbWheels = this->getNbWheels();
	NxReal* speeds = new NxReal[nbWheels];

	for(int i=0; i<nbWheels; i++)
	{
		//NxVec3 wheelPosRel = this->getWheel(i)->getWheelPos() - this->getBodyPos();
		NxReal angPosWheel = ((NxWheel2*)this->getWheel(i))->angWheelRelRobot;
		speeds[i] = -NxMath::sin(angPosWheel) * ( speedX * NxMath::cos( -angRobo ) + speedY * NxMath::sin( angRobo ) ) +
			NxMath::cos(angPosWheel) * ( speedX * NxMath::sin( -angRobo ) + speedY * NxMath::cos( angRobo ) ) +
			speedAng * this->wheelsRadius;
		speeds[i] /= ((NxWheel2*)this->getWheel(i))->getRadius();
	}

	//NxMat33 omniMatrix1;
	//NxMat33 omniMatrix2;

	//omniMatrix1.setRow(0, NxVec3(-0.5446,	0.8387,		1.0000));
	//omniMatrix1.setRow(1, NxVec3(-0.5446,	-0.8387,	1.0000));
	//omniMatrix1.setRow(2, NxVec3(0.7071,	-0.7071,	1.0000));

	//omniMatrix2.setRow(0, NxVec3(0.7071,	0.7071,		1.0000));

	////omniMatrix1.setRow(0, NxVec3(-0.5446,	0.8387,		1.0000));
	////omniMatrix1.setRow(1, NxVec3(-0.5446,	-0.8387,	1.0000));
	////omniMatrix1.setRow(2, NxVec3(0.7071,	-0.7071,	1.0000));

	////omniMatrix2.setRow(0, NxVec3(0.7071,	0.7071,		1.0000));

	//NxMat33 controlRobot;
	//controlRobot.zero();

	//controlRobot.setColumn( 0, NxVec3( speedX * NxMath::cos( -angRobo ) + speedY * NxMath::sin( angRobo ), 
	//	speedX * NxMath::sin( -angRobo ) + speedY * NxMath::cos( angRobo ),
	//	speedAng * 90/*NxRobot::robotRadius*/ ) );

	//omniMatrix1 *= controlRobot;
	//omniMatrix2 *= controlRobot;

	//NxVec3 speedAxleWheel1 = omniMatrix1.getColumn(0);
	//NxVec3 speedAxleWheel2 = omniMatrix2.getColumn(0);

	//speeds[0] = speedAxleWheel1.x;
	//speeds[1] = speedAxleWheel1.y;
	//speeds[2] = speedAxleWheel1.z;
	//speeds[3] = speedAxleWheel2.x;

	//LIMITANTE DE VELOCIDADE
	NxReal biggestValue = NxMath1::getBiggestAbsoluteValue(speeds, nbWheels);
	if(biggestValue > 0.00001)
	{
		//Valor maximo da nossa roda 109,95574287564276334619251841478 rad/s (17,5 rps)
		NxReal maxSpeed = 110.;
		for( int i = 0; i < nbWheels; i++ )
		{
			speeds[i] = speeds[i] / biggestValue * maxSpeed;
			//if(speeds[i]>110.){
			//	printf("teste: %f\n", speeds[i]);
			//}
		}
	}
	else
	{
		for( int i = 0; i < nbWheels; i++ )
		{
			speeds[i] = 0;
		}
	}

	return speeds;
}

void NxRobot::infinitePath()
{
	static int i[10] = {0};//=0;
	//if( i>7 ) i=0;
	static bool flags[10][8] = { false };

	if(flags[id-1][7]==true) 
	{
		for(int j=0; j<8; j++)
		{
			flags[id-1][j] = false;
		}
	}

	for(int j=0; j<8; j++)
	{
		if(flags[id-1][j]==false)
		{
			i[id-1]=j;
			break;
		}
	}

	NxVec3 pontos[8];
	pontos[0].x = 1000;
	pontos[0].y = 1000;

	pontos[1].x = 2000;
	pontos[1].y = 1000;

	pontos[2].x = 2000;
	pontos[2].y = -1000;

	pontos[3].x = 1000;
	pontos[3].y = -1000;

	pontos[4].x = -1000;
	pontos[4].y = 1000;

	pontos[5].x = -2000;
	pontos[5].y = 1000;

	pontos[6].x = -2000;
	pontos[6].y = -1000;

	pontos[7].x = -1000;
	pontos[7].y = -1000;

	NxVec3 posRobot = this->getPos();
	NxReal distance = NxMath1::calcDistanceVec2D(pontos[i[id-1]].x, pontos[i[id-1]].y, posRobot.x, posRobot.y);
	if( distance < 100 ){
		flags[id-1][i[id-1]]=true;
	}

	goToThisPose( pontos[i[id-1]].x, pontos[i[id-1]].y, 3* NxPi / 2.);
}

void NxRobot::controlRobotByWheels(float speedWheel1, float speedWheel2, float speedWheel3, float speedWheel4, float dribblerSpeed, float kickerSpeed)
{
	if(this){
		//Control kicker
		this->kicker->controlKicker( kickerSpeed, this );

		//Control dribbler
		this->dribbler->controlDribbler( dribblerSpeed );

		//Control wheels
		NxReal* wheelsSpeeds = new NxReal[4];
		wheelsSpeeds[0] = speedWheel1;
		wheelsSpeeds[1] = speedWheel2;
		wheelsSpeeds[2] = speedWheel3;
		wheelsSpeeds[3] = speedWheel4;
		controlWheels( wheelsSpeeds ); 
	}
}

void NxRobot::controlRobot(float speedX, float speedY, float speedAng, float dribblerSpeed, float kickerSpeed)
{
	if(this){
		//Execute kicker
		this->kicker->controlKicker( kickerSpeed, this );

		//Control dribbler
		this->dribbler->controlDribbler( dribblerSpeed );

		//Control wheels
		NxReal* wheelsSpeeds = calcWheelSpeedFromRobotSpeed(speedAng, speedX, speedY ); //omnidirecionalidade
		controlWheels( wheelsSpeeds ); 
	}
}

void NxRobot::controlWheels( NxReal* wheelsSpeeds )
{
	int nbWheels = this->getNbWheels();
	NxReal* torqueWheels = new NxReal[nbWheels];
	for(int indexWheel=0; indexWheel<nbWheels; indexWheel++)
	{
		NxWheel2* wheel = ((NxWheel2*) this->getWheel(indexWheel));
		NxReal currentWheelSpeed = wheel->getAxleSpeed();
		//torqueWheels.push_back(calcTorqueFromWheelSpeed(wheelsSpeeds[indexWheel], currentWheelSpeed, indexScene, indexRobot, indexWheel));
		torqueWheels[indexWheel] = wheel->calcTorqueFromWheelSpeed(wheelsSpeeds[indexWheel], currentWheelSpeed);
	}
	//printf("VELOCID: %f %f %f %f\n",wheelsSpeeds[0], wheelsSpeeds[1], wheelsSpeeds[2], wheelsSpeeds[3] );
	delete wheelsSpeeds;
	//printf("TORQUES: %f %f %f %f\n",torqueWheels[0], torqueWheels[1], torqueWheels[2], torqueWheels[3] );
	//LIMITANTE DE TORQUE
	NxReal biggestValue = NxMath1::getBiggestAbsoluteValue(torqueWheels, nbWheels);
	if(biggestValue > 0.00001){
		//TODO: Levantar esse parametro http://www.robotshop.com/lynxmotion-ghm-04-gear-head-motor.html Torque: 99.04oz.in (7.1 Kg-cm) Reduction: 50:1
		NxReal maxTorque = 7100.;//4100.;//
		if(biggestValue > maxTorque){
			for( int i = 0; i < nbWheels; i++ ){
				torqueWheels[i] = torqueWheels[i] / biggestValue * maxTorque;
			}
		}
	}
	else{
		for( int i = 0; i < nbWheels; i++ ){
			torqueWheels[i] = 0;
		}
	}


	this->control( torqueWheels );//torqueWheels[0], torqueWheels[1], torqueWheels[2], torqueWheels[3] );

	this->updateVehicle();
}