#include <boost/functional/hash.hpp>
#include <vector>
#include <utility>
#include <unordered_set>

#include "util/joiner.h"

using namespace std;

class Rectangle {
public:
	Rectangle(IndexSegment segment1, IndexSegment segment2):
			segment1(segment1), segment2(segment2), mark_(0)
	{ 
		pair<int, int> p;
		for (int i = segment1.first; i < segment1.second; i++)
		{
			p = make_pair(i, segment2.first);
			boundary_pts_.push_back(p);
			left_.push_back(p);
		}
		if (segment2.second > segment2.first + 1)
		{
			for (int i = segment1.first; i < segment1.second; i++)
			{
				p = make_pair(i, segment2.second - 1);
				boundary_pts_.push_back(p);
				right_.push_back(p);
			}
		}
		else if (segment2.second == segment2.first + 1)
		{
			right_ = left_;
		}
		else
		{
			LOG(ERROR) << "Rectangle: upper bound (exclusive) not greater than lower bound";
		}
		// remove bottom right element that should be in bottom_
		right_.pop_front();
		// remove top left element that should be in top_
		left_.pop_back();
		
		for (int i = segment2.first + 1; i < segment2.second - 1; i++)
		{
			p = make_pair(segment1.first, i);
			boundary_pts_.push_back(p);
			bottom_.push_back(p);
		}
		if (segment1.second > segment1.first + 1)
		{
			for (int i = segment2.first + 1; i < segment2.second - 1; i++)
			{
				p = make_pair(segment1.second - 1, i);
				boundary_pts_.push_back(p);
				top_.push_back(p);
			}
		}
		else if (segment1.second == segment1.first + 1)
		{
			bottom_ = top_;
		}
		else
		{
			LOG(ERROR) << "Rectangle: upper bound (exclusive) not greater than lower bound";
		}
		// add bottom right element to bottom_
		bottom_.push_back(make_pair(segment1.first, segment2.second - 1));
		// add top left element to top_
		top_.push_front(make_pair(segment1.second - 1, segment2.first));
	}
	
	IndexSegment segment1;
	IndexSegment segment2;
	
	vector<pair<int, int>> boundary_pts()
	{
		return boundary_pts_;
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
		predecessors_.insert(rect);
	}
	
	void add_successor(Rectangle* rect)
	{
		successors_.insert(rect);
	}
	
	unordered_set<Rectangle*> successors()
	{
		return successors_;
	}
	
	int indegree()
	{
		return predecessors_.size();
	}
	
	list<pair<int, int>> top() {
		return top_;
	}
	
	list<pair<int, int>> right() {
		return right_;
	}
	
	list<pair<int, int>> bottom() {
		return bottom_;
	}
	
	list<pair<int, int>> left() {
		return left_;
	}
	
	bool is_temp_marked()
	{
		return mark_ == 1;
	}
	
	bool is_not_marked()
	{
		return mark_ == 0;
	}
	
	void mark_permanent()
	{
		mark_ = 2;
	}
	
	void mark_temp()
	{
		mark_ = 1;
	}
	
private:
	unordered_set<Rectangle*> predecessors_;
	unordered_set<Rectangle*> successors_;
	vector<pair<int, int>> boundary_pts_;
	
	/* 
	 * boundaries on 4 sides of the rectangle
	 * All 4 lists are disjoint:
	 *	bottom-left corner is in left_;
	 * 	top-left corner is in top_;
	 *	bottom-right corner is in bottom_;
	 *	top-right corner is in right_.
	 * All are ordered in ascending order of row/column index
	 */
	list<pair<int, int>> top_;
	list<pair<int, int>> right_;
	list<pair<int, int>> bottom_;
	list<pair<int, int>> left_;
	
	int mark_;
};