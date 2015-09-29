#include <CGAL/bounding_box.h>
#include <CGAL/number_utils.h>

#include "easylogging++.h"
#include "QuadTree.h"
using namespace std;

int QuadTree::max_id_ = -1;

vector<int> QuadTree::default_indices(int n)
{
    vector<int> inds;
    for (int i = 0; i < n; i++)
    {
        inds.push_back(i);
    }
    return inds;
}

QuadTree::QuadTree(const vector<Point_2> &point_set,
                   const vector<int> & indices)
{
    max_id_++;
    id_ = max_id_;

    pos_x_dir_ = Direction_2(1, 0);
    pos_y_dir_ = Direction_2(0, 1);
    neg_x_dir_ = Direction_2(-1, 0);
    neg_y_dir_ = Direction_2(0, -1);

   point_set1_ = point_set;

    indices_ = indices;
}

QuadTree::QuadTree(const vector<Point_2> &point_set):
    QuadTree(point_set, default_indices(point_set.size()))
{
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
}

double QuadTree::quadtree_dist(QuadTree that)
{
    return CGAL::sqrt(CGAL::squared_distance(this->center(), that.center()));
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
 * Params:
 * indices: the indices of points in the point set of current node.
 * indices: the indices of points in each quadrant. Will be modified.
 * It should be passed in as an empty vector of vectors.
 */
vector<vector<Point_2>> QuadTree::partition(vector<Point_2> point_set,
                                            vector<int> indices,
                                            vector<vector<int>> &ch_indices)
{
    for (int i = 0; i < 4; i++)
    {
        ch_indices.push_back(vector<int>());
    }
    vector<vector<Point_2>> ch_point_sets(4, vector<Point_2>());
    for (int i = 0; i < point_set.size(); i++)
    {
        Point_2 pt = point_set[i];
        Direction_2 dir(pt - center_);
        if ((dir >= pos_x_dir_) && (dir < pos_y_dir_))
        {
            VLOG(8) << "1st quadrant";
            ch_point_sets[0].push_back(pt);
            ch_indices[0].push_back(indices[i]);
        }
        else if ((dir >= pos_y_dir_) && (dir < neg_x_dir_))
        {
            VLOG(8) << "2nd quadrant";
            ch_point_sets[1].push_back(pt);
            ch_indices[1].push_back(indices[i]);
        }
        else if ((dir >= neg_x_dir_) && (dir < neg_y_dir_))
        {
            VLOG(8) << "3rd quadrant";
            ch_point_sets[2].push_back(pt);
            ch_indices[2].push_back(indices[i]);
        } else
        {
            VLOG(8) << "4th quadrant";
            ch_point_sets[3].push_back(pt);
            ch_indices[3].push_back(indices[i]);
        }
    }
    return ch_point_sets;
}


/*
 * Currently point_set1_ will not be used after calling this function
 */
void QuadTree::subdivide() {
    vector<vector<int>> ch_indices;
    vector<vector<Point_2>> ch_point_sets = partition(point_set1_, indices_, ch_indices);

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
        ch_[i] = new QuadTree(ch_point_sets[i], ch_indices[i]);
        ch_[i]->init();
    }
}

int QuadTree::id()
{
    return id_;
}

QuadTree* QuadTree::child(int index)
{
    return ch_[index];
}

/*
 * Return an array of quad-trees (children of current node).
 */
QuadTree** QuadTree::children()
{
    return ch_;
}

double QuadTree::radius()
{
    return radius_;
}

Point_2 QuadTree::center()
{
    return center_;
}

vector<int> QuadTree::indices()
{
    return indices_;
}

int QuadTree::size()
{
	return point_set1_.size();
}
