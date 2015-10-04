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
		VLOG(5) << "(" << x.first.first << ", " << x.first.second << ") -> " <<
				x.second.first->size() << " points curve1 " <<
				x.second.second->size() << " points curve2 " << std::endl;
	}
}

/*
 * Insert points on one curve into grid
 */
void Grid::init(Vector_2 offset_vec)
{
	VLOG(6) << "insert grid";
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
			QuadTree* treeNode = it->second.first;
			treeNode->insert(pt, k);
		}
	}
	print_grid();
	VLOG(6) << "finish grid";
}


