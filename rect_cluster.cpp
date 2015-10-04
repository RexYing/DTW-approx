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
		// check if the node contains points from curve 1
		if (it->second.first->is_empty())
		{
			continue;
		}
		// find the neighboring nodes that contain points from curve 2
		vector<QuadTree*> nbrs = grid_->neighbors(it->first, 1);
		//VLOG(7) << it->second.first->to_string() << " ---------- ";
		for (auto& qt : nbrs)
		{
			//VLOG(7) << qt->to_string();
			//WSPD wspd(&qt, 1 / eps);
		}
	}
}