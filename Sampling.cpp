
#include "Sampling.h"
#include "easylogging++.h"
using namespace std;

Sampling::Sampling(const Curve &curve1, const Curve &curve2, double lb, double ub, double eps):
    lb_(lb),
    ub_(ub),
    eps_(eps)
{
    curve1_ = curve1;
    curve2_ = curve2;
    //qt_ = new QuadTreeTwoClasses(curve1, curve2);
    //QuadTreeTwoClasses q(curve1, curve2);
}

void Sampling::build_quadtree()
{
    vector<Point_2> curve1;
    vector<Point_2> curve2;
    QuadTreeTwoClasses q(curve1, curve2);
}
