//IndexCardBook.c
/************************************************************************************************
���ϸ�Ī: IndexCardBook.c
��    ��: ����ī���� ���� ���α׷�
�� �� ��: �ӴٴϿ�
�ۼ�����: 2022/07/13
*************************************************************************************************/
#include "WordCardBinder.h"
#include "IndexCardBook.h"
#include "IndexCard.h"


/************************************************************************************************
��    Ī: IndexCardBook_Create
��    ��: ����ī������ �����.
��    ��: ����
��    ��: ����
*************************************************************************************************/
void IndexCardBook_Create(IndexCardBook* indexCardBook) {
	BinaryTree_Create(&(indexCardBook->indexCards));
	indexCardBook->length = 0;
}
/************************************************************************************************
��    Ī: IndexCardBook_TakeIn
��    ��: ����ī������ ����ī�带 �����.
��    ��: �ܾ�ī�� ��ũ
��    ��: ����ī�� ��ũ
*************************************************************************************************/
IndexCard* IndexCardBook_TakeIn(IndexCardBook* indexCardBook, WordCard* index) {
	BinaryNode* node;
	IndexCard* link;
	IndexCard indexCard;
	char alphabet;

	alphabet = index->word.spelling[0];
	if (alphabet >= 97 && alphabet <= 122) {
		alphabet -= 32;
	}
	indexCard.alphabet = alphabet;
	node = Search(&(indexCardBook->indexCards), &indexCard, CompareAlphabets);
	if (node == NULL) {
		node = Insert(&(indexCardBook->indexCards), &indexCard, sizeof(IndexCard), CompareAlphabets);
		indexCardBook->length++;
		IndexCard_Create((IndexCard*)(node + 1), 3);
	}
	link = (IndexCard*)(node + 1);
	IndexCard_TakeIn(link, index);

	return link;
}
/************************************************************************************************
��    Ī: IndexCardBook_Find
��    ��: ����ī�������� ã�´�.
��    ��: ���ĺ�
��    ��: ����ī�� ��ũ
*************************************************************************************************/
IndexCard* IndexCardBook_Find(IndexCardBook* indexCardBook, char alphabet) {
	BinaryNode* node;
	IndexCard* link = NULL;
	IndexCard indexCard;

	if (alphabet >= 97 && alphabet <= 122) {
		alphabet -= 32;
	}
	indexCard.alphabet = alphabet;
	node = Search(&(indexCardBook->indexCards), &indexCard, CompareAlphabets);
	if (node != NULL) {
		link = (IndexCard*)(node + 1);
	}

	return link;
}
/************************************************************************************************
��    Ī: IndexCardBook_Arrange
��    ��: ����ī������ �����Ѵ�.
��    ��: ����
��    ��: ����
*************************************************************************************************/
void IndexCardBook_Arrange(IndexCardBook* indexCardBook) {
	MakeBalance(&(indexCardBook->indexCards), sizeof(IndexCard));
	ArrangeIndexCards(indexCardBook->indexCards.root);
}
/************************************************************************************************
��    Ī: ArrangeIndexCards
��    ��: ����ī�������� ����ī����� �����ϴ� ����Լ�
��    ��: ����
��    ��: ����
*************************************************************************************************/
void ArrangeIndexCards(BinaryNode* node) {
	if (node != NULL) {
		ArrangeIndexCards(node->left);
		IndexCard_Arrange((IndexCard*)(node + 1));
		ArrangeIndexCards(node->right);
	}
}
/************************************************************************************************
��    Ī: IndexCardBook_TakeOut
��    ��: ����ī�������� ����.
��    ��: ���ĺ�, �ܾ�ī�� ��ũ
��    ��: ����ī�� ��ũ
*************************************************************************************************/
IndexCard* IndexCardBook_TakeOut(IndexCardBook* indexCardBook, char alphabet, WordCard* index) {
	IndexCard indexCard;
	IndexCard* link;
	BinaryNode* node;
	Long location;

	if (alphabet >= 97 && alphabet <= 122) {
		alphabet -= 32;
	}
	indexCard.alphabet = alphabet;
	node = Search(&(indexCardBook->indexCards), &indexCard, CompareAlphabets);
	link = (IndexCard*)(node + 1);
	location = IndexCard_Find(link, index);
	IndexCard_TakeOut(link, location);
	if (link->length == 0) {
		BinaryTree_Delete(&(indexCardBook->indexCards), &indexCard, sizeof(IndexCard), CompareAlphabets);
		indexCardBook->length--;
		link = NULL;
	}

	return link;
}
/************************************************************************************************
��    Ī: MakeList
��    ��: ����ī�� ����� �����.
��    ��: ����
��    ��: ����ī���, ����
*************************************************************************************************/
void MakeList(IndexCardBook* indexCardBook, IndexCard* (*indexCards), Long* count) {
	MakeKeys(&(indexCardBook->indexCards), indexCards, count, sizeof(IndexCard));
}
/************************************************************************************************
��    Ī: IndexCardBook_Destroy
��    ��: ����ī������ ���ش�.
��    ��: ����
��    ��: ����
*************************************************************************************************/
void IndexCardBook_Destroy(IndexCardBook* indexCardBook) {
	DestroyIndexCards(indexCardBook->indexCards.root);
	BinaryTree_Destroy(&(indexCardBook->indexCards));
}
/************************************************************************************************
��    Ī: DestroyIndexCards
��    ��: ����ī����� ���ִ� ����Լ�.
��    ��: ����
��    ��: ����
*************************************************************************************************/
void DestroyIndexCards(BinaryNode* node) {
	if (node != NULL) {
		DestroyIndexCards(node->left);
		DestroyIndexCards(node->right);
		IndexCard_Destroy((IndexCard*)(node + 1));
	}
}

/************************************************************************************************/
//���ĺ� ��
int CompareAlphabets(void* one, void* other) {
	IndexCard* one_ = (IndexCard*)one;
	IndexCard* other_ = (IndexCard*)other;
	int ret;

	if (one_->alphabet > other_->alphabet) {
		ret = 1;
	}
	else if (one_->alphabet == other_->alphabet) {
		ret = 0;
	}
	else if (one_->alphabet < other_->alphabet) {
		ret = -1;
	}

	return ret;
}