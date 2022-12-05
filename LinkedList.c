//LinkedList.c
/************************************************************************************************
파일명칭: LinkedList.c
기    능: 연결리스트 라이브러리
작 성 자: 임다니엘
작성일자: 2022/06/14
*************************************************************************************************/
#include "LinkedList.h"
#include <stdlib.h>
#include <memory.h>

/************************************************************************************************
명    칭: Create
기    능: 연결리스트를 만든다.
입    력: 없음
출    력: 없음
*************************************************************************************************/
void Create(LinkedList* linkedList) {
	linkedList->head = NULL;
	linkedList->tail = NULL;
	linkedList->length = 0;
	linkedList->current = NULL;
}
/************************************************************************************************
명    칭: InsertBefore
기    능: 앞에서 삽입한다.
입    력: 위치, 항목
출    력: 위치
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
명    칭: InsertAfter
기    능: 뒤에서 삽입한다.
입    력: 위치, 항목
출    력: 위치
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
명    칭: AppendFromHead
기    능: 맨앞에 추가한다.
입    력: 항목
출    력: 위치
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
명    칭: AppendFromTail
기    능: 맨앞에 추가한다.
입    력: 항목
출    력: 위치
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
명    칭: Delete
기    능: 삭제한다.
입    력: 위치
출    력: 위치
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
명    칭: DeleteFromHead
기    능: 맨앞에서 삭제한다.
입    력: 없음
출    력: 위치
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
명    칭: DeleteFromTail
기    능: 맨뒤에서 삭제한다.
입    력: 없음
출    력: 위치
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
명    칭: DeleteAllItems
기    능: 모든 노드들을 삭제한다.
입    력: 없음
출    력: 없음
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
명    칭: LinearSearchUnique
기    능: 중복이 없을때의 선형검색을 한다.
입    력: 조건
출    력: 위치
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
명    칭: LinearSearchDuplicate
기    능: 중복이 있을때의 선형검색을 한다.
입    력: 조건
출    력: 위치들, 개수
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
명    칭: First
기    능: 처음으로 이동한다.
입    력: 없음
출    력: 위치
*************************************************************************************************/
Node* First(LinkedList* linkedList) {
	linkedList->current = linkedList->head;

	return linkedList->current;
}
/************************************************************************************************
명    칭: Previous
기    능: 이전으로 이동한다.
입    력: 없음
출    력: 위치
*************************************************************************************************/
Node* Previous(LinkedList* linkedList) {
	linkedList->current = linkedList->current->previous;

	return linkedList->current;
}
/************************************************************************************************
명    칭: Next
기    능: 다음으로 이동한다.
입    력: 없음
출    력: 위치
*************************************************************************************************/
Node* Next(LinkedList* linkedList) {
	linkedList->current = linkedList->current->next;

	return linkedList->current;
}
/************************************************************************************************
명    칭: Last
기    능: 마지막으로 이동한다.
입    력: 없음
출    력: 위치
*************************************************************************************************/
Node* Last(LinkedList* linkedList) {
	linkedList->current = linkedList->tail;

	return linkedList->current;
}
/************************************************************************************************
명    칭: Move
기    능: 입력받은 위치로 이동한다.
입    력: 위치
출    력: 위치
*************************************************************************************************/
Node* Move(LinkedList* linkedList, Node* index) {
	linkedList->current = index;

	return linkedList->current;
}
/************************************************************************************************
명    칭: GetAt
기    능: 항목을 읽어온다
입    력: 위치
출    력: 항목
*************************************************************************************************/
void GetAt(LinkedList* linkedList, Node* index, void* object, size_t size) {
	linkedList->current = index;
	memcpy(object, linkedList->current + 1, size);
}
/************************************************************************************************
명    칭: Destroy
기    능: 연결리스트를 없앤다.
입    력: 없음
출    력: 없음
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