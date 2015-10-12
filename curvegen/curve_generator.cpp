#include <boost/regex.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <chrono>
#include <random>
#include <stdlib.h>

#include "curve_generator.h"

Points CurveGenerator::line(
		pair<double, double> start, double angle, double step, int n)
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
  split_regex(flags, spec, boost::regex(kDelimiter));
	 
	int n;
	pair<double, double> start;
	double angle;
	double step;
	for (string flag : flags)
	{
		pair<string, string> flag_pair = parse_flag(flag);
		if (flag_pair.first == kStart)
		{
			start = parse_point(flag_pair.second);
		}
		else if (flag_pair.first == kAngle)
		{
			angle = strtod(flag_pair.second.c_str(), NULL);
		}
		else if (flag_pair.first == kStep)
		{
			step = strtod(flag_pair.second.c_str(), NULL);
		}
		else if (flag_pair.first == kN)
		{
			n = (int)(strtol(flag_pair.second.c_str(), NULL, 10));
		}
		else
		{
			LOG(ERROR) << "Unrecognized flag " << flag_pair.first;
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
	
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  default_random_engine generator (seed);
  uniform_real_distribution<double> angle_distribution(angle_range.first, angle_range.second);
	uniform_real_distribution<double> step_distribution(step_range.first, step_range.second);
	
	pts.push_back(start);
	pair<double, double> prev = start;
	for (int i = 0; i < n - 1; i++)
	{
		double angle = angle_distribution(generator);
		double step = step_distribution(generator);
		pair<double, double> curr = 
				make_pair(prev.first + step * cos(angle), prev.second + step * sin(angle));
		pts.push_back(curr);
		prev = curr;
	}
	return pts;
}

Points CurveGenerator::rand(string spec)
{
	vector<string> flags;
  split_regex(flags, spec, boost::regex(kDelimiter));
	
	int n;
	pair<double, double> start;
	pair<double, double> angle_range;
	pair<double, double> step_range;
	for (string flag : flags)
	{
		pair<string, string> flag_pair = parse_flag(flag);
		if (flag_pair.first == kStart)
		{
			start = parse_point(flag_pair.second);
		}
		else if (flag_pair.first == kAngle)
		{
			angle_range = parse_point(flag_pair.second);
		}
		else if (flag_pair.first == kStep)
		{
			step_range = parse_point(flag_pair.second);
		}
		else if (flag_pair.first == kN)
		{
			n = (int)(strtol(flag_pair.second.c_str(), NULL, 10));
		}
		else
		{
			LOG(ERROR) << "Unrecognized flag " << flag_pair.first;
		}
	}
	return rand(start, angle_range, step_range, n);
}

pair<double, double> CurveGenerator::parse_point(string pt_str)
{
	vector<string> splitted;
	split_regex(splitted, pt_str, boost::regex(",[\\s]*"));
	if (splitted.size() != 2)
	{
		LOG(ERROR) << "CurveGenerator: point format error";
		return make_pair(0.0, 0.0);
	}
	else
	{
		int pos = splitted[0].find("(");
		double x = strtod(splitted[0].substr(pos + 1).c_str(), NULL);
		pos = splitted[1].find(")");
		double y = strtod(splitted[1].substr(0, pos).c_str(), NULL);
		return make_pair(x, y);
	}
}
	
std::pair<string, string> CurveGenerator::parse_flag(string flag_str)
{
	vector<string> key_and_val;
	split_regex(key_and_val, flag_str, boost::regex("="));
	if (key_and_val.size() != 2)
	{
		LOG(ERROR) << "CurveGenerator: flag must be of the form key=val";
	}
	else
	{
		return make_pair(key_and_val[0], key_and_val[1]);
	}
}
	