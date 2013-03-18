#include <gtest/gtest.h>
#include <Component.h>

using namespace LibIntelligence;


class ComponentTest : public testing::Test {
 protected:

  virtual void SetUp() {
    component = new Component();
  }

  virtual void TearDown() {
    delete component;
  }

  Component *component;
};


TEST_F(ComponentTest, DefaultConstructor) {
  EXPECT_TRUE(component->isActive());
  // The following should be refactored, meanwhile
  // we'll use the old syntax
  EXPECT_EQ(1.0, component->isWorking());
}

TEST_F(ComponentTest, SetActive) {
  component->setNotActive();
  EXPECT_FALSE(component->isActive());

  component->setActive();
  EXPECT_TRUE(component->isActive());
}

TEST_F(ComponentTest, SetWorking) {
  // As noted before this should be refactored

  // Common cases
  component->setWorking(0.0);
  EXPECT_EQ(0.0, component->isWorking());

  component->setWorking(0.5);
  EXPECT_EQ(0.5, component->isWorking());

  component->setWorking(0.8);
  EXPECT_EQ(0.8, component->isWorking());

  // Unexpected case
  // should be between 0.0 and 1.0
  //component->setWorking(2.0);
  //EXPECT_EQ(1.0, component->isWorking());

  //component->setWorking(-1.0);
  //EXPECT_EQ(0.0, component->isWorking());
}
