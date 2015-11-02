#ifndef CURVE_H
#define CURVE_H

#include <CGAL/Simple_cartesian.h>
#include <utility>
#include <vector>

typedef CGAL::Simple_cartesian<double> Kernel;
typedef Kernel::Point_2 Point_2;

/* 
 * A segment of consecutive indices from the first int to last int, 
 * inclusive for the first index and exclusive for the second index.
 */
typedef std::pair<int, int> IndexSegment;

typedef std::vector<Point_2> Curve;

#endif