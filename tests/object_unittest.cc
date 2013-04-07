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
  
  //creating an array of tester points
  Point testPoints[10];
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


TEST_F(ObjectTest, UpdatePosition) {
  for(int i = 0; i<10; i++){
    object->updatePosition(testPoints[i]);
	EXPECT_EQ( 0., object->x() );
	EXPECT_EQ( 0., object->y() );
  }
}


//XXX WARNING: O teste é arbitrário, a dimensão da tolerância depende do usuário!
TEST_F(ObjectTest, UpdatePositionWithFilter) {
  //Tolerância definida pelo usuário
  double TOL = 0.01;
  
  //adaptar qtd de iterações ao framerate da camera, iterações necessárias para se estabilizar o filtro
  for(int i = 0; i < 120; i++)
    object->updatePositionWithFilter((i%2) ? Point(-5, -2) : Point(-20, -2));
  
  //Testando estabilização inicial
  for(int i = 0; i < 10; i++){
	Object previous(*object);
	object->updatePositionWithFilter((i%2) ? Point(-5, -2) : Point(-20, -2));
	EXPECT_NEAR(object->x(), previous.x(), (1 + TOL)*7.5); //7.5 é a média da distância entre -5 e -20
	EXPECT_NEAR(object->y(), previous.y(), 1+TOL);
  }

  {//Testando entrada de ponto anômalo
  Object previous(*object);
  object->updatePositionWithFilter(Point(-200, -2)); //ponto de pico (anômalo)
  object->updatePositionWithFilter(Point(-5, -2)); //seq de 3 pontos corretos
  object->updatePositionWithFilter(Point(-20, -2));
  object->updatePositionWithFilter(Point(-5, -2));
  EXPECT_NEAR(object->x(), previous.x(), (1 + TOL)*7.5); //7.5 é a média da distância entre -5 e -20
  EXPECT_NEAR(object->y(), previous.y(), 1+TOL);
  }
  
  {//Testando entrada de ponto anômalo em y 
    Object previous(*object);
    object->updatePositionWithFilter(Point(-20, -2000)); //ponto de pico (anômalo)
    object->updatePositionWithFilter(Point(-5, -2)); //seq de 3 pontos corretos
    object->updatePositionWithFilter(Point(-20, -2));
    object->updatePositionWithFilter(Point(-5, -2));
	EXPECT_NEAR(object->x(), previous.x(), (1 + TOL)*7.5); //7.5 é a média da distância entre -5 e -20
    EXPECT_NEAR(object->y(), previous.y(), 1+TOL);
  }
}

//TEST_F(ObjectTest, zzzzz) {
//	int a = 1;
//}
//TODO test updateSpeed
//void updateSpeed(double time)

//TODO test setOrientationWithFilter
//void setOrientationWithFilter(qreal o)