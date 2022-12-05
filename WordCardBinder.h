//WordCardBinder.h
/************************************************************************************************
파일명칭: WordCardBinder.h
기    능: 단어카드철 관리 프로그램 자료형 및 함수 선언
작 성 자: 임다니엘
작성일자: 2022/06/17
*************************************************************************************************/
#ifndef _WORDCARDBINDER_H
#define _WORDCARDBINDER_H
#include "LinkedList.h"

typedef signed long int Long;
typedef struct _word {
	char spelling[16];
	char wordClass[16];
	char meaning[16];
	char exampleSentence[64];
}Word;
typedef struct _evaluation {
	Long quiz;
	Long correct;
	float correctRate;
}Evaluation;
typedef struct _wordCard {
	Word word;
	Evaluation evaluation;
}WordCard;
typedef struct _wordCardBinder {
	LinkedList wordCards;
	Long length;
	WordCard* current;
}WordCardBinder;

void WordCardBinder_Create(WordCardBinder* wordCardBinder);
Long Load(WordCardBinder* wordCardBinder, WordCardBinder* memorizedWords);
WordCard* TakeIn(WordCardBinder* wordCardBinder, WordCard wordCard);
void FindBySpelling(WordCardBinder* wordCardBinder, char(*spelling), WordCard** (*indexes), Long* count);
void FindByMeaning(WordCardBinder* wordCardBinder, char(*meaning), WordCard** (*indexes), Long* count);
WordCard TakeOut(WordCardBinder* wordCardBinder, WordCard* index);
Long CheckWordClass(WordCardBinder* wordCardBinder, char(*wordClass));
Long CheckMeaning(WordCardBinder* wordCardBinder, char(*meaning));
Long CheckExampleSentence(WordCardBinder* wordCardBinder, char(*exampleSentence));
WordCard* WordCardBinder_Move(WordCardBinder* wordCardBinder, WordCard* index);
WordCard* WordCardBinder_First(WordCardBinder* wordCardBinder);
WordCard* WordCardBinder_Previous(WordCardBinder* wordCardBinder);
WordCard* WordCardBinder_Next(WordCardBinder* wordCardBinder);
WordCard* WordCardBinder_Last(WordCardBinder* wordCardBinder);
Long Save(WordCardBinder* wordCardBinder, WordCardBinder* memorizedWords);
void WordCardBinder_Destroy(WordCardBinder* wordCardBinder);

int CompareSpellings(void* one, void* other);
int CompareMeanings(void* one, void* other);
int CompareWordCards(void* one, void* other);


#endif //_WORDCARDBINDER_H