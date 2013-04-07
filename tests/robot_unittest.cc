#include <gtest/gtest.h>
#include <Robot.h>

using namespace LibIntelligence;


class RobotTest : public ::testing::Test {
 protected:

  virtual void SetUp() {
    robot = new Robot();
  }

  virtual void TearDown() {
    delete robot;
  }

  Robot *robot;
};


//Robot(QObject* parent=0, int id=0, int colorCode=0, TeamColor color=BLUE);
TEST_F(RobotTest, DefaultConstructor) {
  EXPECT_EQ(BLUE, robot->color());
  EXPECT_EQ(NULL, robot->parent());
  EXPECT_EQ(0, robot->wheels().size());
}


//TODO test copy constructor
//Robot(const Robot&);


//TODO test prototype constructor
//Robot(const Dribbler&, const Kicker&, const Body&, const Battery&, const QVector<Wheel>&);


//TODO test destructor???
//~Robot();


//void setColor(TeamColor);
//TeamColor color() const;
TEST_F(RobotTest, SetColor) {
  robot->setColor(YELLOW);
  EXPECT_EQ(YELLOW, robot->color());

  robot->setColor(BLUE);
  EXPECT_EQ(BLUE, robot->color());
}


//TODO test dribbler
//const Dribbler& dribbler() const;
//Dribbler& dribbler();
//bool canDribble() const;


//TODO test kicker
//const Kicker& kicker() const;
//Kicker& kicker();
//bool canKick() const;


//TODO test body
//const Body& body() const;
//Body& body();


//TODO test battery
//const Battery& battery() const;
//Battery& battery();


//TODO test wheels
//const QVector<Wheel>& wheels() const;
//QVector<Wheel>& wheels();
//quint32 wheelsSize() const;


//TODO test stage
//Stage* stage() const;
//void setStage(Stage* stage);


//TODO test team
//Team* team() const;
//void setTeam(Team* team);


//TODO test enemy team
//Team* enemyTeam() const;


//TODO test goal
//Goal* goal() const;


//TODO test enemy goal
//Goal* enemyGoal() const;


//TODO test command
//const Command& command() const;
//Command& command();
//void setCommand(const Command&);
//void setCommandWheels(const QVector<qreal>& speed);
//void newCommand();
//void kick(qreal=1.0);
//void dribble(qreal=1.0);


//TODO test id
//void setId(quint8);
//quint8 id() const;


//TOOD test pattern id
//void setPatternId(quint8);
//quint8 patternId() const;


//TODO test working
//void setAllWorking();
//void setAllNotWorking();
//bool isWorking() const;
//bool isNotWorking() const;


//TODO test string representation
//string toString() const;


//TODO test setting parent
//void setParent(QObject*);


//TODO test last toucher
//void setLastToucher(bool value);
//bool isLastToucher();

//TODO test touched
//bool checkIfTouched();
//void setTouchedBall(bool value);
//bool touchedBall();
