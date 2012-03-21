#include "BGT.h"
#include "Goal.h"
#include "Ball.h"
#include "Simulation.h"
#include "Team.h"
#include <QDataStream>
#include "StageX.h"
#include "tree_util.hh"
#include <algorithm>
#include "Sampler.h"
#include "StateMachine.h"

#define LOGGING

using namespace LibIntelligence::Plays;

BGT::BGT(QObject *parent, Team* myTeam, Stage* stage):
Play(parent,myTeam,stage),
	log("C:\\Users\\Bill\\Desktop\\log.dat"),
	maxIter(1000),//50000), //dado retirado do zickler pagina 72
	mi_(100.), //valor de mi igual ao usado pelo zickler no experimento da pagina 73
	timer(new QTimer(this)),
	planningInterruped_(false),
	execSteps(1), //baseado em zickler pagina 92
	planSteps(3), //baseado em zickler pagina 92
	xInit(NULL)
{
	log.open(QIODevice::WriteOnly);

	connect(timer, SIGNAL(timeout()), this, SLOT(planningInterruped()));
	for(int i=0; i<myTeam->size(); i++)
		execSkills.push_back(new Goto(this, myTeam->at(i)));
	tree_ = new tree<StageY*>();

	//xInit = new StageY(*(this->stage()));//cria copia do stage em xInit
	//populateTactics(xInit);
}

void BGT::populateTactics(const StageY* source, StageY* target)
{
	if(source){
		Team* myT;
		Team* enemyT;
		myT = target->getTeamFromColor(this->team()->color());
		enemyT = target->getTeamFromOtherColor(this->team()->color());
		QQueue<Tactic*>* myTactics = myT->color() == TeamColor::BLUE ? ((StageY*)source)->getBlueTactics() : ((StageY*)source)->getYellowTactics();
		QQueue<Tactic*>* enemyTactics = enemyT->color() == TeamColor::BLUE ? ((StageY*)source)->getBlueTactics() : ((StageY*)source)->getYellowTactics();

		//Zickler43* z = new Zickler43(this, myT->at(0));
		//z->setCurrentState( z->getStateByName( myTactics->at(0)->getCurrentState()->objectName() ) );
		//target->pushTactic((Tactic*)z, myT->color());
		for(int i=0; i<myT->size(); i++) {
			//Goalkeeper* z = new Goalkeeper(this, myT->at(i), 1000);
			//z->setCurrentState( z->getStateByName( myTactics->at(i)->getCurrentState()->objectName() ) );
			//target->pushTactic((Tactic*)z, myT->color());
			Zickler43* z = new Zickler43(this, myT->at(0));
			z->setCurrentState( z->getStateByName( myTactics->at(0)->getCurrentState()->objectName() ) );
			target->pushTactic((Tactic*)z, myT->color());
		}
		//Zickler43* z1 = new Zickler43(this, enemyT->at(0));
		//z1->setCurrentState( z1->getStateByName( enemyTactics->at(0)->getCurrentState()->objectName() ) );
		//target->pushTactic((Tactic*)z1, enemyT->color());
		for(int i=0; i<enemyT->size(); i++) {
			Zickler43* z1 = new Zickler43(this, enemyT->at(0));
			z1->setCurrentState( z1->getStateByName( enemyTactics->at(0)->getCurrentState()->objectName() ) );
			target->pushTactic((Tactic*)z1, enemyT->color());
			//Goalkeeper* z = new Goalkeeper(this, enemyT->at(i), 1000);
			//z->setCurrentState( z->getStateByName( enemyTactics->at(i)->getCurrentState()->objectName() ) );
			//target->pushTactic((Tactic*)z, enemyT->color());

		}
	}
	else{
		Team* myT;
		Team* enemyT;
		myT = target->getTeamFromColor(this->team()->color());
		enemyT = target->getTeamFromOtherColor(this->team()->color());
		for(int i=0; i<myT->size(); i++) 
			target->pushTactic((Tactic*) new Zickler43(this, myT->at(i)), myT->color());
		for(int i=0; i<enemyT->size(); i++) 
			target->pushTactic((Tactic*) new Zickler43(this, enemyT->at(i)), enemyT->color());
	}
}

