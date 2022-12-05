//IndexCard.h
/************************************************************************************************
파일명칭: IndexCard.h
기    능: 색인카드 관리 프로그램 자료형 및 함수 선언
작 성 자: 임다니엘
작성일자: 2022/07/13
*************************************************************************************************/
#ifndef _INDEXCARD_H
#define _INDEXCARD_H
#include "Array.h"

typedef signed long int Long;
typedef struct _wordCard WordCard;
typedef struct _indexCard {
	char alphabet;
	Array wordCards;
	Long capacity;
	Long length;
}IndexCard;

void IndexCard_Create(IndexCard* indexCard, Long capacity);
Long IndexCard_TakeIn(IndexCard* indexCard, WordCard* index);
Long IndexCard_Find(IndexCard* indexCard, WordCard* index);
Long IndexCard_FindFirstTime(IndexCard* indexCard, char (*spelling));
void IndexCard_Arrange(IndexCard* indexCard);
Long IndexCard_TakeOut(IndexCard* indexCard, Long location);
WordCard* IndexCard_GetAt(IndexCard* indexCard, Long location);
void IndexCard_Destroy(IndexCard* indexCard);

int CompareWordCards_Location(void* one, void* other);
int CompareSpellings_Location(void* one, void* other);
int CompareSpellings_Locations(void* one, void* other);

#endif //_INDEXCARD_H