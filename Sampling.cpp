#include <math.h>
#include <CGAL/bounding_box.h>
#include <CGAL/number_utils.h>

#include "Sampling.h"
#include "easylogging++.h"
using namespace std;

Sampling::Sampling(const Curve &curve1, const Curve &curve2, double lb, double ub, double eps):
    curve1_(curve1),
    curve2_(curve2),
    lb_(lb),
    ub_(ub),
    eps_(eps)
{
    len_cell_ = round(ceil(log2(2 * ub_)));
    /*qt_ = new QuadTreeTwoClasses(curve1, curve2);
    qt_->init();*/
}

/*
 * Insert points on one curve into grid
 */
void Sampling::insert_grid()
{
    for (auto& pt : curve1_)
    {
        long i = pt.x() / len_cell_;
        long j = pt.y() / len_cell_;
        GridIndex ind = make_pair(i, j);
        Grid::const_iterator it = grid_.find(ind);
        if (it == grid_.end()) { Curve* c1 = new vector<Point_2>();
            Curve* c2 = new vector<Point_2>();
            c1->push_back(pt);
            SetPair set_pair = make_pair(c1, c2);
            grid_.emplace(ind, set_pair);
            VLOG(1) << "new";
        }
        else
        {
            Curve* pt_set_ptr = it->second.first;
            pt_set_ptr->push_back(pt);
        }
    }

    for (auto& pt : curve2_)
    {
        long i = pt.x() / len_cell_;
        long j = pt.y() / len_cell_;
        GridIndex ind = make_pair(i, j);
        Grid::const_iterator it = grid_.find(ind);
        if (it == grid_.end())
        {
            Curve* c1;
            Curve* c2;
            c2->push_back(pt);
            SetPair set_pair = make_pair(c1, c2);
            grid_.emplace(ind, set_pair);
        }
        else
        {
            Curve* pt_set_ptr = it->second.second;
            pt_set_ptr->push_back(pt);
        }
    }
}

void Sampling::print_grid(Grid grid)
{
    for (auto& x: grid)
    {
        VLOG(5) << "(" << x.first.first << ", " << x.first.second << ") -> " <<
            x.second.first->size() << " points curve1 " <<
            x.second.second->size() << " points curve2 " << std::endl;
    }
}

/*
 * Construct grid for WSPD
 */
void Sampling::init()
{
    vector<Point_2> all_points(curve1_);
    all_points.reserve(curve1_.size() + curve2_.size());
    all_points.insert(all_points.end(), curve2_.begin(), curve2_.end());
    BBox bbox = CGAL::bounding_box(all_points.begin(), all_points.end());

    VLOG(7) << "insert grid";
    insert_grid();
    VLOG(7) << "print grid";
    print_grid(grid_);
    VLOG(7) << "finish grid";
}

void Sampling::sample()
{
    qt_ = new QuadTreeTwoClasses(curve1_, curve2_);
    qt_->init();
    WSPD wspd(*qt_, 1.0/eps_, lb_);
    vector<pair<QuadTree, QuadTree>> pairs = wspd.pairs;

    // sample from WSPD pairs
    for (auto& p : pairs)
    {
        VLOG(1) << p.first.to_string();
        VLOG(1) << p.second.to_string();

        QuadTreeTwoClasses qt1 = dynamic_cast<QuadTreeTwoClasses&>(p.first);
        QuadTreeTwoClasses qt2 = dynamic_cast<QuadTreeTwoClasses&>(p.second);

        vector<int> indices2 = qt2.indices2();

        vector<int> sample_row_idx;
        for (auto& idx : qt1.indices1())
        {
            sample_row_idx.push_back(idx);
            VLOG(2) << idx;
        }

        vector<int> sample_col_idx;
        sample_col_idx.push_back(indices2[0]);
        Point_2 prev = qt2.point2(indices2[0]);
        for (int i = 1; i < indices2.size(); i++)
        {
            if (CGAL::squared_distance(prev, qt2.point2(indices2[i])) > qt2.radius())
            {
                prev = qt2.point2(indices2[i] - 1);
                sample_col_idx.push_back(indices2[i] - 1);
                VLOG(3) << indices2[i] - 1;
            }
        }
    }

}

Sampling::~Sampling()
{
    for (auto& entry : grid_)
    {
        delete entry.second.first;
        delete entry.second.second;
    }
}

