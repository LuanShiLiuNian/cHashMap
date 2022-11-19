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

	long length;					// Ͱ�ܳ���
	long effective_length;			// ��Ч����
	long occupied_barrel_length;	// ռ��Ͱ����
};

struct cHashMap
{
	struct HashMap(*createHashMap)(int len);
	int (*hashMap_put)(struct HashMap* hashMap, char* key, char* value);
	char* (*hashMap_get)(struct HashMap hashMap, char* key);
	int (*hashMap_remove)(struct HashMap* hashMap, char* key);
};

// ȫ��ֻ��ʼ��һ�μ��� �����������Ϊȫ�ֱ�����HashMap���в���
void InitCHashMap(struct cHashMap* chashMap);