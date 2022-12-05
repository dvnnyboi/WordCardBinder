//BinarySearchTree.c
/************************************************************************************************
파일명칭: BinarySearchTree.c
기    능: 이분검색트리 라이브러리.
작 성 자: 임다니엘
작성일자: 2022/06/27
*************************************************************************************************/
#include "BinarySearchTree.h"
#include <stdlib.h>
#include <memory.h>

/************************************************************************************************
명    칭: Create
기    능: 이분검색트리를 만든다.
입    력: 없음
출    력: 없음
*************************************************************************************************/
void BinaryTree_Create(BinaryTree* binaryTree) {
	binaryTree->root = NULL;
	binaryTree->length = 0;
	binaryTree->balance = 0;
}
/************************************************************************************************
명    칭: Insert
기    능: 이분검색트리에 항목을 삽입한다.
입    력: 조건
출    력: 위치
*************************************************************************************************/
BinaryNode* Insert(BinaryTree* binaryTree, void* key, size_t size, int (*compare)(void*, void*)) {
	BinaryNode* index;
	BinaryNode* it;
	BinaryNode* parent = NULL;

	it = binaryTree->root;
	while (it != NULL) {
		parent = it;
		if (compare(it + 1, key) > 0) {
			it = it->left;
		}
		else {
			it = it->right;
		}
	}
	index = (BinaryNode*)malloc(sizeof(BinaryNode) + size);
	index->left = NULL;
	memcpy(index + 1, key, size);
	index->right = NULL;
	if (parent != NULL) {
		if (compare(parent + 1, key) > 0) {
			parent->left = index;
		}
		else {
			parent->right = index;
		}
		if (compare(binaryTree->root + 1, key) > 0) {
			binaryTree->balance--;
		}
		else {
			binaryTree->balance++;
		}
	}
	else {
		binaryTree->root = index;
	}
	binaryTree->length++;

	return index;
}
/************************************************************************************************
명    칭: Search
기    능: 이분검색트리에서 찾는다.
입    력: 조건
출    력: 위치
*************************************************************************************************/
BinaryNode* Search(BinaryTree* binaryTree, void* key, int (*compare)(void*, void*)) {
	BinaryNode* it;
	
	it = binaryTree->root;
	while (it != NULL && compare(it + 1, key) != 0) {
		if (compare(it + 1, key) > 0) {
			it = it->left;
		}
		else {
			it = it->right;
		}
	}
	return it;
}
/************************************************************************************************
명    칭: Delete
기    능: 이분검색트리에서 지운다.
입    력: 조건
출    력: 위치
*************************************************************************************************/
BinaryNode* BinaryTree_Delete(BinaryTree* binaryTree, void* key, size_t size, int (*compare)(void*, void*)) {
	BinaryNode* index;
	BinaryNode* parent = NULL;
	BinaryNode* it = NULL;
	BinaryNode* previous;

	index = binaryTree->root;
	while (compare(index + 1 , key) != 0) {
		parent = index;
		if (compare(index + 1, key) > 0) {
			index = index->left;
		}
		else {
			index = index->right;
		}
	}
	previous = index;
	it = index->right;
	while (it != NULL && it->left != NULL) {
		previous = it;
		it = it->left;
	}
	if (it == NULL && index->left != NULL) {
		it = index->left;
		while (it->right != NULL) {
			previous = it;
			it = it->right;
		}
	}
	if (previous != index && it != NULL) {
		if (compare(previous + 1, it + 1) > 0) {
			previous->left = it->right;
		}
		else {
			previous->right = it->left;
		}
	}
	if (index->left != it) {
		it->left = index->left;
	}
	if (index->right != it) {
		it->right = index->right;
	}
	if (parent != NULL) {
		if (compare(parent + 1, key) < 0) {
			parent->right = it;
		}
		else {
			parent->left = it;
		}
		if (compare(binaryTree->root + 1, key) < 0) {
			binaryTree->balance--;
		}
		else {
			binaryTree->balance++;
		}
	}
	if (parent == NULL) {
		binaryTree->root = it;
		if (index->right != NULL) {
			binaryTree->balance--;
		}
		else if (index->left != NULL) {
			binaryTree->balance++;
		}
	}

	if (index != NULL) {
		free(index);
		index = NULL;
	}
	binaryTree->length--;

	return index;
}
/************************************************************************************************
명    칭: MakeBalance
기    능: 이분검색트리의 균형을 맞춘다.
입    력: 없음
출    력: 없음
*************************************************************************************************/
void MakeBalance(BinaryTree* binaryTree, size_t size) {
	void(*keys);
	Long count;

	MakeKeys(binaryTree, &keys, &count, size);
	BinaryTree_DeleteAllItems(binaryTree);
	MakeTree(binaryTree, keys, count, size);

	if (keys != NULL) {
		free(keys);
	}
}
/************************************************************************************************
명    칭: MakeKeys
기    능: 조건들을 배열에 옮겨적는다.
입    력: 없음
출    력: 조건들, 개수
*************************************************************************************************/
void MakeKeys(BinaryTree* binaryTree, void* (*keys), Long* count, size_t size) {
	Long index = 0;

	*keys = calloc(binaryTree->length, size);
	*count = 0;

	Node_MakeKeys(binaryTree->root, &index, *keys, count, size);
}
/************************************************************************************************
명    칭: Node_MakeKeys
기    능: 조건들을 배열에 옮겨적는다.
입    력: 위치
출    력: 조건들, 개수
*************************************************************************************************/
void Node_MakeKeys(BinaryNode* node, Long* index, void(*keys), Long* count, size_t size) {
	if (node != NULL) {
		Node_MakeKeys(node->left, index, keys, count, size);
		memcpy(((char(*))keys) + ((*index) * size), node + 1, size);
		(*count)++;
		(*index)++;
		Node_MakeKeys(node->right, index, keys, count, size);
	}
}
/************************************************************************************************
명    칭: BinaryTree_DeleteAllItems
기    능: 모든 항목을 지운다.
입    력: 없음
출    력: 없음
*************************************************************************************************/
void BinaryTree_DeleteAllItems(BinaryTree* binaryTree) {
	Node_DeleteAllItems(binaryTree->root);
	binaryTree->root = NULL;
	binaryTree->length = 0;
	binaryTree->balance = 0;
}
/************************************************************************************************
명    칭: Node_DeleteAllItems
기    능: 모든 항목을 지운다.
입    력: 없음
출    력: 없음
*************************************************************************************************/
void Node_DeleteAllItems(BinaryNode* node) {
	if (node != NULL) {
		Node_DeleteAllItems(node->left);
		Node_DeleteAllItems(node->right);
		free(node);
	}
}
/************************************************************************************************
명    칭: MakeTree
기    능: 트리를 만든다.
입    력: 조건들, 개수
출    력: 없음
*************************************************************************************************/
void MakeTree(BinaryTree* binaryTree, void(*keys), Long count, size_t size) {
	Long remainder;

	binaryTree->root = Node_MakeTree(keys, 0, count - 1, size);
	binaryTree->length = count;
	remainder = count % 2;
	if (remainder == 0) {
		binaryTree->balance = -1;
	}
	else {
		binaryTree->balance = 0;
	}
}
/************************************************************************************************
명    칭: Node_MakeTree
기    능: 트리를 만든다.
입    력: 조건들, 하한선, 상한선
출    력: 위치
*************************************************************************************************/
BinaryNode* Node_MakeTree(void(*keys), Long low, Long high, size_t size) {
	BinaryNode* node = NULL;
	Long middle;

	if (low <= high) {
		middle = (low + high + 1) / 2;
		node = (BinaryNode*)malloc(sizeof(BinaryNode) + size);
		memcpy(node + 1, ((char(*))keys) + (middle * size), size);
		node->left = Node_MakeTree(keys, low, middle - 1, size);
		node->right = Node_MakeTree(keys, middle + 1, high, size);
	}
	return node;
}
/************************************************************************************************
명    칭: GetAt
기    능: 이분검색트리에서 읽어온다.
입    력: 위치
출    력: 조건
*************************************************************************************************/
void BinaryTree_GetAt(BinaryTree* binaryTree, BinaryNode* index, void* key, size_t size) {
	memcpy(key, index + 1, size);
}
/************************************************************************************************
명    칭: Destroy
기    능: 이분검색트리를 없앤다.
입    력: 없음
출    력: 없음
*************************************************************************************************/
void BinaryTree_Destroy(BinaryTree* binaryTree) {
	Node_Destroy(binaryTree->root);
}
/************************************************************************************************
명    칭: Node_Destroy
기    능: 이분검색트리를 없앤다.
입    력: 없음
출    력: 없음
*************************************************************************************************/
void Node_Destroy(BinaryNode* node) {
	if (node != NULL) {
		Node_Destroy(node->left);
		Node_Destroy(node->right);
		free(node);
	}
}