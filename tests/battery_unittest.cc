#include <gtest/gtest.h>
#include <Battery.h>

using namespace LibIntelligence;

class BatteryTest : public testing::Test {
 protected:

  virtual void SetUp() {
    battery = new Battery();
  }

  virtual void TearDown() {
    delete battery;
  }

  Battery *battery;
};

TEST_F(BatteryTest, DefaultConstructor) {
  EXPECT_EQ(NULL, battery->parent());
}
