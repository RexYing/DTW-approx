#include <cmath>

#include "easylogging++.h"
#include "KdTreeWSPD.h"

using namespace std;

KdTreeWSPD::KdTreeWSPD(const vector<Point_2> &point_set, double s) : WSPD(s) 
{
	tree = new Tree(point_set.begin(), point_set.end(), Fair(), TreeTraits());
	VLOG(7) << tree->size();
	
	pairing(tree->root(), tree->root());
}	
	
	
KdTreeNodePairs KdTreeWSPD::pairing(KdTreeNode* t1, KdTreeNode* t2) 
{
	KdTreeNodePairs pairs;
	
	vector<Point_2> pts1;
	t1->tree_items(back_inserter(pts1));
	BBoxInfo bb1(pts1);
	VLOG(7) << bb1.center;
	
	vector<Point_2> pts2;
	t1->tree_items(back_inserter(pts2));
	BBoxInfo bb2(pts2);
	
	// alias for smaller and larger boxes
	double max_radius;
	BBoxInfo sbox, lbox;
	if (bb1.radius >= bb2.radius)
	{
		max_radius = bb1.radius;
		lbox = bb1;
		sbox = bb2;
	}
	else
	{
		max_radius = bb2.radius;
		lbox = bb2;
		sbox = bb1;
	}
	Vector_2 v(bb1.center - bb2.center);
	double dist = CGAL::sqrt(v.squared_length());
	
	// Well-separated condition: diameter * s <= dist between bbox
  if (2 * s * max_radius <= dist) 
	{
		// for the pair of two trivial quadtrees with 1 point that is the same
		// (the only case when radius = dist = 0), we do not insert the pair
		if (dist != 0)
		{
			//pair<Node*, Node*> pair = make_pair(t1, t2);
			//pairs.insert(pair);
		}
  }
	
	return pairs;
}