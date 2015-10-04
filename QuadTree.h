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

    QuadTree** children();
    QuadTree* child(int index);
		QuadTree* ch_[4];


		QuadTree(const vector<Point_2> &point_set);
		QuadTree(const vector<Point_2> &point_set, const vector<int> &indices);
		QuadTree(const vector<Point_2> &point_set, const vector<int> &indices, short curve);

    virtual void init();
		
		void insert(Point_2 pt, int index);

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
    virtual string to_string()
    {
        stringstream sstm;
        CGAL::set_pretty_mode(sstm);
        sstm << "QuadTree: {c=" << center_ << ", r=" << radius_ << ", size=" << size() 
						<< ", rep_pt=" << p << "}";
        return sstm.str();
    }

    int id();
    double radius();
    Point_2 center();
    vector<int> indices();
		virtual int size();
		bool is_empty();
		
		virtual void subdivide();

  protected:

    vector<Point_2> point_set1_;
    vector<int> indices_;

    vector<int> default_indices(int n);

    void set_empty();
    void calc_bbox(vector<Point_2> &point_set);
    vector<vector<Point_2>> partition(vector<Point_2> point_set,
                                      vector<int> indices,
                                      vector<vector<int>> &ch_indices);

  private:

    // each node has a unique id property.
    // a node closer to root always receives a small id.
    static int max_id_;
    int id_;
    double radius_;
    Point_2 center_;
		// optional: 0 for unknown; 1 for first curve; 2 for second curve
		short curve_;
		
		// flag indicating if the cell has been subdivided
		bool has_subdivided_;
		// flag indicating if the cell has be initialized
		bool has_initialized_;

    Direction_2 pos_x_dir_;
    Direction_2 pos_y_dir_;
    Direction_2 neg_x_dir_;
    Direction_2 neg_y_dir_;

};
