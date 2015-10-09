#include <boost/regex.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <random>

#include "curve_generator.h"

Points CurveGenerator::line(
		std::pair<double, double> start, double angle, double step, int n)
{
	Points pts;
	pts.reserve(n);
	double x = start.first;
	double y = start.second;
	for (int i = 0; i < n; i++)
	{
		pts.push_back(std::make_pair(x + i * step * cos(angle), y + i * step * sin(angle)));
	}
	return pts;
}

Points CurveGenerator::line(string spec)
{
	vector<string> flags;
  split_regex(flags, spec, regex(kDelimiter));
	 
	int n;
	std::pair<double, double> start;
	double angle;
	double step;
	for (string flag : flags)
	{
		std::pair<string, string> flag_pair = parse_flag(flag);
		switch (flag.first)
		{
			case kstart:
				start = parse_point(flag.second);
				break;
			case kAngle:
				angle = std::stod(flag.second);
				break;
			case kStep:
				step = std::stod(flag.second);
				break;
			case kN:
				n = std::stoi(flag.second);
				break;
			default:
				LOG(ERROR) << "Unrecognized flag " << flag.first;
		}
	}
	return line(start, angle, step, n);
}

Points CurveGenerator::rand(
		std::pair<double, double> start, 
		std::pair<double, double> angle_range,
		std::pair<double, double> step_range,
		int n)
{
	Points pts;
	pts.reserve(n);
	double x = start.first;
	double y = start.second;
	
	std::default_random_engine generator;
  std::uniform_real_distribution<double> angle_distribution(angle_range.first, angle_range.second);
	std::uniform_real_distribution<double> step_distribution(step_range.first, step_range.second);
	
	pts.push_back(start);
	std::pair<double, double> prev = start;
	for (int i = 0; i < n; i++)
	{
		double angle = angle_distribution(generator);
		double step = step_distribution(generator);
		std::pair<double, double> curr = 
				std::make_pair(prev.first + step * cos(angle), prev.second + step * sin(angle));
		pts.push_back(curr);
		prev = curr;
	}
	return pts;
}

Points CurveGenerator::rand(string spec)
{
	vector<string> flags;
  split_regex(flags, spec, regex(kDelimiter));
	
	int n;
	std::pair<double, double> start;
	std::pair<double, double> angle_range;
	std::pair<double, double> step_range;
	for (string flag : flags)
	{
		std::pair<string, string> flag_pair = parse_flag(flag);
		switch (flag.first)
		{
			case kstart:
				start = parse_point(flag.second);
				break;
			case kAngle:
				angle_range = parse_point(flag.second);
				break;
			case kStep:
				step_range = parse_point(flag.second);
				break;
			case kN:
				n = std::stoi(flag.second);
				break;
			default:
				LOG(ERROR) << "Unrecognized flag " << flag.first;
		}
	}
	return rand(start, angle_range, step_range, n);
}

std::pair<double, double> CurveGenerator::parse_point(string pt_str)
{
	vector<string> splitted;
	split_regex(splitted, pt_str, regex(",[\\s]*"));
	if (splitted.size() != 2)
	{
		LOG(ERROR) << "CurveGenerator: point format error";
		return std::make_pair(0.0, 0.0);
	}
	else
	{
		int pos = splitted[0].find("(");
		double x = std::stod(splitted[0].substr(pos + 1));
		pos = splitted[1].find(")");
		double y = std::stod(splitted[1].substr(0, pos));
		return std::make_pair(x, y);
	}
}
	
std::pair<string, string> CurveGenerator::parse_flag(string flag_str)
{
	vector<string> key_and_val;
	split_regex(splitted, pt_str, regex("="));
	if (key_and_val.size() != 2)
	{
		LOG(ERROR) << "CurveGenerator: flag must be of the form key=val";
	}
	else
	{
		return std::make_pair(key_and_val[0], key_and_val[1]);
	}
}
	