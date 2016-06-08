#include <algorithm>
#include <limits>
#include <math.h>

#include <CGAL/number_utils.h>

#include "frechet.h"

using namespace std;

Frechet::Frechet(const Curve &curve1, const Curve &curve2):
		curve1_(curve1), curve2_(curve2)
{ }

double Frechet::naive()
{
	// rolling array (not used since correspondence is needed)
	// vector<double> prev(curve2_.size(), numeric_limits<double>::max());
	// vector<double> curr;
	
	// init DP table
	vector<vector<double>> dp(curve1_.size(), vector<double>(curve2_.size()));
	
	// init path_dir
	path_dir_ = new vector<vector<double>*>();
	for (int i = 0; i < curve1_.size(); i++) {
		vector<double>* tmp = new vector<double>();
		for (int j = 0; j < curve2_.size(); j++) {
			tmp->push_back(0);
		}
		path_dir_->push_back(tmp);
	}
	
	// first row
	dp[0][0] = CGAL::sqrt(CGAL::squared_distance(curve1_[0], curve2_[0]));
	for (int j = 1; j < curve2_.size(); j++)
	{
		dp[0][j] = max(dp[0][j - 1], + CGAL::sqrt(CGAL::squared_distance(curve1_[0], curve2_[j])));
		(*(*path_dir_)[0])[j] = 1; // left
	}

	for (int i = 1; i < curve1_.size(); i++)
	{
		// first column
		dp[i][0] = max(dp[i - 1][0], CGAL::sqrt(CGAL::squared_distance(curve1_[i - 1], curve2_[0])));
		(*(*path_dir_)[i])[0] = 2; // down
		
		for (int j = 1; j < curve2_.size(); j++)
		{
			double dist = CGAL::sqrt(CGAL::squared_distance(curve1_[i], curve2_[j]));
			double prev_left = max(dp[i][j - 1], dist);
			double prev_diag = max(dp[i - 1][j - 1], dist);
			double prev_down = max(dp[i - 1][j], dist);
		
			dp[i][j] = std::min(prev_left, std::min(prev_diag, prev_down));
			
			// alignment info
			if (prev_left < prev_diag)
			{
				if (prev_left < prev_down)
				{
					(*(*path_dir_)[i])[j] = 1; // left
				}
				else
				{
					(*(*path_dir_)[i])[j] = 2; // down
				}
			}
			else
			{
				if (prev_diag > prev_down)
				{
					(*(*path_dir_)[i])[j] = 2; // down
				}
				else
				{
					(*(*path_dir_)[i])[j] = 3; // diag
				}
			}
		}
		
	}
	return dp[curve1_.size() - 1][curve2_.size() - 1];
}

vector<pair<int, int>> Frechet::correspondence()
{
	vector<pair<int, int>> path;
	pair<int, int> curr = make_pair(curve1_.size() - 1, curve2_.size() - 1);
	path.push_back(curr);
	while (curr.first != 0 || curr.second != 0)
	{
		int dir = (*(*path_dir_)[curr.first])[curr.second];
		switch (dir) {
			case 1: // left
				curr = make_pair(curr.first, curr.second - 1);
				path.push_back(curr);
				break;
			case 2: // down
				curr = make_pair(curr.first - 1, curr.second);
				path.push_back(curr);
				break;
			case 3: // diag
				curr = make_pair(curr.first - 1, curr.second - 1);
				path.push_back(curr);
				break;
			default:
				cerr << "Error in tracing correspondence at (" << curr.first << ", " << curr.second << ").";
		}
		
		//cout << curr.first << " " << curr.second << endl;
	}
	return path;
}
