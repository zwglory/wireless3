//====================================================================
//SpineEncoder.h: Interface of the SpineEncoder class.
//
//----Author: baobao7766
//----Time:   2015/02/19
//====================================================================
#include <vector>
#include <string>
#include <itpp/itcomm.h>

#include "RNG.h"
#include "Mapping.h"
#include "Hash_func.h"

using namespace std;

//template<class codetype, class symtype>

class SpineEncoder{
public:
	SpineEncoder();
	SpineEncoder(size_t k_in, int c_in, int precision_in, int L_in){
		/* This is a test constructor for encoder, please do not use it in implementation. */
		this->n = 21;
	};
	SpineEncoder(size_t k_in, int c_in, int precision_in, int L_in, int mlen);
	virtual ~SpineEncoder();

	vector<double> encoder(int* message);	

	double power;				// power of symbol

	//Divide message into blocks
	int* divm2bl(int* message);

private:
	size_t n;
	size_t k;
	uint32_t spine_value;		// spine after hash
	uint32_t* spine;			// message after hash
	int len_spine;				// spine length
	int sym_len;				// length of output symbols
	int c;						// mapping length
	int precision;				// max mapping length
	int L;						// number of passes

};
