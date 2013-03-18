#include <gtest/gtest.h>
#include <Ball.h>

using namespace LibIntelligence;

class BallTest : public testing::Test {
 protected:

  virtual void SetUp() {
    ball = new Ball();
  }

  virtual void TearDown() {
    delete ball;
  }

  Ball *ball;
};


TEST_F(BallTest, DefaultConstructor) {
  EXPECT_EQ(0.0, ball->radius());
}

TEST_F(BallTest, SetRadius) {
  double radius;
  radius = 12.34;
  ball->setRadius(radius);
  EXPECT_EQ(radius, ball->radius());
}

TEST_F(BallTest, CopyConstructor) {
  ball->setRadius(1234.1234);
  ball->setX(12341.1234124);
  ball->setY(12341234.12341);
  Ball *ball_other = new Ball(*ball);
  EXPECT_EQ(ball->radius(), ball_other->radius());
  EXPECT_EQ(ball->x(), ball_other->x());
  EXPECT_EQ(ball->y(), ball_other->y());
  delete ball_other;
}
