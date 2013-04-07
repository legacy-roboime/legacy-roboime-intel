#include <gtest/gtest.h>
#include <Actuator.h>
#include <limits.h>

using namespace LibIntelligence;

class ActuatorTest : public ::testing::Test {
 protected:

  virtual void SetUp() {
    actuator = new Actuator();
  }

  virtual void TearDown() {
    delete actuator;
  }

  Actuator *actuator;
};

TEST_F(ActuatorTest, DefaultConstructor) {
  EXPECT_EQ(0.0, actuator->speed());
  EXPECT_EQ(NULL, actuator->parent());
}

TEST_F(ActuatorTest, SetSpeed) {
  double speed;

  speed = 666;
  actuator->setSpeed(speed);
  EXPECT_EQ(speed, actuator->speed());
}
