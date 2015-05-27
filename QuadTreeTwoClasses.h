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

    void init();

    int get_size(int index);
    Point_2 get_representative(int index);

  protected:
    void subdivide();

  private:
    vector<Point_2> point_set2_;

    int set_sizes_[2];
    // representatives in point_set1 and point_set2
    Point_2 representatives_[2];

    void choose_representatives();

};

