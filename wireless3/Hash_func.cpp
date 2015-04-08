//====================================================================
//Hash_func.cpp: Implement of the Hash_func.h.
//
//----Author: baobao7766
//----Time:   2015/02/19
//====================================================================

#include <itpp/itcomm.h>
#include "Hash_func.h"

//==================================
// Hash function
//==================================
// 非SpineEncoder类的成员，准备单独创建一个hash类

uint32_t hash_func(uint32_t spine_value, int message_block){
	
	// lookup3Init
	uint32_t * state = new uint32_t [3];
	state[0] = 0xdeadbeef + spine_value;
	state[1] = 0xdeadbeef + spine_value;
	state[2] = 0xdeadbeef + spine_value;

	// lookup3Update
	state[1] += message_block;
    state[2] ^= state[1]; state[2] -= rot(state[1],14); 
    state[0] ^= state[2]; state[0] -= rot(state[2],11); 
    state[1] ^= state[0]; state[1] -= rot(state[0],25); 
    state[2] ^= state[1]; state[2] -= rot(state[1],16); 
    state[0] ^= state[2]; state[0] -= rot(state[2],4);  
    state[1] ^= state[0]; state[1] -= rot(state[0],14); 
    state[2] ^= state[1]; state[2] -= rot(state[1],24); 

	// lookup3Digest
	return state[2];
}

uint32_t rot(uint32_t x, int k){
	// 函数功能：循环移位
	return ((x << k)|(x >> (32-k)));
}












