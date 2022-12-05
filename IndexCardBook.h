//IndexCardBook.h
/************************************************************************************************
파일명칭: IndexCardBook.h
기    능: 색인카드집 관리 프로그램 자료형 및 함수 선언
작 성 자: 임다니엘
작성일자: 2022/07/13
*************************************************************************************************/
#ifndef _INDEXCARDBOOK_H
#define _INDEXCARDBOOK_H
#include "BinarySearchTree.h"

typedef signed long int Long;
typedef struct _wordCard WordCard;
typedef struct _indexCard IndexCard;
typedef struct _indexCardBook {
	BinaryTree indexCards;
	Long length;
}IndexCardBook;

void IndexCardBook_Create(IndexCardBook* indexCardBook);
IndexCard* IndexCardBook_TakeIn(IndexCardBook* indexCardBook, WordCard* index);
IndexCard* IndexCardBook_Find(IndexCardBook* indexCardBook, char alphabet);
void IndexCardBook_Arrange(IndexCardBook* indexCardBook);
	void ArrangeIndexCards(BinaryNode* node);
IndexCard* IndexCardBook_TakeOut(IndexCardBook* indexCardBook, char alphabet, WordCard* index);
void MakeList(IndexCardBook* indexCardBook, IndexCard* (*indexCards), Long* count);
void IndexCardBook_Destroy(IndexCardBook* indexCardBook);
	void DestroyIndexCards(BinaryNode* node);

int CompareAlphabets(void* one, void* other);

#endif //_INDEXCARDBOOK_H