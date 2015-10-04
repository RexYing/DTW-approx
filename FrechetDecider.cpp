#include "FrechetDecider.h"

FrechetDecider::FrechetDecider(const Curve &curve1, const Curve &curve2):
		curve1_(curve1),
		curve2_(curve2)
{ }

bool FrechetDecider::dfs(double sq_dist, Curve::const_iterator it1, Curve::const_iterator it2)
{
	// base case
	if (it1 == curve1_.end() && it2 == curve2_.end())
	{
			return true;
	}
	if (CGAL::squared_distance(*it1, *it2) <= sq_dist)
	{
			// go diagonally first
			if (it1 != curve1_.end() && it2 != curve2_.end())
			{
					if (dfs(sq_dist, it1 + 1, it2 + 1))
							return true;
			}

			// move iterator on one curve
			if (it1 != curve1_.end())
			{
					if (dfs(sq_dist, it1 + 1, it2))
							return true;
			}
			if (it2 != curve2_.end())
			{
					if (dfs(sq_dist, it1, it2 + 1))
							return true;
			}
	}
	return false;
}

bool FrechetDecider::is_at_least_frechet(double dist)
{
  return dfs(dist * dist, curve1_.begin(), curve2_.begin());
}

double FrechetDecider::bin_search_frechet(const vector<double> &dists)
{
    int low = 0;
    int high = dists.size();
    if (dists.empty())
    {
        LOG(ERROR) << "List of candidate distances is empty";
        return 0;
    }
    int mid;
    while (low < high)
    {
        mid = (low + high) / 2;
        if (is_at_least_frechet(dists[mid]))
        {
            high = mid;
        }
        else
        {
            low = mid + 1;
        }
    }
    LOG(INFO) << "Frechet decider binary search result: low idx " << low << " high idx " << high;
    if (high == dists.size())
    {
        LOG(WARNING) <<
            "The largest distance candidate is still less than Frechet distance, returning -1";
        return -1;
    }
    return dists[high];
}

pair<int, int> FrechetDecider::size()
{
	return make_pair(curve1_.size(), curve2_.size());
}
