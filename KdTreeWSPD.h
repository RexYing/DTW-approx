#include <CGAL/bounding_box.h>
#include <CGAL/number_utils.h>
#include <CGAL/Kd_tree.h>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Search_traits_2.h>
#include <CGAL/tags.h>

#include "easylogging++.h"
#include "WSPD.h"

using namespace std;

typedef CGAL::Simple_cartesian<double> Kernel;
typedef Kernel::Point_2 Point_2;
typedef CGAL::Search_traits_2<Kernel> TreeTraits;
// fair splitter
typedef CGAL::Fair<TreeTraits> Fair;
typedef CGAL::Kd_tree<TreeTraits, Fair, CGAL::Tag_true> Tree;
typedef CGAL::Kd_tree_node<TreeTraits, Fair, CGAL::Tag_true> KdTreeNode;

typedef Kernel::Direction_2 Direction_2;
typedef Kernel::Vector_2 Vector_2;
typedef Kernel::Iso_rectangle_2 BBox; //bounding box for 2D point sets

typedef unordered_set<pair<KdTreeNode*, KdTreeNode*>, 
											boost::hash<std::pair<KdTreeNode*, KdTreeNode*>> > KdTreeNodePairs;

struct BBoxInfo 
{
	BBoxInfo() {}
	
	BBoxInfo(vector<Point_2> &point_set)
	{
    bbox = CGAL::bounding_box(point_set.begin(), point_set.end());
    center = Point_2((bbox.xmin() + bbox.xmax()) / 2,
                     (bbox.ymin() + bbox.ymax()) / 2);

    Vector_2 v(bbox.max() - center);
    radius = CGAL::sqrt(v.squared_length());
	}
	
	BBox bbox;
	Point_2 center;
	double radius;
	
	double dist(BBoxInfo that)
	{
		Vector_2 v(center - that.center);
		return CGAL::sqrt(v.squared_length());
	}
};

/*
 * Implementation of WSPD using K-d Tree as the underlying structure.
 */
class KdTreeWSPD: public WSPD
{
public:
	KdTreeWSPD(const vector<Point_2> &point_set, double s);
	
	KdTreeNodePairs pairs;
	
private:
	Tree* tree;
	
	KdTreeNodePairs pairing(KdTreeNode* t1, KdTreeNode* t2);
	
};
