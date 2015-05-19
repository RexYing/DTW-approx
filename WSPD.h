#include "QuadTree.h"

class WSPD
{
	public:
		WSPD(QuadTree tree);

		vector<Pair<QuadTree, QuadTree>> pairing(QuadTree t1, QuadTree t2);

  private:
    vector<Kernel> distances;

}

