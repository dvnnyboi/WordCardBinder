//IndexCard.c
/************************************************************************************************
���ϸ�Ī: IndexCard.c
��    ��: ����ī�� ���� ���α׷�
�� �� ��: �ӴٴϿ�
�ۼ�����: 2022/07/13
*************************************************************************************************/
#include "WordCardBinder.h"
#include "IndexCard.h"
#include <string.h>


/************************************************************************************************
��    Ī: IndexCard_Create
��    ��: ����ī�带 �����.
��    ��: �Ҵ緮
��    ��: ����
*************************************************************************************************/
void IndexCard_Create(IndexCard* indexCard, Long capacity) {
	Array_Create(&(indexCard->wordCards), capacity, sizeof(WordCard*));
	indexCard->capacity = capacity;
	indexCard->length = 0;
}
/************************************************************************************************
��    Ī: IndexCard_TakeIn
��    ��: ����ī�忡 �ܾ�ī�带 �����.
��    ��: �ܾ�ī�� ��ũ
��    ��: ��ġ
*************************************************************************************************/
Long IndexCard_TakeIn(IndexCard* indexCard, WordCard* index) {
	Long location;

	if (indexCard->capacity > indexCard->length) {
		location = Store(&(indexCard->wordCards), indexCard->length, &index, sizeof(WordCard*));
	}
	else {
		location = AppendFromRear(&(indexCard->wordCards), &index, sizeof(WordCard*));
		indexCard->capacity++;
	}
	indexCard->length++;

	return location;
}
/************************************************************************************************
��    Ī: IndexCard_Find
��    ��: ����ī�忡�� ã�´�.
��    ��: �ܾ�ī�� ��ũ
��    ��: ��ġ
*************************************************************************************************/
Long IndexCard_Find(IndexCard* indexCard, WordCard* index) {
	Long location;

	location = Array_LinearSearchUnique(&(indexCard->wordCards), index, sizeof(WordCard*), CompareWordCards_Location);

	return location;
}
/************************************************************************************************
��    Ī: IndexCard_FindFirstTime
��    ��: ����ī�忡�� �ߺ��� �ܾ��� ù��°�� ã�´�.
��    ��: �ܾ�ī�� ��ũ
��    ��: ��ġ
*************************************************************************************************/
Long IndexCard_FindFirstTime(IndexCard* indexCard, char (*spelling)) {
	Long location;
	
	location = Array_LinearSearchUnique(&(indexCard->wordCards), spelling, sizeof(WordCard*), CompareSpellings_Location);

	return location;
}
/************************************************************************************************
��    Ī: IndexCard_Arrange
��    ��: ����ī�带 �����Ѵ�.
��    ��: ����
��    ��: ����
*************************************************************************************************/
void IndexCard_Arrange(IndexCard* indexCard) {
	SelectionSort(&(indexCard->wordCards), sizeof(WordCard*), CompareSpellings_Locations);
}
/************************************************************************************************
��    Ī: IndexCard_TakeOut
��    ��: ����ī�忡�� �ܾ�ī�带 ����.
��    ��: ��ġ
��    ��: ��ġ
*************************************************************************************************/
Long IndexCard_TakeOut(IndexCard* indexCard, Long location) {
	location = Array_Delete(&(indexCard->wordCards), location, sizeof(WordCard*));
	indexCard->capacity--;
	indexCard->length--;

	return location;
}
/************************************************************************************************
��    Ī: IndexCard_GetAt
��    ��: ����ī�忡�� �о�´�.
��    ��: ��ġ
��    ��: �ܾ�ī�� ��ũ
*************************************************************************************************/
WordCard* IndexCard_GetAt(IndexCard* indexCard, Long location) {
	WordCard* index;

	Array_GetAt(&(indexCard->wordCards), location, &index, sizeof(WordCard*));

	return index;
}
/************************************************************************************************
��    Ī: IndexCard_Destroy
��    ��: ����ī�带 ���ش�.
��    ��: ����
��    ��: ����
*************************************************************************************************/
void IndexCard_Destroy(IndexCard* indexCard) {
	Array_Destroy(&(indexCard->wordCards));
}

/************************************************************************************************/
//�ܾ� ��
int CompareWordCards_Location(void* one, void* other) {
	WordCard** one_ = (WordCard**)one;
	int ret = -1;

	if ( *one_ == other) {
		ret = 0;
	}

	return ret;
}
//ö�� ��
int CompareSpellings_Locations(void* one, void* other) {
	WordCard** one_ = (WordCard**)one;
	WordCard** other_ = (WordCard**)other;

	return strcmp((*one_)->word.spelling, (*other_)->word.spelling);
}
int CompareSpellings_Location(void* one, void* other) {
	WordCard** one_ = (WordCard**)one;
	char (*other_) = (char(*))other;

	return strcmp((*one_)->word.spelling, other_);
}