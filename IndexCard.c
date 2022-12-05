//IndexCard.c
/************************************************************************************************
파일명칭: IndexCard.c
기    능: 색인카드 관리 프로그램
작 성 자: 임다니엘
작성일자: 2022/07/13
*************************************************************************************************/
#include "WordCardBinder.h"
#include "IndexCard.h"
#include <string.h>


/************************************************************************************************
명    칭: IndexCard_Create
기    능: 색인카드를 만든다.
입    력: 할당량
출    력: 없음
*************************************************************************************************/
void IndexCard_Create(IndexCard* indexCard, Long capacity) {
	Array_Create(&(indexCard->wordCards), capacity, sizeof(WordCard*));
	indexCard->capacity = capacity;
	indexCard->length = 0;
}
/************************************************************************************************
명    칭: IndexCard_TakeIn
기    능: 색인카드에 단어카드를 끼운다.
입    력: 단어카드 링크
출    력: 위치
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
명    칭: IndexCard_Find
기    능: 색인카드에서 찾는다.
입    력: 단어카드 링크
출    력: 위치
*************************************************************************************************/
Long IndexCard_Find(IndexCard* indexCard, WordCard* index) {
	Long location;

	location = Array_LinearSearchUnique(&(indexCard->wordCards), index, sizeof(WordCard*), CompareWordCards_Location);

	return location;
}
/************************************************************************************************
명    칭: IndexCard_FindFirstTime
기    능: 색인카드에서 중복된 단어중 첫번째를 찾는다.
입    력: 단어카드 링크
출    력: 위치
*************************************************************************************************/
Long IndexCard_FindFirstTime(IndexCard* indexCard, char (*spelling)) {
	Long location;
	
	location = Array_LinearSearchUnique(&(indexCard->wordCards), spelling, sizeof(WordCard*), CompareSpellings_Location);

	return location;
}
/************************************************************************************************
명    칭: IndexCard_Arrange
기    능: 색인카드를 정리한다.
입    력: 없음
출    력: 없음
*************************************************************************************************/
void IndexCard_Arrange(IndexCard* indexCard) {
	SelectionSort(&(indexCard->wordCards), sizeof(WordCard*), CompareSpellings_Locations);
}
/************************************************************************************************
명    칭: IndexCard_TakeOut
기    능: 색인카드에서 단어카드를 뺀다.
입    력: 위치
출    력: 위치
*************************************************************************************************/
Long IndexCard_TakeOut(IndexCard* indexCard, Long location) {
	location = Array_Delete(&(indexCard->wordCards), location, sizeof(WordCard*));
	indexCard->capacity--;
	indexCard->length--;

	return location;
}
/************************************************************************************************
명    칭: IndexCard_GetAt
기    능: 색인카드에서 읽어온다.
입    력: 위치
출    력: 단어카드 링크
*************************************************************************************************/
WordCard* IndexCard_GetAt(IndexCard* indexCard, Long location) {
	WordCard* index;

	Array_GetAt(&(indexCard->wordCards), location, &index, sizeof(WordCard*));

	return index;
}
/************************************************************************************************
명    칭: IndexCard_Destroy
기    능: 색인카드를 없앤다.
입    력: 없음
출    력: 없음
*************************************************************************************************/
void IndexCard_Destroy(IndexCard* indexCard) {
	Array_Destroy(&(indexCard->wordCards));
}

/************************************************************************************************/
//단어 비교
int CompareWordCards_Location(void* one, void* other) {
	WordCard** one_ = (WordCard**)one;
	int ret = -1;

	if ( *one_ == other) {
		ret = 0;
	}

	return ret;
}
//철자 비교
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