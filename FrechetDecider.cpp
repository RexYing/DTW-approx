
#include "easylogging++.h"
#include "FrechetDecider.h"

FrechetDecider::FrechetDecider(const Curve &curve1, const Curve &curve2)
{
    this->curve1 = curve1;
    this->curve2 = curve2;
}

bool FrechetDecider::dfs(double sq_dist, Curve::iterator it1, Curve::iterator it2)
{
    if (CGAL::squared_distance(*it1, *it2) <= sq_dist)
    {
        // base case
        if (it1 == curve1.end() && it2 == curve2.end())
        {
            return true;
        }

        // go diagonally first
        if (it1 != curve1.end() && it2 != curve2.end())
        {
            if (dfs(sq_dist, it1 + 1, it2 + 1))
                return true;
        }

        // move iterator on one curve
        if (it1 != curve1.end())
        {
            if (dfs(sq_dist, it1 + 1, it2))
                return true;
        }
        if (it2 != curve2.end())
        {
            if (dfs(sq_dist, it1, it2 + 1))
                return true;
        }
    }
    return false;
}

bool FrechetDecider::is_at_least_frechet(double dist)
{
    return dfs(dist * dist, curve1.begin(), curve2.begin());
}

