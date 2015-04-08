//====================================================================
//itppExtend.h: Implement of itppExtend.
//
//----Author: baobao7766
//----Time:   2015/03/30
//====================================================================

#include "itppExtend.h"

/* Symbol to itpp::vec */
vec uint_to_vec(vector<double> a, int len){
	vec b = zeros(len);
	vector<double>::iterator ia = a.begin();
	for(int i=0; i!=len; ++i){
		b(i) = *(a.begin()+i);
	}
	return b;
}

/* Message to itpp::bvec */
bvec uint_to_bvec(int* a, int len){
	bvec b = to_bvec(zeros(len));
	for(int i=0; i!=len; ++i){
		b(i) = (a[i] == 1);
	}
	return b;
}
bvec uint_to_bvec(vector<int> a){
	bvec b = to_bvec(zeros(a.size()));
	for(int i=0; i!=a.size(); ++i){
		b(i) = (*(a.begin()+i) == 1)?1:0;
	}
	return b;
}

/* itpp::vec to Symbol */
vector<double> vec_to_uint(vec b, int len){
	vector<double> a;
	for(int i=0; i!=len; ++i){
		a.push_back(b(i));
	}
	return a;
}

/* itpp::bvec to message */
int* bvec_to_uint(bvec b, int len){
	int* a = new int [len];
	for(int i=0; i!=len; ++i){
		a[i] = b(i)?1:0;
	}
	return a;
}