BGT::~BGT()
{
	log.close();

	delete timer;
	for(int i=0; i<execSkills.size(); i++)
		delete execSkills.at(i);
	delete tree_;
}


void BGT::step()
{
	static unsigned int time = 0;
	static QQueue<StageY*> solution = QQueue<StageY*>();

	if((time % (planSteps + execSteps)) == 0){ //planejar e executar
		solution.clear();
		//timer->start(1000./60.);
		timer->singleShot(1000./60., this, SLOT(planningInterruped()));
		planning(&solution);
		//timer->stop();
		planningInterruped_ = false;
		solution.pop_back();//remove xInit
		StageY* stage = solution.takeLast(); //solucao invertica <xBest, ..., x2, x1, xInit>
		goToStage(stage);
	}
	else if(!solution.isEmpty()){ //executar
		StageY* stage = solution.takeLast(); //solucao invertica <xBest, ..., x2, x1, xInit>
		goToStage(stage);
	}
	else{
		time = 0;
		solution.clear();
		//timer->start(1000./60.);
		timer->singleShot(1000./60., this, SLOT(planningInterruped()));
		planning(&solution);
		//timer->stop();
		planningInterruped_ = false;
		solution.pop_back();//remove xInit
		StageY* stage = solution.takeLast(); //solucao invertica <xBest, ..., x2, x1, xInit>
		goToStage(stage);
	}

	//int t = tree_->size();
	tree<StageY*>::iterator itr, end;
	itr = tree_->begin();
	end = tree_->end();
	while(itr!=end){
		//cout << (*itr)->blueTeam()->at(0)->x() << endl;
		if(*itr)
			delete (*itr);
		++itr;
	}
	//delete xInit;
	xInit = NULL;
	tree_->clear();
	//xInit;
	time++;
	time %= planSteps + execSteps;
}

void BGT::planning(QQueue<StageY*>* solution)
{
	if(xInit == NULL){
		xInit = new StageY(*(this->stage()));//cria copia do stage em xInit
		populateTactics(NULL, xInit);
	}
	tree<StageY*>::iterator top, ptr1, ptr2;
	top = ptr1 = tree_->begin();
	ptr2 = tree_->insert(ptr1, xInit);
	qreal bestEval = evalState(xInit);
	qreal tmpEval;
	StageY* xBest = xInit;
	bool busy = false;
	bool first = true;
	StageY* lastXL;

	bool busyOtim;
	unsigned int iter = 1;
	while(planningInterruped_ == false && iter <= maxIter){
		StageY* x;
		StageY* xL;
		iter++;
		if(first){
			x = xInit;
			first = false;
		}
		else{
			if(busy)
				x = lastXL;//xL;//
			else
				x = selectNodeBGT(mi_); 
		}
		busyOtim = busy;	
		xL = tacticsDrivenPropagate(*x);
		busy = xL->busy();
		if(validate(xL)){
			tmpEval = evalState(xL);

			if(tmpEval < bestEval){
				bestEval = tmpEval;
				xBest = xL;
			}
			if(busyOtim){
				//ptr1 = ptr2;
				ptr2 = tree_->append_child(ptr2, xL);
			}
			else{
				ptr2 = find(tree_->begin(), tree_->end(), x);
				//ptr1 = ptr2;
				ptr2 = tree_->append_child(ptr2, xL);
			}
			if(isGoal(xL))
				traceBack(xL, solution);
		}
		else{
			//TODO: colocar o rollBack no BGT anytime
			//if(busy)
			//	rollBack(x);
			busy=false;
		}
		//printf("%f\n",xL->blueTeam()->at(0)->x());
		lastXL = xL;
		//kptree::print_tree_bracketed(*tree_, std::cout);
	}

	traceBack(xBest, solution);

#ifdef LOGGING
	tree<StageY*>::pre_order_iterator itr, end;
	itr = tree_->begin();
	end = tree_->end();

	QDataStream out(&log);   // write the data
	while(itr!=end){
		Ball* ball = (*itr)->ball();
		Team* yellowTeam = (*itr)->yellowTeam();
		Team* blueTeam = (*itr)->blueTeam();
		QString str("3 0 ");
		for(int i=0; i<5; i++){
			Robot* robot = yellowTeam->at(i);
			str += QString::number(robot->x()) + " " + QString::number(robot->y()) + " " + QString::number(robot->orientation()) + " " + QString::number(robot->speedX()) + " " + QString::number(robot->speedY()) + " " + QString::number(0) + " " + QString::number(0) + " " + QString::number(0) + " ";
		}
		//cout << str.toStdString() << endl;
		for(int i=0; i<5; i++){
			Robot* robot = blueTeam->at(i);
			str += QString::number(robot->x()) + " " + QString::number(robot->y()) + " " + QString::number(robot->orientation()) + " " + QString::number(robot->speedX()) + " " + QString::number(robot->speedY()) + " " + QString::number(0) + " " + QString::number(0) + " " + QString::number(0) + " ";
		}
		str += QString::number(ball->x()) + " " + QString::number(ball->y()) + " " + QString::number(ball->speedX()) + " " + QString::number(ball->speedY()) + " " + QString::number(0) + "\n";
		out << str;
		out << "\n";
		out << (*itr)->getBlueTactics()->at(4)->getCurrentState()->objectName() << "\n";
		log.flush();
		++itr;
	}
#endif
}

