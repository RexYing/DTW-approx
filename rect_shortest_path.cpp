#include <algorithm>
#include <limits>
#include <math.h>

#include <CGAL/number_utils.h>

#include "rect_shortest_path.h"

RectShortestPath::RectShortestPath(
		const Curve &curve1, 
		const Curve &curve2, 
		const list<Rectangle*>& sorted_rects, 
		const CellToRect& inv_rects):
		curve1_(curve1), curve2_(curve2), sorted_rects_(sorted_rects), inv_rects_(inv_rects)
{ 
	// put origin in the shortest path first
	shortest_path_.emplace(make_pair(0, 0), 0.0);
}

CellToDouble RectShortestPath::compute_shortest_path()
{
	
	for (auto rect : sorted_rects_)
	{
		// all cells in the rectangle are assumed to have the same approximate value
		pair<int, int> sample_pt = rect->sample();
		double approx_val = 
				CGAL::sqrt(CGAL::squared_distance(curve1_[sample_pt.first], curve2_[sample_pt.second]));
		compute_left_edge(rect, approx_val);
		// bottom is calculated after left because the bottom left corner is in rect->top()
		compute_bottom_edge(rect, approx_val);
		
		compute_right_edge(rect, approx_val);
		
		compute_top_edge(rect, approx_val);
	}
	return shortest_path_;
}

void RectShortestPath::compute_left_edge(Rectangle* rect, double approx_val)
{
	double prev_dist = numeric_limits<double>::max();
	double prev_left_dist = numeric_limits<double>::max();
	// the point below the current point (predecessor)
	pair<int, int> st = rect->left().front();
	auto it = shortest_path_.find(make_pair(st.first - 1, st.second));
	if (it != shortest_path_.end())
	{
		prev_dist = it->second 
				+ CGAL::sqrt(CGAL::squared_distance(curve1_[st.first - 1], curve2_[st.second]));
	}
	else if (st.first == 0 && st.second == 0) // special case: at starting point
	{
		prev_dist = CGAL::sqrt(CGAL::squared_distance(curve1_[st.first], curve2_[st.second]));
	}
	
	it = shortest_path_.find(make_pair(st.first - 1, st.second - 1));
	if (it != shortest_path_.end())
	{
		prev_left_dist = it->second
				+ CGAL::sqrt(CGAL::squared_distance(curve1_[st.first - 1], curve2_[st.second - 1]));
	}

	for (auto pt : rect->left())
	{
		if ((pt.first == 0) && (pt.second == 0))
		{
			continue;
		}
		
		double curr_dist = numeric_limits<double>::max();
		double curr_left_dist = numeric_limits<double>::max();
		it = shortest_path_.find(make_pair(pt.first, pt.second - 1));
		if (it != shortest_path_.end())
		{
			curr_left_dist = it->second
					+ CGAL::sqrt(CGAL::squared_distance(curve1_[pt.first], curve2_[pt.second - 1]));
		}
		
		curr_dist = std::min(curr_dist, std::min(curr_left_dist, std::min(prev_left_dist, prev_dist)));
		shortest_path_.emplace(pt, curr_dist);
		
		// update prev dists
		prev_dist = curr_dist + approx_val;
		prev_left_dist = curr_left_dist;
	}
}

