#include "FrechetDecider.h"

FrechetDecider::FrechetDecider(const Curve &curve1, const Curve &curve2):
		curve1_(curve1),
		curve2_(curve2)
{ }

/* inefficient: use bottom up instead */
bool FrechetDecider::dfs(double sq_dist, int index1, int index2)
{
	// base case
	if (index1 == curve1_.size() - 1 && index2 == curve2_.size() - 1)
	{
			return true;
	}
	pair<int, int> index = make_pair(index1, index2);
	
	// if found, the index is NOT reachable
	if (index_set_.find(index) != index_set_.end())
	{
		return false;
	}
	
	if (CGAL::squared_distance(curve1_[index1], curve2_[index2]) <= sq_dist)
	{
		// go diagonally first
		if (index1 < curve1_.size() - 1 && index2 < curve2_.size() - 1)
		{
			if (dfs(sq_dist, index1 + 1, index2 + 1))
			{
				index_set_.insert(index);
				return true;
			}
		}

		// move index of one curve
		if (index1 < curve1_.size() - 1)
		{
			if (dfs(sq_dist, index1 + 1, index2))
			{
				index_set_.insert(index);
				return true;
			}
		}
		if (index2 < curve2_.size() - 1)
		{
			if (dfs(sq_dist, index1, index2 + 1))
			{
				index_set_.insert(index);
				return true;
			}
		}
	}
	index_set_.insert(index);
	return false;
}

/*
bool bfs(double sq_dist)
{
	queue<pair<int, int> > q;
	int k = 0;
	if (CGAL::squared_distance(curve1_[0], curve2_[0]) > sq_dist)
	{
		return false;
	}
	q.push(make_pair(0, 0));
	while (!q.empty())
	{
		pair<int, int> index = q.pop();
		// repeated index
		if (q.front() == index)
		{
			q.pop();
		}
		if (index.first < curve1_.size() - 1)
		{
			if (index.second < curve2_.size() - 1)
			{
				if (CGAL::squared_distance(curve1_[index.first + 1], curve2_[0])
*/

bool FrechetDecider::is_at_least_frechet(double dist)
{
	if (dist > prev_dist_)
	{
		index_set_.clear();
	}
	prev_dist_ = dist;
  return dfs(dist * dist, 0, 0);
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
