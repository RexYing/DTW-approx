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
        if (it == grid_.end()) {
            Curve* c1 = new vector<Point_2>();
            Curve* c2 = new vector<Point_2>();
            c1->push_back(pt);
            SetPair set_pair = make_pair(c1, c2);
            grid_.emplace(ind, set_pair);
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
            Curve* c1 = new vector<Point_2>;
            Curve* c2 = new vector<Point_2>;
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

    VLOG(6) << "insert grid";
    insert_grid();
    VLOG(6) << "print grid";
    print_grid(grid_);
    VLOG(6) << "finish grid";
}

void Sampling::add_samples_WSPD(QuadTreeTwoClasses* grid_qt1, QuadTreeTwoClasses* grid_qt2)
{
    WSPD wspd(grid_qt1, grid_qt2, 1.0/eps_, lb_);
    vector<pair<QuadTree*, QuadTree*>> pairs = wspd.pairs;

    // sample from WSPD pairs
    for (auto& p : pairs)
    {
        VLOG(7) << p.first->to_string();
        VLOG(7) << p.second->to_string();

        QuadTreeTwoClasses* qt1 = dynamic_cast<QuadTreeTwoClasses*>(p.first);
        if (qt1 == NULL)
        {
            LOG(WARNING) << "qt1 null pointer";
        }
        QuadTreeTwoClasses* qt2 = dynamic_cast<QuadTreeTwoClasses*>(p.second);
        if (qt2 == NULL)
        {
            LOG(WARNING) << "qt2 null pointer";
        }


        vector<int> indices2 = qt2->indices2();

        vector<int> sample_row_idx;
        for (auto& idx : qt1->indices1())
        {
            sample_row_idx.push_back(idx);
        }

        if (indices2.size() == 0)
        {
            // the second quad tree node should contain at least a point from the second curve
            LOG(FATAL) << "indices2 is empty";
        }

        vector<int> sample_col_idx;
        sample_col_idx.push_back(indices2[0]);
        Point_2 prev = qt2->point2(indices2[0]);
        for (int i = 1; i < indices2.size(); i++)
        {
            if (CGAL::squared_distance(prev, qt2->point2(indices2[i])) > qt2->radius())
            {
                prev = qt2->point2(indices2[i - 1]);
                sample_col_idx.push_back(indices2[i - 1]);
            }
        }

        for (auto& i : sample_row_idx)
            for (auto& j : sample_col_idx)
            {
                diagonal_samples_.emplace(j - i, make_pair(i, j));
            }
    }
}


void Sampling::sample()
{
    qt_ = new QuadTreeTwoClasses(curve1_, curve2_);
    qt_->init();

    for (auto& grid_elem : grid_)
    {
        QuadTreeTwoClasses* qt = new QuadTreeTwoClasses(*(grid_elem.second.first),
                                                        *(grid_elem.second.second));
        quadtrees_.emplace(grid_elem.first, qt);
    }

//    add_samples_WSPD(qt_, qt_);

    for (auto qt_map_elem : quadtrees_)
    {
        VLOG(7) << "Current grid cell: (" << qt_map_elem.first.first <<
            ", " << qt_map_elem.first.second << ")";
        GridIndex pos = qt_map_elem.first;

        QuadTreeTwoClasses* curr_qt = qt_map_elem.second;
        if (curr_qt->indices1().empty())
        {
            continue;
        }

        vector<GridIndex> neighbor_idx;
        neighbor_idx.push_back(make_pair(pos.first - 1, pos.second - 1));
        neighbor_idx.push_back(make_pair(pos.first - 1, pos.second));
        neighbor_idx.push_back(make_pair(pos.first - 1, pos.second + 1));
        neighbor_idx.push_back(make_pair(pos.first, pos.second - 1));
        neighbor_idx.push_back(make_pair(pos.first, pos.second));
        neighbor_idx.push_back(make_pair(pos.first, pos.second + 1));
        neighbor_idx.push_back(make_pair(pos.first + 1, pos.second - 1));
        neighbor_idx.push_back(make_pair(pos.first + 1, pos.second));
        neighbor_idx.push_back(make_pair(pos.first + 1, pos.second + 1));

        for (auto& idx : neighbor_idx)
        {
            QuadTreeGrid::const_iterator neighbor_it = quadtrees_.find(idx);
            if ((neighbor_it != quadtrees_.end()) && (!neighbor_it->second->indices2().empty()))
            {
                VLOG(7) << "Add sample for node (" << idx.first << ", " << idx.second << ")";
                add_samples_WSPD(curr_qt, neighbor_it->second);
            }
        }
    }

}

string Sampling::view_samples()
{
    int table[curve1_.size()][curve2_.size()];
    for (auto& entry : diagonal_samples_)
    {
        table[entry.second.first][entry.second.second] = true;
    }

    stringstream sstm;
    CGAL::set_pretty_mode(sstm);
    for (auto& row : table)
    {
        for (auto& col : row)
        {
            char c = col ? '.' : ' ';
            sstm << c;
        }
        sstm << '\n';
    }
    return sstm.str();
}

Sampling::~Sampling()
{
    for (auto& entry : grid_)
    {
        delete entry.second.first;
        delete entry.second.second;
    }
}

