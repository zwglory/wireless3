//====================================================================
//RNG.cpp: Implementation of the RNG class.
//
//----Author: baobao7766
//----Time:   2015/03/02
//====================================================================

#include "RNG.h"
#include "Hash_func.h"

using namespace std;

extern uint32_t hash_func(uint32_t spine_value, size_t message_block);

//=================================
// RNG __init__
//=================================

RNG::RNG(){
	
}

RNG::RNG(uint32_t spine_value){
	this->seed = spine_value;
	this->i = 21215;
	this->next_values.clear();
	uint16_t a[2] = {(this->seed >> 16) & 0xFFFF, this->seed & 0xFFFF};

	vector<uint16_t> vec(a, a+2);
	this->next_values = vec;
}

RNG::~RNG(){
	if(!(this->next_values).empty()){
		(this->next_values).clear();
	}
}

//=================================
// RNG next_product
//=================================

uint16_t RNG::next(){

	// scalars and vectors
	uint16_t val = 0;
	
	// If there are no values ready to be returned, produce new values
	if (next_values.empty()){
		this->produce_more_values();
		//cout << "produce more values..." << endl;
	}

	// 数组移位并输出，然后删除数组的最后一个元素
	vector<uint16_t>::iterator ivec = (this->next_values).begin();
	val = *ivec;
	//cout << "next_values countting... " << endl;
	next_values.erase(ivec);

	return val;
}

void RNG::produce_more_values(){
	/*
		helper function, produces the next values to be output from the RNG, and
		adds them to self.next_values. This function should only be called 
		from within the RNG class
	*/
	uint32_t hash_output;
	hash_output = hash_func(this->seed, this->i);
	this->next_values.push_back((hash_output >> 16) & 0xFFFF);
	this->next_values.push_back(hash_output & 0xFFFF);

	// increment current index
    this->i = (this->i + 522) & ((1 << 16) - 1);
}

