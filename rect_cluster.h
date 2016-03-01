#include "grid.h"
#include "rect_shortest_path.h"
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
	
	/*
	 * Partition by sequential WSPD
	 */
	void sequential_partition();
	
	/*
	 * compute approximate DTW by computing shortest path for the graph of rectangles after partition
	 */
	double compute_approx_dtw();
	
	// export rectangles and their boundary points
	string export_rects();
	// summarize the ractangle partition
	string summarize();
	
protected:
	void gen_rect(vector<IndexSegment> segs1, vector<IndexSegment> segs2);

private:
	Curve curve1_;
	Curve curve2_;
	double lb_;
	double ub_;
	double eps_;
	// the side length of individual grid cells
	long len_cell_;

	Grid* grid_;
	unordered_set<Rectangle*> rects_;
	// maps boundary points to the rectangle they belong to
	unordered_map<pair<int, int>, Rectangle*, boost::hash<pair<int, int>> > inv_rects_;
	unordered_map<pair<int, int>, double, boost::hash<pair<int, int>> > shortest_path_;
	
	// contains the same elements as rects_, but is topologically sorted
	list<Rectangle*> sorted_rects_;
	
	void build_rect_graph();
	
  /*
   * topologically sort the rectangles after having built the dependency graph.
   */
	void topo_sort();
	// used for topological sort
	void visit(Rectangle* rect);
};
