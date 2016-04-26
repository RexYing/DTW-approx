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

bool FrechetDecider::dp(double sq_dist)
{
	bool* curr = new bool[curve1_.size()];
	bool* prev = new bool[curve2_.size()];
	
	// first row
	prev[0] = true;
	// the (0, 0) entry is set to 1 to mark it as feasible
	alignment_[0].push_back(1);
	for (int i = 1; i < curve2_.size(); i++)
	{
		if (prev[i - 1] && CGAL::squared_distance(curve1_[0], curve2_[i]) <= sq_dist)
		{
			prev[i] = true;
			alignment_[0].push_back(1);
		}
		else
		{
			prev[i] = false;
			alignment_[0].push_back(0);
		}
	}
	
	for (int i = 1; i < curve1_.size(); i++)
	{
		// first column
		if (prev[0] && CGAL::squared_distance(curve1_[i], curve2_[0]) <= sq_dist)
		{
			curr[0] = true;
			alignment_[i].push_back(2);
		}
		else
		{
			curr[0] = false;
			alignment_[i].push_back(0);
		}
		
		for (int j = 1; j < curve2_.size(); j++)
		{
			curr[j] = false;
			if (CGAL::squared_distance(curve1_[i], curve2_[j]) <= sq_dist)
			{
				if (prev[j] || (j > 0 && prev[j - 1]) || (j > 0 && curr[j - 1]))
				{
					curr[j] = true;
				}
				
				if (j > 0 && prev[j - 1])
				{
					alignment_[i].push_back(3); // lower-left
				}
				else if (j > 0 && curr[j - 1])
				{
					alignment_[i].push_back(1); // left
				}
				else if (prev[j])
				{
					alignment_[i].push_back(2); // down
				}
				else
				{
					alignment_[i].push_back(0);
				}
			}
			else
			{
				alignment_[i].push_back(0);
			}
		}
		
		delete[] prev;
		prev = curr;
		curr = new bool[curve2_.size()];
	}
	
	bool feasible = prev[curve2_.size() - 1];
	delete[] prev;
	delete[] curr;
	return feasible;
}

void FrechetDecider::init_alignment()
{
	alignment_.clear();
	for (int i = 0; i < curve1_.size(); i++)
	{
		vector<int> row;
		alignment_.push_back(row);
	}
}

vector<pair<int, int>> FrechetDecider::trace_alignment()
{
	vector<pair<int, int>> path;
	pair<int, int> curr = make_pair(curve1_.size() - 1, curve2_.size() - 1);
	path.push_back(curr);
	while (curr.first != 0 || curr.second != 0)
	{
		int dir = alignment_[curr.first][curr.second];
		switch (dir)
		{
			case 1:
				curr = make_pair(curr.first, curr.second - 1);
				break;
			case 2:
				curr = make_pair(curr.first - 1, curr.second);
				break;
			case 3:
				curr = make_pair(curr.first - 1, curr.second - 1);
				break;
			case 0:
				//LOG(ERROR) << "Frechet path not feasible at (" << curr.first << ", " << curr.second << ")";
				break;
			default:
				LOG(ERROR) << "Unrecognized direction when tracing approximate Frechet alignment: " << dir;
		}
		path.push_back(curr);
		//cout << curr.first << " " << curr.second << endl;
	}
	return path;
}

bool FrechetDecider::is_at_least_frechet(double dist)
{
	/*
	if (dist > prev_dist_)
	{
		index_set_.clear();
	}
	prev_dist_ = dist;
  return dfs(dist * dist, 0, 0);
	*/
	init_alignment();
	return dp(dist * dist);
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
		is_at_least_frechet(dists[high]); // to make sure the alignment is correct
    return dists[high];
}

pair<int, int> FrechetDecider::size()
{
	return make_pair(curve1_.size(), curve2_.size());
}
