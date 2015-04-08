//====================================================================
//Mapping.h: Interface of the Mapping class.
//
//----Author: baobao7766
//----Time:   2015/03/05
//====================================================================

#ifndef _MAPPING_H_
#define _MAPPING_H_

#include <string>
#include <itpp/itcomm.h>

class SymbolMap{
public:
	SymbolMap();
	SymbolMap(int a, int b):c(8), precision(8){
		// 编码部分是以precision取8为标准的
		c = a;
		precision = b;
	};
	virtual ~SymbolMap();

	uint8_t map_func(uint16_t value);
/* 
	我们认为使用 hash 函数编码得到的码元是均匀分布的
	所以，在求编码后信号的功率时，使用离散均匀分布的二阶中心距
	作为信号的平均功率，P = (N - 1)*(N + 1) / 12.0
*/
	double get_signal_average_power();

private:
	int c;
	int precision;
};
#endif