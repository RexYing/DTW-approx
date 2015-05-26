#include<vector>

#include "easylogging++.h"
#include "QuadTree.h"
using namespace std;

typedef vector<Point_2> Curve;
/*
 * Solver for discrete Frechet distance decision problem.
 *
 * Dependencies: QuadTree, WSPD
 */
class FrechetDecider
{
  public:
    FrechetDecider(const vector<Point_2> &curve1, const vector<Point_2> &curve2);

    // Test if Frechet distance is at least dist,
    // i.e. there exists a feasible path in free space diagram.
    bool is_at_least_frechet(double dist);

    /*
     * Perform binary search on the distances. Return the smallest number in the list
     * that is at least Frechet distance.
     *
     * Params:
     * dists: Vector of distance candidates, sorted in ascending order.
     */
    double bin_search_frechet(vector<double> &dists);

  private:
    vector<Point_2> curve1;
    vector<Point_2> curve2;

    bool dfs(double sq_dist, Curve::iterator it1, Curve::iterator it2);

};

