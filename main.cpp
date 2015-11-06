#include <boost/program_options.hpp>
#include <chrono>
#include <iostream>
#include <fstream>
#include <vector>
#include <CGAL/Simple_cartesian.h>


#include "easylogging++.h"
#include "FrechetDecider.h"
#include "naive_dtw.h"
#include "QuadTree.h"
#include "rect_cluster.h"

namespace po = boost::program_options;
using namespace std;

INITIALIZE_EASYLOGGINGPP

typedef CGAL::Simple_cartesian<double> Kernel;
typedef Kernel::Point_2 Point_2;
typedef Kernel::Segment_2 Segment_2;

typedef vector<Point_2> Curve;

const double DEFAULT_EPS = 0.3;

void deallocDouble2D(double** arr, int l)
{
   for (int i = 0; i < l; i++)
   {
       delete[] arr[i];
   }
   delete[] arr;
}

Curve readCurve(ifstream &inFile, int dim)
{
    Curve curve;
    int m; // curve length
    inFile >> m;

    for (int i = 0; i < m; i++)
    {
        double x, y;
        if (dim == 2)
        {
            inFile >> x >> y;
            Point_2 p(x, y);
            curve.push_back(p);
        }
    }
    return curve;
}


int main(int argc, char* argv[])
{
	START_EASYLOGGINGPP(argc, argv);
		
	string curve1_filename;
	string curve2_filename;
	string rects_filename = "";
	double eps;

	try {
		po::options_description desc("Allowed options");
		desc.add_options()
			("help", "produce help message")
			("rects_output_file", po::value<string>(&rects_filename), "set output file for rectangles")
			("curve1", po::value<string>(&curve1_filename), "set input file for curve1")
			("curve2", po::value<string>(&curve2_filename), "set input file for curve2")
			("eps", po::value<double>(&eps)->default_value(DEFAULT_EPS), "set approximation ratio")
		;

		po::variables_map vm;
		po::parsed_options parsed = 
				po::command_line_parser(argc, argv).options(desc).allow_unregistered().run();     
		po::store(parsed, vm);
		po::notify(vm);    

		if (vm.count("help")) {
			cout << desc << "\n";
			return 0;
		}

		if (vm.count("rects_output_file")) {
			cout << "Curve is written to " << rects_filename << ".\n";
		} else {
			cout << "Rectangles not exported" << ".\n";
		}
		
		if (vm.count("curve1")) {
			cout << "Curve1 is read from " << curve1_filename << ".\n";
		} else {
			LOG(ERROR) << "curve1 not specified" << ".\n";
		}
		
		if (vm.count("curve2")) {
			cout << "Curve2 is read from " << curve2_filename << ".\n";
		} else {
			LOG(ERROR) << "curve2 not specified" << ".\n";
		}
	}
	catch(exception& e) {
		cerr << "error: " << e.what() << "\n";
		return 1;
	}
	catch(...) {
		cerr << "Exception of unknown type!\n";
	}
	
	int d = 2; // dimension

	ifstream inFile;
	
	inFile.open(curve1_filename);
	Curve alpha = readCurve(inFile, d);
	inFile.close();

	inFile.open(curve2_filename);
	Curve beta = readCurve(inFile, d);
	inFile.close();

	/* approximate Frechet */
	
	int m = alpha.size();
	int n = beta.size();

	vector<Point_2> all_points(alpha);
	all_points.reserve(alpha.size() + beta.size());
	all_points.insert(all_points.end(), beta.begin(), beta.end());

	QuadTree qt(all_points);
	qt.init();
	double s = 1; // need n-approx only

	WSPD wspd(&qt, s);
	//NodePairs pairs = wspd.pairs;
	vector<double> dists = wspd.distances();

	VLOG(7) << "WSPD dists:";
	for (int i = 0; i < dists.size(); i++)
	{
			VLOG(7) << dists[i];
	}
	
	//WSPD* kdtree_wspd = new KdTreeWSPD(all_points, s);
	FrechetDecider fd(alpha, beta);
	VLOG(6) << "HERE";
	double approx_frechet = fd.bin_search_frechet(dists);
	LOG(INFO) << "Approximate Frechet distance: " << approx_frechet;

	double dtw_lb = approx_frechet / (s + 1);
	double dtw_ub = approx_frechet * (s + 1) * max(m, n);
	LOG(INFO) << "Dynamic Time Warping range: [" << dtw_lb << ", " << dtw_ub << "]";

	//Sampling sampling(alpha, beta, dtw_lb, dtw_ub, eps);
	//sampling.init();
	//sampling.sample();

	//ofstream outFile;
	//outFile.open("samples.out");
	//outFile << sampling.view_samples();
	
	/* Compute approximate DTW */
	
	LOG(INFO) << "Computing shortest path through rectangles";
	const auto approx_dtw_begin = chrono::high_resolution_clock::now(); // or use steady_clock 
	
	RectCluster rect(alpha, beta, dtw_lb, dtw_ub, eps);
	rect.partition();
	
	// compute approximate DTW
	double approx_dtw = rect.compute_approx_dtw();
	
	auto approx_dtw_time = chrono::high_resolution_clock::now() - approx_dtw_begin;
	LOG(INFO) << "Finished approximate DTW computation\n" 
			<< "Elapsed time: " << chrono::duration<double, std::milli>(approx_dtw_time).count() / 1000 
			<< "seconds.\n";
			
	LOG(INFO) << "Approximate DTW distance between the given 2 curves: " << approx_dtw;
	
	LOG(INFO) << "Exporting rectangles ...";
	LOG(INFO) << rect.summarize();
	ofstream rect_file;
	if (rects_filename.compare("") != 0)
	{
		rect_file.open (rects_filename);
		if (rect_file != NULL)
		{
			rect_file << rect.export_rects() << endl;
			rect_file.close();
		}
		else {
			LOG(ERROR) << "Error opening file for exporting rectangles";
		}
	}
	
	// ---------------- Run Naive DTW algorithm ---------------------------
	
	LOG(INFO) << "Computing exact DTW using the naive DP algorithm";
	const auto exact_dtw_begin = chrono::high_resolution_clock::now(); // or use steady_clock 
	
	NaiveDTW naiveDtw(alpha, beta);
	double exact_dtw = naiveDtw.compute_DTW();
	
	auto exact_dtw_time = chrono::high_resolution_clock::now() - exact_dtw_begin;
	
	LOG(INFO) << "Finished exact DTW computation\n" 
			<< "Elapsed time: " << chrono::duration<double, std::milli>(exact_dtw_time).count() / 1000 
			<< "seconds.\n";
			
	LOG(INFO) << "The exact DTW distance between the given 2 curves: " << exact_dtw;
	
	LOG(INFO) << "The approximation ratio: " << (approx_dtw - exact_dtw) / exact_dtw * 100 << "%.";

	return 0;
}
