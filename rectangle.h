#ifndef RECTANGLE
#define RECTANGLE

#include <boost/functional/hash.hpp>
#include <vector>
#include <utility>
#include <unordered_set>

#include "util/joiner.h"
#include "QuadTree.h"

using namespace std;

class Rectangle {
public:
	Rectangle(IndexSegment segment1, IndexSegment segment2):
			segment1(segment1), segment2(segment2), mark_(0)
	{
		height_ = segment1.second - segment1.first;
		width_ = segment2.second - segment2.first;
		
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
		
		// add bottom left corner to bottom_
		bottom_.push_back(make_pair(segment1.first, segment2.first));
		// add top left corner to top_
		top_.push_back(make_pair(segment1.second - 1, segment2.first));
		
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
			top_ = bottom_;
		}
		else
		{
			LOG(ERROR) << "Rectangle: upper bound (exclusive) not greater than lower bound";
		}
		
		if (segment2.second > segment2.first + 1)
		{
			// add bottom right corner element to bottom_
			bottom_.push_back(make_pair(segment1.first, segment2.second - 1));
			// add top right corner elements to top_
			top_.push_back(make_pair(segment1.second - 1, segment2.second - 1));
		}
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
	
	vector<pair<int, int>> top() {
		return top_;
	}
	
	vector<pair<int, int>> right() {
		return right_;
	}
	
	vector<pair<int, int>> bottom() {
		return bottom_;
	}
	
	vector<pair<int, int>> left() {
		return left_;
	}
	
	int width()
	{
		return width_;
	}
	
	int height()
	{
		return height_;
	}
	
	// upper-right corner of the rectangle
	pair<int, int> upper_right() {
		return top_.back();
	}
	
	/*
	 * A sample point in rectangle (no guarantee of the point's relative position in the rectangle.
	 */
	pair<int, int> sample()
	{
		return make_pair(segment1.first, segment2.first);
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
	int width_;
	int height_;
	unordered_set<Rectangle*> predecessors_;
	unordered_set<Rectangle*> successors_;
	vector<pair<int, int>> boundary_pts_;
	
	/* 
	 * boundaries on 4 sides of the rectangle
	 * All 4 vectors are disjoint except at corners, which are contained in 2 of the vectors
	 * All are ordered in ascending order of row/column index
	 */
	vector<pair<int, int>> top_;
	vector<pair<int, int>> right_;
	vector<pair<int, int>> bottom_;
	vector<pair<int, int>> left_;
	
	int mark_;
};

#endif
