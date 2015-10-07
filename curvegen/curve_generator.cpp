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

Points CurveGenerator::rand(
		std::pair<double, double> start, 
		pair<double, double> angle_range,
		pair<double, double> step_range,
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