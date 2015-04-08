//====================================================================
//Mapping.cpp: Implementation of the SymbolMap class.
//
//----Author: baobao7766
//----Time:   2015/03/05
//====================================================================

#include "Mapping.h"


//=============================
// __init__
//=============================
SymbolMap::SymbolMap(){
	
}

SymbolMap::~SymbolMap(){
	
}


//=============================
// Mapping function
//=============================
// value a 16-bit pseudo random number, 
// to be mapped into a constellation point

uint8_t SymbolMap::map_func(uint16_t value){

	// calculate a bitmask to extract only the lowest c bits
	uint8_t mask = (1 << this->c) - 1;
	// Get only the lowest c bits
	uint8_t bits = value & mask;

	// "map" bits into a constellation point by adding precision
    int addedPrecisionBits = this->precision - this->c;
	uint8_t symbol = bits << addedPrecisionBits;

	return symbol;
}

double SymbolMap::get_signal_average_power(){
	/* The constellation is Discrete Uniform Distribution */
	int num_constellation = (1 << this->c);
	double power = (num_constellation-1)*(num_constellation+1)/12.0;
	/* Symbol 增大到原来的 2^^(precision) 倍，Power 便增大到原来的 2^^(2*precision) 倍 */
	/* Symbol 增大到原来的 1 << precision 倍，Power 便增大到原来的 1<<(2*precision) 倍 */
	int added_precision_bits = this->precision - this->c;
	power *= (1 << (2 * added_precision_bits));

	return power;
}