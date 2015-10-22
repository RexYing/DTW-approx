#include "easylogging++.h"
#include "gtest/gtest.h"
#include "rect_shortest_path.h"

INITIALIZE_EASYLOGGINGPP

namespace {

// The fixture for testing class RectClusterTest.
class RectShortestPathTest : public ::testing::Test {
protected:

	Rectangle* single_rect;
	RectShortestPath* impl_single;
	
	Rectangle* rect00;
	Rectangle* rect10;
	Rectangle* rect01;
	Rectangle* rect11;
	RectShortestPath* impl_2_2;

  RectShortestPathTest() {
		single_rect = new Rectangle(make_pair(0, 10), make_pair(0, 8));
		
		rect00 = new Rectangle(make_pair(0, 10), make_pair(0, 8));
		rect01 = new Rectangle(make_pair(0, 10), make_pair(8, 20));
		rect10 = new Rectangle(make_pair(10, 20), make_pair(0, 8));
		rect11 = new Rectangle(make_pair(10, 20), make_pair(8, 20));
  }

  virtual ~RectShortestPathTest() {
    // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  virtual void SetUp() {
		// Single rectangle test
		list<Rectangle*> sorted_rects;
		sorted_rects.push_back(single_rect);
		Curve curve1 = GenHorizontalCurve(10, 0);
		Curve curve2 = GenHorizontalCurve(8, 1);
		
		CellToRect inv_rects;
		for (auto pt : single_rect->boundary_pts())
		{
			inv_rects.emplace(pt, single_rect);
		}
		
    impl_single = new RectShortestPath(curve1, curve2, sorted_rects, inv_rects);
		
		// 2-by-2 rectangles test
		list<Rectangle*> sorted_rects_2_2 = {rect00, rect10, rect01, rect11};
		Curve curve3 = GenHorizontalCurve(20, 0);
		Curve curve4 = GenHorizontalCurve(20, 1);
		
		CellToRect inv_rects_2_2;
		for (auto rect : sorted_rects_2_2)
		{
			for (auto pt : rect->boundary_pts())
			{
				inv_rects_2_2.emplace(pt, rect);
			}
		}
    impl_2_2 = new RectShortestPath(curve3, curve4, sorted_rects_2_2, inv_rects_2_2);
  }

  virtual void TearDown() {

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

// Tests that the RectShortestPath::find_window_min method finds minimum in a sliding window.
TEST_F(RectShortestPathTest, MethodFindWindowWin) {
	//vector<double>
	const int start_idx = 5;
  const int win_size = 5;
	CellToDouble shortest_path;
	double vals[10] = {1, 4, 2, 8, 5, 7, 9, 3, 11, 5};
	for (int i = 0; i < 10; i++)
	{
		shortest_path.emplace(make_pair(i, 0), vals[i]);
	}
	
	impl_single->set_shortest_path(shortest_path);

	vector<double> window_min = 
			impl_single->find_window_min(single_rect->left(), start_idx, win_size);
	
	vector<double> expected_window_min = {2, 2, 3, 3, 3};
	ASSERT_EQ(expected_window_min.size(), window_min.size()) 
			<< "Window min length is incorrect";
  for (int i = 0; i < window_min.size(); ++i) {
		EXPECT_EQ(expected_window_min[i], window_min[i]) << "Window min is incorrect at index " << i;
	}
}

// Tests that RectShortestPath computes shortest path on left edges correctly
TEST_F(RectShortestPathTest, MethodComputeLeftEdge) {
	impl_single->compute_left_edge(single_rect, 1);
	CellToDouble shortest_path = impl_single->shortest_path();
	
	EXPECT_EQ(0, shortest_path[make_pair(0, 0)]);
	EXPECT_EQ(1, shortest_path[make_pair(1, 0)]);
	EXPECT_EQ(2, shortest_path[make_pair(2, 0)]);
	EXPECT_EQ(3, shortest_path[make_pair(3, 0)]);
	
	EXPECT_EQ(0, shortest_path.count(make_pair(10, 0)));
}

// Tests that RectShortestPath computes shortest path on bottom edges correctly
TEST_F(RectShortestPathTest, MethodComputeBottomEdge) {
	impl_single->compute_bottom_edge(single_rect, 1);
	CellToDouble shortest_path = impl_single->shortest_path();
	
	EXPECT_EQ(0, shortest_path[make_pair(0, 0)]);
	EXPECT_EQ(1, shortest_path[make_pair(0, 1)]);
	EXPECT_EQ(2, shortest_path[make_pair(0, 2)]);
	
	EXPECT_EQ(0, shortest_path.count(make_pair(0, 8)));
}

// Tests that RectShortestPath computes shortest path on left edges correctly
TEST_F(RectShortestPathTest, MethodComputeRightEdge) {
	impl_single->compute_left_edge(single_rect, 1);
	impl_single->compute_bottom_edge(single_rect, 1);
	impl_single->compute_right_edge(single_rect, 1);

	CellToDouble shortest_path = impl_single->shortest_path();
	
	// extract right edge values
	vector<double> actual_val;
	for (int i = 0; i < 10; i++)
	{
		actual_val.push_back(shortest_path[make_pair(i, 7)]);
	}
	
	vector<double> expected_val = {7, 7, 7, 7, 7, 7, 7, 7, 8, 9};
	for (int i = 0; i < 10; i++)
	{
		EXPECT_EQ(expected_val[i], actual_val[i]) 
				<< "The shortest path to (" << i << ", 7) is not correct";
	}
	
	EXPECT_EQ(0, shortest_path.count(make_pair(10, 0)));
}

}  // namespace
