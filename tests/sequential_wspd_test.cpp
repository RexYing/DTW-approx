#include "easylogging++.h"
#include "gtest/gtest.h"
#include "sequential_wspd.h"

INITIALIZE_EASYLOGGINGPP

namespace {

// The fixture for testing class RectClusterTest.
class SequentialWSPDTest : public ::testing::Test {
protected:

	SequentialWSPD* seq_wspd;

  SequentialWSPDTest() {
    Curve curve1 = GenHorizontalCurve(16, 0);
		Curve curve2 = GenHorizontalCurve(16, 8);
		seq_wspd = new SequentialWSPD(curve1, curve2, 2);
  }

  virtual ~SequentialWSPDTest() {
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
	
private:
	Curve GenHorizontalCurve(int n, double y)
	{
		Curve curve;
		for (int i = 0; i < n; i++)
		{
			Point_2 pt(i, y);
			curve.push_back(pt);
		}
		return curve;
	}
};

// Tests that the SequentialWSPD correctly pairs point sets
TEST_F(SequentialWSPDTest, MethodPairing) {
	std::vector<IndexSegment> segs1 = seq_wspd->segs1();
	std::vector<IndexSegment> segs2 = seq_wspd->segs2();
	LOG(INFO) << segs1.size();
  for (int i = 0; i < segs1.size(); i++) 
	{
		LOG(INFO) << "(" << segs1[i].first << ", " << segs1[i].second << ")  (" 
				<< segs2[i].first << ", " << segs2[i].second << ")";
	}
  EXPECT_EQ(16, segs1.size());
}

}  // namespace
