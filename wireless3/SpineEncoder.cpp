//========================================================================
//SpineEncoder.cpp: Implementation of SpineEncoder class.
//
//----Author: baobao7766
//----Time:   2015/02/19
//========================================================================

#include <vector>
#include <string>

#include "SpineEncoder.h"

using namespace std;

extern uint32_t hash_func(uint32_t spine_value, size_t message_block);

//=================================
// Spinal code __init__
//=================================
SpineEncoder::SpineEncoder(){
	this->spine_value = 0;
	this->sym_len = 0;
	this->power = 0;
	/* Can not encode */
}

SpineEncoder::SpineEncoder(size_t k_in, int c_in, int precision_in, int L_in, int mlen){
	this->spine_value = 0;
	this->sym_len = 0;
	this->power = 0;

	this->k = k_in;
	this->n = mlen;
	this->len_spine = (this->n+this->k-1) / this->k;

	this->L = L_in;
	this->c = c_in;
	this->precision = precision_in;
}

SpineEncoder::~SpineEncoder(){
	if(NULL!=spine){
		delete [] spine;
		spine = NULL;
	}
}

//==================================
// Divide message into blocks
//==================================
/*	
	SpineEncoder类的私有成员
	输入：按 bit 读取的原数据转化为 int 类型后的数据，为了方便实验没有使用严格的二进制数据
	输出：按 k 划分后的信息数组，数组中的每个元素为 k-bit 的值，但实际将其定义成了 int 类型的数组
	待改进：待测试结束后，可以直接使用按二进制读入的文件数据作为待编码数据。
*/

int* SpineEncoder::divm2bl(int* message){
	size_t k = this->k;
	size_t n = this->n;
	size_t len = this->len_spine;

	int* message_block = new int [len];

	/* initial message_block */
	for(int i=0; i!=len; ++i){
		message_block[i] = 0;
	}//end for

	/* dividing */
	for(int j=0; j!=len; ++j){
		for(int i=0; i!=k; ++i){
			message_block[j] += ((message[k*j+i]==1)?(1<<(k-i-1)):0);
		}//end for
		cout << message_block[j] << '\t';
	}//end for

	return message_block;
}

//==================================
// Encoder
//==================================
vector<double> SpineEncoder::encoder(int* message){

	/* scalars and vectors for encoder */
	size_t k = this->k;
	int c = this->c;
	int precision = this->precision;
	int L = this->L;
	int len_spine = this->len_spine;	

	int* message_block;						// 按 k 分块后的 message
	this->sym_len = L*len_spine;			// length of output symbols

	this->spine = new uint32_t [(this->n+k-1)/k];
	cout << "Spine length : " << len_spine << endl;	

	RNG* rngs = new RNG [len_spine];		// RNG 对象数组 rngs[len_spine]
	//uint8_t* symbols = new uint8_t [this->sym_len];			// output symbols of the encoder
	vector<double> symbols;


	uint16_t* rng_out = new uint16_t [len_spine];

	SymbolMap mapper(c, precision);			// SymbolMap 对象

	/* divide message into blocks. */
	cout << "---------------divide message into blocks-----------------" << endl
		 << "message block: " << endl;
	message_block = divm2bl(message);
	cout << endl;


	/* hash the message into spine[]. */
	cout << "--------------hash the message------------------" << endl 
		 << "Spine values: " << endl;
	for(int i=0; i!=len_spine; ++i){
		this->spine_value = hash_func(this->spine_value, message_block[i]);
		this->spine[i] = this->spine_value;
		rngs[i] = RNG(this->spine_value);
		cout << spine_value << '\t';
	}
	cout << '\n' << "Hash complete." << endl;


	/* making 3 passes, mapping to the symbol. */
	cout << "------------making 3 passes, mapping to the symbol------------" << endl;

	for(int j=0; j!=L; ++j){
		for(int i=0; i!=len_spine; ++i){
			//cout << "j: " << j << '\t' << "i: " << i << endl;
			rng_out[i] = rngs[i].next();

			//cout << "num, rngs and symbols: " << endl;
			symbols.push_back(mapper.map_func(rng_out[i]));

			/*cout << i+j*len_spine << " —— " << rng_out[i] << " —— " 
				<< symbols[i+j*len_spine] << endl;*/
		}
	}
	cout << endl;

	this->power = mapper.get_signal_average_power();
	cout << "Symbol power getting complete." << endl;

	// release memory
	delete []message_block;
	delete []rng_out;
	delete []rngs;

	return symbols;
}









