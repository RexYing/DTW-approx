#include <iostream>
#include <fstream>
#include <CGAL/Simple_cartesian.h>
using namespace std;

typedef CGAL::Simple_cartesian<double> Kernel;
typedef Kernel::Point_2 Point_2;
typedef Kernel::Segment_2 Segment_2;

void deallocDouble2D(double** arr, int l)
{
   for (int i = 0; i < l; i++)
   {
       delete[] arr[i];
   }
   delete[] arr;
}


int main(int argc, char* argv[])
{
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
    double** alpha;
    double** beta;

    inFile >> d >> m;
    alpha = new double*[m];
    for (int i = 0; i < m; i++)
    {
        alpha[i] = new double[d];
        for (int j = 0; j < d; j++)
            inFile >> alpha[i][j];
    }

    inFile >> n;
    beta = new double*[n];
    for (int i = 0; i < n; i++)
    {
        beta[i] = new double[d];
        for (int j = 0; j < d; j++)
            inFile >> beta[i][j];
    }

    deallocDouble2D(alpha, m);
    deallocDouble2D(beta, n);
    return 0;
}
