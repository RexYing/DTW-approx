#include <utility>

#ifndef QUADTREE_H
#define QUADTREE_H

#include "QuadTree.h"
#include "QuadTreeTwoClasses.h"

#endif

typedef vector<pair<QuadTree*, QuadTree*>> NodePairs;

class WSPD
{
	public:
		WSPD(QuadTree* tree, double s);
    WSPD(QuadTreeTwoClasses* tree, double s, double lb);

		vector<pair<QuadTree*, QuadTree*>> pairing(QuadTree* t1, QuadTree* t2);
		vector<pair<QuadTree*, QuadTree*>> pairing2(QuadTreeTwoClasses* t1, QuadTreeTwoClasses* t2);
		vector<pair<QuadTree*, QuadTree*>> pairs;

		vector<pair<QuadTree*, QuadTree*>> traverse(QuadTreeTwoClasses* tree, double ub);

    // separation constant
    double s;

    vector<double> distances();

  private:
    double lb_;

    vector<double> distances_;
    void collect_distances();

};