void RectShortestPath::compute_bottom_edge(Rectangle* rect, double approx_val)
{
	double prev_dist = numeric_limits<double>::max();
	double prev_down_dist = numeric_limits<double>::max();
	// the point below the current point (predecessor)
	pair<int, int> st = rect->bottom().front();
	auto it = shortest_path_.find(make_pair(st.first, st.second - 1));
	if (it != shortest_path_.end())
	{
		prev_dist = it->second 
				+ CGAL::sqrt(CGAL::squared_distance(curve1_[st.first], curve2_[st.second - 1]));
	}
	else if (st.first == 0 && st.second == 0) // special case: at starting point
	{
		prev_dist = CGAL::sqrt(CGAL::squared_distance(curve1_[st.first], curve2_[st.second]));
	}
	
	it = shortest_path_.find(make_pair(st.first - 1, st.second - 1));
	if (it != shortest_path_.end())
	{
		prev_down_dist = it->second
				+ CGAL::sqrt(CGAL::squared_distance(curve1_[st.first - 1], curve2_[st.second - 1]));
	}
	
	for (auto pt : rect->bottom())
	{
		if ((pt.first == 0) && (pt.second == 0))
		{
			continue;
		}
		
		double curr_dist = numeric_limits<double>::max();
		double curr_down_dist = numeric_limits<double>::max();
		it = shortest_path_.find(make_pair(pt.first - 1, pt.second));
		if (it != shortest_path_.end())
		{
			curr_down_dist = it->second
					+ CGAL::sqrt(CGAL::squared_distance(curve1_[pt.first - 1], curve2_[pt.second]));
		}
		
		curr_dist = std::min(curr_dist, std::min(curr_down_dist, std::min(prev_down_dist, prev_dist)));
		shortest_path_.emplace(pt, curr_dist);
		
		// update prev dists
		prev_dist = curr_dist + approx_val;
		prev_down_dist = curr_down_dist;
	}
}

void RectShortestPath::compute_right_edge(Rectangle* rect, double approx_val)
{
	// check degenerate case
	if (rect->width() < 1)
	{
		LOG(ERROR) << "Rectangle width < 1";
	}
	else if (rect->width() == 1)
	{
		//no need to compute since left=right
		return;
	}

	if (rect->width() >= rect->height())
	{
		compute_right_edge_case1(
				rect->right(), rect->left(), rect->width(), rect->right().size(), approx_val);
	}
	else
	{
		compute_right_edge_case1(
				rect->right(), rect->left(), rect->width(), rect->width(), approx_val);
		// the upper part of right edge exceeding the width
		compute_right_edge_case2(
				rect->right(), rect->left(), rect->width(), rect->width(), approx_val);
	}
}

/*
 * Params:
 *	dist: the number of cell distance between the edge on which we compute shortest path, and its
 *		opposing edge (inclusive on both sides)
 */
void RectShortestPath::compute_right_edge_case1(
		const vector<pair<int, int>>& right, 
		const vector<pair<int, int>>& left, 
		int dist,
		int num_pts, 
		double approx_val)
{
	vector<double> left_mins;
	for (auto p : left)
	{
		LOG_IF(!shortest_path_.count(p), ERROR) 
				<< "Left edge not computed yet when computing right edge";
		if (left_mins.empty())
		{
			left_mins.push_back(shortest_path_[p]);
		}
		else
		{
			left_mins.push_back(std::min(shortest_path_[p], left_mins.back()));
		}
	}
	
	// start from the second cell in the edge since we already have the first one when computing 
	// left/bottom
	int idx = 1;
	pair<int, int> p = right.front();
	double prev_val = shortest_path_[p];

	// assume the bottom right corner is already computed when computing the bottom edge
	for (int i = 1; i < num_pts; i++)
	{
		p = right[i];
		
		// case 1: q on left edge below p
		double min1 = left_mins[i] + approx_val * (dist - 1);
		
		// case 2: q on horizontal segment not after the diagonal that crosses p
		pair<int, int> qj = make_pair(p.first - idx, p.second - idx);
		LOG_IF(!shortest_path_.count(qj), ERROR)
				<< "Bottom edge not computed yet when computing right edge: (" 
				<< qj.first << ", " << qj.second << ")";
		double min2 = shortest_path_[qj] + approx_val * idx;
		
		// case 3: q on horizontal segment after the diagonal that crosses p
		double min3 = prev_val + approx_val;
		
		double curr_val = std::min(min1, std::min(min2, min3));
		if (shortest_path_.count(p))
		{
			if (curr_val < shortest_path_[p])
			{
				LOG(INFO) << "replace corner value";
				shortest_path_[p] = curr_val;
			}
		}
		else
		{
			shortest_path_.emplace(p, curr_val);
		}
		
		// update
		idx++;
		prev_val = curr_val;
	}
}

