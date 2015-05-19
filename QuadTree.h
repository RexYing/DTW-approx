#pragma once
#include <sstream>
#include <vector>
#include <CGAL/Simple_cartesian.h>
#include "easylogging++.h"

using namespace std;

typedef CGAL::Simple_cartesian<double> Kernel;
typedef Kernel::Point_2 Point_2;
typedef Kernel::Direction_2 Direction_2;
typedef Kernel::Vector_2 Vector_2;
typedef Kernel::Iso_rectangle_2 BBox; //bounding box for 2D point sets

class QuadTree
{
	public:
		// every node has 4 children
		static const int QT_NODE_CAPACITY = 4;

		BBox bbox;

    // representative point (an arbitrary point in a non-empty node
		Point_2 p;

    enum NodeType {EMPTY, LEAF, NODE};
    NodeType node_type;

		QuadTree* ch[4];

		QuadTree(vector<Point_2> &point_set);

    double quadtree_dist(QuadTree that);

    string stringify_point_2(Point_2 p)
    {
        stringstream sstm;
        CGAL::set_pretty_mode(sstm);
        sstm << p;
        return sstm.str();
    }

    string stringify_bbox(BBox bbox)
    {
        stringstream sstm;
        CGAL::set_pretty_mode(sstm);
        sstm << "Bounding box: " << bbox;
        return sstm.str();
    }
    string to_string()
    {
        stringstream sstm;
        CGAL::set_pretty_mode(sstm);
        sstm << "QuadTree: {center=" << center_ << ", radius=" << radius_ << ", rep_pt=" << p << "}";
        return sstm.str();
    }

    double get_radius();
    Point_2 get_center();

  private:

    double radius_;
    Point_2 center_;

    Direction_2 pos_x_dir_;
    Direction_2 pos_y_dir_;
    Direction_2 neg_x_dir_;
    Direction_2 neg_y_dir_;

    void calcBBox(vector<Point_2> &point_set);

    void subdivide(vector<Point_2> &point_set);
};
