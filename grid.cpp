#include <math.h>
#include <CGAL/number_utils.h>

#include "easylogging++.h"
#include "grid.h"

Grid::Grid(const Curve &curve1, const Curve &curve2, double lb, double ub, long len_cell):
    curve1_(curve1),
    curve2_(curve2),
    lb_(lb),
    ub_(ub),
		len_cell_(len_cell)
{ }

void Grid::print_grid()
{
	for (auto& x: grid_)
	{
		VLOG(6) << "(" << x.first.first << ", " << x.first.second << ") -> " <<
				x.second.first->size() << " points curve1 " <<
				x.second.second->size() << " points curve2 " << std::endl;
	}
}

/*
 * Insert points on one curve into grid
 */
void Grid::insert_grid(Vector_2 offset_vec)
{
	for (int k = 0; k < curve1_.size(); k++)
	{
		Point_2 pt = curve1_[k] + offset_vec;
		long i = pt.x() / len_cell_;
		long j = pt.y() / len_cell_;
		GridIndex ind = make_pair(i, j);
		GridMap::const_iterator it = grid_.find(ind);
		if (it == grid_.end()) {
			Curve c1;
			Curve c2;
			c1.push_back(pt);
			
			vector<int> indices1 = {k};
			vector<int> indices2;
			
			QuadTree* t1 = new QuadTree(c1, indices1, 1);
			QuadTree* t2 = new QuadTree(c2, indices2, 2);
			TreeNodePair pair = make_pair(t1, t2);
			grid_.emplace(ind, pair);
		}
		else
		{
			QuadTree* treeNode = it->second.first;
			treeNode->insert(pt, k);
		}
	}

	for (int k = 0; k < curve2_.size(); k++)
	{
		Point_2 pt = curve2_[k] + offset_vec;
		long i = pt.x() / len_cell_;
		long j = pt.y() / len_cell_;
		GridIndex ind = make_pair(i, j);
		GridMap::const_iterator it = grid_.find(ind);
		if (it == grid_.end())
		{
			Curve c1;
			Curve c2;
			c2.push_back(pt);
			
			vector<int> indices1;
			vector<int> indices2 = {k};
			
			QuadTree* t1 = new QuadTree(c1, indices1, 1);
			QuadTree* t2 = new QuadTree(c2, indices2, 2);
			TreeNodePair pair = make_pair(t1, t2);
			grid_.emplace(ind, pair);
		}
		else
		{
			QuadTree* treeNode = it->second.second;
			treeNode->insert(pt, k);
		}
	}
}

void Grid::init(Vector_2 offset_vec)
{
	VLOG(6) << "Building grid";
	insert_grid(offset_vec);
	// initialize quadtrees
	for (auto it = begin(); it != end(); it++)
	{
		it->second.first->init();
		it->second.second->init();
	}
	print_grid();
	VLOG(6) << "finish grid";
}

vector<QuadTree*> Grid::neighbors(GridIndex grid_idx, short curve)
{
	vector<QuadTree*> nbrs;
	long x = grid_idx.first;
	long y = grid_idx.second;
	if (curve != 1 && curve != 2)
	{
		LOG(ERROR) << "Grid::neighbors: unrecognized curve number";
		return nbrs;
	}
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <=1; j++)
		{
			GridMap::const_iterator it = grid_.find(make_pair(x + i, y + j));
			if (it != grid_.end())
			{
				// The number for paired_curve is 2 if curve is 1, and vice versa.
				QuadTree* nbr_qt = curve == 1 ? it->second.second : it->second.first;
				if (!nbr_qt->is_empty())
				{
					nbrs.push_back(nbr_qt);
				}
			}
		}
	}
	return nbrs;
}

GridMap::const_iterator Grid::begin()
{
	return grid_.begin();
}

GridMap::const_iterator Grid::end()
{
	return grid_.end();
}
