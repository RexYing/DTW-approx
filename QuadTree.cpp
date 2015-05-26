#include <CGAL/bounding_box.h>
#include <CGAL/number_utils.h>

#include "easylogging++.h"
#include "QuadTree.h"
using namespace std;

int QuadTree::max_id_ = -1;

QuadTree::QuadTree(vector<Point_2> &point_set)
{
    max_id_++;
    id_ = max_id_;

    pos_x_dir_ = Direction_2(1, 0);
    pos_y_dir_ = Direction_2(0, 1);
    neg_x_dir_ = Direction_2(-1, 0);
    neg_y_dir_ = Direction_2(0, -1);

    point_set1_ = point_set;
}

/*
 * Set properties for empty tree
 */
void QuadTree::set_empty()
{
    center_ = Point_2(0, 0);
    radius_ = -1; // empty: radius is invalid
    node_type = EMPTY;
}

void QuadTree::init()
{
    VLOG(7) << ("creating quad tree...");
    switch (point_set1_.size())
    {
        case 0:
            set_empty();
            break;
        case 1:
            node_type = LEAF;
            calc_bbox(point_set1_);
            p = point_set1_[0];
            break;
        default:
            node_type = NODE;
            calc_bbox(point_set1_);
            p = point_set1_[0];
            subdivide();
    }
    VLOG(8) << to_string();
}

double QuadTree::quadtree_dist(QuadTree that)
{
    return CGAL::sqrt(CGAL::squared_distance(this->get_center(), that.get_center()));
}

void QuadTree::calc_bbox(vector<Point_2> &point_set)
{
    bbox = CGAL::bounding_box(point_set.begin(), point_set.end());
    center_ = Point_2((bbox.xmin() + bbox.xmax()) / 2,
                      (bbox.ymin() + bbox.ymax()) / 2);

    Vector_2 v(bbox.max() - center_);
    radius_ = CGAL::sqrt(v.squared_length());
}


/*
 * Currently point_set1_ will not be used after calling this function
 */
void QuadTree::subdivide() {
    vector<vector<Point_2>> ch_point_sets(4, vector<Point_2>());
    for (auto& pt : point_set1_)
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
            ch_point_sets[2].push_back(pt);
        } else
        {
            VLOG(8) << "4st quadrant";
            ch_point_sets[3].push_back(pt);
        }
    }
    // continue to subdivide if more than 1 quadrant has points
    int num_ch = 0;
    for (int i = 0; i < 4; i++)
    {
        if (!ch_point_sets[i].empty())
        {
            num_ch++;
        }
    }
    if (num_ch <= 1)
    {
        return;
    }

    //subdivide
    for (int i = 0; i < 4; i++)
    {
        //QuadTree qt = QuadTree(ch_point_sets[i]);
        //ch[i] = &qt;
        ch[i] = new QuadTree(ch_point_sets[i]);
        ch[i]->init();
    }
}

int QuadTree::id()
{
    return id_;
}

double QuadTree::get_radius()
{
    return radius_;
}

Point_2 QuadTree::get_center()
{
    return center_;
}



