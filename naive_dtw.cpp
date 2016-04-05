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
	}
	
	for (int i = 1; i < curve1_.size(); i++)
	{
		// first column
		curr.push_back(prev[0] + CGAL::sqrt(CGAL::squared_distance(curve1_[i - 1], curve2_[0])));
		
		for (int j = 1; j < curve2_.size(); j++)
		{
			double prev_left = 
					curr[j - 1] + CGAL::sqrt(CGAL::squared_distance(curve1_[i], curve2_[j - 1]));
			double prev_diag =
					prev[j - 1] + CGAL::sqrt(CGAL::squared_distance(curve1_[i - 1], curve2_[j - 1]));
			double prev_down =
					prev[j] + CGAL::sqrt(CGAL::squared_distance(curve1_[i - 1], curve2_[j]));
					
			curr.push_back(std::min(prev_left, std::min(prev_diag, prev_down)));
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
