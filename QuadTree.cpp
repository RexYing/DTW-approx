#include <CGAL/bounding_box.h>
#include <CGAL/number_utils.h>

#include "easylogging++.h"
#include "QuadTree.h"
using namespace std;

QuadTree::QuadTree(vector<Point_2> &point_set)
{
    pos_x_dir_ = Direction_2(1, 0);
    pos_y_dir_ = Direction_2(0, 1);
    neg_x_dir_ = Direction_2(-1, 0);
    neg_y_dir_ = Direction_2(0, -1);


    VLOG(7) << ("creating quad tree...");
    switch (point_set.size())
    {
        case 0:
            center_ = Point_2(0, 0);
            radius_ = -1; // empty: radius is invalid
            node_type = EMPTY;
            break;
        case 1:
            node_type = LEAF;
            calcBBox(point_set);
            p = point_set[0];
            break;
        default:
            node_type = NODE;
            calcBBox(point_set);
            p = point_set[0];
            subdivide(point_set);
    }
    VLOG(8) << to_string();
}

double QuadTree::quadtree_dist(QuadTree that)
{
    return CGAL::sqrt(CGAL::squared_distance(this->get_center(), that.get_center()));
}

void QuadTree::calcBBox(vector<Point_2> &point_set)
{
    bbox = CGAL::bounding_box(point_set.begin(), point_set.end());
    center_ = Point_2((bbox.xmin() + bbox.xmax()) / 2,
                      (bbox.ymin() + bbox.ymax()) / 2);

    Vector_2 v(bbox.max() - center_);
    radius_ = CGAL::sqrt(v.squared_length());
}


void QuadTree::subdivide(vector<Point_2> &point_set)
{
    vector<vector<Point_2>> ch_point_sets(4, vector<Point_2>());
    for (auto& pt : point_set)
    {
        Direction_2 dir(pt - center_);
        if ((dir >= pos_x_dir_) && (dir < pos_y_dir_))
        {
            VLOG(8) << "1st quadrant";
            ch_point_sets[0].push_back(pt);
        }
        else if ((dir >= pos_y_dir_) && (dir < neg_x_dir_))
        {
            VLOG(8) << "2st quadrant";
            ch_point_sets[1].push_back(pt);
        }
        else if ((dir >= neg_x_dir_) && (dir < neg_y_dir_))
        {
            VLOG(8) << "3st quadrant";
            ch_point_sets[2].push_back(pt); } else
        {
            VLOG(8) << "4st quadrant";
            ch_point_sets[3].push_back(pt);
        }
    }
    for (int i = 0; i < 4; i++)
    {
        //QuadTree qt = QuadTree(ch_point_sets[i]);
        //ch[i] = &qt;
        ch[i] = new QuadTree(ch_point_sets[i]);
    }
}

double QuadTree::get_radius()
{
    return radius_;
}

Point_2 QuadTree::get_center()
{
    return center_;
}



