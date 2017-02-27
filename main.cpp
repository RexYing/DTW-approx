#include <boost/program_options.hpp>
#include <chrono>
#include <iostream>
#include <fstream>
#include <vector>
#include <CGAL/Simple_cartesian.h>


#include "easylogging++.h"
#include "frechet.h"
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

const double DEFAULT_EPS = 0.5;

void deallocDouble2D(double** arr, int l)
{
   for (int i = 0; i < l; i++)
   {
       delete[] arr[i];
   }
   delete[] arr;
}



void export_alignment(string filename, vector<pair<int, int>> alignment)
{
	ofstream align_file;
	align_file.open (filename);
	if (align_file)
	{
		for (int i = alignment.size() - 1; i >= 0; i--)
		{
			align_file << alignment[i].first << " " << alignment[i].second << endl;
		}
		align_file.close();
	}
	else {
		LOG(ERROR) << "Error opening file for exporting alignment info";
	}
}

int main(int argc, char* argv[])
{
	START_EASYLOGGINGPP(argc, argv);
		
	string curve1_filename;
	string curve2_filename;
	string rects_filename = "";
	string align_filename = "";
	string exact_align_filename = "";
	string approx_frechet_align_filename = "";
	string exact_frechet_align_filename = "";
	double eps;
	// set to true to compute the DTW alignment of curves
	bool trace_alignment = false;

	try {
		po::options_description desc("Allowed options");
		desc.add_options()
			("help", "produce help message")
			("rects_output_file", po::value<string>(&rects_filename), "set output file for rectangles")
			("align_output_file", po::value<string>(&align_filename), "set output file for DTW alignment info")
			("exact_align_output_file", po::value<string>(&exact_align_filename), "set output file for exact DTW alignment info")
			("approx_frechet_align_output_file", po::value<string>(&approx_frechet_align_filename), "set output file for approximate Frechet alignment info")
			("exact_frechet_align_output_file", po::value<string>(&exact_frechet_align_filename), "set output file for exact Frechet alignment info")
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
			LOG(INFO) << desc << "\n";
			return 0;
		}

		if (vm.count("rects_output_file")) {
			LOG(INFO) << "Curve is written to " << rects_filename << ".\n";
		} else {
			LOG(INFO) << "Rectangles not exported" << ".\n";
		}
		
		if (vm.count("align_output_file")) {
			trace_alignment = true;
			LOG(INFO) << "Alignment info is written to " << align_filename << ".\n";
		}
		
		if (vm.count("exact_align_output_file")) {
			LOG(INFO) << "Exact alignment info is written to " << exact_align_filename << ".\n";
		}
		
		if (vm.count("approx_frechet_align_output_file")) {
			LOG(INFO) << "Approximate Frechet alignment info is written to " << approx_frechet_align_filename << ".\n";
		}
		
		if (vm.count("exact_frechet_align_filename")) {
			LOG(INFO) << "Exact Frechet alignment info is written to " << exact_frechet_align_filename << ".\n";
		}
		
		if (vm.count("curve1")) {
			LOG(INFO) << "Curve1 is read from " << curve1_filename << ".\n";
		} else {
			LOG(ERROR) << "curve1 not specified" << ".\n";
		}
		
		if (vm.count("curve2")) {
			LOG(INFO) << "Curve2 is read from " << curve2_filename << ".\n";
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
	LOG(INFO) << "Use Frechet decider... " ;
	/* FrechetDecider fd(alpha, beta);
	double approx_frechet = fd.bin_search_frechet(dists);
	LOG(INFO) << "Approximate Frechet distance: " << approx_frechet;
	
	// export approximate Frechet alignment
	if (approx_frechet_align_filename.compare("") != 0)
	{
		vector<pair<int, int>> approx_frechet_alignment = fd.trace_alignment();
		export_alignment(approx_frechet_align_filename, approx_frechet_alignment);
		LOG(INFO) << "Finished exporting approximate Frechet alignment";
	} 

	double dtw_lb = approx_frechet / (s + 1);
	double dtw_ub = approx_frechet * (s + 1) * max(m, n); */
	double dtw_lb = 0.0001;
	double dtw_ub = 1000000;
	LOG(INFO) << "Dynamic Time Warping range: [" << dtw_lb << ", " << dtw_ub << "]"; 

	//Sampling sampling(alpha, beta, dtw_lb, dtw_ub, eps);
	//sampling.init();
	//sampling.sample();

	//ofstream outFile;
	//outFile.open("samples.out");
	//outFile << sampling.view_samples();
	
	/* Compute approximate DTW */
	
	LOG(INFO) << "Computing shortest path through rectangles";
	
	RectCluster rect(alpha, beta, dtw_lb, dtw_ub, eps);
	//rect.partition();
	rect.sequential_partition();
	
	LOG(INFO) << "Exporting rectangles ...";
	LOG(INFO) << rect.summarize();
	ofstream rect_file;
	if (rects_filename.compare("") != 0)
	{
		rect_file.open (rects_filename);
		if (rect_file)
		{
			rect_file << rect.export_rects() << endl;
			rect_file.close();
		}
		else {
			LOG(ERROR) << "Error opening file for exporting rectangles";
		}
	}
	
	
	double approx_dtw = 0; 
	vector<pair<int, int>> approx_alignment;
	const auto approx_dtw_begin = chrono::high_resolution_clock::now(); // or use steady_clock 

	if (trace_alignment)
	{
		// compute approximate DTW with alignment info
		approx_alignment = rect.compute_approx_dtw(true, approx_dtw);
	}
	else
	{
		// compute approximate DTW without alignment info
		rect.compute_approx_dtw(false, approx_dtw);
	}	
	
	auto approx_dtw_time = chrono::high_resolution_clock::now() - approx_dtw_begin;
	LOG(INFO) << "Approximate DTW distance between the give 2 curves: " << approx_dtw;
	LOG(INFO) << "Finished approximate DTW computation\n" 
			<< "Elapsed time: " << chrono::duration<double, std::milli>(approx_dtw_time).count() / 1000 
			<< "seconds.\n";
	
	// export approximate DTW alignment
	if (trace_alignment)
	{ 
		export_alignment(align_filename, approx_alignment);
		LOG(INFO) << "Finished exporting approximate DTW alignment";
	}
	
	// ---------------- Run Naive DTW algorithm ---------------------------
	
	LOG(INFO) << "Computing exact DTW using the naive DP algorithm";
	const auto exact_dtw_begin = chrono::high_resolution_clock::now(); // or use steady_clock 
	
	NaiveDTW naiveDtw(alpha, beta);
	double exact_dtw = naiveDtw.compute_DTW();
	vector<pair<int, int>> exact_alignment = naiveDtw.trace_alignment();
	
	auto exact_dtw_time = chrono::high_resolution_clock::now() - exact_dtw_begin;
	
	LOG(INFO) << "The exact DTW distance between the given 2 curves: " << exact_dtw;
	LOG(INFO) << "Finished exact DTW computation\n" 
			<< "Elapsed time: " << chrono::duration<double, std::milli>(exact_dtw_time).count() / 1000 
			<< "seconds.\n";
			
	// export exact DTW alignment
	if (exact_align_filename.compare("") != 0)
	{
		export_alignment(exact_align_filename, exact_alignment);
		LOG(INFO) << "Finished exporting exact DTW alignment";
	}
	
	// ---------------- Run Naive Frechet algorithm ---------------------------
	
	if (!exact_frechet_align_filename.empty()) {
		LOG(INFO) << "Computing exact Frechet using the naive DP algorithm";
		const auto exact_frechet_begin = chrono::high_resolution_clock::now(); // or use steady_clock 
		
		Frechet frechet(alpha, beta);
		double exact_frechet = frechet.naive();
		vector<pair<int, int>> exact_frechet_alignment = frechet.correspondence();
		
		auto exact_frechet_time = chrono::high_resolution_clock::now() - exact_frechet_begin;
		
		LOG(INFO) << "The exact Frechet distance between the given 2 curves: " << exact_frechet;
		LOG(INFO) << "Finished exact Frechet computation\n" 
				<< "Elapsed time: " 
				<< chrono::duration<double, std::milli>(exact_frechet_time).count() / 1000 
				<< "seconds.\n";
				
		// export exact frechet alignment
		LOG(INFO) << exact_frechet_align_filename;
		export_alignment(exact_frechet_align_filename, exact_frechet_alignment);
		LOG(INFO) << "Finished exporting exact Frechet alignment";
		
	}

	return 0;
}
