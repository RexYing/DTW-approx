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
    QuadTreeTwoClasses(vector<Point_2> &point_set1, vector<Point_2> &point_set2);

    void init();

  private:
    vector<Point_2> point_set2_;
};

