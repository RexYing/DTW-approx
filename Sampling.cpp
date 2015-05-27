
#include "Sampling.h"
#include "easylogging++.h"
using namespace std;

Sampling::Sampling(const Curve &curve1, const Curve &curve2, double lb, double ub, double eps):
    curve1_(curve1),
    curve2_(curve2),
    lb_(lb),
    ub_(ub),
    eps_(eps)
{
    qt_ = new QuadTreeTwoClasses(curve1, curve2);
    qt_->init();
}

