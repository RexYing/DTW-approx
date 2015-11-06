#include <math.h>
#include <vector>

#include "easylogging++.h"

using namespace std;

typedef vector<pair<double, double>> Points;

class CurveGenerator
{
public:
	Points line(pair<double, double> start, double angle, double step, int n);
	Points line(string spec);
	
	Points rand_monotonic(
			pair<double, double> start, 
			pair<double, double> angle_range,
			pair<double, double> step_range,
			int n);
	Points rand_monotonic(string spec);
	
	Points rand(
			pair<double, double> start, 
			pair<double, double> angle_normal,
			double step_poisson_lambda,
			int n);
	Points rand(string spec);
	
private:
	const string kDelimiter = "[\\s]+";
	const string kStart = "start";
	const string kAngle = "angle";
	const string kStep = "step";
	const string kN = "n";
	
	pair<double, double> parse_point(string pt_str);
	pair<string, string> parse_flag(string flag_str);
};