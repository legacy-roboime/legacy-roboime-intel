#include <gtest/gtest.h>
#include <Body.h>


using namespace LibIntelligence;

class BodyTest : public ::testing::Test {
 protected:

  virtual void SetUp() {
    body_default = new Body();
    body = new Body(NULL, 11.11, 22.22, 33.33);
    body_copy = new Body(*body);
    body_copy_pointer = new Body(body);
  }

  virtual void TearDown() {
    delete body_default;
    delete body_copy_pointer;
    delete body_copy;
    delete body;
  }

  Body *body_default;
  Body *body;
  Body *body_copy;
  Body *body_copy_pointer;
};


TEST_F(BodyTest, DefaultConstructor) {
  EXPECT_EQ(0.0, body_default->height());
  EXPECT_EQ(0.0, body_default->radius());
  EXPECT_EQ(0., body_default->cut());
}

TEST_F(BodyTest, Constructor) {
  EXPECT_EQ(11.11, body->height());
  EXPECT_EQ(22.22, body->radius());
  EXPECT_EQ(33.33, body->cut());
}

TEST_F(BodyTest, CopyConstructor_PointerParameter) {
  EXPECT_EQ(11.11, body_copy_pointer->height());
  EXPECT_EQ(22.22, body_copy_pointer->radius());
  EXPECT_EQ(33.33, body_copy_pointer->cut());
}

TEST_F(BodyTest, CopyConstructor_NonPointerParameter) {
  EXPECT_EQ(11.11, body_copy->height());
  EXPECT_EQ(22.22, body_copy->radius());
  EXPECT_EQ(33.33, body_copy->cut());
}

TEST_F(BodyTest, SetHeight) {
  body->setHeight(12.34);
  EXPECT_EQ(12.34, body->height());
}

TEST_F(BodyTest, SetRadius) {
  body->setRadius(12.34);
  EXPECT_EQ(12.34, body->radius());
}

TEST_F(BodyTest, SetCut) {
  body->setCut(1.23);
  EXPECT_EQ(1.23, body->cut());
}


//TODO more body tests
