#include <utility>
#include <vector>

#include "curve.h"

using namespace std;


class Frechet
{
public:
	Frechet(const Curve &curve1, const Curve &curve2);
	
	/*
	 * Compute Frechet distance using naive DP algorithm.
	 */
	double naive();
	
	vector<pair<int, int>> correspondence();
	
	 ~Frechet()
	{
		for (auto it = path_dir_->begin(); it < path_dir_->end(); it++) {
			delete *it;
		}
		delete path_dir_;
	}
	
private:
	Curve curve1_;
	Curve curve2_;
	
	/*
	 * For each cell:
	 * 0: not on optimal path
	 * 1: the previous cell on the path is on the left
	 * 2: the previous cell on the path is below
	 * 3: the previous cell on the path is on the lower-left
	 */
	vector<vector<double>*>* path_dir_;
};
