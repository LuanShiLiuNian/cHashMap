#include <stdio.h>
#include "cHashMap.h"

int main() {
	struct cHashMap hashMapFunc;
	InitCHashMap(&hashMapFunc);

	struct HashMap hashMap = hashMapFunc.createHashMap(NULL);
	hashMapFunc.hashMap_put(&hashMap, "name", "xiaozhang");
	printf("%s", hashMapFunc.hashMap_get(hashMap,"name"));
}