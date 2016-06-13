#ifndef ICP_H
#define ICP_H

#include <boost/program_options.hpp>
#include <chrono>
#include <iostream>
#include <fstream>
#include <vector>
#include <CGAL/Simple_cartesian.h>
#include <Eigen/Geometry>

#include "curve.h"
#include "easylogging++.h"
#include "naive_dtw.h"
#include "rect_cluster.h"

class ICP 
{
public:
	ICP(const Curve &curve1, const Curve &curve2, string method);
	ICP(const Curve &curve1, const Curve &curve2, string method, double eps);
	
	Eigen::Affine2d FindBestRigidTransformation2D(Eigen::Matrix2Xd in, Eigen::Matrix2Xd out);
	
	Eigen::Affine2d RegisterCurves();
	
private:
	// initialize curveMat1_ and curveMat2_ according to curve correspondence
	void CorrespondenceToMatrix2D(vector<pair<int, int>> correspondence);
		
	const string DTW_DP_METHOD = "DTW-DP";
	const string DTW_APPROX_METHOD = "DTW-approx";
	const string FRECHET_DP_METHOD = "Frechet-DP";
	// the default eps if using approximation algorithms
	const double DEFAULT_EPS = 0.5;
	
	Curve curve1_;
	Curve curve2_;
	string method_;
	double eps_;

	// input curves in matrix form
	Eigen::Matrix2Xd curveMat1_;
	Eigen::Matrix2Xd curveMat2_;
};

#endif