#include <gtest/gtest.h>
#include <Object.h>

using namespace LibIntelligence;

class ObjectTest : public ::testing::Test {
 protected:

   virtual void SetUp() {
     object_default = new Object();
     object = new Object(-11.11, -22.22, -3.33, -4.44, -5.55, -6.66);
     //There's no such Constructor!
     //object_copy_pointer = new Object(object);
     object_copy = new Object(*object);
   }

   virtual void TearDown() {
     delete object_default;
     //delete object_copy_pointer;
     delete object_copy;
     delete object;
   }

  Object *object_default;
  //Object *object_copy_pointer;
  Object *object_copy;
  Object *object;
};

TEST_F(ObjectTest, SetOrientation) {
  object->setOrientation(0.123);
  EXPECT_EQ(0.123, object->orientation());
}

TEST_F(ObjectTest, SetSpeedFromVector) {
  Vector *temp_vector = new Vector(1.5, -2.5);
  object->setSpeed(*temp_vector);
  EXPECT_EQ(1.5, (object->speed()).x());
  EXPECT_EQ(-2.5, (object->speed()).y());
  delete temp_vector;
}

TEST_F(ObjectTest, SetSpeed) {
  object->setSpeed(1.5, -2.5);
  EXPECT_EQ(1.5, (object->speed()).x());
  EXPECT_EQ(-2.5, (object->speed()).y());
}

TEST_F(ObjectTest, SetAngSpeed) {
  object->setAngSpeed(-3.33);
  EXPECT_EQ(-3.33, object->angSpeed());
}