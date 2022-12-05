//Array.c
/************************************************************************************************
파일명칭: Array.c
기    능: 배열라이브러리 소스코드
작 성 자: 임다니엘
작성일자: 2022/05/13
*************************************************************************************************/
#include "Array.h"
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <memory.h>
/************************************************************************************************
명    칭: Create
기    능: 배열을 만든다.
입    력: 할당량
출    력: 없음
*************************************************************************************************/
void Array_Create(Array* array, Long capacity, size_t size) {
	array->front = calloc(capacity, size);
	array->capacity = capacity;
	array->length = 0;
}
/************************************************************************************************
명    칭: Store
기    능: 배열에 항목을 저장한다.
입    력: 위치, 항목
출    력: 위치
*************************************************************************************************/
Long Store(Array* array, Long index, void* object, size_t size) {
	memcpy(((char(*))array->front) + (index * size), object, size);
	array->length++;

	return index;
}
/************************************************************************************************
명    칭: Insert
기    능: 배열에 항목을 추가한다.
입    력: 위치, 항목
출    력: 위치
*************************************************************************************************/
Long Array_Insert(Array* array, Long index, void* object, size_t size) {
	Long i = 0;
	void(*temp) = NULL;

	temp = calloc(array->capacity + 1, size);
	while (i < index) {
		memcpy(((char(*))temp) + (i * size), ((char(*))array->front) + (i * size), size);
		i++;
	}
	while (i < array->length) {
		memcpy(((char(*))temp) + ((i + 1) * size), ((char(*))array->front) + (i * size), size);
		i++;
	}
	if (array->front != NULL) {
		free(array->front);
	}
	array->front = temp;
	array->capacity++;
	memcpy(((char(*))array->front) + (index * size), object, size);
	array->length++;

	return index;
}
/************************************************************************************************
명    칭: AppendFromFront
기    능: 배열의 맨앞에 항목을 추가한다.
입    력: 항목
출    력:위치
*************************************************************************************************/
Long AppendFromFront(Array* array, void* object, size_t size) {
	Long index = 0;
	Long i = 0;
	void(*temp) = NULL;

	temp = calloc(array->capacity + 1, size);
	while (i < array->length) {
		memcpy(((char(*))temp) + ((i + 1)* size), ((char(*))array->front) + (i * size), size);
		i++;
	}
	if (array->front != NULL) {
		free(array->front);
	}
	array->front = temp;
	array->capacity++;
	memcpy(((char(*))array->front) + (index * size), object, size);
	array->length++;

	return index;
}
/************************************************************************************************
명    칭: AppendFromRear
기    능: 배열의 맨 뒤에 항목을 추가한다.
입    력: 항목
출    력: 위치
*************************************************************************************************/
Long AppendFromRear(Array* array, void* object, size_t size) {
	Long index;
	Long i = 0;
	void(*temp) = NULL;

	temp = calloc(array->capacity + 1, size);
	while (i < array->length) {
		memcpy(((char(*))temp) + (i * size), ((char(*))array->front) + (i * size), size);
		i++;
	}
	if (array->front != NULL) {
		free(array->front);
	}
	array->front = temp;
	array->capacity++;
	index = array->capacity - 1;
	memcpy(((char(*))array->front) + (index * size), object, size);
	array->length++;

	return index;
}
/************************************************************************************************
명    칭: Delete
기    능: 배열에서 항목을 지운다.
입    력: 위치
출    력: 위치
*************************************************************************************************/
Long Array_Delete(Array* array, Long index, size_t size) {
	Long i = 0;
	void(*temp) = NULL;

	if (array->capacity > 1) {
		temp = calloc(array->capacity - 1, size);
	}
	while (i < index) {
		memcpy(((char(*))temp) + (i * size), ((char(*))array->front) + (i * size), size);
		i++;
	}
	i++;
	while (i < array->length) {
		memcpy(((char(*))temp) + ((i - 1) * size), ((char(*))array->front) + (i * size), size);
		i++;
	}
	if (array->front != NULL) {
		free(array->front);
		array->front = NULL;
	}
	if (array->capacity > 1) {
		array->front = temp;
	}
	array->length--;
	array->capacity--;
	index = -1;

	return index;
}
/************************************************************************************************
명    칭: DeleteFromFront
기    능: 배열의 첫 항목을 지운다.
입    력:
출    력: 위치
*************************************************************************************************/
Long DeleteFromFront(Array* array, size_t size) {
	Long index;
	Long i = 1;
	void(*temp) = NULL;

	if (array->capacity > 1) {
		temp = calloc(array->capacity - 1, size);
	}
	while (i < array->length) {
		memcpy(((char(*))temp) + ((i - 1) * size), ((char(*))array->front) + (i * size), size);
		i++;
	}
	if (array->front != NULL) {
		free(array->front);
		array->front = NULL;
	}
	if (array->capacity > 1) {
		array->front = temp;
	}
	array->length--;
	array->capacity--;
	index = -1;

	return index;
}
/************************************************************************************************
명    칭: DeleteFromRear
기    능: 배열의 마지막 항목을 지운다.
입    력: 없음
출    력: 위치
*************************************************************************************************/
Long DeleteFromRear(Array* array, size_t size) {
	Long index;
	Long i = 0;
	void(*temp) = NULL;

	if (array->capacity > 1) {
		temp = calloc(array->capacity - 1, size);
	}
	while (i < array->capacity - 1) {
		memcpy(((char(*))temp) + (i * size), ((char(*))array->front) + (i * size), size);
		i++;
	}
	if (array->front != NULL) {
		free(array->front);
		array->front = NULL;
	}
	if (array->capacity > 1) {
		array->front = temp;
	}
	array->length--;
	array->capacity--;
	index = -1;

	return index;
}
/************************************************************************************************
명    칭: Clear
기    능: 배열의 모든 항목을 지운다.
입    력:
출    력:
*************************************************************************************************/
void Clear(Array* array) {
	if (array->front != NULL) {
		free(array->front);
		array->front = NULL;
	}
	array->length = 0;
	array->capacity = 0;
}
/************************************************************************************************
명    칭: Modify
기    능: 배열의 항목을 고친다.
입    력: 위치, 항목
출    력: 위치
*************************************************************************************************/
Long Modify(Array* array, Long index, void* object, size_t size) {
	memcpy(((char(*))array->front) + (index * size), object, size);

	return index;
}
/************************************************************************************************
명    칭: LinearSearchUnique
기    능: 중복없을때의 선형검색을 한다.
입    력: 조건
출    력: 위치
*************************************************************************************************/
Long Array_LinearSearchUnique(Array* array, void* key, size_t size, int (*compare)(void*, void*)) {
	Long index = -1;
	Long i = 0;

	while (i < array->length && compare(((char(*))array->front) + (i * size), key) != 0) {
		i++;
	}
	if (i < array->length) {
		index = i;
	}

	return index;
}
/************************************************************************************************
명    칭: LinearSearchDuplicate
기    능: 중복있을때의 선형검색을 한다.
입    력: 조건
출    력: 위치들, 개수
*************************************************************************************************/
void Array_LinearSearchDuplicate(Array* array, void* key, Long* (*indexes), Long* count, size_t size, int (*compare)(void*, void*)) {
	Long i = 0;
	Long j = 0;

	*count = 0;
	*indexes = (Long(*))calloc(array->length, sizeof(Long));
	while (i < array->length) {
		if(compare(((char(*))array->front) + (i * size), key) == 0){
			(*indexes)[j] = i;
			(*count)++;
			j++;
		}
		i++;
	}
}
/************************************************************************************************
명    칭: BinarySearchUnique
기    능: 중복이 없을 때의 이분검색을 한다.
입    력: 조건
출    력: 위치
*************************************************************************************************/
Long BinarySearchUnique(Array* array, void* key, size_t size, int (*compare)(void*, void*)) {
	Long index = -1;
	Long low = 0;
	Long high = array->length - 1;
	Long middle = (low + high) / 2;

	while (low <= high && compare(((char(*))array->front) + (middle * size), key) != 0) {
		if (compare(((char(*))array->front) + (middle * size), key) < 0) {
			low = middle + 1;
		}
		else {
			high = middle - 1;
		}
		middle = (low + high) / 2;
	}
	if (low <= high) {
		index = middle;
	}
	return index;
}
/************************************************************************************************
명    칭: BinarySearchDuplicate
기    능: 중복이 있을 때의 이분검색을 한다.
입    력: 조건
출    력: 위치들, 개수
*************************************************************************************************/
void BinarySearchDuplicate(Array* array, void* key, Long* (*indexes), Long* count, size_t size, int (*compare)(void*, void*)) {
	Long low = 0;
	Long high = array->length - 1;
	Long middle = (low + high) / 2;
	Long i;
	Long j = 0;

	*count = 0;
	*indexes = (Long(*))calloc(array->length, sizeof(Long));
	while (low <= high && compare(((char(*))array->front) + (middle * size), key) != 0) {
		if (compare(((char(*))array->front) + (middle * size), key) < 0) {
			low = middle + 1;
		}
		else {
			high = middle - 1;
		}
		middle = (low + high) / 2;
	}
	i = middle - 1;
	while (i >= low && compare(((char(*))array->front) + (i * size), key) == 0) {
		i--;
	}
	i++;
	while (i <= high && compare(((char(*))array->front) + (i * size), key) == 0) {
		(*indexes)[j] = i;
		(*count)++;
		j++;
		i++;
	}

}
/************************************************************************************************
명    칭: SelectionSort
기    능: 배열을 오름차순으로 정렬한다.
입    력:
출    력:
*************************************************************************************************/
void SelectionSort(Array* array, size_t size, int (*compare)(void*, void*)) {
	void* temp = NULL;
	Long i = 0;
	Long j;
	Long smallest;

	temp = malloc(size);
	while (i < array->length - 1) {
		smallest = i;
		j = i + 1;
		while (j < array->length) {
			if (compare(((char(*))array->front) + (smallest * size), ((char(*))array->front) + (j * size)) > 0) {
				smallest = j;
			}
			j++;
		}
		memcpy(temp, ((char(*))array->front) + (smallest * size), size);
		memcpy(((char(*))array->front) + (smallest * size), ((char(*))array->front) + (i * size), size);
		memcpy(((char(*))array->front) + (i * size), temp, size);
		i++;
	}
	if (temp != NULL) {
		free(temp);
	}
}
/************************************************************************************************
명    칭: BubbleSort
기    능: 배열을 오름차순으로 정렬한다.
입    력:
출    력:
*************************************************************************************************/
void BubbleSort(Array* array, size_t size, int (*compare)(void*, void*)) {
	void* temp = NULL;
	Long i = array->length - 2;
	Long j;
	Long count = 1;

	temp = malloc(size);
	while (i >= 0 && count > 0) {
		count = 0;
		j = 0;
		while (j <= i) {
			if (compare(((char(*))array->front) + (j * size), ((char(*))array->front) + ((j + 1) * size)) > 0) {
				memcpy(temp, ((char(*))array->front) + (j * size), size);
				memcpy(((char(*))array->front) + (j * size), ((char(*))array->front) + ((j + 1) * size), size);
				memcpy(((char(*))array->front) + ((j + 1) * size), temp, size);
				count++;
			}
			j++;
		}
		i--;
	}
	if (temp != NULL) {
		free(temp);
	}
}
/************************************************************************************************
명    칭: InsertionSort
기    능: 삽입정렬을 한다.
입    력:
출    력:
*************************************************************************************************/
void InsertionSort(Array* array, size_t size, int (*compare)(void*, void*)) {
	Long i = 1;
	Long j;
	void* temp = NULL;

	temp = malloc(size);
	while (i < array->length) {
		memcpy(temp, ((char(*))array->front) + (i * size), size);
		j = i - 1;
		while (j >= 0 && compare(((char(*))array->front) + (j * size), temp) > 0) {
			memcpy(((char(*))array->front) + ((j + 1) * size), ((char(*))array->front) + (j * size), size);
			j--;
		}
		memcpy(((char(*))array->front) + ((j + 1) * size), temp, size);
		i++;
	}
	if (temp != NULL) {
		free(temp);
	}
}
/************************************************************************************************
명    칭: Merge
기    능: 두 배열을 병합한다.
입    력:
출    력:
*************************************************************************************************/
void Merge(Array* array, Array* one, Array* other, size_t size, int (*compare)(void*, void*)) {
	Long i = 0;
	Long j = 0;
	Long k = 0;

	if( array->front != NULL){
		free(array->front);
	}
	array->front = calloc(one->length + other->length, size);
	array->capacity = one->length + other->length;
	while (i < one->length && j < other->length) {
		if (compare(((char(*))one->front) + (i * size), ((char(*))other->front) + (j * size)) <= 0) {
			memcpy(((char(*))array->front) + (k * size), ((char(*))one->front) + (i * size), size);
			k++;
			i++;
		}
		else {
			memcpy(((char(*))array->front) + (k * size), ((char(*))other->front) + (j * size), size);
			k++;
			j++;
		}
	}
	while (i < one->length) {
		memcpy(((char(*))array->front) + (k * size), ((char(*))one->front) + (i * size), size);
		k++;
		i++;
	}
	while (j < other->length) {
		memcpy(((char(*))array->front) + (k * size), ((char(*))other->front) + (j * size), size);
		k++;
		j++;
	}
	array->length = one->length + other->length;
}
/************************************************************************************************
명    칭: GetAt
기    능: 배열에서 항목을 읽어온다.
입    력: 위치
출    력: 항목
*************************************************************************************************/
void Array_GetAt(Array* array, Long index, void* object, size_t size) {
	memcpy(object, ((char(*))array->front) + (index * size), size);
}
/************************************************************************************************
명    칭: Destroy
기    능: 배열을 없앤다.
입    력: 없음
출    력: 없음
*************************************************************************************************/
void Array_Destroy(Array* array) {
	if (array->front != NULL) {
		free(array->front);
	}
}