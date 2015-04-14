//====================================================================
//SpineDecoder.cpp: Implementation of the SpineDecoder class.
//
//----Author: baobao7766
//----Time:   2015/03/19
//====================================================================

#include "SpineDecoder.h"

using namespace std;

//=================================
// Spinal decode __init__
//=================================
SpineDecoder::SpineDecoder(){
/* 
	initalize wavefront with node1 [0, 0, []]
	node1:	spine_value = 0, 
			path_metric = 0,
			path = []
*/
	CodeTreeNode node1;
	this->wavefront.push_back(node1);
}

SpineDecoder::SpineDecoder(vector<double> rec_symbol, size_t k_in, int B_in, int d_in, int spineLen_in, int L_in){
/*
	@param k the number of bits in each block fed into the hash function
    @param B beam width: the maximum number of sub-trees that the decoder
        keeps when advancing to next spine value
    @param d "depth": the number of layers in each of the sub-trees that
        the decoder keeps when advancing to next spine value
	@param spineLen the length of the spine and symbol
*/
	this->symbol = rec_symbol;
	this->B = B_in;
	this->d = d_in;
	this->k = k_in;	
	this->L = L_in;
	this->spineLen = spineLen_in;

/* 
	initalize wavefront with node1 [0, 0, []]
	node1:	spine_value = 0, 
			path_metric = 0,
			path = []
*/
	CodeTreeNode node1;
	this->wavefront.push_back(node1);
}

SpineDecoder::~SpineDecoder(){
	if(!this->wavefront.empty())
		this->wavefront.clear();
	if(!symbol.empty())
		symbol.clear();
}


//==================================
// Update CodeTree
//==================================
void SpineDecoder::expand_wavefront(vector<int> sym_spine){

	/* 变量的定义和初始化是按照变量的用途分类的 */

	int edge_metric = 0;
	int distance = 0;

	RNG rng;
	SymbolMap mapper;
	uint8_t node_symbol = 0;

	vector<CodeTreeNode> wavefront = this->wavefront;
	vector<CodeTreeNode> new_wavefront;

	/* 数组 wavefront 中的每一个元素对应 spine_t 的一个节点 */
	vector<CodeTreeNode>::iterator iwave = wavefront.begin();

	/* 创建树的过程是一层一层的创建，使用 vector 添加或删除节点时，实现起来比较方便 */
	while(iwave != wavefront.end()){
		for(int edge=0; edge!=(1<<(this->k)); ++edge){//edge 也就是 mt，这个循环用来遍历可能出现的 k-bit m.
			CodeTreeNode new_node;

			/* 1. update spine_value */
			new_node.spine_value = hash_func(iwave->spine_value, edge);
			rng = RNG(new_node.spine_value);
			for(int i=0; i!=this->L; ++i){
				node_symbol = mapper.map_func(rng.next());
				distance = sym_spine[i] - node_symbol;
				edge_metric = distance * distance;
			}//end for

			/* 2.update path_metric */
			new_node.path_metric = iwave->path_metric + edge_metric;

			/* 3.update path */
			new_node.path = iwave->path;
			new_node.path.push_back(edge);

			/* Finaly, update wavefront */
			new_wavefront.push_back(new_node);

		}//end for
		++iwave;
	}//end while
	this->wavefront = new_wavefront;

	if(!wavefront.empty())
		wavefront.clear();

	if(!new_wavefront.empty())
		new_wavefront.clear();
}

//==================================
// Reduce CodeTree
//==================================
void SpineDecoder::prune_wavefront(){
	/* 从上面的 Update 后的 wavefront 中选出 B 个最优的节点
	简化（reduce） CodeTree */ 

	/* Get paramters */
	int k = this->k;
	int B = this->B;
	int d = this->d;
	vector<CodeTreeNode> wavefront = this->wavefront;

	/* Trees */
	size_t num_subtree_nodes = (1 << (k * (d - 1)));
	map<int, vector<CodeTreeNode>> subtrees;
	vector<CodeTreeNode> beam;
	vector<vector<CodeTreeNode>> prunTree;

	/* Itertors */
	vector<CodeTreeNode>::iterator iwave = wavefront.begin();
	vector<CodeTreeNode>::iterator ibeam = beam.begin();
	vector<vector<CodeTreeNode>>::iterator iprunTree;

	/* 1. Split nodes according to subtree.
	   subtrees is a map, first is min path_metric in the beam, 
	   second is beam -- vector of 2^(k(d-1)) nodes */
	while(!wavefront.empty()){
		vector<CodeTreeNode>::iterator ibegin = wavefront.begin();
		if(wavefront.size() > num_subtree_nodes){
			vector<CodeTreeNode>::iterator iend = wavefront.begin() + num_subtree_nodes;
			for(iwave = wavefront.begin(); 
				iwave != iend;
				++iwave){
				beam.push_back(*iwave);
			}//end for
			subtrees.insert(make_pair(minTree(beam), beam));
			wavefront.erase(ibegin, iend);
		}
		else{
			for(iwave = wavefront.begin(); 
				iwave != wavefront.end();
				++iwave){
				beam.push_back(*iwave);
			}//end for
			subtrees.insert(make_pair(minTree(beam), beam));
			wavefront.erase(ibegin, wavefront.end());
		}// end if
		if(!beam.empty())
			beam.clear();
	}//end while
	//cout << "Split nodes complete" << endl;

	/* 2. Reduce subtrees to sequensly prunTree 
		也就是把 subtrees 中的 value 部分移植到 vector 中，
		那么这一步的意义是利用 map 容器对 key 的排序功能*/
	map<int, vector<CodeTreeNode>>::iterator itree = subtrees.begin();
	while(itree != subtrees.end()){
		prunTree.push_back(itree->second);
		++itree;
	}// end while
	//cout << "Reduce subtrees complete" << endl;

	/* 3. Update wavefront with reduced subtrees.
		从排好序的 subtrees 中，读取前 B 个 subtree，
		但是首先要判断 prunTree 中是否有 B 棵树*/
	if(prunTree.size() >= size_t(B)){
		for(iprunTree = prunTree.begin();
			iprunTree != prunTree.begin()+B;
			++iprunTree){
			for(ibeam=iprunTree->begin(); 
				ibeam!=iprunTree->end(); 
				++ibeam){
				wavefront.push_back(*ibeam);  // update wavefront
			}// end for
		}//end for
	}else{
		for(iprunTree = prunTree.begin();
			iprunTree != prunTree.end();
			++iprunTree){
			for(ibeam = iprunTree->begin(); 
				ibeam != iprunTree->end(); 
				++ibeam){
				wavefront.push_back(*ibeam);  // update wavefront
			}// end for
		}//end for
	}//end if
	this->wavefront = wavefront;
	//cout << "Update wavefront complete" << endl;

	/* Release */
	if(!prunTree.empty())
		prunTree.clear();
	if(!subtrees.empty())
		subtrees.clear();
	if(!wavefront.empty())
		wavefront.clear();
}

