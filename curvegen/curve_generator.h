#include <math.h>
#include <vector>

#include "easylogging++.h"

typedef std::vector<std::pair<double, double>> Points;

class CurveGenerator
{
public:
	Points line(std::pair<double, double> start, double angle, double step, int n);
	
	Points rand(
			std::pair<double, double> start, 
			pair<double, double> angle_range,
			pair<double, double> step_range,
			int n);
};