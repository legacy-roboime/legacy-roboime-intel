#include "Stage.h"
#include "Ball.h"
#include "Team.h"
#include "Goal.h"
#include <cmath>
#include <cfloat>
#include <vector>
#include <limits.h>
#include <map>

using namespace LibIntelligence;

Stage::Stage()
	: QObject(),
	//TODO: make default ball/sizes/...
	lineWidth_(0.0),
	fieldLength_(0.0),
	fieldWidth_ (0.0),
	boundaryWidth_(0.0),
	refereeWidth_(0.0),
	centerCircleRadius_(0.0),
	defenseRadius_(0.0),
	defenseStretch_(0.0),
	freeKickDistance_(0.0),
	penaltySpotDistance_(0.0),//from field
	penaltyLineDistance_(0.0),//from spot
    cmdReferee_('H'),
	ball_(new Ball(21.5)),
	blueGoal_(new Goal()),
	yellowGoal_(new Goal()),
	blueTeam_(new Team(this, BLUE)),
	yellowTeam_(new Team(this, YELLOW)),
    isLeftSideBlueGoal_(true)
{
	lastToucherRobot_=NULL;
}

Stage::Stage(const Stage& stage)
	: QObject(),
	isLeftSideBlueGoal_(stage.isLeftSideBlueGoal_)
{
	//TODO: make default ball/sizes/...
	this->_time_left = stage.getTimeLeft();
	this->cmdReferee_ = stage.getCmdReferee();
	this->lineWidth_ = stage.lineWidth();
	this->fieldLength_ = stage.fieldLength();
	this->fieldWidth_ = stage.fieldWidth();
	this->boundaryWidth_ = stage.boundaryWidth();
	this->refereeWidth_ = stage.refereeWidth();
	this->centerCircleRadius_ = stage.centerCircleRadius();
	this->defenseRadius_ = stage.defenseRadius();
	this->defenseStretch_ = stage.defenseStretch();
	this->freeKickDistance_ = stage.freeKickDistance();
	this->penaltySpotDistance_ = stage.penaltySpotDistance();
	this->penaltyLineDistance_ = stage.penaltyLineDistance();
	ball_ = new Ball(*(stage.ball()));

	blueGoal_ = new Goal(*(stage.blueGoal()));
	yellowGoal_ = new Goal(*(stage.yellowGoal()));
	blueTeam_ = new Team(this, *(stage.blueTeam()));
	yellowTeam_ = new Team(this, *(stage.yellowTeam()));
	lastToucherRobot_=NULL;
}

Stage::~Stage()
{
	delete ball_;
	delete blueGoal_;
	delete yellowGoal_;
	delete blueTeam_;
	delete yellowTeam_;
}

/*const*/ Ball* Stage::ball() const
{
	return ball_;
}

void Stage::setBlueTeam(Team* t)
{
	blueTeam_ = t;
}

void Stage::setYellowTeam(Team* t)
{
	yellowTeam_ = t;
}

/*const*/ Goal* Stage::blueGoal() const
{
	return blueGoal_;
}

/*const*/ Goal* Stage::yellowGoal() const
{
	return yellowGoal_;
}

/*const*/ Team* Stage::blueTeam() const
{
	return blueTeam_;
}

/*const*/ Team* Stage::yellowTeam() const
{
	return yellowTeam_;
}

Ball* Stage::ball()
{
	return ball_;
}

Goal* Stage::blueGoal()
{
	return blueGoal_;
}

Goal* Stage::yellowGoal()
{
	return yellowGoal_;
}

Team* Stage::blueTeam()
{
	return blueTeam_;
}

Team* Stage::yellowTeam()
{
	return yellowTeam_;
}

void Stage::setBall(Ball* b)
{
	ball_ = b;
}

void Stage::setBlueGoal(Goal* g)
{
	blueGoal_ = g;
}

void Stage::setYellowGoal(Goal* g)
{
	yellowGoal_ = g;
}

void Stage::setLineWidth(qreal w)
{
	lineWidth_ = w;
}

qreal Stage::lineWidth() const
{
	return lineWidth_;
}

void Stage::setFieldLength(qreal l)
{
	fieldLength_ = l;
}

qreal Stage::fieldLength() const
{
	return fieldLength_;
}

void Stage::setFieldWidth(qreal w)
{
	fieldWidth_ = w;
}

qreal Stage::fieldWidth() const
{
	return fieldWidth_;
}

void Stage::setBoundaryWidth(qreal w)
{
	boundaryWidth_ = w;
}

