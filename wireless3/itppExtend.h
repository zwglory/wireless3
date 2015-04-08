//====================================================================
//itppExtend.h: Interface of itppExtend.
//
//----Author: baobao7766
//----Time:   2015/03/30
//====================================================================

#ifndef ITPPEXTEND_H
#define ITPPEXTEND_H

#include <itpp\itcomm.h>

using namespace std;
using namespace itpp;

/* Symbol to itpp::vec */
vec uint_to_vec(vector<double> a, int len);

/* Message to itpp::bvec */
bvec uint_to_bvec(int* a, int len);
bvec uint_to_bvec(vector<int> a);

/* itpp::vec to Symbol */
vector<double> vec_to_uint(vec b, int len);

/* itpp::bvec to message */
int* bvec_to_uint(bvec b, int len);

#endif