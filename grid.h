#ifndef GRID
#define GRID

#include <unordered_map>

#include "WSPD.h"
using namespace std;

struct pairhash {
  public:
    template <typename T, typename U>
    std::size_t operator()(const std::pair<T, U> &x) const
    {
        return std::hash<T>()(x.first) ^ std::hash<U>()(x.second);
    }
};

typedef vector<Point_2> Curve;

typedef pair<long, long> GridIndex;
// a pair of point sets in grid
typedef pair<Curve*, Curve*> SetPair;

typedef unordered_map<GridIndex, TreeNodePair, pairhash> GridMap;

/*
 * A grid with each cell containing a number of points from each curve.
 */
class Grid
{
public:
	Grid(const Curve &curve1, const Curve &curve2, double lb, double ub, long len_cell);
	
	// Build grid
	// shift the grid by the given vector
	void init(Vector_2 offset_vector);
	
	void print_grid();
	
	/*
	 * Gets neighboring nodes for a given grid_idx and curve (1 or 2).
	 * Returns the neighboring nodes that contain points from the other curve.
	 */
	vector<QuadTree*> neighbors(GridIndex grid_idx, short curve);
	
	GridMap::const_iterator begin();
	GridMap::const_iterator end();
	
private:
	Curve curve1_;
	Curve curve2_;
	double lb_;
	double ub_;
	// the side length of individual grid cells
	long len_cell_;
	
	GridMap grid_;
	
	// inserts points into grid at initialization
	void insert_grid(Vector_2 offset_vector);
};

#endif
