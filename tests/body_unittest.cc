#include <gtest/gtest.h>
#include <Body.h>


using namespace LibIntelligence;

class BodyTest : public testing::Test {
 protected:

  virtual void SetUp() {
    body = new Body();
  }

  virtual void TearDown() {
    delete body;
  }

  Body *body;
};


TEST_F(BodyTest, Constructor) {
  EXPECT_EQ(0.0, body->height());
  EXPECT_EQ(0.0, body->radius());
  EXPECT_EQ(0., body->cut());
}


TEST_F(BodyTest, SetDimensions) {
  body->setHeight(12.34);
  EXPECT_EQ(12.34, body->height());

  body->setRadius(12.34);
  EXPECT_EQ(12.34, body->radius());

  body->setCut(1.23);
  EXPECT_EQ(1.23, body->cut());
}


//TODO more robot tests
