#include <boost/functional/hash.hpp>
#include <unordered_set>
#include <utility>

#ifndef QUADTREE_H
#define QUADTREE_H

#include "QuadTree.h"
#include "QuadTreeTwoClasses.h"

#endif


/*
 * Hash an unordered pair of quad tree pointers
 */
struct unordered_pair_hash {
  inline std::size_t operator()(const std::pair<QuadTree*, QuadTree*> & p) const {
		std::hash<QuadTree*> qt_hasher;
    return qt_hasher(p.first) ^ qt_hasher(p.second);
  }
};

typedef unordered_set<pair<QuadTree*, QuadTree*>, 
											boost::hash<std::pair<QuadTree*, QuadTree*>> > NodePairs;

class WSPD
{
	public:
		WSPD(QuadTree* tree, double s);
    WSPD(QuadTreeTwoClasses* tree, double s, double lb);
    WSPD(QuadTreeTwoClasses* tree1, QuadTreeTwoClasses* tree2, double s, double lb);

		NodePairs pairing(QuadTree* t1, QuadTree* t2);
		NodePairs pairing2(QuadTreeTwoClasses* t1, QuadTreeTwoClasses* t2);
		//NodePairs traverse(QuadTreeTwoClasses* tree, double ub);

    vector<double> distances();
		
    // separation constant
    double s;
		NodePairs pairs;
		
		virtual string to_string()
    {
        stringstream sstm;
        CGAL::set_pretty_mode(sstm);
				for (auto& pair : pairs) {
					sstm << "{" << pair.first->to_string() << ", " << pair.second->to_string() << "}\n" ;
				}
        return sstm.str();
    }

  private:
    double lb_;

    vector<double> distances_;
    void collect_distances();

};

