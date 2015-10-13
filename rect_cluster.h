#include <unordered_map>

#include "grid.h"
#include "rectangle.h"

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
	
	// export rectangles and their boundary points
	string export_rects();
	// summarize the ractangle partition
	string summarize();

private:
	Curve curve1_;
	Curve curve2_;
	double lb_;
	double ub_;
	double eps_;
	// the side length of individual grid cells
	long len_cell_;

	Grid* grid_;
	vector<Rectangle> rects_;
	// maps boundary points to the rectangle they belong to
	unordered_map<pair<int, int>, Rectangle, boost::hash<pair<int, int>> > inv_rects_;
	
	void gen_rect(WSPD wspd);
	
	//void build_rect_graph();
};
