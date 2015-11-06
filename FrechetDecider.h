#include <boost/functional/hash.hpp>
#include <queue>
#include <vector>
#include <unordered_set>

#include "easylogging++.h"
#include "curve.h"
using namespace std;

/*
 * Solver for discrete Frechet distance decision problem.
 *
 * Dependencies: curve.h, WSPD.h
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
    double bin_search_frechet(const vector<double> &dists);
		
		pair<int, int> size();

  private:
    const vector<Point_2> curve1_;
    const vector<Point_2> curve2_;
		unordered_set<pair<int, int>, boost::hash<pair<int, int>> > index_set_;
		/* The distance used by the previous run of the decision problem. */
		double prev_dist_ = 0;

    bool dfs(double sq_dist, int index1, int index2);
		bool dp(double sq_dist);

};

