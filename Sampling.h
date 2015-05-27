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

typedef unordered_map<GridIndex, SetPair, pairhash> Grid;
/*
 * Sampling from the dynamic programming matrix for dtw, edit distance
 */
class Sampling
{
	public:
		Sampling(const Curve &curve1, const Curve &curve2, double lb, double ub, double eps);
    void init();

    ~Sampling();

	private:
		Curve curve1_;
		Curve curve2_;
		double lb_;
		double ub_;
		double eps_;
    // the side length of individual grid cells
    long len_cell_;

    QuadTreeTwoClasses* qt_;

    unordered_map<GridIndex, SetPair, pairhash> grid_;
    unordered_map<GridIndex, QuadTreeTwoClasses*, pairhash> quadtrees_;

    void insert_grid();
    void print_grid(Grid grid);
};
