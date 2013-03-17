#include <gtest/gtest.h>
#include <Robot.h>

using namespace LibIntelligence;


class RobotTest : public testing::Test {
 protected:

  virtual void SetUp() {
    robot = new Robot();
  }

  virtual void TearDown() {
    delete robot;
  }

  Robot *robot;
};


TEST_F(RobotTest, Constructor) {
  EXPECT_EQ(BLUE, robot->color());
  EXPECT_EQ(NULL, robot->parent());
  EXPECT_EQ(0, robot->wheels().size());
}


TEST_F(RobotTest, SetColor) {
  robot->setColor(YELLOW);
  EXPECT_EQ(YELLOW, robot->color());

  robot->setColor(BLUE);
  EXPECT_EQ(BLUE, robot->color());
}


//TODO more robot tests
