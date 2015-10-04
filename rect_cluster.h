#include <unordered_map>

#include "grid.h"
using namespace std;

/*
 * Partition of Dynamic Programming integer coordinate points into family of disjoint rectangles
 */
class RectCluster
{
public:
	RectCluster(const Curve &curve1, const Curve &curve2, double lb, double ub, double eps);
	
	/*
	 * Partition DP integer coordinate point set into family of disjoint rectangles.
	 * Each rectangle and its boundary points are stored.
	 */
	void partition();

private:
	Curve curve1_;
	Curve curve2_;
	double lb_;
	double ub_;
	double eps_;
	// the side length of individual grid cells
	long len_cell_;

	Grid* grid_;
};
