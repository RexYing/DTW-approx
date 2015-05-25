#include<vector>

#include "easylogging++.h"
#include "QuadTree.h"
using namespace std;

/*
 * Solver for discrete Frechet distance decision problem.
 *
 * Dependencies: QuadTree, WSPD
 */
class FrechetDecider
{
  public:
    FrechetDecider(vector<Point_2> curve1, vector<Point_2> curve2);

    // Test if Frechet distance is at least dist,
    // i.e. there exists a feasible path in free space diagram.
    bool is_at_least_frechet(double dist);

  private:
    vector<Point_2> curve1;
    vector<Point_2> curve2;
};

