#include <gtest/gtest.h>
#include <Dribbler.h>


using namespace LibIntelligence;

class DribblerTest : public testing::Test {
 protected:

  virtual void SetUp() {
    dribbler = new Dribbler();
  }

  virtual void TearDown() {
    delete dribbler;
  }

  Dribbler *dribbler;
};


TEST_F(DribblerTest, Constructor) {
  EXPECT_EQ(0.0, dribbler->speed());
  EXPECT_EQ(NULL, dribbler->parent());
}


//TODO: test da classe filho ~> test da classe papi
//TODO more robot tests