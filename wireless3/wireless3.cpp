//========================================================================
//wireless3.cpp:  Simulation of Spinal Codes over AWGN.
//
//----Author: baobao7766
//----Time:   2015/02/19
//========================================================================

#include <itpp/itcomm.h>	//itpp::vec, AWGN_Channel, it_file, BERC, Real_Timer, linspace()
#include <fstream>			//std::ifstream, ofstream, seekg(), tellg(), read()
#include <iostream>			//std::cout, cin
#include <iterator>			//std::back_inserter

#include "SpineEncoder.h"	//SpineEncoder::encoder(), sym_len
#include "SpineDecoder.h"	//SpineDecoder::decoder()
#include "itppExtend.h"

#define ENCODE_START ;
//#define DECODE_START ;

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
	//uint8_t * sym_trans = new uint8_t [k*spineLen];
	//uint8_t * sym_recv = new uint8_t [k*spineLen];
	vector<double> sym_trans;

	ofstream sym_file("Data\\symbol.txt", ios::binary);

	SpineEncoder sc(k, c, precision, L, mlen);
	sym_trans = sc.encoder(message);

	vector<double>::iterator isym = sym_trans.begin();
	while(isym != sym_trans.end()){
		//cout << *isym << '\t';
		sym_file << *isym << '\t';
		++isym;
	}
	cout << endl;

	//=============================
	// release file stream
	//=============================
	infile.close();
	outfile.close();
	sym_file.close();
	cout << "Symbol writing complete." << endl;
	cout << "Symbol length: " << L*spineLen << " byte" << endl
		 << "Spinal coding complete." << endl;

//=====================================================================================
// channel with itpp
//=====================================================================================
	cout << "====================== channel with itpp =====================" << endl;
	/* before decoding */
	vec trans_sym = zeros(L*spineLen);
	vec recive_sym = zeros(L*spineLen);

	/* after decoding */
	bvec trans_bits = to_bvec(zeros(mlen));
	bvec recive_bits = to_bvec(zeros(mlen));

	/* Channel scalars and vectors */
	double Ec, Eb;
	vec EbN0dB, EbN0, N0, noise_variance, bit_error_rate; 

	//AWGN_Channel awgn_channel;     //The AWGN channel class
	it_file ff;                    //For saving the results to file
	BERC berc;                     //Used to count the bit errors
	//Real_Timer tt;                 //The timer used to measure the execution time

	/* Init */
	Ec = sc.power;					//The transmitted energy per symbol.
	Eb = Ec;						//The transmitted energy per bit.
	EbN0dB = linspace(10,50,30);	//Simulate for 10 Eb/N0 values from 0 to 9 dB.
	EbN0 = inv_dB(EbN0dB);			//Calculate Eb/N0 in a linear scale instead of dB. 
	N0 = Eb * pow(EbN0,-1.0);		//N0 is the variance of the (complex valued) noise.

	/* malloc to itpp::vec */
	trans_sym = uint_to_vec(sym_trans, L*spineLen);
	//cout << "trans_sym: " << '\n' << trans_sym << endl;
	trans_bits = uint_to_bvec(message, mlen);
	//cout << "trans_bits: " << '\n' << trans_bits << endl;

	//Allocate storage space for the result vector. 
	//The "false" argument means "Do not copy the old content of the vector to the new storage area."
	bit_error_rate.set_size(EbN0dB.length(),false);
	
	for(int i=0; i!=EbN0dB.length(); ++i){
		//Show how the simulation progresses:
		cout << "Now simulating Eb/N0 value number " << i+1 << " of " << EbN0dB.length() << endl;

		//Set the noise variance of the AWGN channel:
		AWGN_Channel awgn_channel(N0(i));     //The AWGN channel class
		//awgn_channel.set_noise(N0(i));

		//Run the transmited symbols through the channel using the () operator:
		recive_sym = awgn_channel(trans_sym);
		//recive_sym = trans_sym + 0.2*sqrt(N0(i)/2)*randn(k*spineLen);
		//cout << "recive_sym: " << recive_sym << endl;

		/* decoding, sequence of 1 and 0, after decoded */
		vector<double> sym_recv = vec_to_uint(recive_sym, k*spineLen);

		SpineDecoder desc(sym_recv, k, B, d, spineLen, L);
		vector<int> de_message = desc.decoder();

		/* Check if de_message and message in the same length. */
		if(de_message.size() != mlen){
			for(size_t count = de_message.size();
				count != mlen;
				--count)
				de_message.erase(de_message.end()-1);
		}

		recive_bits = uint_to_bvec(de_message);

		//Calculate the bit error rate:
		berc.clear();                               //Clear the bit error rate counter
		berc.count(trans_bits,recive_bits);			//Count the bit errors
		bit_error_rate(i) = berc.get_errorrate();   //Save the estimated BER in the result vector

		if(!sym_recv.empty())
			sym_recv.clear();
		if(!de_message.empty())
			de_message.clear();

	}//end for

	//Print the results:
	/*cout << "trans bits: " << endl << trans_bits << endl;
	cout << "the last recive bits: " << endl << recive_bits << endl;*/
	cout << endl;
	cout << "EbN0dB = " << EbN0dB << " [dB]" << endl;
	cout << "BER = " << bit_error_rate << endl;
	cout << "Saving results to ./sc_result_file.it" << endl;
	cout << endl;

	//Save the results to file:
	ff.open("Results/sc_result_file.it");
	ff << Name("EbN0dB") << EbN0dB;
	ff << Name("ber") << bit_error_rate;
	ff.close();

	

	return 0;
}

