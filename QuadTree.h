#pragma once
#include <vector>
#include <CGAL/Simple_cartesian.h>
using namespace std;

typedef CGAL::Simple_cartesian<double> Kernel;
typedef Kernel::Point_2 Point_2;
typedef Kernel::Iso_rectangle_2 BBox; //bounding box for 2D point sets

class QuadTree
{
	public:
		// every node has 4 children
		static const int QT_NODE_CAPACITY = 4;

		BBox bbox;

		Point_2 p;

		QuadTree* ch[4];

		QuadTree(vector<Point_2> &pointSet);
};
