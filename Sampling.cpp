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
    len_cell_ = 1 << (int)(round(ceil(log2(2 * ub_))));
    VLOG(6) << "Grid side length:   " << len_cell_;
    /*qt_ = new QuadTreeTwoClasses(curve1, curve2);
    qt_->init();*/
}

/*
 * Insert points on one curve into grid
 */
void Sampling::insert_grid()
{
    for (int k = 0; k < curve1_.size(); k++)
    {
        Point_2 pt = curve1_[k];
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

            vector<int>* indices1 = new vector<int>();
            vector<int>* indices2 = new vector<int>();
            indices1->push_back(k);
            grid_indices_.emplace(ind, make_pair(indices1, indices2));
        }
        else
        {
            Curve* pt_set_ptr = it->second.first;
            pt_set_ptr->push_back(pt);

            vector<int>* indices1 = grid_indices_[ind].first;
            indices1->push_back(k);
        }
    }

    for (int k = 0; k < curve2_.size(); k++)
    {
        Point_2 pt = curve2_[k];
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

            vector<int>* indices1 = new vector<int>();
            vector<int>* indices2 = new vector<int>();
            indices2->push_back(k);
            grid_indices_.emplace(ind, make_pair(indices1, indices2));
        }
        else
        {
            Curve* pt_set_ptr = it->second.second;
            pt_set_ptr->push_back(pt);

            vector<int>* indices2 = grid_indices_[ind].second;
            indices2->push_back(k);
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
    print_grid(grid_);
    VLOG(6) << "finish grid";
}

bool output = false;
void Sampling::add_samples_WSPD(QuadTreeTwoClasses* grid_qt1, QuadTreeTwoClasses* grid_qt2)
{
    WSPD wspd(grid_qt1, grid_qt2, 1.0/eps_, lb_);
    NodePairs pairs = wspd.pairs;

    // sample from WSPD pairs
    // sample J_i
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


        vector<int> indices1 = qt1->indices1();
        vector<int> indices2 = qt2->indices2();

        vector<int> sample_row_idx;
        for (auto& idx : indices1)
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
                int idx = indices2[i - 1];
                sample_col_idx.push_back(idx);
                // expand
                if (idx < curve2_.size() - 1)
                    sample_col_idx.push_back(idx + 1);
                if (idx > 0)
                    sample_col_idx.push_back(idx - 1);
            }
        }

        // reverse direction

        int end_idx = indices2.size() - 1;
        sample_col_idx.push_back(indices2[end_idx]);
        prev = qt2->point2(end_idx);
        for (int i = end_idx - 1; i >= 0; i--)
        {
            if (CGAL::squared_distance(prev, qt2->point2(indices2[i])) > qt2->radius())
            {
                prev = qt2->point2(indices2[i + 1]);
                int idx = indices2[i + 1];
                sample_col_idx.push_back(idx);
                // expand
                if (idx < curve2_.size() - 1)
                    sample_col_idx.push_back(idx + 1);
                if (idx > 0)
                    sample_col_idx.push_back(idx - 1);
            }
        }



        for (auto& i : sample_row_idx)
            for (auto& j : sample_col_idx)
            {
                unordered_map<long, vector<pair<long, long>>* >::const_iterator it =
                    diagonal_samples_.find(j - i);
                if (it == diagonal_samples_.end())
                {
                    vector<pair<long, long>>* pairs = new vector<pair<long, long>>();
                    pairs->push_back(make_pair(i, j));
                    diagonal_samples_.emplace(j - i, pairs);
                }
                else
                {
                    it->second->push_back(make_pair(i, j));
                }
            }

        /*
        if (output) {
        for (auto& idx : sample_row_idx)
            cout << idx << " ";
        cout << endl;
        for (auto& idx : sample_col_idx)
            cout << idx << " ";
        cout << endl;
        }*/
    }

    // sample I_j
    for (auto& p : pairs)
    {
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

        vector<int> indices1 = qt1->indices1();
        vector<int> indices2 = qt2->indices2();

        vector<int> sample_col_idx;
        for (auto& idx : indices2)
        {
            sample_col_idx.push_back(idx);
        }

        if (indices1.size() == 0)
        {
            // the first quad tree node should contain at least a point from the first curve
            LOG(FATAL) << "indices1 is empty";
        }

        vector<int> sample_row_idx;
        sample_row_idx.push_back(indices1[0]);
        Point_2 prev = qt1->point1(indices1[0]);
        for (int i = 1; i < indices1.size(); i++)
        {
            if (CGAL::squared_distance(prev, qt1->point1(indices1[i])) > qt1->radius())
            {
                prev = qt1->point1(indices1[i - 1]);
                int idx = indices1[i - 1];
                sample_row_idx.push_back(idx);
                // expand
                if (idx < curve1_.size() - 1)
                    sample_row_idx.push_back(idx + 1);
                if (idx > 0)
                    sample_row_idx.push_back(idx - 1);
            }
        }

        // reverse direction
        int end_idx = indices1.size() - 1;
        sample_row_idx.push_back(indices1[end_idx]);
        prev = qt1->point1(end_idx);
        for (int i = end_idx - 1; i >= 0; i--)
        {
            if (CGAL::squared_distance(prev, qt1->point1(indices1[i])) > qt1->radius())
            {
                prev = qt1->point1(indices1[i + 1]);
                int idx = indices1[i + 1];
                sample_row_idx.push_back(idx);
                // expand
                if (idx < curve1_.size() - 1)
                    sample_row_idx.push_back(idx + 1);
                if (idx > 0)
                    sample_row_idx.push_back(idx - 1);
            }
        }


        for (auto& i : sample_row_idx)
            for (auto& j : sample_col_idx)
            {
                unordered_map<long, vector<pair<long, long>>* >::const_iterator it =
                    diagonal_samples_.find(j - i);
                if (it == diagonal_samples_.end())
                {
                    vector<pair<long, long>>* pairs = new vector<pair<long, long>>();
                    pairs->push_back(make_pair(i, j));
                    diagonal_samples_.emplace(j - i, pairs);
                }
                else
                {
                    it->second->push_back(make_pair(i, j));
                }
            }

        if (output) {
        for (auto& idx : sample_row_idx)
            cout << idx << " ";
        cout << endl;
        for (auto& idx : sample_col_idx)
            cout << idx << " ";
        cout << endl;
        }
    }
}


