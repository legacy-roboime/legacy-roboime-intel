#include "Stage.h"
#include "Ball.h"
#include "Team.h"
#include "Goal.h"
#include <cmath>
#include <limits.h>

using namespace LibIntelligence;

Stage::Stage()
	: QObject(),
	isLeftSideBlueGoal_(true),
	cmdReferee_('H'),
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
	ball_(new Ball(21.5)),
	blueGoal_(new Goal()),
	yellowGoal_(new Goal()),
	blueTeam_(new Team(this, BLUE)),
	yellowTeam_(new Team(this, YELLOW))
{}

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

		qreal dy = team->at(i)->y() - this->ball()->y();
		qreal dx = team->at(i)->x() - this->ball()->x();

		qreal Distance = sqrt(dy * dy + dx * dx);

		if(Distance < MinDistance && team->at(i)->canKick()){
			MinDistance = Distance;
			bestFit = team->at(i);
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
map<qreal, Robot*> Stage::getClosestPlayersToPoint(const Team* team, Point* point) const
{
	map<qreal, Robot*> robots;
	for(int i = 0; i < team->count(); i++) {

		qreal dy = team->at(i)->y() - point->y();
		qreal dx = team->at(i)->x() - point->x();

		qreal d = sqrt(dy * dy + dx * dx);

		robots[d] = team->at(i);
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
		if(team->at(i)->canKick()){
			qreal dy = team->at(i)->y() - point->y();
			qreal dx = team->at(i)->x() - point->x();

			qreal d = sqrt(dy * dy + dx * dx);

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
