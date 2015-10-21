#include <unordered_map>
#include <unordered_set>

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
	
	/*
	 * compute shortest path for the graph of rectangles after partition
	 */
	void compute_shortest_path();
	
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
	unordered_set<Rectangle*> rects_;
	// maps boundary points to the rectangle they belong to
	unordered_map<pair<int, int>, Rectangle*, boost::hash<pair<int, int>> > inv_rects_;
	unordered_map<pair<int, int>, double, boost::hash<pair<int, int>> > shortest_path_;
	
	// contains the same elements as rects_, but is topologically sorted
	list<Rectangle*> sorted_rects_;
	
	void gen_rect(WSPD wspd);
	
	void build_rect_graph();
	
  /*
   * topologically sort the rectangles after having built the dependency graph.
   */
	void topo_sort();
	// used for topological sort
	void visit(Rectangle* rect);
	
	// compute shortest path values for the boundary points of a given rectangle
	void compute_left_edge(Rectangle* rect, double approx_val);
	void compute_right_edge(Rectangle* rect, double approx_val);
	void compute_top_edge(Rectangle* rect, double approx_val);
	void compute_bottom_edge(Rectangle* rect, double approx_val);
	
	void compute_right_edge_case1(
		const vector<pair<int, int>>& right, 
		const vector<pair<int, int>>& left, 
		int dist,
		int num_pts, 
		double approx_val);
	void compute_right_edge_case2(
		const vector<pair<int, int>>& right, 
		const vector<pair<int, int>>& left, 
		int dist,
		int start_idx, 
		double approx_val);
	vector<double> find_window_min(const vector<pair<int, int>>& edge, int start_idx, int win_size);
};
