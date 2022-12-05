//Array.c
/************************************************************************************************
���ϸ�Ī: Array.c
��    ��: �迭���̺귯�� �ҽ��ڵ�
�� �� ��: �ӴٴϿ�
�ۼ�����: 2022/05/13
*************************************************************************************************/
#include "Array.h"
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <memory.h>
/************************************************************************************************
��    Ī: Create
��    ��: �迭�� �����.
��    ��: �Ҵ緮
��    ��: ����
*************************************************************************************************/
void Array_Create(Array* array, Long capacity, size_t size) {
	array->front = calloc(capacity, size);
	array->capacity = capacity;
	array->length = 0;
}
/************************************************************************************************
��    Ī: Store
��    ��: �迭�� �׸��� �����Ѵ�.
��    ��: ��ġ, �׸�
��    ��: ��ġ
*************************************************************************************************/
Long Store(Array* array, Long index, void* object, size_t size) {
	memcpy(((char(*))array->front) + (index * size), object, size);
	array->length++;

	return index;
}
/************************************************************************************************
��    Ī: Insert
��    ��: �迭�� �׸��� �߰��Ѵ�.
��    ��: ��ġ, �׸�
��    ��: ��ġ
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
��    Ī: AppendFromFront
��    ��: �迭�� �Ǿտ� �׸��� �߰��Ѵ�.
��    ��: �׸�
��    ��:��ġ
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
��    Ī: AppendFromRear
��    ��: �迭�� �� �ڿ� �׸��� �߰��Ѵ�.
��    ��: �׸�
��    ��: ��ġ
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
��    Ī: Delete
��    ��: �迭���� �׸��� �����.
��    ��: ��ġ
��    ��: ��ġ
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
��    Ī: DeleteFromFront
��    ��: �迭�� ù �׸��� �����.
��    ��:
��    ��: ��ġ
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
��    Ī: DeleteFromRear
��    ��: �迭�� ������ �׸��� �����.
��    ��: ����
��    ��: ��ġ
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
��    Ī: Clear
��    ��: �迭�� ��� �׸��� �����.
��    ��:
��    ��:
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
��    Ī: Modify
��    ��: �迭�� �׸��� ��ģ��.
��    ��: ��ġ, �׸�
��    ��: ��ġ
*************************************************************************************************/
Long Modify(Array* array, Long index, void* object, size_t size) {
	memcpy(((char(*))array->front) + (index * size), object, size);

	return index;
}
/************************************************************************************************
��    Ī: LinearSearchUnique
��    ��: �ߺ��������� �����˻��� �Ѵ�.
��    ��: ����
��    ��: ��ġ
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
��    Ī: LinearSearchDuplicate
��    ��: �ߺ��������� �����˻��� �Ѵ�.
��    ��: ����
��    ��: ��ġ��, ����
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
��    Ī: BinarySearchUnique
��    ��: �ߺ��� ���� ���� �̺а˻��� �Ѵ�.
��    ��: ����
��    ��: ��ġ
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
��    Ī: BinarySearchDuplicate
��    ��: �ߺ��� ���� ���� �̺а˻��� �Ѵ�.
��    ��: ����
��    ��: ��ġ��, ����
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
��    Ī: SelectionSort
��    ��: �迭�� ������������ �����Ѵ�.
��    ��:
��    ��:
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
��    Ī: BubbleSort
��    ��: �迭�� ������������ �����Ѵ�.
��    ��:
��    ��:
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
��    Ī: InsertionSort
��    ��: ���������� �Ѵ�.
��    ��:
��    ��:
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
��    Ī: Merge
��    ��: �� �迭�� �����Ѵ�.
��    ��:
��    ��:
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
��    Ī: GetAt
��    ��: �迭���� �׸��� �о�´�.
��    ��: ��ġ
��    ��: �׸�
*************************************************************************************************/
void Array_GetAt(Array* array, Long index, void* object, size_t size) {
	memcpy(object, ((char(*))array->front) + (index * size), size);
}
/************************************************************************************************
��    Ī: Destroy
��    ��: �迭�� ���ش�.
��    ��: ����
��    ��: ����
*************************************************************************************************/
void Array_Destroy(Array* array) {
	if (array->front != NULL) {
		free(array->front);
	}
}