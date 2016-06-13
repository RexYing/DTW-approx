#ifndef RECT_CLUSTER_H
#define RECT_CLUSTER_H

#include "grid.h"
#include "rect_shortest_path.h"
#include "rectangle.h"
#include "sequential_wspd.h"

/*
 * Partition of Dynamic Programming integer coordinate points into family of disjoint rectangles
 */
class RectCluster
{
public:
	
	RectCluster(const Curve &curve1, const Curve &curve2, double lb, double ub, double eps);
	
	/* 
	 * simplified algorithm: no longer needs lb and ub if near-linear theoretical guarantee is not 
	 * needed. As long as the spread of the input sequence points is polynomial, the runtime will be
	 * good.
	 */
	RectCluster(const Curve &curve1, const Curve &curve2, double eps);
	
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
	 * Computes approximate DTW by computing shortest path for the graph of rectangles after partition.
	 * The bool parameter trace_alignment is set to true if the DTW alignment is needed.
	 */
	vector<pair<int, int>> compute_approx_dtw(bool trace_alignment, double& approx_dtw);
	
	// export rectangles and their boundary points
	string export_rects();
	// summarize the ractangle partition
	string summarize();
	
protected:
	/*
	 * Generates rectangles by taking cross product.
	 */
	void gen_rect(vector<IndexSegment> segs1, vector<IndexSegment> segs2);
	
	/*
	 * Generates rectangles by taking a pair of segments from each vector of the same index.
	 */
	void gen_rect_from_pair(vector<IndexSegment> segs1, vector<IndexSegment> segs2);

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

#endif
