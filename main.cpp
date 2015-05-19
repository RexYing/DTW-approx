#include <iostream>
#include <fstream>
#include <vector>
#include <CGAL/Simple_cartesian.h>

#include "QuadTree.h"
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

    Point_2 p(1,1), q(10,10);
    std::cout << "p = " << p << std::endl;
    std::cout << "q = " << q.x() << " " << q.y() << std::endl;
    std::cout << "sqdist(p,q) = "
      << CGAL::squared_distance(p,q) << std::endl;

    Segment_2 s(p,q);
    Point_2 mm(5, 9);

    std::cout << "m = " << mm << std::endl;
    std::cout << "sqdist(Segment_2(p,q), m) = "
      << CGAL::squared_distance(s,mm) << std::endl;
    std::cout << "p, q, and m ";
    switch (CGAL::orientation(p,q,mm)){
      case CGAL::COLLINEAR:
        std::cout << "are collinear\n";
        break;
      case CGAL::LEFT_TURN:
        std::cout << "make a left turn\n";
        break;
      case CGAL::RIGHT_TURN:
        std::cout << "make a right turn\n";
        break;
    }
    std::cout << " midpoint(p,q) = " << CGAL::midpoint(p,q) << std::endl;

    ifstream inFile;
    inFile.open(argv[1]);

    int d; // dimension
    int m; // num points on first curve
    int n; // num points on second curve

    inFile >> d;
    Curve alpha = readCurve(inFile, d);
    Curve beta = readCurve(inFile, d);

    QuadTree qt(alpha);

    return 0;
}
