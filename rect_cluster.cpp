#include <algorithm>
#include <limits>
#include <math.h>
#include <random>

#include <CGAL/number_utils.h>

#include "easylogging++.h"
#include "rect_cluster.h"

RectCluster::RectCluster(
		const Curve &curve1, const Curve &curve2, double lb, double ub, double eps):
    curve1_(curve1),
    curve2_(curve2),
    lb_(lb),
    ub_(ub),
    eps_(eps)
{
  len_cell_ = 1 << (int)(round(ceil(log2(2 * ub_))));
  VLOG(6) << "Grid side length:   " << len_cell_;
	
	// random shift
	std::default_random_engine generator;
  std::uniform_real_distribution<double> distribution(0.0, (double) len_cell_);
	double rand_shift_x = distribution(generator);
	double rand_shift_y = distribution(generator);
	Vector_2 vec(rand_shift_x, rand_shift_y);
	LOG(INFO) << "Random shift: " << vec;
	
	// Build grid
	grid_ = new Grid(curve1, curve2, lb, ub, len_cell_);
	grid_->init(vec);
}

void RectCluster::partition()
{
	for (auto it = grid_->begin(); it != grid_->end(); it++)
	{
		// quad-tree from containing points from curve1
		QuadTree* curr_qt = it->second.first;
		// check if the node contains points from curve 1
		if (curr_qt->is_empty())
		{
			continue;
		}
		// find the neighboring nodes that contain points from curve 2
		vector<QuadTree*> nbrs = grid_->neighbors(it->first, 1);
		//VLOG(7) << it->second.first->to_string() << " ---------- ";
		for (auto& qt : nbrs)
		{
			WSPD wspd(curr_qt, qt, 1 / eps_, lb_);
			gen_rect(wspd);
		}
	}
	
	// after generating all rectangles, build a dependency graph of them and topologically sort them
	build_rect_graph();
	topo_sort();
}

void RectCluster::gen_rect(WSPD wspd)
{
	for (auto it = wspd.begin(); it != wspd.end(); it++)
	{
		for (IndexSegment seg1 : it->first->idx_segments())
		{
			for (IndexSegment seg2 : it->second->idx_segments())
			{
				Rectangle* rect = new Rectangle(seg1, seg2);
				rects_.insert(rect);
				// populate inv_rects
				vector<pair<int, int>> coords = rect->boundary_pts();
				
				for (auto coord : coords)
				{
					VLOG(9) << coord.first << ", " << coord.second;
					if (inv_rects_.find(coord) != inv_rects_.end())
					{
						LOG(ERROR) << "RectCluster: index (" << coord.first << ", " << coord.second 
								<< ") is in multiple rectangels";
					}
					inv_rects_.emplace(coord, rect);
				}
			}
		}
	}
}

string RectCluster::export_rects()
{
	stringstream sstm;
/* 	bool matrix[curve1_.size()][curve2_.size()] = {0};
	for (auto it = inv_rects_.begin(); it != inv_rects_.end(); it++)
	{
		pair<int, int> idx = it->first;
		matrix[idx.first][idx.second] = true;
	}
	for (int i = 0; i < curve1_.size(); i++)
	{
		for (int j = 0; j < curve2_.size(); j++)
		{
			sstm << (matrix[i][j] ? "." : " ");
		}
		sstm << endl;
	}
	return sstm.str(); */
	sstm << curve1_.size() << " " << curve2_.size() << endl;
	for (auto rect : rects_)
	{
		sstm << rect->to_string() << endl;
	}
	return sstm.str();
}

string RectCluster::summarize()
{
	stringstream sstm;
	sstm << "{nRects=" << rects_.size() << ", nBdPts=" << inv_rects_.size() << "} ";
	return sstm.str();
}

