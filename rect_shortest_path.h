#ifndef RECT_SHORTEST_PATH_H
#define RECT_SHORTEST_PATH_H

#include <list>
#include <unordered_map>

#include "grid.h"
#include "rectangle.h"

typedef unordered_map<pair<int, int>, Rectangle*, boost::hash<pair<int, int>> > CellToRect;
typedef unordered_map<pair<int, int>, double, boost::hash<pair<int, int>> > CellToDouble;
typedef unordered_map<pair<int, int>, pair<int, int>, boost::hash<pair<int, int>> > CellToPrecedent;

/*
 * Class that computes shortest path based on a DAG formed by rectangle clusters
 */
class RectShortestPath
{
public:
	RectShortestPath(
			const Curve &curve1, 
			const Curve &curve2, 
			const list<Rectangle*>& sorted_rects, 
			const CellToRect& inv_rects,
			bool trace_alignment);
			
	CellToDouble compute_shortest_path();
	
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
			double approx_val, Rectangle* rect);
	void compute_right_edge_case2(
			const vector<pair<int, int>>& right, 
			const vector<pair<int, int>>& left, 
			int dist,
			int start_idx, 
			double approx_val);
			
	/*
	 * Find minimum of a sliding window. The last sliding window ends at the end of the edge vector.
	 * params:
	 *   start_idx: the first sliding window is from start_idx-win_size+1 to start_idx
	 *   min_idx: the index of the min of the sliding windows
	 */
	vector<double> find_window_min(
			const vector<pair<int, int>>& edge, 
			int start_idx, 
			int win_size,
			vector<pair<int, int>>& min_indices);
	
	CellToDouble shortest_path()
	{
		return shortest_path_;
	}
	
	/*
	 * Trace back the entire shortest path, from the pair (n-1, m-1) to (0, 0).
	 * To trace the alignment, the shortest path must have been computed first.
	 */
	vector<pair<int, int>> trace_alignment();
	
	/* For tests only */
	void set_shortest_path(CellToDouble shortest_path)
	{
		shortest_path_ = shortest_path;
	}
	
private:
	Curve curve1_;
	Curve curve2_;
	
	bool trace_alignment_;
	
	list<Rectangle*> sorted_rects_;
	// maps boundary points to the rectangle they belong to
	CellToRect inv_rects_;
	CellToDouble shortest_path_;
	
	// only used to trace the alignment (path in the DP table)
	CellToPrecedent precedents_;
};

#endif
