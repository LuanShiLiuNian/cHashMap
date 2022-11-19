#pragma once
struct Node
{
	char* key;
	char* value;

	struct Node* prev;
	struct Node* next;
};

struct HashMap {
	struct Node* hasahBucket;

	long length;					// 桶总长度
	long effective_length;			// 有效长度
	long occupied_barrel_length;	// 占用桶长度
};

struct cHashMap
{
	struct HashMap(*createHashMap)(int len);
	int (*hashMap_put)(struct HashMap* hashMap, char* key, char* value);
	char* (*hashMap_get)(struct HashMap hashMap, char* key);
	int (*hashMap_remove)(struct HashMap* hashMap, char* key);
};

// 全局只初始化一次即可 传入变量可作为全局变量对HashMap进行操作
void InitCHashMap(struct cHashMap* chashMap);