#include <unordered_map>

#include "grid.h"
using namespace std;

struct Rectangle {
	Rectangle(IndexSegment segment1, IndexSegment segment2):
			segment1(segment1), segment2(segment2)
	{ }
	
	IndexSegment segment1;
	IndexSegment segment2;
};

/*
 * Partition of Dynamic Programming integer coordinate points into family of disjoint rectangles
 */
class RectCluster
{
public:
	RectCluster(const Curve &curve1, const Curve &curve2, double lb, double ub, double eps);
	
	/*
	 * Partition DP integer coordinate point set into family of disjoint rectangles.
	 * Each rectangle and its boundary points are stored.
	 */
	void partition();
	
	// Visualizes rectangles and boundary points
	void visualize();

private:
	Curve curve1_;
	Curve curve2_;
	double lb_;
	double ub_;
	double eps_;
	// the side length of individual grid cells
	long len_cell_;

	Grid* grid_;
	vector<Rectangle> rects;
	// maps points to the rectangle they belong to
	unordered_map<pair<int, int>, Rectangle, boost::hash<pair<int, int>> > inv_rects;
	
	void gen_rect(WSPD wspd);
};
