#include <math.h>
#include <vector>

#include "easylogging++.h"

typedef std::vector<std::pair<double, double>> Points;

class CurveGenerator
{
public:
	Points line(std::pair<double, double> start, double angle, double step, int n);
	Points line(string spec);
	
	Points rand(
			std::pair<double, double> start, 
			std::pair<double, double> angle_range,
			std::pair<double, double> step_range,
			int n);
	Points rand(string spec);
	
private:
	const string kDelimiter = "[\\s]+";
	const string kStart = "start";
	const string kAngle = "angle";
	const string kStep = "step";
	const string kN = "n";
	
	std::pair<double, double> parse_point(string pt_str);
	std::pair<string, string> parse_flag(string flag_str);
};