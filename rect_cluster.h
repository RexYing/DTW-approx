#include <unordered_map>

#include "grid.h"
#include "util/joiner.h"

struct Rectangle {
	Rectangle(IndexSegment segment1, IndexSegment segment2):
			segment1(segment1), segment2(segment2)
	{ }
	
	IndexSegment segment1;
	IndexSegment segment2;
	
	vector<pair<int, int>> boundary_pts()
	{
		vector<pair<int, int>> coords;
		for (int i = segment1.first; i < segment1.second; i++)
		{
			coords.push_back(make_pair(i, segment2.first));
			if (segment2.second > segment2.first + 1)
			{
				coords.push_back(make_pair(i, segment2.second - 1));
			}
		}
		for (int i = segment2.first + 1; i < segment2.second - 1; i++)
		{
			coords.push_back(make_pair(segment1.first, i));
			if (segment1.second > segment1.first + 1)
			{
				coords.push_back(make_pair(segment1.second - 1, i));
			}
		}
		return coords;
	}
	
	string to_string()
	{
		stringstream sstm;
		Joiner::on(" ").join({segment1.first, segment1.second, segment2.first, segment2.second});
		return sstm.str();
	}
};

/*
 * Partition of Dynamic Programming integer coordinate points into family of disjoint rectangles
 */
class RectCluster
{
public:
	RectCluster(const Curve &curve1, const Curve &curve2, double lb, double ub, double eps);
	
	/*
	 * Partition DP integer coordinate point set into family of disjoint rectangles.
	 * Each rectangle and its boundary points are stored.
	 */
	void partition();
	
	// export rectangles and their boundary points
	string export_rects();
	// summarize the ractangle partition
	string summarize();

private:
	Curve curve1_;
	Curve curve2_;
	double lb_;
	double ub_;
	double eps_;
	// the side length of individual grid cells
	long len_cell_;

	Grid* grid_;
	vector<Rectangle> rects_;
	// maps boundary points to the rectangle they belong to
	unordered_map<pair<int, int>, Rectangle, boost::hash<pair<int, int>> > inv_rects_;
	
	void gen_rect(WSPD wspd);
};