void BGT::goToStage(StageY* stage)
{
	const Team* myTeamS = this->team();
	Team* myTeamD = stage->getTeamFromColor(myTeamS->color());
	for(int i=0; i<myTeamS->size(); i++){
		Robot* robotS = myTeamS->at(i);
		Robot* robotD =  myTeamD->at(i);
		execSkills.at(i)->setPoint(robotD->x(), robotD->y());
		execSkills.at(i)->setOrientation(robotD->orientation());
		execSkills.at(i)->setSpeed(3000.); //sqrt(robotD->speedX()*robotD->speedX() + robotD->speedY()*robotD->speedY())); TODO: usar o comentado depois q implementarem o calculo da velocidade
		robotS->kick(robotD->kicker().speed()); 
		robotS->dribble(robotD->dribbler().speed());
		execSkills.at(i)->step();
	}
}

void BGT::planningInterruped()
{
	planningInterruped_ = true;
}

bool BGT::validate(StageY* stage)
{
	//TODO: implementar
	return true;
}

bool BGT::isGoal(StageY* stage)
{
	return stage->isGoal(this->team()->color()); //é estado objetivo se a gente tiver feito gol
}

qreal BGT::evalState(StageY* stage)
{
	// avaliacao do estado do jogo conmsiderando.
	// Valor alto ruim e valor baixo bom.
	Team* team = stage->getTeamFromColor(this->team()->color());
	Ball* ball = stage->ball();
	Goal* enemyGoal;
	Goal* myGoal;
	enemyGoal = stage->getGoalFromOtherColor(team->color());
	myGoal = stage->getGoalFromColor(team->color());

	qreal dx1 = enemyGoal->x() - stage->ball()->x();
	qreal dy1 = enemyGoal->y() - stage->ball()->y();

	qreal dx2 = myGoal->x() - stage->ball()->x();
	qreal dy2 = myGoal->y() - stage->ball()->y();

	qreal dist1 = sqrt(dx1*dx1 + dy1*dy1);
	qreal dist2 = sqrt(dx2*dx2 + dy2*dy2);

	return (-dist2 + dist1);
}

