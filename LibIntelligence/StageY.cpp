#include "StageY.h"
#include "Zickler43.h"

using namespace LibIntelligence;
using namespace Tactics;

StageY::StageY(const Stage& stage) : Stage(stage)
{
	//a insercao das táticas na fila soh sera feita no populateTactics da play bk-bgt
	this->blueTactics = QQueue<Tactic*>();
	this->yellowTactics = QQueue<Tactic*>();
}

StageY::StageY(const StageY& stage) : Stage(stage)
{
	//a copia (insercao das táticas na fila) soh sera feita no populateTactics da play bk-bgt
	this->blueTactics = QQueue<Tactic*>();
	this->yellowTactics = QQueue<Tactic*>();

	//this->blueTactics = QQueue<StateMachine*>(stage.blueTactics);
	//for(int i=0; i<this->blueTactics.size(); i++)
	//	this->blueTactics.at(i)->setStage(this); //importante o setStage vim antes do setRobot por
	//for(int i=0; i<this->blueTeam()->size(); i++)
	//	this->blueTactics.at(i)->setRobot(this->blueTeam()->at(i));

	//this->yellowTactics = QQueue<Tactic*>(stage.yellowTactics);
	//for(int i=0; i<this->yellowTactics.size(); i++)
	//	this->yellowTactics.at(i)->setStage(this);
	//for(int i=0; i<this->yellowTeam()->size(); i++)
	//	this->yellowTactics.at(i)->setRobot(this->yellowTeam()->at(i));

	/*for(int i=0; i<stage.blueTactics.size(); i++)
		this->blueTactics.push_back(new StateMachine(stage.blueTactics.at(i)));
	for(int i=0; i<stage.yellowTactics.size(); i++)
		this->yellowTactics.push_back(new StateMachine(stage.yellowTactics.at(i)));*/

	/*for(int i=0; i<stage.blueTactics.size(); i++)
		this->blueTactics.push_back(new Tactics::Zickler43(stage.blueTactics.at(i)));
	for(int i=0; i<stage.yellowTactics.size(); i++)
		this->yellowTactics.push_back(new Tactics::Zickler43(stage.yellowTactics.at(i)));*/
}

StageY::~StageY(void)
{
	for(int i=0; i<blueTactics.size(); i++)
		delete blueTactics.at(i);
	for(int i=0; i<yellowTactics.size(); i++)
		delete yellowTactics.at(i);
}

void StageY::pushTactic(Tactic* tactic, TeamColor color)
{
	if(color == TeamColor::BLUE)
		blueTactics.push_back(tactic);
	else
		yellowTactics.push_back(tactic);
}

bool StageY::busy()
{
	bool busy = true;
	for(int i = 0; i < blueTactics.size(); i++){
		busy = busy && blueTactics.at(i)->busy();
	}
	for(int i = 0; i < yellowTactics.size(); i++){
		busy = busy && yellowTactics.at(i)->busy();
	}
	return busy;
}

QQueue<Tactic*>* StageY::getBlueTactics()
{
	return &blueTactics;
}

QQueue<Tactic*>* StageY::getYellowTactics()
{
	return &yellowTactics;
}

//StageY& StageY::operator=(const StageY& stage)
//{
//
//}

bool StageY::operator==(const StageY& stage)
{
	if(this == &stage)
		return true;
	else
		return false;
}

bool StageY::operator==(const StageY* stage)
{
	if(this == stage)
		return true;
	else
		return false;
}