void RectShortestPath::compute_right_edge_case2(
		const vector<pair<int, int>>& right, 
		const vector<pair<int, int>>& left, 
		int dist,
		int start_idx, 
		double approx_val)
{
	vector<double> window_mins = find_window_min(left, start_idx, dist);
	
	pair<int, int> p = right[start_idx - 1];
	double prev_val = shortest_path_[p];
	// assume the value for the previous cell which falls into case1 has already been computed
	for (int i = start_idx; i < right.size(); i++)
	{
		p = right[i];
		
		// case 1: has to pass through its previous point on the edge
		double min1 = prev_val + approx_val;
		
		// case 2: need minimum in a sliding window
		double min2 = window_mins[i - start_idx] + approx_val * (dist - 1);
		
		double curr_val = std::min(min1, min2);
		
		if (shortest_path_.count(p))
		{
			if (curr_val < shortest_path_[p])
			{
				LOG(INFO) << "replace corner value";
				shortest_path_[p] = curr_val;
			}
		}
		else
		{
			shortest_path_.emplace(p, curr_val);
		}
		
		// update
		prev_val = curr_val;
	}
}

vector<double> RectShortestPath::find_window_min(
		const vector<pair<int, int>>& edge, int start_idx, int win_size)
{
	// minimum value on the opposing edge within a sliding window
	vector<double> window_min;
	// subsequence maintained to efficiently find the minimum in window
	list<double> subseq;
	list<int> subseq_indices;
	
	// build the subsequence from start_idx-win_size+1 to start_idx-1
	for (int i = start_idx - win_size + 1; i < start_idx; i++)
	{
		LOG_IF(!shortest_path_.count(edge[i]), ERROR)
				<< "find_window_min: Opposing edge not computed yet when computing top/right edge (" 
				<< edge[i].first << ", " << edge[i].second << ")";
		double curr_val = shortest_path_[edge[i]];
		while (!subseq.empty() && subseq.back() >= curr_val)
		{
			subseq.pop_back();
			subseq_indices.pop_back();
		}
		subseq.push_back(curr_val);
		subseq_indices.push_back(i);
	}
	
	// start filling in the window_min from start_idx
	for (int i = start_idx; i < edge.size(); i++)
	{
		LOG_IF(!shortest_path_.count(edge[i]), ERROR)
				<< "find_window_min: Opposing edge not computed yet when computing top/right edge";
		double curr_val = shortest_path_[edge[i]];
		if (!subseq_indices.empty() && subseq_indices.front() == i - win_size)
		{
			subseq.pop_front();
			subseq_indices.pop_front();
		}
		// insert new value
		while (!subseq.empty() && subseq.back() >= curr_val)
		{
			subseq.pop_back();
			subseq_indices.pop_back();
		}
		subseq.push_back(curr_val);
		subseq_indices.push_back(i);
		// min is the first element
		window_min.push_back(subseq.front());
	}
	return window_min;
}	

void RectShortestPath::compute_top_edge(Rectangle* rect, double approx_val)
{
	// check degenerate case
	if (rect->height() < 1)
	{
		LOG(ERROR) << "Rectangle height < 1";
	}
	else if (rect->height() == 1)
	{
		//no need to compute since top=bottom
		return;
	}
	
	if (rect->height() >= rect->width())
	{
		compute_right_edge_case1(
				rect->top(), rect->bottom(), rect->height(), rect->top().size(), approx_val);
	}
	else
	{
		compute_right_edge_case1(
				rect->top(), rect->bottom(), rect->height(), rect->height(), approx_val);
		// the upper part of right edge exceeding the width
		compute_right_edge_case2(
				rect->top(), rect->bottom(), rect->height(), rect->height(), approx_val);
	}
}
