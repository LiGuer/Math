#ifndef HASH_TABLE_H
#define HASH_TABLE_H
#include <stdio.h>
#include <stdlib.h>
template<class T>
/***************************************************************************
*	HashTable 
*	[运算复杂度]:
		* Search: O(1)	* Insert: O(1)	* Delete: O(1)
****************************************************************************/
class HashTable {
	/*-------------------------------- 基础量 --------------------------------*/
	typedef long long int64;
	int64 MaxSize, deltaMemSize = 1e3, memSize = deltaMemSize, size = 0;
	int64* HashMem;
	T* ValueMem;
	int64 (*hashFunction)(T* x);
	/*-------------------------------- 基础函数 --------------------------------*/
	HashTable() { init(); }
	~HashTable() { delete HashMem; delete ValueMem;}
	void init(int64(*_hashFunction)(T* x), int64 _MaxSize) {
		hashFunction = _hashFunction;
		MaxSize = _MaxSize;
		HashMem = (int64*)malloc(sizeof(int64) * MaxSize);
		for (int i = 0; i < MaxSize; i++)HashMem[i] = -1;
		ValueMem = (T*)realloc(ValueMem, sizeof(T) * memSize);
	}

	int64 insert(T* x) {
		//[0] check exist
		int64 index = search(x);
		if (index != -1) return index;
		//[1] if full, Reallocate ValueMem if needed
		if (size + 1 >= memSize) {
			memSize += deltaMemSize;
			ValueMem = (T*)realloc(vocab, sizeof(T) * memSize);
		}
		//[2] set Hash, save x
		int64 hash = hashFunction(x);
		while (HashMem[hash] != -1) hash = (hash + 1) % MaxSize; //如果该位置已有，则向下,若至表底则回开头
		HashMem[hash] = size;
		ValueMem[size] = x;
		size++;
		return size - 1;
	}
	int64 search(T* x) {
		int64 hash = hashFunction(x);
		while (HashMem[hash] != -1 && ValueMem[HashMem[hash]] != x)
			hash = (hash + 1) % MaxSize;
		return HashMem[hash];
	}
	void erase(T* x) {
		int64 hash = hashFunction(x);
		while (HashMem[hash] != -1 && ValueMem[HashMem[hash]] != x)
			hash = (hash + 1) % MaxSize;
		if (HashMem[hash] == -1)return;
		int64 index = HashMem[hash];
		HashMem[hash] = -1;
		memset(ValueMem[index], 0, sizeof(T));
	}
};
#endif