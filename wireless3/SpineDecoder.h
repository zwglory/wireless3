//====================================================================
//SpineDecoder.h: Interface of the SpineDecoder class.
//
//----Author: baobao7766
//----Time:   2015/03/19
//====================================================================

#include <map>
#include <utility>
#include "RNG.h"
#include "Mapping.h"
#include "Hash_func.h"
#include "CodeTreeNode.h"

using namespace std;

class SpineDecoder{
public:
	SpineDecoder();
	SpineDecoder(vector<double> rec_symbol, size_t k_in, int B_in, int d_in, int spineLen_in, int L_in);
	~SpineDecoder();

	//Decoder
	vector<int> decoder();
	
private:
	vector<double> symbol;
	int B, d, spineLen, L;
	size_t k;
	vector<CodeTreeNode> wavefront;		//对象数组，存储编码树 d 层的所有编码节点 CodeTreeNode

	void expand_wavefront(vector<int> sym_depth);
	void prune_wavefront();
	vector<int> get_most_likely();

	inline int minTree(vector<CodeTreeNode> beam);
};
