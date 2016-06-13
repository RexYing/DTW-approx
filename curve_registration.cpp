#include <boost/program_options.hpp>
#include <chrono>
#include <iostream>
#include <fstream>
#include <vector>
#include <CGAL/Simple_cartesian.h>

#include "icp.h"

namespace po = boost::program_options;
using namespace std;

INITIALIZE_EASYLOGGINGPP

const double DEFAULT_EPS = 0.5;

int main(int argc, char* argv[])
{
	START_EASYLOGGINGPP(argc, argv);
		
	string curve1_filename;
	string curve2_filename;
	string output_dir = "";
	string method = "DTW-approx";
	double eps;
	
	try {
		po::options_description desc("Allowed options");
		desc.add_options()
			("help", "produce help message")
			("output_dir", po::value<string>(&output_dir), "set intermediate output dir")
			("curve1", po::value<string>(&curve1_filename), "set input file for curve1")
			("curve2", po::value<string>(&curve2_filename), "set input file for curve2")
			("method", po::value<string>(&method), "set curve similarity measure for defining correspondence")
			("eps", po::value<double>(&eps)->default_value(DEFAULT_EPS), "set approximation ratio (if choosing DTW-approx)")
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

		if (vm.count("output_dir")) {
			LOG(INFO) << "Output is written to " << output_dir << ".\n";
		} else {
			LOG(INFO) << "No file output is produced" << ".\n";
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
		
		if (vm.count("method")) {
			LOG(INFO) << "Correspondence is defined by " << method << ".\n";
		} else {
			LOG(INFO) << "Use DTW-approx (by default)" << ".\n";
		}
		
		if (vm.count("eps")) {
			LOG(INFO) << "Epsilon is set to " << eps << ".\n";
		} else {
			LOG(ERROR) << "Epsilon is set to " << eps << " (by default).\n";
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
	
	ICP icp(alpha, beta, "DTW-approx", eps);
	
	
}
