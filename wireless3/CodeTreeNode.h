//====================================================================
//CodeTreeNode.h: Interface of the CodeTree class.
//
//----Author: baobao7766
//----Time:   2015/03/23
//====================================================================
#ifndef _CODETREENODE_H_
#define _CODETREENODE_H_

#include <vector>

using namespace std;

class CodeTreeNode{
public:
	CodeTreeNode(){
		path_metric = 0;
		spine_value = 0;
	}
	~CodeTreeNode(){}

	int path_metric;
	uint32_t spine_value;
	vector<int> path;	// 默认为空，不需要初始化。

private:

};
#endif