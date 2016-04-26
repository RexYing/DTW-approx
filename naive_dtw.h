#include <boost/functional/hash.hpp>
#include <unordered_map>
#include <utility>
#include <vector>

#include "curve.h"

using namespace std;

typedef unordered_map<pair<int, int>, pair<int, int>, boost::hash<pair<int, int>> > CellToPrecedent;

class NaiveDTW
{
public:
	NaiveDTW(const Curve &curve1, const Curve &curve2);
	
	double compute_DTW();
	
	vector<pair<int, int>> trace_alignment();
	
private:
	Curve curve1_;
	Curve curve2_;
	CellToPrecedent alignment_;
};
