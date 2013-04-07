#include <gtest/gtest.h>
#include <Kicker.h>

using namespace LibIntelligence;


class KickerTest : public testing::Test {
 protected:

  virtual void SetUp() {
    kicker_default = new Kicker();
	kicker = new Kicker(NULL, 11.11);
	kicker_copy_pointer = new Kicker(kicker);
	kicker_copy = new Kicker(*kicker);
  }

  virtual void TearDown() {
    delete kicker_default;
	delete kicker_copy_pointer;
	delete kicker_copy;
	delete kicker;
  }

  Kicker *kicker_default;
  Kicker *kicker;
  Kicker *kicker_copy;
  Kicker *kicker_copy_pointer;
};


TEST_F(KickerTest, DefaultConstructor) {
  EXPECT_EQ(0.0, kicker_default->speed());
}

TEST_F(KickerTest, Constructor) {
  EXPECT_EQ(11.11, kicker->speed());
}

TEST_F(KickerTest, CopyConstructorNonPointerParameter) {
  EXPECT_EQ(11.11, kicker_copy->speed());
}

TEST_F(KickerTest, CopyConstructorPointerParameter) {
  EXPECT_EQ(11.11, kicker_copy_pointer->speed());
}


//TODO more kicker tests