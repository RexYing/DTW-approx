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

int main()
{
		ofstream outFile;

		outFile.open("tests/t3.dtw");
		outFile << 1000 << endl;
    gen_x(outFile, 1000, 0, 50);
    outFile.close();

    outFile.open("tests/t4.dtw");
    outFile << 1000 << endl;
    gen_x(outFile, 1000, -5, 50);
    outFile.close();
}

