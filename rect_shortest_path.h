#include <list>
#include <unordered_map>

#include "grid.h"
#include "rectangle.h"

typedef unordered_map<pair<int, int>, Rectangle*, boost::hash<pair<int, int>> > CellToRect;
typedef unordered_map<pair<int, int>, double, boost::hash<pair<int, int>> > CellToDouble;

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
			const CellToRect& inv_rects);
			
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
	vector<double> find_window_min(const vector<pair<int, int>>& edge, int start_idx, int win_size);
	
	CellToDouble shortest_path()
	{
		return shortest_path_;
	}
	
	/* For tests only */
	void set_shortest_path(CellToDouble shortest_path)
	{
		shortest_path_ = shortest_path;
	}
	
private:
	Curve curve1_;
	Curve curve2_;
	
	list<Rectangle*> sorted_rects_;
	// maps boundary points to the rectangle they belong to
	CellToRect inv_rects_;
	CellToDouble shortest_path_;
};