qreal Stage::boundaryWidth() const
{
	return boundaryWidth_;
}

void Stage::setRefereeWidth(qreal w)
{
	refereeWidth_ = w;
}

qreal Stage::refereeWidth() const
{
	return refereeWidth_;
}

void Stage::setCenterCircleRadius(qreal r)
{
	centerCircleRadius_ = r;
}

qreal Stage::centerCircleRadius() const
{
	return centerCircleRadius_;
}

void Stage::setDefenseRadius(qreal r)
{
	defenseRadius_ = r;
}

qreal Stage::defenseRadius() const
{
	return defenseRadius_;
}

void Stage::setDefenseStretch(qreal s)
{
	defenseStretch_ = s;
}

qreal Stage::defenseStretch() const
{
	return defenseStretch_;
}

void Stage::setFreeKickDistance(qreal d)
{
	freeKickDistance_ = d;
}

qreal Stage::freeKickDistance() const
{
	return freeKickDistance_;
}

void Stage::setPenaltySpotDistance(qreal d)
{
	penaltySpotDistance_ = d;
}

qreal Stage::penaltySpotDistance() const
{
	return penaltySpotDistance_;
}

void Stage::setPenaltyLineDistance(qreal d)
{
	penaltyLineDistance_ = d;
}

qreal Stage::penaltyLineDistance() const
{
	return penaltyLineDistance_;
}

void Stage::setCmdReferee(char cmdReferee){
	cmdReferee_ = cmdReferee;
}

char Stage::getCmdReferee() const{
	return cmdReferee_;
}

void Stage::setTimeLeft(double time_left){
	this->_time_left = time_left;
}

double Stage::getTimeLeft() const{
	return this->_time_left;
}

bool Stage::isLeftSideBlueGoal(){
	return isLeftSideBlueGoal_;
}

void Stage::setIsLeftSideBlueGoal(bool s){
	isLeftSideBlueGoal_ = s;
}

Team* Stage::getTeamFromColor(const TeamColor& color)
{
	if(color == BLUE)
		return blueTeam_;
	else
		return yellowTeam_;
}

Team* Stage::getTeamFromOtherColor(const TeamColor& color)
{
	if(color == YELLOW)
		return blueTeam_;
	else
		return yellowTeam_;
}

Goal* Stage::getGoalFromOtherColor(const TeamColor& color)
{
	if(color == YELLOW)
		return blueGoal_;
	else
		return yellowGoal_;
}

Goal* Stage::getGoalFromColor(const TeamColor& color)
{
	if(color == BLUE)
		return blueGoal_;
	else
		return yellowGoal_;
}

bool Stage::isGoal(const TeamColor& color)
{
	Goal* goal = getGoalFromOtherColor(color);
	qreal xMax = goal->x() > 0 ? goal->x() + goal->depth() : goal->x();
	qreal xMin = goal->x() > 0 ? goal->x() : goal->x() - goal->depth();
	qreal yMax = goal->y() + (goal->width() / 2.);
	qreal yMin = goal->y() - (goal->width() / 2.);
	//TODO: colocar goal 3D
	if(ball_->x() < xMax && ball_->x() > xMin && ball_->y() > yMin && ball_->y() < yMax)
		return true;
	else 
		return false;
}

bool Stage::inField(const Object& obj)
{
	if(obj.x() <  fieldLength_/2. && 
	   obj.x() > -fieldLength_/2. && 
	   obj.y() <  fieldWidth_/2.  && 
	   obj.y() > -fieldWidth_/2. )

		return true;
	else 
		return false;
}

Robot* Stage::getClosestPlayerToBallThatCanKick() const
{
    qreal MinDistance = 999999;

    Team* team = blueTeam_;
    Robot* bestFit = team->at(0);
    for(int i = 0; i < team->count(); i++) {
        if(team->at(i)->isActive()){
            qreal dy = team->at(i)->y() - this->ball()->y();
            qreal dx = team->at(i)->x() - this->ball()->x();

            qreal Distance = sqrt(dy * dy + dx * dx);

            if(Distance < MinDistance && team->at(i)->canKick()){
                MinDistance = Distance;
                bestFit = team->at(i);
            }
        }
    }

    team = yellowTeam_;
    for(int i = 0; i < team->count(); i++) {
        if(team->at(i)->isActive()){
            qreal dy = team->at(i)->y() - this->ball()->y();
            qreal dx = team->at(i)->x() - this->ball()->x();

            qreal Distance = sqrt(dy * dy + dx * dx);

            if(Distance < MinDistance && team->at(i)->canKick()){
                MinDistance = Distance;
                bestFit = team->at(i);
            }
        }
    }

    return bestFit;
}

