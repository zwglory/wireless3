//====================================================================
//Hash_func.h: Interface of the Hash_func class.
//
//----Author: baobao7766
//----Time:   2015/02/19
//====================================================================

#ifndef _HASH_FUNC_H_
#define _HASH_FUNC_H_

#include <itpp/itcomm.h>

uint32_t hash_func(uint32_t spine_value, int message_block);
uint32_t rot(uint32_t x, int k);
#endif
















