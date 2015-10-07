#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include<iostream>
#include<fstream>

using namespace std;

const string DEFAULT_OUTPUT_FILE = "tests/curve.dtw";
const string DEFAULT_CURVE_TYPE = "line";


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

int main(int argc, char* argv[])
{
	string filename = DEFAULT_OUTPUT_FILE;
	string curve_type = DEFAULT_CURVE_TYPE;
	
	try {

		po::options_description desc("Allowed options");
		desc.add_options()
			("help", "produce help message")
			("output_file", po::value<string>(), "set output file for curve generated")
			("curve_type", po::value<string>(), "set type of curves")
		;

		po::variables_map vm;        
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);    

		if (vm.count("help")) {
			cout << desc << "\n";
			return 0;
		}

		if (vm.count("output_file")) {
			filename = vm["output_file"].as<string>();
			cout << "Curve is output to " << filename << ".\n";
		} else {
			cout << "Output file was not set.\n";
		}
		
		if (vm.count("curve_type")) {
			curve_type = vm["curve_type"].as<string>();
			cout << "Curve type is set to " << curve_type << ".\n";
		} else {
			cout << "Curve type was not set.\n";
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

	// parallel equidistance
	int n = 500;
	outFile.open("tests/t5.dtw");
	outFile << n << endl;
	gen_x(outFile, n, 0, 10);
	outFile.close();

	outFile.open("tests/t6.dtw");
	outFile << n << endl;
	gen_x(outFile, n, -5, 10);
	outFile.close();

	return 0;
}

