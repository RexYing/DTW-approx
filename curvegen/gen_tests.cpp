#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include <iostream>
#include <fstream>

#include "curvegen/curve_generator.h"
#include "easylogging++.h"

using namespace std;

INITIALIZE_EASYLOGGINGPP

const string DEFAULT_OUTPUT_FILE = "tests/curve.dtw";
const string DEFAULT_CURVE_TYPE = "line";
const string DEFAULT_PARAMS = "start=(0,0) angle=0.5 step=1 n=500";


void gen_x(ofstream &outFile, int n, int offset, double step)
{
    for (int i = 0; i < n; i++)
    {
        outFile << i * step << " " << offset + i * step << endl;
    }
}

void gen_x_reverse(ofstream &outFile, int n, int offset, int step)
{
    for (int i = n - 1; i >= 0; i--)
    {
        outFile << i * step << " " << offset + i * step << endl;
    }
}

void export_points(ofstream &out_file, Points pts)
{
	out_file << pts.size() << endl;
	for (auto pt : pts)
	{
		out_file << pt.first << " " << pt.second << endl;
	}
}

int main(int argc, char* argv[])
{
	string filename;
	string curve_type;
	string params;
	
	try {

		po::options_description desc("Allowed options");
		desc.add_options()
			("help", "produce help message")
			("output_file", po::value<string>(&filename)->default_value(DEFAULT_OUTPUT_FILE), 
					"set output file for curve generated")
			("curve_type", po::value<string>(&curve_type)->default_value(DEFAULT_CURVE_TYPE), 
					"set type of curve")
			("params", po::value<string>(&params)->default_value(DEFAULT_PARAMS), 
					"set parameters for the curve")
		;

		po::variables_map vm;        
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);    

		if (vm.count("help")) {
			cout << desc << "\n";
			return 0;
		}

		if (vm.count("output_file")) {
			cout << "Curve is written to " << filename << ".\n";
		} else {
			cout << "Use default output file " << filename << ".\n";
		}
		
		if (vm.count("curve_type")) {
			cout << "Curve type is set to " << curve_type << ".\n";
		} else {
			cout << "Use default curve type " << curve_type << ".\n";
		}
		
		if (vm.count("params")) {
			cout << "Curve parameter is set to " << params << ".\n";
		} else {
			cout << "Use default number of points on curve " << params << ".\n";
		}
	}
	catch(exception& e) {
		cerr << "error: " << e.what() << "\n";
		return 1;
	}
	catch(...) {
		cerr << "Exception of unknown type!\n";
	}
    

	ofstream outFile;
	CurveGenerator gen;
	Points pts;

	// parallel equidistance
	outFile.open(filename);
	if (curve_type == "line")
	{
		pts = gen.line(params);
	}
	else if (curve_type == "rand")
	{
		pts = gen.rand(params);
	} 
	else
	{
		LOG(ERROR) << "Unrecognized curve type";
	}
	export_points(outFile, pts);
/* 	outFile << n << endl;
	gen_x(outFile, n, 0, 10);
	outFile.close();

	outFile.open("tests/t6.dtw");
	outFile << n << endl;
	gen_x(outFile, n, -5, 10);
	outFile.close(); */

	return 0;
}

