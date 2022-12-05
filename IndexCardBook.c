//IndexCardBook.c
/************************************************************************************************
파일명칭: IndexCardBook.c
기    능: 색인카드집 관리 프로그램
작 성 자: 임다니엘
작성일자: 2022/07/13
*************************************************************************************************/
#include "WordCardBinder.h"
#include "IndexCardBook.h"
#include "IndexCard.h"


/************************************************************************************************
명    칭: IndexCardBook_Create
기    능: 색인카드집을 만든다.
입    력: 없음
출    력: 없음
*************************************************************************************************/
void IndexCardBook_Create(IndexCardBook* indexCardBook) {
	BinaryTree_Create(&(indexCardBook->indexCards));
	indexCardBook->length = 0;
}
/************************************************************************************************
명    칭: IndexCardBook_TakeIn
기    능: 색인카드집에 색인카드를 끼운다.
입    력: 단어카드 링크
출    력: 색인카드 링크
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
명    칭: IndexCardBook_Find
기    능: 색인카드집에서 찾는다.
입    력: 알파벳
출    력: 색인카드 링크
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
명    칭: IndexCardBook_Arrange
기    능: 색인카드집을 정리한다.
입    력: 없음
출    력: 없음
*************************************************************************************************/
void IndexCardBook_Arrange(IndexCardBook* indexCardBook) {
	MakeBalance(&(indexCardBook->indexCards), sizeof(IndexCard));
	ArrangeIndexCards(indexCardBook->indexCards.root);
}
/************************************************************************************************
명    칭: ArrangeIndexCards
기    능: 색인카드집에서 색인카드들을 정리하는 재귀함수
입    력: 없음
출    력: 없음
*************************************************************************************************/
void ArrangeIndexCards(BinaryNode* node) {
	if (node != NULL) {
		ArrangeIndexCards(node->left);
		IndexCard_Arrange((IndexCard*)(node + 1));
		ArrangeIndexCards(node->right);
	}
}
/************************************************************************************************
명    칭: IndexCardBook_TakeOut
기    능: 색인카드집에서 뺀다.
입    력: 알파벳, 단어카드 링크
출    력: 색인카드 링크
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
명    칭: MakeList
기    능: 색인카드 목록을 만든다.
입    력: 없음
출    력: 색인카드들, 개수
*************************************************************************************************/
void MakeList(IndexCardBook* indexCardBook, IndexCard* (*indexCards), Long* count) {
	MakeKeys(&(indexCardBook->indexCards), indexCards, count, sizeof(IndexCard));
}
/************************************************************************************************
명    칭: IndexCardBook_Destroy
기    능: 색인카드집을 없앤다.
입    력: 없음
출    력: 없음
*************************************************************************************************/
void IndexCardBook_Destroy(IndexCardBook* indexCardBook) {
	DestroyIndexCards(indexCardBook->indexCards.root);
	BinaryTree_Destroy(&(indexCardBook->indexCards));
}
/************************************************************************************************
명    칭: DestroyIndexCards
기    능: 색인카드들을 없애는 재귀함수.
입    력: 없음
출    력: 없음
*************************************************************************************************/
void DestroyIndexCards(BinaryNode* node) {
	if (node != NULL) {
		DestroyIndexCards(node->left);
		DestroyIndexCards(node->right);
		IndexCard_Destroy((IndexCard*)(node + 1));
	}
}

/************************************************************************************************/
//알파벳 비교
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