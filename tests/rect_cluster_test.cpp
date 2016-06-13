#include "easylogging++.h"
#include "gtest/gtest.h"
#include "rect_cluster.h"

INITIALIZE_EASYLOGGINGPP

namespace {

// The fixture for testing class RectClusterTest.
class RectClusterTest : public ::testing::Test {
protected:

	RectCluster* rect_cluster;

  RectClusterTest() {
    
  }

  virtual ~RectClusterTest() {
    // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  virtual void SetUp() {
    // Code here will be called immediately after the constructor (right
    // before each test).
  }

  virtual void TearDown() {
    // Code here will be called immediately after each test (right
    // before the destructor).
  }

  // Objects declared here can be used by all tests in the test case for Foo.
};

// Tests that the RectCluster::find_window_min method finds minimum in a sliding window.
TEST_F(RectClusterTest, MethodFindWindowWin) {
  const int win_size = 5;
  EXPECT_EQ(5, win_size);
}

// Tests that Foo does Xyz.
TEST_F(RectClusterTest, DoesXyz) {
  // Exercises the Xyz feature of Foo.
}

}  // namespace
