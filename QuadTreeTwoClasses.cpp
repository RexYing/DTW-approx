
#include "easylogging++.h"
#include "QuadTreeTwoClasses.h"

using namespace std;

QuadTreeTwoClasses::QuadTreeTwoClasses(vector<Point_2> point_set1,
                                       vector<Point_2> point_set2):
  QuadTree(point_set1), point_set2_(point_set2)
{ }

void QuadTreeTwoClasses::init()
{
  cout << point_set2_[0] << endl;
}
