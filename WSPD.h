#include <boost/functional/hash.hpp>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#ifndef QUADTREE_H
#define QUADTREE_H

#include "QuadTree.h"
#include "QuadTreeTwoClasses.h"

/*
 * Hash an unordered pair of quad tree pointers
 */
struct unordered_pair_hash 
{
  inline std::size_t operator()(const std::pair<QuadTree*, QuadTree*> & p) const {
		std::hash<QuadTree*> qt_hasher;
    return qt_hasher(p.first) ^ qt_hasher(p.second);
  }
};

typedef pair<QuadTree*, QuadTree*> TreeNodePair;

typedef unordered_set<TreeNodePair, boost::hash<TreeNodePair> > NodePairs;

class WSPD
{
	public:
		WSPD(double s);
		WSPD(QuadTree* tree, double s);
		WSPD(QuadTree* tree1, QuadTree* tree2, double s);
		WSPD(QuadTree* tree1, QuadTree* tree2, double s, double lb);

		NodePairs pairing2(QuadTreeTwoClasses* t1, QuadTreeTwoClasses* t2);
		//NodePairs traverse(QuadTreeTwoClasses* tree, double ub);

    vector<double> distances();
		
    // separation constant
    double s;
		
		// the begin iterator of the node pairs after WSPD
		NodePairs::const_iterator begin()
		{
			return pairs.begin();
		}
		
		// the end iterator of the node pairs after WSPD
		NodePairs::const_iterator end()
		{
			return pairs.end();
		}
		
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
		NodePairs pairs;

		NodePairs pairing(QuadTree* t1, QuadTree* t2);
    vector<double> distances_;
    void collect_distances();

};

#endif
