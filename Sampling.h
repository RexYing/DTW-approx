
#include "QuadTreeTwoClasses.h"

using namespace std;

typedef vector<Point_2> Curve;
/*
 * Sampling from the dynamic programming matrix for dtw, edit distance
 */
class Sampling
{
	public:
		Sampling(const Curve &curve1, const Curve &curve2, double lb, double ub, double eps);

	private:
		Curve curve1_;
		Curve curve2_;
		double lb_;
		double ub_;
		double eps_;

    QuadTreeTwoClasses* qt_;

    void build_quadtree();
};
