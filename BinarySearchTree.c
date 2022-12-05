//BinarySearchTree.c
/************************************************************************************************
���ϸ�Ī: BinarySearchTree.c
��    ��: �̺а˻�Ʈ�� ���̺귯��.
�� �� ��: �ӴٴϿ�
�ۼ�����: 2022/06/27
*************************************************************************************************/
#include "BinarySearchTree.h"
#include <stdlib.h>
#include <memory.h>

/************************************************************************************************
��    Ī: Create
��    ��: �̺а˻�Ʈ���� �����.
��    ��: ����
��    ��: ����
*************************************************************************************************/
void BinaryTree_Create(BinaryTree* binaryTree) {
	binaryTree->root = NULL;
	binaryTree->length = 0;
	binaryTree->balance = 0;
}
/************************************************************************************************
��    Ī: Insert
��    ��: �̺а˻�Ʈ���� �׸��� �����Ѵ�.
��    ��: ����
��    ��: ��ġ
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
��    Ī: Search
��    ��: �̺а˻�Ʈ������ ã�´�.
��    ��: ����
��    ��: ��ġ
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
��    Ī: Delete
��    ��: �̺а˻�Ʈ������ �����.
��    ��: ����
��    ��: ��ġ
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
��    Ī: MakeBalance
��    ��: �̺а˻�Ʈ���� ������ �����.
��    ��: ����
��    ��: ����
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
��    Ī: MakeKeys
��    ��: ���ǵ��� �迭�� �Ű����´�.
��    ��: ����
��    ��: ���ǵ�, ����
*************************************************************************************************/
void MakeKeys(BinaryTree* binaryTree, void* (*keys), Long* count, size_t size) {
	Long index = 0;

	*keys = calloc(binaryTree->length, size);
	*count = 0;

	Node_MakeKeys(binaryTree->root, &index, *keys, count, size);
}
/************************************************************************************************
��    Ī: Node_MakeKeys
��    ��: ���ǵ��� �迭�� �Ű����´�.
��    ��: ��ġ
��    ��: ���ǵ�, ����
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
��    Ī: BinaryTree_DeleteAllItems
��    ��: ��� �׸��� �����.
��    ��: ����
��    ��: ����
*************************************************************************************************/
void BinaryTree_DeleteAllItems(BinaryTree* binaryTree) {
	Node_DeleteAllItems(binaryTree->root);
	binaryTree->root = NULL;
	binaryTree->length = 0;
	binaryTree->balance = 0;
}
/************************************************************************************************
��    Ī: Node_DeleteAllItems
��    ��: ��� �׸��� �����.
��    ��: ����
��    ��: ����
*************************************************************************************************/
void Node_DeleteAllItems(BinaryNode* node) {
	if (node != NULL) {
		Node_DeleteAllItems(node->left);
		Node_DeleteAllItems(node->right);
		free(node);
	}
}
/************************************************************************************************
��    Ī: MakeTree
��    ��: Ʈ���� �����.
��    ��: ���ǵ�, ����
��    ��: ����
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
��    Ī: Node_MakeTree
��    ��: Ʈ���� �����.
��    ��: ���ǵ�, ���Ѽ�, ���Ѽ�
��    ��: ��ġ
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
��    Ī: GetAt
��    ��: �̺а˻�Ʈ������ �о�´�.
��    ��: ��ġ
��    ��: ����
*************************************************************************************************/
void BinaryTree_GetAt(BinaryTree* binaryTree, BinaryNode* index, void* key, size_t size) {
	memcpy(key, index + 1, size);
}
/************************************************************************************************
��    Ī: Destroy
��    ��: �̺а˻�Ʈ���� ���ش�.
��    ��: ����
��    ��: ����
*************************************************************************************************/
void BinaryTree_Destroy(BinaryTree* binaryTree) {
	Node_Destroy(binaryTree->root);
}
/************************************************************************************************
��    Ī: Node_Destroy
��    ��: �̺а˻�Ʈ���� ���ش�.
��    ��: ����
��    ��: ����
*************************************************************************************************/
void Node_Destroy(BinaryNode* node) {
	if (node != NULL) {
		Node_Destroy(node->left);
		Node_Destroy(node->right);
		free(node);
	}
}