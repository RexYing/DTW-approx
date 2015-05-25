
#include "easylogging++.h"
#include "FrechetDecider.h"

FrechetDecider::FrechetDecider(vector<Point_2> curve1, vector<Point_2> curve2)
{
    this->curve1 = curve1;
    this->curve2 = curve2;
}

bool is_at_least_frechet(double dist)
{
    return true;
}