void Sampling::sample()
{
    qt_ = new QuadTreeTwoClasses(curve1_, curve2_);
    qt_->init();

    for (auto& grid_elem : grid_)
    {
        GridIndex idx = grid_elem.first;
        pair<vector<int>*, vector<int>*> indices_pair = grid_indices_[idx];

        QuadTreeTwoClasses* qt = new QuadTreeTwoClasses(*(grid_elem.second.first),
                                                        *(indices_pair.first),
                                                        *(grid_elem.second.second),
                                                        *(indices_pair.second));
        qt->init();
        quadtrees_.emplace(grid_elem.first, qt);
    }


    for (auto qt_map_elem : quadtrees_)
    {
        VLOG(6) << "Current grid cell: (" << qt_map_elem.first.first <<
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
                //if (idx.first == 8){ output = true;
                VLOG(6) << "Add sample for node (" << idx.first << ", " << idx.second << ")";
                //} else output = false;
                add_samples_WSPD(curr_qt, neighbor_it->second);
            }
        }
    }

}

string Sampling::view_samples()
{
    stringstream sstm;
    CGAL::set_pretty_mode(sstm);

    vector<vector<bool>> table(curve1_.size(), vector<bool>(curve2_.size(), false));

    for (auto& entry : diagonal_samples_)
    {
        for (auto& pair : *(entry.second))
        {
            table[pair.first][pair.second] = true;
        }
    }

    long count = 0;
    for (int i = 0; i < curve1_.size(); i++)
    {
        for (int j = 0; j < curve2_.size(); j++)
        {
            if (table[i][j])
            {
                sstm << '.';
                count++;
            }
            else
            {
                sstm << ' ';
            }
        }
        sstm << '\n';
    }

    stringstream sstm_count;
    sstm_count << "Total sample points: " << count << endl;
    VLOG(5) << "Total sample points: " << count << endl;
    return sstm_count.str() + sstm.str();
}

Sampling::~Sampling()
{
    for (auto& entry : grid_)
    {
        delete entry.second.first;
        delete entry.second.second;
    }
}