/// Retorna o jogador do time escolhido mais próximo à bola
/// team : time do jogador
/// retorno: jogador mais próximo
Robot* Stage::getClosestPlayerToBall(const Team* team) const
{
	Robot* bestFit = NULL;

	qreal MinDistance = DBL_MAX;
	for(int i = 0; i < team->count(); i++) {

		qreal dy = team->at(i)->y() - this->ball()->y();
		qreal dx = team->at(i)->x() - this->ball()->x();

		qreal Distance = sqrt(dy * dy + dx * dx);

		if(Distance < MinDistance) {
			MinDistance = Distance;
			bestFit = team->at(i);
		}
	}

	return bestFit;
}

Robot* Stage::getClosestPlayerToBallThatCanKick(const Team* team) const
{
	Robot* bestFit = team->at(0);

	qreal MinDistance = 999999;
	for(int i = 0; i < team->count(); i++) {
		if(team->at(i)->isActive()){
			qreal dy = team->at(i)->y() - this->ball()->y();
			qreal dx = team->at(i)->x() - this->ball()->x();

			qreal Distance = sqrt(dy * dy + dx * dx);

			if(Distance < MinDistance && team->at(i)->canKick()){
				MinDistance = Distance;
				bestFit = team->at(i);
			}
		}
	}

	return bestFit;
}

/// Retorna um map do time escolhido em ordem de proximidade da bola
/// team : time do jogador
/// retorno: map dos robots em ordem de proximidade da bola
map<qreal, Robot*> Stage::getClosestPlayersToBall(const Team* team) const
{
	return getClosestPlayersToPoint(team, ball_);
}

/// Retorna um map do time escolhido em ordem de proximidade do ponto
/// team : time do jogador
/// retorno: map dos robots em ordem de proximidade do ponto
map<qreal, Robot*> Stage::getClosestPlayersToPoint(const Team* team, const Point* point) const
{
	map<qreal, Robot*> robots;
	for(int i = 0; i < team->count(); i++) {
		if(team->at(i)->isActive()){
			qreal dy = team->at(i)->y() - point->y();
			qreal dx = team->at(i)->x() - point->x();

			qreal d = sqrt(dy * dy + dx * dx);

			robots[d] = team->at(i);
		}
	}

	return robots;
}

/// Retorna um map do time escolhido em ordem de proximidade do ponto
/// team : time do jogador
/// retorno: map dos robots em ordem de proximidade do ponto
map<qreal, Robot*> Stage::getClosestPlayersToPoint(const Team* team, qreal x, qreal y) const
{
	map<qreal, Robot*> robots;
	for(int i = 0; i < team->count(); i++) {
		if(team->at(i)->isActive()){
			qreal dy = team->at(i)->y() - y;
			qreal dx = team->at(i)->x() - x;

			qreal d = sqrt(dy * dy + dx * dx);

			robots[d] = team->at(i);
		}
	}

	return robots;
}

/// Retorna um map do time escolhido em ordem de proximidade do ponto
/// team : time do jogador
/// retorno: map dos robots em ordem de proximidade do ponto
map<qreal, Robot*> Stage::getClosestPlayersToPointThatCanKick(const Team* team, Point* point) const
{
	map<qreal, Robot*> robots;
	for(int i = 0; i < team->count(); i++) {
		if(team->at(i)->isActive()){
			qreal dy = team->at(i)->y() - point->y();
			qreal dx = team->at(i)->x() - point->x();

			qreal d = sqrt(dy * dy + dx * dx);
			qreal working = team->at(i)->kicker().isWorking();
			if(working<=0)
				d *= 99999;
			else
				d *= 1 - working/2.;

			robots[d] = team->at(i);
		}
	}

	return robots;
}

/// Retorna um map do time escolhido em ordem de proximidade da bola
/// team : time do jogador
/// retorno: map dos robots em ordem de proximidade da bola
map<qreal, Robot*> Stage::getClosestPlayersToBallThatCanKick(const Team* team) const
{
	return getClosestPlayersToPointThatCanKick(team, (Point*)ball_);
}



//----------------------- INÍCIO DO getBestPositionsToIndirectPass -----------------
//----------------------- INÍCIO DO getBestPositionsToIndirectPass -----------------
//----------------------- INÍCIO DO getBestPositionsToIndirectPass -----------------
//qtd_x e qtd_y definem a qtd de pontos a serem mapeados
//alteração: qtd_x define a precisão!