void RectCluster::build_rect_graph()
{
	for (auto rect : rects_)
	{
		// add predecessors by finding the right and top elements of the right and top boundaries 
		// respectively
		for (pair<int, int> p : rect->top())
		{
			if (p.first < curve1_.size() - 1)
			{
				pair<int, int> next = make_pair(p.first + 1, p.second);
				auto it = inv_rects_.find(next);
				if (it == inv_rects_.end())
				{
					VLOG(6) << "Upper neighbor is not a boundary point: (" 
							<< next.first << ", " << next.second << ")";
				}
				else
				{
					rect->add_successor(it->second);
					it->second->add_predecessor(rect);
				}
			}
		}
		
		for (pair<int, int> p : rect->right())
		{
			if (p.second < curve2_.size() - 1)
			{
				pair<int, int> next = make_pair(p.first, p.second + 1);
				auto it = inv_rects_.find(next);
				if (it == inv_rects_.end())
				{
					VLOG(6) << "Right neighbor is not a boundary point: (" 
							<< next.first << ", " << next.second << ")";
				}
				else
				{
					rect->add_successor(it->second);
					it->second->add_predecessor(rect);
				}
			}
		}
		
		// top-right corner: need to add the rectangle that contains the upper-right corner
		pair<int, int> p = rect->right().back();
		if ((p.first < curve1_.size() - 1) && (p.second < curve2_.size() - 1))
		{
			pair<int, int> next = make_pair(p.first + 1, p.second + 1);
			auto it = inv_rects_.find(next);
			if (it == inv_rects_.end())
			{
				VLOG(6) << "Upper right neighbor is not a boundary point: (" 
						<< next.first << ", " << next.second << ")";
			}
			else
			{
				rect->add_successor(it->second);
				it->second->add_predecessor(rect);
			}
		}
		
	}
}

/*
 * topologically sort the rectangles after having built the dependency graph
 */
void RectCluster::topo_sort()
{
	// start with the rectangle that contains (0, 0)
	LOG_IF(!inv_rects_.count(make_pair(0, 0)), ERROR) << "Origin is not a boundary point";
	Rectangle* base_rect = inv_rects_[make_pair(0, 0)];
	visit(base_rect);
	for (auto rect : rects_)
	{
		if (rect->is_not_marked())
		{
			LOG(WARNING) << "There is a point from which origin cannot reach";
			// these rectangles do not need to be included in graph
			//visit(rect);
		}
	}
}

void RectCluster::visit(Rectangle* rect)
{
	if (rect->is_temp_marked())
	{
		LOG(ERROR) << "Rectangle graph is not a DAG!";
		return;
	}
	if (rect->is_not_marked())
	{
		rect->mark_temp();
		for (auto succ_rect : rect->successors())
		{
			visit(succ_rect);
		}
		rect->mark_permanent();
		sorted_rects_.push_front(rect);
	}
}

void RectCluster::compute_shortest_path()
{
	// put origin in the shortest path first
	shortest_path_.emplace(make_pair(0, 0), 0.0);
	
	for (auto rect : sorted_rects_)
	{
		// all cells in the rectangle are assumed to have the same approximate value
		pair<int, int> sample_pt = rect->sample();
		double approx_val = 
				CGAL::sqrt(CGAL::squared_distance(curve1_[sample_pt.first], curve2_[sample_pt.second]));
		compute_left_edge(rect, approx_val);
		// bottom is calculated after left because the bottom left corner is in rect->top()
		compute_bottom_edge(rect, approx_val);
	}
}

void RectCluster::compute_left_edge(Rectangle* rect, double approx_val)
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

void RectCluster::compute_bottom_edge(Rectangle* rect, double approx_val)
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
	it = shortest_path_.find(make_pair(st.first - 1, st.second - 1));
	if (it != shortest_path_.end())
	{
		prev_down_dist = it->second
				+ CGAL::sqrt(CGAL::squared_distance(curve1_[st.first - 1], curve2_[st.second - 1]));
	}
	
	for (auto pt : rect->left())
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

void RectCluster::compute_right_edge(Rectangle* rect, double approx_val)
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
void RectCluster::compute_right_edge_case1(
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
		left_mins.push_back(std::min(shortest_path_[p], left_mins.back()));
	}
	
	int idx = 0;
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
			<< "Bottom edge not computed yet when computing right edge";
		double min2 = shortest_path_[qj] + approx_val * idx;
		
		// case 3: q on horizontal segment after the diagonal that crosses p
		double min3 = prev_val + approx_val;
		
		double curr_val = std::min(min1, std::min(min2, min3));
		LOG_IF(!shortest_path_.count(p), ERROR)
			<< "A value on right edge already exists";
		shortest_path_.emplace(p, curr_val);
		
		// update
		idx++;
		prev_val = curr_val;
	}
}

void RectCluster::compute_right_edge_case2(
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
		
		LOG_IF(!shortest_path_.count(p), ERROR)
			<< "compute_edge_case2: A value on edge already exists";
		shortest_path_.emplace(p, curr_val);
		
		// update
		prev_val = curr_val;
	}
}

vector<double> RectCluster::find_window_min(
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
				<< "find_window_min: Opposing edge not computed yet when computing top/right edge";
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

void RectCluster::compute_top_edge(Rectangle* rect, double approx_val)
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
	