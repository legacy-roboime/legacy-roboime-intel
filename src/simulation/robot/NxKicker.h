#pragma once

#include "NxShapeDesc.h"

class NxRobot;
class Simulation;

class NxKicker
{
private:
	Simulation* simulation;
public:
	NxShapeDesc* kickerShapeDesc; //Shape Carregada do Modelo, contem informaçoes apenas do estado no inicio (carregamento)
	//float calcTorqueFromSpeed(); 	//TODO: implementar o controlador
	void controlKicker( float kickerSpeed, NxRobot* robot );
	NxKicker(Simulation* simulation);
	~NxKicker(void);
};