map<qreal, Point*> Stage::getBestIndirectPositions(const Team* team, int qtd_x) const
{
		map<qreal, Point*> finalCandidates;
	int qtd_y = qtd_x;



//-------------------- início do getBestPositions
		
	//define a qtd de pontos a serem mapeados
	
	
	vector< vector<Point*> > matrix(qtd_y, vector<Point*>(qtd_x));
	vector< vector<Line*> > matrixLines(qtd_y, vector<Line*>(qtd_x));
	vector< vector<bool> > matrixCandidates(qtd_y, vector<bool>(qtd_x));
	vector< vector<qreal> > matrixDistance1(qtd_y, vector<qreal>(qtd_x));
	vector< vector<qreal> > matrixDistance2(qtd_y, vector<qreal>(qtd_x));
	
		

	Point* finalTarget = team->enemyGoal(); //alvo final, onde deve ser executado o chute
											//AJUSTAR PARA O MEIO DO MAIOR ESPAÇO VAZIO EXISTENTE NO ENEMYGOAL!!!!

	Point* start = (Point*)ball_;

			
	bool tempCandidate = false; //bool temporário que receberá true caso não haja robô inimigo obstruindo
		

		

	//definindo o retângulo que conterá os pontos da matriz dos pontos candidatos
	qreal len = ( 0.42 * fieldLength_ - defenseRadius_ - 2.*team->at(0)->body().radius() );
	qreal wid = (0.42 * fieldWidth_ - 2.*team->at(0)->body().radius() ); //nao esquecer de somar aqui o raio do robô também
	qreal delta_x = 2.*len / qtd_x,
		  delta_y = 2.*wid / qtd_y,
		  temp_x = -1.*len,
		  temp_y = -1.*wid;


	//Calculando primeiro passe: Distance1
	for(int j = 0; j < qtd_y; j++){
		if(j==0) temp_y = -1.*wid;

		for(int i = 0; i < qtd_x; i++){
			if(i==0) temp_x = -1.*len;
			matrix[i][j] = new Point(temp_x, temp_y); //matriz de pontos candidatos
			matrixLines[i][j] = new Line(start->x(), start->y(), matrix[i][j]->x(), matrix[i][j]->y() ); //matriz de linhas entre a bola e cada ponto candidato
			matrixCandidates[i][j] = false;
			matrixDistance1[i][j] = -1.;
			matrixDistance2[i][j] = -1.;
			tempCandidate = true;
			
			//atualizando valores das coordenadas
			temp_x += delta_x;
			temp_y += delta_y;

			//passando por todos os robôs inimigos
			for(int k = 0; k < team->enemyTeam()->size(); k++){ 
				Point* temp = new Point(team->enemyTeam()->at(k)->x() , team->enemyTeam()->at(k)->y()); //ponto onde se encontra o robô inimigo
				qreal temp_dist = matrixLines[i][j]->distanceTo(*temp); //distância da linha até o robô inimigo
					
				if( temp_dist <= (team->enemyTeam()->at(k)->body().radius() + ball()->radius()) ){ //se não houver algum robô na trajetória...
					tempCandidate = false;
				}
				delete temp;
			}
			
			//passando por todos os nossos robôs
			for(int m = 0; m < team->size(); m++){ 
				Point* temp2 = new Point(team->at(m)->x() , team->at(m)->y()); //ponto onde se encontra o robô
				qreal temp_dist2 = matrixLines[i][j]->distanceTo(*temp2); //distância da linha até o robô
					
				if(temp_dist2 <= (team->at(m)->body().radius() + ball()->radius()) ){ //se não houver robô amigo na trajetória...
					tempCandidate = false;
					//qreal tempDist = Line(matrix[i][j]->x(), matrix[i][j]->y(), temp2->x(), temp2->y()).length();
				}
				delete temp2;
			}

			if(tempCandidate==true){
				matrixDistance1[i][j] = Line(matrix[i][j]->x(), matrix[i][j]->y(), start->x(), start->y()).length();
				matrixCandidates[i][j] = true; //se nao houver inimigo nem amigo obstruindo, o ponto é candidato.
			}
		}
	}
	
	//Calculando chute para o gol: Distance2
	for(int j=0; j < qtd_y; j++){
		for(int i = 0; i < qtd_x; i++){
			if(matrixCandidates[i][j] == true){ //só calcula a segunda trajetória se o primeiro passe for possível
				delete matrixLines[i][j];
				matrixLines[i][j] = new Line(finalTarget->x(), finalTarget->y(), matrix[i][j]->x(), matrix[i][j]->y() ); //linha entre pt candidato e finalTarget
				tempCandidate = true;
						
				//passando por todos os robôs inimigos
				for(int k = 0; k < team->enemyTeam()->size(); k++){ 
					Point* temp = new Point(team->enemyTeam()->at(k)->x() , team->enemyTeam()->at(k)->y()); //ponto onde se encontra o robô inimigo
					qreal temp_dist = matrixLines[i][j]->distanceTo(*temp); //distância da linha até o robô inimigo
					
					if( temp_dist <= (team->enemyTeam()->at(k)->body().radius() + ball()->radius()) ){ //se não houver algum robô na trajetória...
						tempCandidate = false;
						matrixCandidates[i][j] = false;
					}
					delete temp;
				}

				//passando por todos os nossos robôs
				for(int m = 0; m < team->size(); m++){ 
					Point* temp2 = new Point(team->at(m)->x() , team->at(m)->y()); //ponto onde se encontra o robô
					qreal temp_dist2 = matrixLines[i][j]->distanceTo(*temp2); //distância da linha até o robô
					
					if( temp_dist2 <= (team->at(m)->body().radius() + ball()->radius()) ){ //se não houver robô amigo na trajetória...
						tempCandidate = false; //se houver obstrução, o ponto deixa de ser candidato.
						matrixCandidates[i][j] = false;
					}
					delete temp2;
				}
			}
			if(matrixCandidates[i][j] == true){
				matrixDistance2[i][j] = Line(matrix[i][j]->x(), matrix[i][j]->y(), finalTarget->x(), finalTarget->y()).length();
			}
		}//for int i
	}//for int j
					


	
	
	bool allFalseCheck = false;
	for(int j = 0; j < qtd_y; j++){
		for(int i = 0; i < qtd_x; i++){
			if(matrixCandidates[i][j] == true){
				qreal totalDist = matrixDistance1[i][j] + matrixDistance2[i][j];
				finalCandidates[ totalDist ] = matrix[i][j];
				allFalseCheck = true;
			}
		}
	}


	for(int j = 0; j < qtd_y; j++){
		for(int i = 0; i < qtd_x; i++){
			//delete matrix[i][j];
			delete matrixLines[i][j];
		}
	}
	


	//-------------------- fim do getBestPositions

		
	if(allFalseCheck==false){
		//adicionar ponto genérico no map
		return finalCandidates;
	}

	else
		return finalCandidates;
}
//-----------------------   FIM DO getBestPositionsToIndirectPass   -----------------
//-----------------------   FIM DO getBestPositionsToIndirectPass   -----------------
//-----------------------   FIM DO getBestPositionsToIndirectPass   -----------------








