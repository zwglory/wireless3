========================================================================
wireless3.cpp:  Simulation of Spinal Codes over AWGN.

----Author: baobao7766
----Time:   2015/02/19
========================================================================

#include <itpp/itcomm.h>
#include <fstream>
#include <iostream>	 //std::cout, std::cin
#include <iterator>  //std::back_inserter

#include "SpineEncoder.h"
#include "SpineDecoder.h"

using namespace itpp;
using namespace std;

int main(int argc, char **argv)
{
	// scalars and vectors
	int k = 3,
		c = 8,
		precision = 8,
		L = 3,
		B = 4,
		d = 2;

//=====================================================================================
// 读文件中的数据作为信源
// message 为以字节读入的数据，num 为转换为 bit 的原数据
//=====================================================================================
	cout << "======================= Reading data =========================" << endl;
	ifstream infile("Data\\TestFile.txt", ios::binary);
	ofstream outfile("Data\\binary.txt", ios::binary);
	
	// get length of file
	infile.seekg (0, infile.end);
	int length = infile.tellg();
	infile.seekg (0, infile.beg);
	
	std::cout << "Reading " << length << " characters... " << endl;
	
	char* rawdata = new char [length];
	
	infile.read (rawdata,length);
	cout << "Content in message is: " << '\n' << rawdata << endl;

	cout << "Rawdata in binary is: " << endl;
	int mlen = length * 8;
	int* message = new int [mlen];
	for (int j=0; j!=length; ++j){
		for (int i=0; i != 8; ++i){
			message[i+j*8] = (((0x80 & (rawdata[j] << i)) == 128)?1:0);
			cout << message[i+j*8] << " ";	
			outfile << message[i+j*8];      // 文件对应的二进制结果输出到binary中
		}
	}
	cout << '\n' << "Writing complete." << endl;

//=====================================================================================
// channel coding
//=====================================================================================
	cout << "====================== SpineEncode start =====================" << endl;
	int spineLen = (mlen+k-1)/k;
	vector<double> sym_trans;

	ofstream sym_file("Data\\symbol.txt", ios::binary);

	SpineEncoder sc(k, c, precision, L, mlen);
	int* blocks = sc.divm2bl(message);

	return 0;
}
