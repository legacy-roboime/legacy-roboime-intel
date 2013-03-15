#include <gtest/gtest.h>
#include <Robot.h>

using namespace LibIntelligence;

TEST(RobotTest, Construct) {
  Robot r = Robot();
  EXPECT_EQ(BLUE, r.color());

  r.setColor(YELLOW);
  EXPECT_EQ(YELLOW, r.color());
}
