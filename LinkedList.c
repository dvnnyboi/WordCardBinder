//LinkedList.c
/************************************************************************************************
���ϸ�Ī: LinkedList.c
��    ��: ���Ḯ��Ʈ ���̺귯��
�� �� ��: �ӴٴϿ�
�ۼ�����: 2022/06/14
*************************************************************************************************/
#include "LinkedList.h"
#include <stdlib.h>
#include <memory.h>

/************************************************************************************************
��    Ī: Create
��    ��: ���Ḯ��Ʈ�� �����.
��    ��: ����
��    ��: ����
*************************************************************************************************/
void Create(LinkedList* linkedList) {
	linkedList->head = NULL;
	linkedList->tail = NULL;
	linkedList->length = 0;
	linkedList->current = NULL;
}
/************************************************************************************************
��    Ī: InsertBefore
��    ��: �տ��� �����Ѵ�.
��    ��: ��ġ, �׸�
��    ��: ��ġ
*************************************************************************************************/
Node* InsertBefore(LinkedList* linkedList, Node* index, void* object, size_t size) {
	linkedList->current = (Node*)malloc(sizeof(Node) + size);
	memcpy(linkedList->current + 1, object, size);
	linkedList->current->next = index;
	if (linkedList->head != index) {
		linkedList->current->previous = index->previous;
		index->previous->next = linkedList->current;
	}
	else {
		linkedList->current->previous = linkedList->current;
		linkedList->head = linkedList->current;
	}
	index->previous = linkedList->current;
	linkedList->length++;

	return linkedList->current;
}
/************************************************************************************************
��    Ī: InsertAfter
��    ��: �ڿ��� �����Ѵ�.
��    ��: ��ġ, �׸�
��    ��: ��ġ
*************************************************************************************************/
Node* InsertAfter(LinkedList* linkedList, Node* index, void* object, size_t size) {
	linkedList->current = (Node*)malloc(sizeof(Node) + size);
	memcpy(linkedList->current + 1, object, size);
	linkedList->current->previous = index;
	if (linkedList->tail != index) {
		linkedList->current->next = index->next;
		index->next->previous = linkedList->current;
	}
	else {
		linkedList->current->next = linkedList->current;
		linkedList->tail = linkedList->current;
	}
	index->next = linkedList->current;
	linkedList->length++;

	return linkedList->current;
}
/************************************************************************************************
��    Ī: AppendFromHead
��    ��: �Ǿտ� �߰��Ѵ�.
��    ��: �׸�
��    ��: ��ġ
*************************************************************************************************/
Node* AppendFromHead(LinkedList* linkedList, void* object, size_t size) {
	linkedList->current = (Node*)malloc(sizeof(Node) + size);
	memcpy(linkedList->current + 1, object, size);
	linkedList->current->previous = linkedList->current;
	if (linkedList->head != NULL) {
		linkedList->current->next = linkedList->head;
		linkedList->head->previous = linkedList->current;
	}
	else {
		linkedList->current->next = linkedList->current;
		linkedList->tail = linkedList->current;
	}
	linkedList->head = linkedList->current;
	linkedList->length++;

	return linkedList->current;
}
/************************************************************************************************
��    Ī: AppendFromTail
��    ��: �Ǿտ� �߰��Ѵ�.
��    ��: �׸�
��    ��: ��ġ
*************************************************************************************************/
Node* AppendFromTail(LinkedList* linkedList, void* object, size_t size) {
	linkedList->current = (Node*)malloc(sizeof(Node) + size);
	memcpy(linkedList->current + 1, object, size);
	linkedList->current->next = linkedList->current;
	if (linkedList->tail != NULL) {
		linkedList->current->previous = linkedList->tail;
		linkedList->tail->next = linkedList->current;
	}
	else {
		linkedList->current->previous = linkedList->current;
		linkedList->head = linkedList->current;
	}
	linkedList->tail = linkedList->current;
	linkedList->length++;

	return linkedList->current;
}
/************************************************************************************************
��    Ī: Delete
��    ��: �����Ѵ�.
��    ��: ��ġ
��    ��: ��ġ
*************************************************************************************************/
Node* Delete(LinkedList* linkedList, Node* index) {
	if (index != index->previous && index != index->next) {
		index->previous->next = index->next;
		index->next->previous = index->previous;
		linkedList->current = index->previous;
	}
	else if (index == index->previous && index != index->next) {
		index->next->previous = index->next;
		linkedList->head = index->next;
		linkedList->current = index->next;
	}
	else if (index != index->previous && index == index->next) {
		index->previous->next = index->previous;
		linkedList->tail = index->previous;
		linkedList->current = index->previous;
	}
	else {
		linkedList->head = NULL;
		linkedList->tail = NULL;
		linkedList->current = NULL;
	}
	if (index != NULL) {
		free(index);
		index = NULL;
	}
	linkedList->length--;

	return index;
}
/************************************************************************************************
��    Ī: DeleteFromHead
��    ��: �Ǿտ��� �����Ѵ�.
��    ��: ����
��    ��: ��ġ
*************************************************************************************************/
Node* DeleteFromHead(LinkedList* linkedList) {
	Node* index;
	
	index = linkedList->head;
	if (linkedList->head != linkedList->tail) {
		linkedList->head = index->next;
		linkedList->head->previous = linkedList->head;
		linkedList->current = linkedList->head;
	}
	else {
		linkedList->head = NULL;
		linkedList->tail = NULL;
		linkedList->current = NULL;
	}
	if (index != NULL) {
		free(index);
		index = NULL;
	}
	linkedList->length--;

	return index;
}
/************************************************************************************************
��    Ī: DeleteFromTail
��    ��: �ǵڿ��� �����Ѵ�.
��    ��: ����
��    ��: ��ġ
*************************************************************************************************/
Node* DeleteFromTail(LinkedList* linkedList) {
	Node* index;

	index = linkedList->tail;
	if (linkedList->head != linkedList->tail) {
		linkedList->tail = index->previous;
		linkedList->tail->next = linkedList->tail;
		linkedList->current = linkedList->tail;
	}
	else {
		linkedList->head = NULL;
		linkedList->tail = NULL;
		linkedList->current = NULL;
	}
	if (index != NULL) {
		free(index);
		index = NULL;
	}
	linkedList->length--;

	return index;
}
/************************************************************************************************
��    Ī: DeleteAllItems
��    ��: ��� ������ �����Ѵ�.
��    ��: ����
��    ��: ����
*************************************************************************************************/
void DeleteAllItems(LinkedList* linkedList) {
	Node* previous = NULL;

	linkedList->current = linkedList->head;
	while (linkedList->current != previous) {
		linkedList->head = linkedList->current->next;
		previous = linkedList->current;
		if (linkedList->current != NULL) {
			free(linkedList->current);
		}
		linkedList->current = linkedList->head;
	}
	linkedList->head = NULL;
	linkedList->tail = NULL;
	linkedList->length--;
	linkedList->current = NULL;
}
/************************************************************************************************
��    Ī: LinearSearchUnique
��    ��: �ߺ��� �������� �����˻��� �Ѵ�.
��    ��: ����
��    ��: ��ġ
*************************************************************************************************/
Node* LinearSearchUnique(LinkedList* linkedList, void* key, int(*compare)(void*, void*)) {
	Node* index = NULL;
	Node* it;
	Node* previous = NULL;

	it = linkedList->head;
	while (it != previous && compare(it + 1, key) != 0) {
		previous = it;
		it = it->next;
	}
	if (it != previous) {
		index = it;
	}
	return index;
}
/************************************************************************************************
��    Ī: LinearSearchDuplicate
��    ��: �ߺ��� �������� �����˻��� �Ѵ�.
��    ��: ����
��    ��: ��ġ��, ����
*************************************************************************************************/
void LinearSearchDuplicate(LinkedList* linkedList, void* key, Node** (*indexes), Long* count, int(*compare)(void*, void*)) {
	Node* it;
	Node* previous = NULL;
	Long i = 0;
	
	*count = 0;
	*indexes = (Node * (*))calloc(linkedList->length, sizeof(Node*));
	it = linkedList->head;
	while (it != previous) {
		if (compare(it + 1, key) == 0) {
			(*indexes)[i] = it;
			(*count)++;
			i++;
		}
		previous = it;
		it = it->next;
	}
}
/************************************************************************************************
��    Ī: First
��    ��: ó������ �̵��Ѵ�.
��    ��: ����
��    ��: ��ġ
*************************************************************************************************/
Node* First(LinkedList* linkedList) {
	linkedList->current = linkedList->head;

	return linkedList->current;
}
/************************************************************************************************
��    Ī: Previous
��    ��: �������� �̵��Ѵ�.
��    ��: ����
��    ��: ��ġ
*************************************************************************************************/
Node* Previous(LinkedList* linkedList) {
	linkedList->current = linkedList->current->previous;

	return linkedList->current;
}
/************************************************************************************************
��    Ī: Next
��    ��: �������� �̵��Ѵ�.
��    ��: ����
��    ��: ��ġ
*************************************************************************************************/
Node* Next(LinkedList* linkedList) {
	linkedList->current = linkedList->current->next;

	return linkedList->current;
}
/************************************************************************************************
��    Ī: Last
��    ��: ���������� �̵��Ѵ�.
��    ��: ����
��    ��: ��ġ
*************************************************************************************************/
Node* Last(LinkedList* linkedList) {
	linkedList->current = linkedList->tail;

	return linkedList->current;
}
/************************************************************************************************
��    Ī: Move
��    ��: �Է¹��� ��ġ�� �̵��Ѵ�.
��    ��: ��ġ
��    ��: ��ġ
*************************************************************************************************/
Node* Move(LinkedList* linkedList, Node* index) {
	linkedList->current = index;

	return linkedList->current;
}
/************************************************************************************************
��    Ī: GetAt
��    ��: �׸��� �о�´�
��    ��: ��ġ
��    ��: �׸�
*************************************************************************************************/
void GetAt(LinkedList* linkedList, Node* index, void* object, size_t size) {
	linkedList->current = index;
	memcpy(object, linkedList->current + 1, size);
}
/************************************************************************************************
��    Ī: Destroy
��    ��: ���Ḯ��Ʈ�� ���ش�.
��    ��: ����
��    ��: ����
*************************************************************************************************/
void Destroy(LinkedList* linkedList) {
	Node* previous = NULL;

	linkedList->current = linkedList->head;
	while (linkedList->current != previous) {
		linkedList->head = linkedList->current->next;
		previous = linkedList->current;
		if (linkedList->current != NULL) {
			free(linkedList->current);
		}
		linkedList->current = linkedList->head;
	}
}