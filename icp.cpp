#include "easylogging++.h"
#include "icp.h"

ICP::ICP(const Curve &curve1, const Curve &curve2, string method, double eps):
		curve1_(curve1),
    curve2_(curve2),
		method_(method),
		eps_(eps)
{ }

ICP::ICP(const Curve &curve1, const Curve &curve2, string method):
		ICP(curve1, curve2, method, DEFAULT_EPS)
{ }

void ICP::CorrespondenceToMatrix2D(const vector<pair<int, int>> &correspondence)
{
	curveMat1_(2, correspondence.size());
	curveMat2_(2, correspondence.size());
	for (int i = 0; i < correspondence.size(); i++)
	{
		int idx = correspondence[i].first;
		curveMat1_(0, i) = curve1_[idx].x();
		curveMat1_(1, i) = curve1_[idx].y();
		idx = correspondence[i].second;
		curveMat2_(0, i) = curve2_[idx].x();
		curveMat2_(1, i) = curve2_[idx].y();
	}
}

/* 
 * Given two sets of 3D points, find the rigid transformation (rotation + translation)
 * which best maps the first set to the second.
 */
Eigen::Affine2d ICP::FindBestRigidTransformation2D(Eigen::Matrix2Xd in, Eigen::Matrix2Xd out)
{
  vector<pair<int, int>> correspondence;
	// curve alignment
	if (method_.compare(DTW_APPROX_METHOD) != 0)
	{
		RectCluster rect(curve1_, curve2_, eps_);
		rect.sequential_partition();
		
		double approx_dtw = 0; 
		
		const auto approx_dtw_begin = chrono::high_resolution_clock::now(); // or use steady_clock 

		correspondence = rect.compute_approx_dtw(true, approx_dtw);
		auto approx_dtw_time = chrono::high_resolution_clock::now() - approx_dtw_begin;
		LOG(INFO) << "DTW: " << approx_dtw << " -- Time: " 
        << chrono::duration<double, std::milli>(approx_dtw_time).count() / 1000 
        << "seconds.\n";
	}
	
	// Kabsch algorithm
	CorrespondenceToMatrix2D(correspondence);
	
	// substraction of centroid
	Eigen::Vector2d ctr1 = Eigen::Vector2d::Zero();
  Eigen::Vector2d ctr2 = Eigen::Vector2d::Zero();
  for (int col = 0; col < in.cols(); col++) {
    ctr1 += curveMat1_.col(col);
    ctr2 += curveMat2_.col(col);
  }
  ctr1 /= curveMat1_.cols();
  ctr2 /= curveMat2_.cols();
  for (int col = 0; col < in.cols(); col++) {
    curveMat1_.col(col) -= ctr1;
    curveMat2_.col(col) -= ctr2;
  }
	
	// SVD
	Eigen::MatrixXd Cov = curveMat1_ * curveMat2_.transpose();
  Eigen::JacobiSVD<Eigen::MatrixXd> svd(Cov, Eigen::ComputeThinU | Eigen::ComputeThinV);

  // Find the rotation
  double d = (svd.matrixV() * svd.matrixU().transpose()).determinant();
  if (d > 0)
    d = 1.0;
  else
    d = -1.0;
  Eigen::Matrix2d I = Eigen::Matrix2d::Identity(2, 2);
  //I(2, 2) = d;
  Eigen::Matrix2d R = svd.matrixV() * I * svd.matrixU().transpose();

  // The final transform
  Eigen::Affine2d affine2;
  affine2.linear() = R;
  affine2.translation() = ctr2 - R * ctr1;
  return affine2;
}

Eigen::Affine2d ICP::RegisterCurves()
{
	
	
}

