#include <vector>

#include "easylogging++.h"
#include "QuadTree.h"

using namespace std;

/*
 * Quad tree in which data points have 2 distinct classes.
 */
class QuadTreeTwoClasses: public QuadTree
{
  public:
    QuadTreeTwoClasses(const vector<Point_2> &point_set1, const vector<Point_2> &point_set2);
    QuadTreeTwoClasses(const vector<Point_2> &point_set1,
                       const vector<int> &indices1,
                       const vector<Point_2> &point_set2,
                       const vector<int> &indices2);

    void init();

    int get_size(int index);
    Point_2 get_representative(int index);

    vector<int> indices1()
    {
        return indices_;
    }

    vector<int> indices2()
    {
        return indices2_;
    }

    Point_2 point2(int idx)
    {
        return point_set2_[idx];
    }

    string to_string()
    {
        stringstream sstm;
        CGAL::set_pretty_mode(sstm);
        sstm << "QuadTree: {center=" << center() << ", radius=" << radius() << ", curve1=[";
        for (auto& pt : point_set1_)
        {
            sstm << pt << " ";
        }
        sstm << "], curve2=[";
        for (auto& pt : point_set2_)
        {
            sstm << pt << " ";
        }
        sstm << "]}";

        return sstm.str();
    }

  protected:
    void subdivide();

  private:
    vector<Point_2> point_set2_;
    vector<int> indices2_;

    int set_sizes_[2];
    // representatives in point_set1 and point_set2
    Point_2 representatives_[2];

    void choose_representatives();

};

