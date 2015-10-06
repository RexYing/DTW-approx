#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include<iostream>
#include<fstream>

using namespace std;

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
  /*
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
        ("compression", po::value<int>(), "set compression level");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
      cout << desc << "\n";
      return 0;
    }

    if (vm.count("compression")) {
      cout << "Compression level was set to " 
          << vm["compression"].as<double>() << ".\n";
    } else {
      cout << "Compression level was not set.\n";
    }
    */

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

