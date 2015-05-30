#include <iostream>
#include <fstream>
#include <vector>
#include <CGAL/Simple_cartesian.h>

#include "QuadTree.h"
#include "FrechetDecider.h"
#include "Sampling.h"
#include "easylogging++.h"
using namespace std;

INITIALIZE_EASYLOGGINGPP

typedef CGAL::Simple_cartesian<double> Kernel;
typedef Kernel::Point_2 Point_2;
typedef Kernel::Segment_2 Segment_2;

typedef vector<Point_2> Curve;

const double DEFAULT_EPS = 0.1;

void deallocDouble2D(double** arr, int l)
{
   for (int i = 0; i < l; i++)
   {
       delete[] arr[i];
   }
   delete[] arr;
}

Curve readCurve(ifstream &inFile, int dim)
{
    Curve curve;
    int m; // curve length
    inFile >> m;

    for (int i = 0; i < m; i++)
    {
        int x, y;
        if (dim == 2)
        {
            inFile >> x >> y;
            Point_2 p(x, y);
            curve.push_back(p);
        }
    }
    return curve;
}


int main(int argc, char* argv[])
{
    START_EASYLOGGINGPP(argc, argv);

    ifstream inFile;
    inFile.open(argv[1]);

    int d; // dimension

    inFile >> d;
    Curve alpha = readCurve(inFile, d);
    Curve beta = readCurve(inFile, d);

    int m = alpha.size();
    int n = beta.size();

    vector<Point_2> all_points(alpha);
    all_points.reserve(alpha.size() + beta.size());
    all_points.insert(all_points.end(), beta.begin(), beta.end());

    QuadTree qt(all_points);
    qt.init();
    double s = 1; // need n-approx only

    WSPD wspd(&qt, s);
    vector<pair<QuadTree*, QuadTree*>> pairs = wspd.pairs;
    vector<double> dists = wspd.distances();

    VLOG(6) << "WSPD dists:";
    for (int i = 0; i < dists.size(); i++)
    {
        VLOG(6) << dists[i];
    }

    FrechetDecider fd(alpha, beta);
    double approx_frechet = fd.bin_search_frechet(dists);
    LOG(INFO) << "Approximate Frechet distance: " << approx_frechet;

    double dtw_lb = approx_frechet / (s + 1);
    double dtw_ub = approx_frechet * (s + 1) * max(m, n);
    LOG(INFO) << "Dynamic Time Warping range: [" << dtw_lb << ", " << dtw_ub << "]";

    Sampling sampling(alpha, beta, dtw_lb, dtw_ub, DEFAULT_EPS);
    sampling.init();
    sampling.sample();

    return 0;
}
