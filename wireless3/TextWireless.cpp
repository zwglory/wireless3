////========================================================================
////wireless3.cpp:  Simulation of Spinal Codes over AWGN.
////
////----Author: baobao7766
////----Time:   2015/02/19
////========================================================================
//
//#include <itpp/itcomm.h>
//#include <fstream>
//#include <iostream>	 //std::cout, std::cin
//#include <iterator>  //std::back_inserter
//
//#include "SpineEncoder.h"
//#include "SpineDecoder.h"
//
//using namespace itpp;
//using namespace std;
//
//int main(int argc, char **argv)
//{
//	// scalars and vectors
//	int k = 3,
//		c = 8,
//		precision = 8,
//		L = 3,
//		B = 4,
//		d = 2;
//
//////=====================================================================================
////// 读文件中的数据作为信源
////// message 为以字节读入的数据，num 为转换为 bit 的原数据
//////=====================================================================================
////	cout << "======================= Reading data =========================" << endl;
////	ifstream infile("Data\\TestEncoder.txt", ios::binary);
////	ofstream outfile("Data\\bTestEncoder.txt", ios::binary);
////	
////	// get length of file
////	infile.seekg (0, infile.end);
////	int length = infile.tellg();
////	infile.seekg (0, infile.beg);
////	
////	std::cout << "Reading " << length << " characters... " << endl;
////	
////	char* rawdata = new char [length];
////	
////	infile.read (rawdata,length);
////	cout << "Content in message is: " << '\n' << rawdata << endl;
////
////	cout << "Rawdata in binary is: " << endl;
////	int mlen = length * 8;
////	int* message = new int [mlen];
////	for (int j=0; j!=length; ++j){
////		for (int i=0; i != 8; ++i){
////			message[i+j*8] = (((0x80 & (rawdata[j] << i)) == 128)?1:0);
////			cout << message[i+j*8] << " ";	
////			outfile << message[i+j*8];      // 文件对应的二进制结果输出到binary中
////		}
////	}
////	cout << '\n' << "Writing complete." << endl;
//
////=====================================================================================
//// channel coding
////=====================================================================================
//	cout << "====================== SpineEncode start =====================" << endl;
//	vector<double> sym_trans;
//
//	ofstream sym_file("Data\\sTestEncoder.txt", ios::binary);
//
//	SpineEncoder sc(k, c, precision, L);
//	
//	
//	/* scalars and vectors for encoder */
//	int len_spine = 7;	
//
//	int* message_block;						// 按 k 分块后的 message
//	int sym_len = L*len_spine;			// length of output symbols
//
//	spine = new uint32_t [(n+k-1)/k];
//	cout << "Spine length : " << len_spine << endl;	
//
//	RNG* rngs = new RNG [len_spine];		// RNG 对象数组 rngs[len_spine]
//	//uint8_t* symbols = new uint8_t [sym_len];			// output symbols of the encoder
//	vector<double> symbols;
//
//
//	uint16_t* rng_out = new uint16_t [len_spine];
//
//	SymbolMap mapper(c, precision);			// SymbolMap 对象
//	/* hash the message into spine[]. */
//	cout << "--------------hash the message------------------" << endl 
//		 << "Spine values: " << endl;
//	for(int i=0; i!=len_spine; ++i){
//		spine_value = hash_func(spine_value, message_block[i]);
//		spine[i] = spine_value;
//		rngs[i] = RNG(spine_value);
//		cout << spine_value << '\t';
//	}
//	cout << '\n' << "Hash complete." << endl;
//
//
//	/* making 3 passes, mapping to the symbol. */
//	cout << "------------making 3 passes, mapping to the symbol------------" << endl;
//
//	for(int j=0; j!=L; ++j){
//		for(int i=0; i!=len_spine; ++i){
//			//cout << "j: " << j << '\t' << "i: " << i << endl;
//			rng_out[i] = rngs[i].next();
//
//			//cout << "num, rngs and symbols: " << endl;
//			symbols.push_back(mapper.map_func(rng_out[i]));
//
//			/*cout << i+j*len_spine << " —— " << rng_out[i] << " —— " 
//				<< symbols[i+j*len_spine] << endl;*/
//		}
//	}
//	cout << endl;
//
//	power = mapper.get_signal_average_power();
//	cout << "Symbol power getting complete." << endl;
//
//	// release memory
//	delete []message_block;
//	delete []rng_out;
//	delete []rngs;
//	
//	vector<double>::iterator isym = sym_trans.begin();
//	while(isym != sym_trans.end()){
//		sym_file << *isym << '\t';
//		++isym;
//	}
//	cout << endl;
//
//	//=============================
//	// release file stream
//	//=============================
//	infile.close();
//	outfile.close();
//	sym_file.close();
//
//	return 0;
//}
