#include<iostream>
#include<fstream>

using namespace std;

void gen_x(ofstream &outFile, int n, int offset, int step)
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
		outFile << 100 << endl;
    gen_x(outFile, 100, 0, 1);
    outFile.close();

    outFile.open("tests/t4.dtw");
    outFile << 100 << endl;
    gen_x(outFile, 100, -5, 1);
    outFile.close();
}