void BGT::traceBack(StageY* stage, QQueue<StageY*>* solution)
{
	solution->push_back(stage);
	tree<StageY*>::iterator loc;
	loc = find(tree_->begin(), tree_->end(), stage);
	//cout << "a " << xInit->blueTeam()->at(0)->y() << endl;
	//cout << "d " << (*loc)->blueTeam()->at(0)->y() << endl;
	//if(xInit == (*loc))
	//cout << "vaca\n";
	while(loc!=tree_->begin()){
		loc = tree_->parent(loc);
		StageY* sta = *loc;
		solution->push_back(sta);
	};
	StageY* sta = *loc;
	solution->push_back(sta);
}

StageY* BGT::selectNodeBGT(qreal mi)
{
	if(averageLeafDepth() / averageBranchingFactor() > mi)
		return randomNonLeaf();
	else 
		return randomLeaf();
}

qreal BGT::averageLeafDepth() //TODO: colocar atributo no nós ppara em tempo de execucao ir deixando armazzenado a quantidade de filhos, dai nao precisa varrer todas  folhas
{
	qreal average = 0;
	qreal count = 0;
	tree<StageY*>::leaf_iterator loc;
	loc = tree_->begin_leaf();
	while(loc!=tree_->end_leaf()){
		average += (qreal)tree_->depth(loc);
		++loc;
		count += 1;
	}
	if(count>0)
		average /= count;
	else
		average = 0;
	return average;
}

qreal BGT::averageBranchingFactor() //TODO: colocar atributo no nós ppara em tempo de execucao ir deixando armazzenado a quantidade de filhos, dai nao precisa varrer todos nos
{
	qreal average = 0;
	qreal nbTotal = (qreal)tree_->size();
	//qreal nbLeaf = 0;
	tree<StageY*>::iterator loc;
	loc = tree_->begin();
	while(loc!=tree_->end()){
		average += (qreal)tree_->number_of_children(loc);
		++loc;
	}
	if(nbTotal>0)
		average /= nbTotal;
	else
		average = 0;
	return average;
}

StageY* BGT::randomLeaf()
{
	tree<StageY*>::leaf_iterator loc;
	loc = tree_->begin_leaf();
	QVector<StageY*> nodes = QVector<StageY*>();
	while(loc!=tree_->end_leaf()){ //TODO: colocar uma flag em cada no para em tempo de execucao atualizar se eh folha ou nao eh, dai nao precisaria varrer todos os nos em cada itr
		if(!(*loc)->busy())
			nodes.push_back(*loc);
		++loc;
	}
	int pos = Sampler::randInt(0, nodes.size()-1);
	return nodes.at(pos);
}

StageY* BGT::randomNonLeaf()
{
	tree<StageY*>::iterator loc;
	loc = tree_->begin();
	QVector<StageY*> nodes = QVector<StageY*>(); //TODO: colocar uma flag em cada no para em tempo de execucao atualizar se eh folha ou nao eh, dai nao precisaria varrer todos os nos em cada itr
	while(loc!=tree_->end()){
		if(!(*loc)->busy() && loc.number_of_children() > 0)
			nodes.push_back(*loc);
		++loc;
	}
	int pos = Sampler::randInt(0, nodes.size()-1);
	return nodes.at(pos);
}

StageY* BGT::tacticsDrivenPropagate(const StageY& stage)
{
	StageX* xL = new StageX(stage);
	populateTactics(&stage, xL);
	QQueue<Tactics::Tactic*>* blueTactics = xL->getBlueTactics();
	QQueue<Tactics::Tactic*>* yellowTactics = xL->getYellowTactics();
	for(int i=4; i<blueTactics->size(); i++){
		blueTactics->at(i)->step();
	}
	for(int i=4; i<yellowTactics->size(); i++){
		yellowTactics->at(i)->step();
	}
	cout << "a1 " << xL->blueTeam()->at(4)->x() << endl;
	xL->simulate();
	cout << "d1 " << xL->blueTeam()->at(4)->x() << "\n\n";
	xL->releaseScene();
	return xL;
}