#include <utility>

#include "QuadTree.h"

class WSPD
{
	public:
		WSPD(QuadTree tree, double s);

		vector<pair<QuadTree, QuadTree>> pairing(QuadTree t1, QuadTree t2);
		vector<pair<QuadTree, QuadTree>> pairs;

    // separation constant
    double s;

    vector<double> distances();

  private:
    vector<double> distances_;
    void collect_distances();

};

