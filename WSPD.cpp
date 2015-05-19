#include <utility>
#include <string.h>
#include <CGAL/number_utils.h>
#include <CGAL/squared_distance_2.h>

#include "easylogging++.h"
#include "WSPD.h"

#define SWAP(x,y) do \
{ \
    unsigned char swap_temp[sizeof(x) == sizeof(y) ? (signed)sizeof(x) : -1]; \
    memcpy(swap_temp,&y,sizeof(x)); \
    memcpy(&y,&x,       sizeof(x)); \
    memcpy(&x,swap_temp,sizeof(x)); \
} while(0)

typedef Kernel::Segment_2 Segment_2;


double dist_rectangles(BBox b1, BBox b2)
{
    /*double min_dist = -1;
    for (int i = 0; i < 4; i++)
    {
        Segment_2 seg1(b1.vertex(i), b1.vertex(i+1));
        for (int j = 0; j < 4; j++)
        {
            Segment_2 seg2(b2.vertex(i), b2.vertex(i+1));
            double curr_dist = CGAL::sqrt(CGAL::squared_distance(seg1, seg2));
            if ((min_dist == -1) || (min_dist > curr_dist))
            {
                min_dist = curr_dist;
            }
        }
    }
    return min_dist;*/
    double x_coord_dist = CGAL::max(CGAL::max(b1.xmin() - b2.xmax(),
                                       b2.xmin() - b1.xmax()),
                             0.0);
    double y_coord_dist = CGAL::max(CGAL::max(b1.ymin() - b2.ymax(),
                                       b2.ymin() - b1.ymax()),
                             0.0);
    return CGAL::sqrt(x_coord_dist * x_coord_dist + y_coord_dist * y_coord_dist);
}

WSPD::WSPD(QuadTree tree, double s)
{
    VLOG(7) << "Constructing WSPD";
    this->s = s;
    pairs = pairing(tree, tree);
    collect_distances();
}

vector<pair<QuadTree, QuadTree>> WSPD::pairing(QuadTree t1, QuadTree t2)
{
    vector<pair<QuadTree, QuadTree>> pairs;

    Vector_2 v = t1.get_center() - t2.get_center();
    double dist = dist_rectangles(t1.bbox, t2.bbox);
    VLOG(8) << t1.to_string() << endl << t2.to_string();
    VLOG(8) << "rect distances: " << dist << endl;

    bool swapped = false;
    if (t1.get_radius() < t2.get_radius())
    {
        swapped = true;
        SWAP(t1, t2);
    }
    // t1 has larger radius by now
    // Well-separated condition: diameter * s <= dist between bbox
    if (2 * s * t1.get_radius() <= dist)
    {
        pairs.push_back(make_pair(t1, t2));
    }
    else
    {
        // pairing the children of t1 with t2
        for (auto& qt : t1.ch)
        {
            if ((*qt).node_type == QuadTree::EMPTY)
            {
                VLOG(9) << "EMPTY";
                continue;
            }
            vector<pair<QuadTree, QuadTree>> p = swapped ?
                    pairing(t2, *qt) : pairing(*qt, t2);

            pairs.reserve(pairs.size() + p.size());
            pairs.insert(pairs.end(), p.begin(), p.end());
            p.clear();
        }
    }
    return pairs;
}

void WSPD::collect_distances()
{
    for (auto& qt_pair : pairs)
    {
        distances_.push_back(qt_pair.first.quadtree_dist(qt_pair.second));
    }
}

vector<double> WSPD::distances()
{
    return distances_;
}

