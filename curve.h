#ifndef CURVE_H
#define CURVE_H

#include <CGAL/Simple_cartesian.h>
#include <iostream>
#include <fstream>
#include <utility>
#include <vector>

using namespace std;

typedef CGAL::Simple_cartesian<double> Kernel;
typedef Kernel::Point_2 Point_2;

/* 
 * A segment of consecutive indices from the first int to last int, 
 * inclusive for the first index and exclusive for the second index.
 */
typedef std::pair<int, int> IndexSegment;

typedef std::vector<Point_2> Curve;

inline Curve readCurve(ifstream &inFile, int dim)
{
    Curve curve;
    int m; // curve length
    inFile >> m;

    for (int i = 0; i < m; i++)
    {
        double x, y;
        if (dim == 2)
        {
            inFile >> x >> y;
            Point_2 p(x, y);
            curve.push_back(p);
        }
    }
    return curve;
}

#endif