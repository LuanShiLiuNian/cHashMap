#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "cHashMap.h"


// ��������
char* hashMap_strdump(char* str);
unsigned int BKDRHash(char* str);
struct HashMap createHashMap(int len);
int hashMap_put(struct HashMap* hashMap, char* key, char* value);
char* hashMap_get(struct HashMap hashMap, char* key);
void hashMapExpansion(struct HashMap* hashMap);
void rePut(struct HashMap* hashMap, struct Node* hashBucket, int length);
void rePutSon(struct HashMap* hashMap, struct Node* node);
int hashMap_remove(struct HashMap* hashMap, char* key);
//

char* hashMap_strdump(char* str) {
	int strSize = strlen(str) + 1;
	char* strPtr = (char*)calloc(strSize, sizeof(char));
	if (strPtr == NULL)
	{
		return NULL;
	}
	strcpy(strPtr, str);
	return strPtr;
}

// hash�㷨
unsigned int BKDRHash(char* str)
{

	unsigned int seed = 131; // 31 131 1313 13131 131313 etc..
	unsigned int hash = 0;

	while (*str)
	{
		hash = hash * seed + (*str++);
	}

	return (hash & 0x7FFFFFFF);
}

struct HashMap createHashMap(int len) {		// hashͰ��С
	int hash_bucket_len = 16;
	if (len != NULL && len > 16)
	{
		hash_bucket_len = len;
	}

	// ��ʼ��СΪ16
	struct Node* node = (struct Node*)calloc(hash_bucket_len, sizeof(struct Node));
	struct HashMap hashMap = { node,hash_bucket_len,0,0 };
	return hashMap;
}

int hashMap_put(struct HashMap* hashMap, char* key, char* value) {

	key = hashMap_strdump(key);
	value = hashMap_strdump(value);

	//printf("%d,%d\n", hashMap->effective_length, (int)(hashMap->length * 0.75));

	if (hashMap->effective_length >= (int)(hashMap->length * 0.75))
	{
		//printf("\n");
		//printf("key=>%s,value=>%s", key, value);
		//printf("\n");
		//printf("\n");
		hashMapExpansion(hashMap);
		hashMap_put(hashMap, key, value);
		return 1;
	}

	struct Node* hashBucket = hashMap->hasahBucket;						// hashͰ
	long* effective_length = &hashMap->effective_length;				// ��Ч����
	long* occupied_barrel_length = &hashMap->occupied_barrel_length;	// ռ��Ͱ����
	long length = hashMap->length;

	int index = BKDRHash(key) % length;									// �����key���±�
	// �ƶ�ָ�뵽��Ӧ�±��λ��
	hashBucket += index;

	if (hashBucket->key == NULL) {
		hashBucket->key = key;
		hashBucket->value = value;

		// ռ��Ͱ����+1
		(*occupied_barrel_length)++;
		// ��Ч����+1
		(*effective_length)++;

		return 1;
	}
	else if (!strcmp(hashBucket->key, key))
	{
		// �����±�һ����key��ͬ
		hashBucket->value = value;
		return 1;
	}
	// ���ϴ�������hash��ͻ

	struct Node* node = hashBucket;			//ָ������ͷ��ָ��

	while (node->next != NULL)
	{
		if (!strcmp(node->key, key))
		{
			node->value = value;
			return 1;
		}
		node = node->next;
	}

	struct Node* link_node = (struct Node*)calloc(1, sizeof(struct Node));
	if (link_node == NULL)
	{
		return 0;
	}

	link_node->key = key;
	link_node->value = value;
	link_node->prev = node;
	link_node->next = NULL;

	node->next = link_node;
	(*effective_length)++;
	link_node = NULL;
	return 1;
}

char* hashMap_get(struct HashMap hashMap, char* key) {
	struct Node* hashBucket = hashMap.hasahBucket;	// hashͰ
	int index = BKDRHash(key) % hashMap.length;		// ���������Ͱ���±�

	// �ƶ�ָ�뵽�±�λ��
	hashBucket += index;

	if (hashBucket->key == NULL) {
		if (hashBucket->next == NULL)
			return NULL;
	}
	else
	{
		// ��ǰ�±�����Ƕ�Ӧ��key�򷵻ص�ǰvalue��ָ��
		if (!strcmp(hashBucket->key, key))
		{
			return hashBucket->value;
		}
	}



	// �����ǰ�±�kye����Ҫȡ��ֵ��key�������������

	// ͷ����
	struct Node* node = hashBucket;

	while (node->next != NULL)
	{
		node = node->next;
		if (node->key != NULL && !strcmp(node->key, key))
		{
			return node->value;
		}
	}

	return NULL;

}

void hashMapExpansion(struct HashMap* hashMap) {
	// ����֮ǰ�� hash Ͱ
	struct Node* oldHashBucket = hashMap->hasahBucket;

	int oldLength = hashMap->length;
	int size = hashMap->length * 2;

	struct Node* newHashBucket = (struct Node*)calloc(size, sizeof(struct Node));

	hashMap->hasahBucket = newHashBucket;
	hashMap->length = size;
	hashMap->effective_length = 0;
	hashMap->occupied_barrel_length = 0;

	rePut(hashMap, oldHashBucket, oldLength);
	free(oldHashBucket);
}

void rePut(struct HashMap* hashMap, struct Node* hashBucket, int length) {

	for (int i = 0; i < length; i++)
	{
		if (hashBucket->key != NULL)
		{
			//printf("key=%s,value=%s\n", hashBucket->key, hashBucket->value);
			hashMap_put(hashMap, hashBucket->key, hashBucket->value);
			if (hashBucket->next != NULL)
			{
				rePutSon(hashMap, hashBucket->next);
			}
		}

		hashBucket++;
	}


}

void rePutSon(struct HashMap* hashMap, struct Node* node) {

	struct Node* sonNode = node;
	struct Node* f = node;
	while (sonNode != NULL)
	{
		//printf("[][]key=%s,value=%s\n", sonNode->key, sonNode->value);
		hashMap_put(hashMap, sonNode->key, sonNode->value);
		sonNode = sonNode->next;
	}
	free(f);
}

int hashMap_remove(struct HashMap* hashMap, char* key) {
	struct Node* hashBucket = hashMap->hasahBucket;
	int index = BKDRHash(key) % hashMap->length;

	hashBucket += index;

	if (hashBucket->key == NULL) {
		if (hashBucket->next == NULL)
			return NULL;
	}
	else
	{
		// ��ǰ�±�����Ƕ�Ӧ��key�򷵻ص�ǰvalue��ָ��
		if (!strcmp(hashBucket->key, key))
		{
			char* tmpKey = hashBucket->key;
			char* tmpValue = hashBucket->value;

			hashBucket->key = NULL;
			hashBucket->value = NULL;

			return 1;
		}
	}

	// �����ǰ�±�kye����Ҫȡ��ֵ��key�������������

	// ͷ����
	struct Node* node = hashBucket;

	while (node->next != NULL)
	{
		node = node->next;
		if (!strcmp(node->key, key))
		{
			char* tmpKey = node->key;
			char* tmpValue = node->value;


			node->key = NULL;
			node->value = NULL;

			free(tmpKey);
			free(tmpValue);
			return 1;
		}
	}

	return NULL;
}

void InitCHashMap(struct cHashMap* chashMap) {
	chashMap->createHashMap = &createHashMap;
	chashMap->hashMap_get = &hashMap_get;
	chashMap->hashMap_put = &hashMap_put;
	chashMap->hashMap_remove = &hashMap_remove;
}