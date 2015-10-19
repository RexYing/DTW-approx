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
		pair<int, int> p = rect->right().front();
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
	for (auto rect : rects_)
	{
		if (rect->is_not_marked())
		{
			visit(rect);
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