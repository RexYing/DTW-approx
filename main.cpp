#include <iostream>
#include <fstream>
#include <vector>
#include <CGAL/Simple_cartesian.h>

#include "QuadTree.h"
#include "WSPD.h"
#include "easylogging++.h"
using namespace std;

INITIALIZE_EASYLOGGINGPP

typedef CGAL::Simple_cartesian<double> Kernel;
typedef Kernel::Point_2 Point_2;
typedef Kernel::Segment_2 Segment_2;

typedef vector<Point_2> Curve;

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
    int m; // num points on first curve
    int n; // num points on second curve

    inFile >> d;
    Curve alpha = readCurve(inFile, d);
    Curve beta = readCurve(inFile, d);

    QuadTree qt(alpha);
    double s = 1; // need n-approx only
    WSPD wspd(qt, s);
    vector<pair<QuadTree, QuadTree>> pairs = wspd.pairs;

    return 0;
}
