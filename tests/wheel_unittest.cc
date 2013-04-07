#include <gtest/gtest.h>
#include <Wheel.h>

using namespace LibIntelligence;


class WheelTest : public testing::Test {
 protected:

  virtual void SetUp() {
    wheel_default = new Wheel();
	wheel = new Wheel(NULL, 11.11, 22.22, 33.33, 44.44);
	wheel_copy = new Wheel(*wheel);
	wheel_copy_pointer = new Wheel(wheel);
  }

  virtual void TearDown() {
    delete wheel_default;
	delete wheel_copy_pointer;
	delete wheel_copy;
	delete wheel;
  }

  Wheel *wheel_default;
  Wheel *wheel;
  Wheel *wheel_copy;
  Wheel *wheel_copy_pointer;
};


TEST_F(WheelTest, DefaultConstructor) {
  EXPECT_EQ(0.0, wheel_default->angle());
  EXPECT_EQ(0.0, wheel_default->radius());
  EXPECT_EQ(0.0, wheel_default->distance());
}

TEST_F(WheelTest, Constructor) {
  EXPECT_EQ(11.11, wheel->angle());
  EXPECT_EQ(22.22, wheel->radius());
  EXPECT_EQ(33.33, wheel->distance());
}

TEST_F(WheelTest, CopyConstructorNonPointerParameter) {
  EXPECT_EQ(11.11, wheel_copy->angle());
  EXPECT_EQ(22.22, wheel_copy->radius());
  EXPECT_EQ(33.33, wheel_copy->distance());
}

TEST_F(WheelTest, CopyConstructorPointerParameter) {
  EXPECT_EQ(11.11, wheel_copy_pointer->angle());
  EXPECT_EQ(22.22, wheel_copy_pointer->radius());
  EXPECT_EQ(33.33, wheel_copy_pointer->distance());
}

TEST_F(WheelTest, SetAngle) {
  wheel->setAngle(11.01);
  EXPECT_EQ(11.01, wheel->angle());
  wheel->setAngle(11.11);
  EXPECT_EQ(11.11, wheel->angle());
}

TEST_F(WheelTest, SetRadius) {
  wheel->setRadius(22.02);
  EXPECT_EQ(22.02, wheel->radius());
  wheel->setRadius(22.22);
  EXPECT_EQ(22.22, wheel->radius());
}

TEST_F(WheelTest, SetDistance) {
  wheel->setDistance(33.03);
  EXPECT_EQ(33.03, wheel->distance());
  wheel->setDistance(33.33);
  EXPECT_EQ(33.33, wheel->distance());
}

//TODO more wheel tests