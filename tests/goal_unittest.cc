#include <gtest/gtest.h>
#include <Goal.h>

using namespace LibIntelligence;


class GoalTest : public ::testing::Test {
 protected:

  virtual void SetUp() {
    goal = new Goal();
  }

  virtual void TearDown() {
    delete goal;
  }

  Goal *goal;
};


TEST_F(GoalTest, DefaultConstructor) {
  EXPECT_EQ(0.0, goal->x());
  EXPECT_EQ(0.0, goal->y());
  EXPECT_EQ(0.0, goal->width());
  EXPECT_EQ(0.0, goal->depth());
  EXPECT_EQ(0.0, goal->wallWidth());
  EXPECT_EQ(0.0, goal->penaltyMark().x());
  EXPECT_EQ(0.0, goal->penaltyMark().y());
  EXPECT_EQ(0.0, goal->penaltyLine());
}

TEST_F(GoalTest, SetX) {
  goal->setX(13.3);
  EXPECT_EQ(13.3, goal->x());
}

TEST_F(GoalTest, SetY) {
  goal->setY(13.3);
  EXPECT_EQ(13.3, goal->y());
}

TEST_F(GoalTest, SetWidth) {
  goal->setWidth(13.3);
  EXPECT_EQ(13.3, goal->width());
}

TEST_F(GoalTest, SetDepth) {
  goal->setDepth(13.3);
  EXPECT_EQ(13.3, goal->depth());
}

TEST_F(GoalTest, SetWallWidth) {
  goal->setWallWidth(13.3);
  EXPECT_EQ(13.3, goal->wallWidth());
}

TEST_F(GoalTest, SetPenaltyMark) {
  goal->setPenaltyMark(13.3, 16.6);
  EXPECT_EQ(13.3, goal->penaltyMark().x());
  EXPECT_EQ(16.6, goal->penaltyMark().y());
}

TEST_F(GoalTest, SetPenaltyLine) {
  goal->setPenaltyLine(13.3);
  EXPECT_EQ(13.3, goal->penaltyLine());
}

// Should we test Goal::randomPoint() ?

