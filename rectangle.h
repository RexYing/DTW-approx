#include <vector>
#include <utility>

#include "util/joiner.h"

using namespace std;

class Rectangle {
public:
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
		sstm << Joiner::on(" ")->join(
				{segment1.first, segment1.second, segment2.first, segment2.second});
		return sstm.str();
	}
	
	void add_predecessor(Rectangle* rect)
	{
		predecessor_.push_back(rect);
	}
	
private:
	vector<Rectangle*> predecessor_;
	
};