#include <algorithm>
#include <limits>
#include <math.h>

#include <CGAL/number_utils.h>

#include "naive_dtw.h"

using namespace std;

NaiveDTW::NaiveDTW(const Curve &curve1, const Curve &curve2):
		curve1_(curve1), curve2_(curve2)
{ }

double NaiveDTW::compute_DTW()
{
	// DTW values on the previous row
	vector<double> prev(curve2_.size(), numeric_limits<double>::max());
	vector<double> curr;
	
	// first row
	prev[0] = 0.0;
	for (int j = 1; j < curve2_.size(); j++)
	{
		prev[j] = prev[j - 1] + CGAL::sqrt(CGAL::squared_distance(curve1_[0], curve2_[j - 1]));
		alignment_.emplace(make_pair(0, j), make_pair(0, j - 1)); // left
	}
	
	for (int i = 1; i < curve1_.size(); i++)
	{
		// first column
		curr.push_back(prev[0] + CGAL::sqrt(CGAL::squared_distance(curve1_[i - 1], curve2_[0])));
		alignment_.emplace(make_pair(i, 0), make_pair(i - 1, 0)); // down
		
		for (int j = 1; j < curve2_.size(); j++)
		{
			double prev_left = 
					curr[j - 1] + CGAL::sqrt(CGAL::squared_distance(curve1_[i], curve2_[j - 1]));
			double prev_diag =
					prev[j - 1] + CGAL::sqrt(CGAL::squared_distance(curve1_[i - 1], curve2_[j - 1]));
			double prev_down =
					prev[j] + CGAL::sqrt(CGAL::squared_distance(curve1_[i - 1], curve2_[j]));
					
			curr.push_back(std::min(prev_left, std::min(prev_diag, prev_down)));
			
			// alignment info
			if (prev_left < prev_diag)
			{
				if (prev_left < prev_down)
				{
					alignment_.emplace(make_pair(i, j), make_pair(i, j - 1)); // left
				}
				else
				{
					alignment_.emplace(make_pair(i, j), make_pair(i - 1, j)); // down
				}
			}
			else
			{
				if (prev_diag > prev_down)
				{
					alignment_.emplace(make_pair(i, j), make_pair(i - 1, j)); // down
				}
				else
				{
					alignment_.emplace(make_pair(i, j), make_pair(i - 1, j - 1)); // diag
				}
			}
		}
		
		// update
		//prev = std::move(curr);
		for (int i = 0; i < curr.size(); i++) 
		{
			prev[i] = curr[i];
		}
		curr.clear();
	}
	return prev.back();
}

vector<pair<int, int>> NaiveDTW::trace_alignment()
{
	vector<pair<int, int>> path;
	pair<int, int> curr = make_pair(curve1_.size() - 1, curve2_.size() - 1);
	path.push_back(curr);
	while (curr.first != 0 || curr.second != 0)
	{
		curr = alignment_[curr];
		path.push_back(curr);
		//cout << curr.first << " " << curr.second << endl;
	}
	return path;
}

