//====================================================================
//RNG.h: Interface of the RNG class.
//
//----Author: baobao7766
//----Time:   2015/03/02
//====================================================================

#ifndef _RNG_H_
#define _RNG_H_

#include <vector>
#include <itpp/itcomm.h>

class RNG{
public:
	RNG();
	RNG(uint32_t spine_value);
	virtual ~RNG();

	uint16_t next();

private:
	int i;						// 用在 hash 函数中的初值
	uint32_t seed;					// 就是 spine_value， 作为RNG种子
	std::vector<uint16_t> next_values ;	// 对应每个 seed 的移位寄存器

	//用于 next()，用 hash 函数产生其余的 rng_output.
	void produce_more_values();
};
#endif