#include "curve.h"

class NaiveDTW
{
public:
	NaiveDTW(const Curve &curve1, const Curve &curve2);
	
	double compute_DTW();
	
private:
	Curve curve1_;
	Curve curve2_;
};
