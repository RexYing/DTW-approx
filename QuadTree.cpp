#include <QuadTree.h>
using namespace std;

QuadTree::QuadTree(vector<Point_2> &pointSet)
{
    cout << "creating quad tree" << endl;
    if (pointSet.size() == 0)
        cerr << "QuadTree: no point exists in a node" << endl;
    else
    {
        p = pointSet[0];
    }
}