Robot* Stage::getClosestOrderPlayerToBall(const Team* team, int order) const
{
	map<qreal, Robot*> m = getClosestPlayersToBall(team);
	map<qreal, Robot*>::iterator it = m.begin();
	for(int i=0; i<order; i++)
		it++;
	return (*it).second;
}

Stage& Stage::operator=(const Stage& stage)
{
	this->_time_left = stage.getTimeLeft();
	this->cmdReferee_ = stage.getCmdReferee();
	this->lineWidth_ = stage.lineWidth();
	this->fieldLength_ = stage.fieldLength();
	this->fieldWidth_ = stage.fieldWidth();
	this->boundaryWidth_ = stage.boundaryWidth();
	this->refereeWidth_ = stage.refereeWidth();
	this->centerCircleRadius_ = stage.centerCircleRadius();
	this->defenseRadius_ = stage.defenseRadius();
	this->defenseStretch_ = stage.defenseStretch();
	this->freeKickDistance_ = stage.freeKickDistance();
	this->penaltySpotDistance_ = stage.penaltySpotDistance();
	this->penaltyLineDistance_ = stage.penaltyLineDistance();
	*(this->ball_) = *(stage.ball());
	*(this->blueGoal_) = *(stage.blueGoal());
	*(this->yellowGoal_) = *(stage.yellowGoal());
	*(this->blueTeam_) = *(stage.blueTeam());
	*(this->yellowTeam_) = *(stage.yellowTeam());
	return *this;
}

bool Stage::operator==(const Stage& stage)
{
	if(this == &stage)
		return true;
	else
		return false;
}

bool Stage::operator==(const Stage* stage)
{
	if(this == stage)
		return true;
	else
		return false;
}