/*	minTree: 
	返回一个深度为2的树的所有子节点中，path_metric 的最小值，
	并将其作为 key 与其对应的树存在 map 容器中	*/
inline int SpineDecoder::minTree(vector<CodeTreeNode> beam){
	vector<CodeTreeNode>::iterator beam_it = beam.begin();
	int min_metric = beam.begin()->path_metric;
	while(beam_it != beam.end()-1){
		if(beam_it->path_metric < min_metric)
			min_metric = beam_it->path_metric;
		++beam_it;
	}
	return min_metric;
}

//==================================
// Most likelihood processing
//==================================
vector<int> SpineDecoder::get_most_likely(){
	vector<CodeTreeNode> wavefront = this->wavefront;
	vector<CodeTreeNode>::iterator wave_it = wavefront.begin();
	
	int k = this->k;

	vector<int> message;
	vector<int> message_block;

	/* 1. Find the node with min_metric */
	int min_metric = wave_it->path_metric;
	CodeTreeNode the_best_node = *wave_it; 
	while(wave_it != wavefront.end()){
		if(wave_it->path_metric < min_metric){
			min_metric = wave_it->path_metric;
			the_best_node = *wave_it;
		}//end if
		++wave_it;
	}//end while

	vector<int>::iterator node_it = the_best_node.path.begin();
	int i = 0;

	/* 2. Get the message blocks */
	cout << "Decoded message block: " << endl;
	while(node_it != the_best_node.path.end()){
		message_block.push_back(*node_it);
		cout << *node_it << '\t';
		++i;
		++node_it;
	}
	cout << endl;

	/* 3. Get the message of 1 and 0 */
	cout << "Decoded message: " << endl;
	vector<int>::iterator im = message_block.begin();
	/*for(int j=0; j!=the_best_node.path.size(); ++j){
		message[j] = (message_block[j]>=4)?1:0;
		message[j+1] = ((message_block[j]-message[j]*4)>=2)?1:0;
		message[j+2] = message_block[j]-message[j]*4-message[j+1]*2;
		cout << message[j] << ' ' << message[j+1] << ' ' << message[j+2] << ' ';
	}*/
	while(im != message_block.end()){
		int a = ((*im)>=4)?1:0;
		int b = (((*im)-a*4)>=2)?1:0;
		int c =  (*im)-a*4-b*2;

		message.push_back(a);
		message.push_back(b);
		message.push_back(c);
		//cout << a << ' ' << b << ' ' << c << ' ';

		++im;
	}

	cout << endl;

	if(!message_block.empty())
		message_block.clear();

	return message;
}

//==================================
// Decoder
//==================================
vector<int> SpineDecoder::decoder(){
	vector<int> sym_spine(this->L); //L个针对同一个spine的symbol

	for(int i=0; i!=spineLen; ++i){//output Si into wavefront after each cycle
		for(int j=0; j!=L; ++j){
			sym_spine[j] = this->symbol[i+j*spineLen];
		}//end for

		//cout << "expand_wavefront..." << endl;
		expand_wavefront(sym_spine);

		//cout << "prune_wavefront..." << endl;
		if(this->wavefront.size() >= size_t(this->B))
			prune_wavefront();
	}//end for

	cout << "Decoding result: " << endl;
	vector<int> message = get_most_likely();

	/*cout << "对比" << endl;
	vector<int>::iterator im = message.begin();
	while(im != message.end()){
		cout << *im << ' ';
		++im;
	}
	cout << endl;*/

	if(!sym_spine.empty())
		sym_spine.clear();

	return message;
}



