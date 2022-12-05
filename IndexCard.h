//IndexCard.h
/************************************************************************************************
���ϸ�Ī: IndexCard.h
��    ��: ����ī�� ���� ���α׷� �ڷ��� �� �Լ� ����
�� �� ��: �ӴٴϿ�
�ۼ�����: 2022/07/13
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