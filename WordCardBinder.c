//WordCardBinder.c
/************************************************************************************************
���ϸ�Ī: WordCardBinder.c
��    ��: �ܾ�ī��ö ���� ���α׷�
�� �� ��: �ӴٴϿ�
�ۼ�����: 2022/06/17
*************************************************************************************************/
#include "WordCardBinder.h"
#include "IndexCardBook.h"
#include "IndexCard.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#pragma warning (disable:4996)

//�׽�Ʈ�ó�����
#if 0
int main(int argc, char* argv[]) {
	WordCardBinder wordCardBinder;
	IndexCardBook indexCardBook;
	WordCard wordCardOne = { {"general", "Adj", "M.normal", "We don't use it for general occasions."} }; 
	WordCard wordCardTwo = { {"right", "Adj", "M.right", "That's not in the right place."}};
	WordCard wordCardThree = { {"annoyed", "Adj", "M.angry", "She was annoyed with her boyfriend."} };
	WordCard wordCardFour = { {"angry", "Adj", "M.angry", "We don't know why he is angry."} };
	WordCard wordCardFive = { {"general", "Noun", "M.general", "The general was a good leader"}};
	WordCard wordCardSix = { {"animal", "Noun", "M.animal", "There was an animal in the woods"} };
	WordCard wordCard;
	WordCard* index;
	WordCard* (*indexes);
	IndexCard* link;
	IndexCard(*indexCards);
	Long count;
	Long i;
	Long j;

	//�����
	WordCardBinder_Create(&wordCardBinder);
	IndexCardBook_Create(&indexCardBook);
#if 0
	//�����ϴ�.
	printf("************************************Load************************************\n");
	count = Load(&wordCardBinder);
	if (count > 0) {
		index = WordCardBinder_First(&wordCardBinder);
		while (index != previous) {
			printf("%s\t%s\t%12s\t\t%s\n", index->word.spelling, index->word.wordClass, index->word.meaning, index->word.exampleSentence);
			previous = index;
			index = WordCardBinder_Next(&wordCardBinder);
		}
	}
#endif

	//�����
	printf("***********************************TakeIn***********************************\n");
	index = TakeIn(&wordCardBinder, wordCardOne);
	link = IndexCardBook_TakeIn(&indexCardBook, index);
	printf("INDEX: %c, CAPACITY: %d, LENGTH: %d\n", link->alphabet, link->capacity, link->length);
	i = 0;
	while (i < link->length) {
		index = IndexCard_GetAt(link, i);
		printf("- %s\n", index->word.spelling);
		i++;
	}
	printf("----------------------------------------------------------------------------\n");
	index = TakeIn(&wordCardBinder, wordCardTwo);
	link = IndexCardBook_TakeIn(&indexCardBook, index);
	printf("INDEX: %c, CAPACITY: %d, LENGTH: %d\n", link->alphabet, link->capacity, link->length);
	i = 0;
	while (i < link->length) {
		index = IndexCard_GetAt(link, i);
		printf("- %s\n", index->word.spelling);
		i++;
	}
	printf("----------------------------------------------------------------------------\n");
	index = TakeIn(&wordCardBinder, wordCardThree);
	link = IndexCardBook_TakeIn(&indexCardBook, index);
	printf("INDEX: %c, CAPACITY: %d, LENGTH: %d\n", link->alphabet, link->capacity, link->length);
	i = 0;
	while (i < link->length) {
		index = IndexCard_GetAt(link, i);
		printf("- %s\n", index->word.spelling);
		i++;
	}
	printf("----------------------------------------------------------------------------\n");
	index = TakeIn(&wordCardBinder, wordCardFour);
	link = IndexCardBook_TakeIn(&indexCardBook, index);
	printf("INDEX: %c, CAPACITY: %d, LENGTH: %d\n", link->alphabet, link->capacity, link->length);
	i = 0;
	while (i < link->length) {
		index = IndexCard_GetAt(link, i);
		printf("- %s\n", index->word.spelling);
		i++;
	}
	printf("----------------------------------------------------------------------------\n");
	index = TakeIn(&wordCardBinder, wordCardFive);
	link = IndexCardBook_TakeIn(&indexCardBook, index);
	printf("INDEX: %c, CAPACITY: %d, LENGTH: %d\n", link->alphabet, link->capacity, link->length);
	i = 0;
	while (i < link->length) {
		index = IndexCard_GetAt(link, i);
		printf("- %s\n", index->word.spelling);
		i++;
	}
	printf("----------------------------------------------------------------------------\n");
	index = TakeIn(&wordCardBinder, wordCardSix);
	link = IndexCardBook_TakeIn(&indexCardBook, index);
	printf("INDEX: %c, CAPACITY: %d, LENGTH: %d\n", link->alphabet, link->capacity, link->length);
	i = 0;
	while (i < link->length) {
		index = IndexCard_GetAt(link, i);
		printf("- %s\n", index->word.spelling);
		i++;
	}
	printf("----------------------------------------------------------------------------\n");
	//����ī�� ����� �����.
	printf("\n***********************************MakeList*********************************\n");
	MakeList(&indexCardBook, &indexCards, &count);
	i = 0;
	while (i < count) {
		link = &(indexCards[i]);
		printf("INDEX: %c, CAPACITY: %d, LENGTH: %d\n", link->alphabet, link->capacity, link->length);
		j = 0;
		while (j < link->length) {
			index = IndexCard_GetAt(link, j);
			printf("- %s\n", index->word.spelling);
			j++;
		}
		printf("----------------------------------------------------------------------------\n");
		i++;
	}
	if (indexCards != NULL) {
		free(indexCards);
	}
	//����ī������ �����Ѵ�.
	printf("\n***********************************Arrange**********************************\n");
	IndexCardBook_Arrange(&indexCardBook);
	MakeList(&indexCardBook, &indexCards, &count);
	i = 0;
	while (i < count) {
		link = &(indexCards[i]);
		printf("INDEX: %c, CAPACITY: %d, LENGTH: %d\n", link->alphabet, link->capacity, link->length);
		j = 0;
		while (j < link->length) {
			index = IndexCard_GetAt(link, j);
			printf("- %s\n", index->word.spelling);
			j++;
		}
		printf("----------------------------------------------------------------------------\n");
		i++;
	}
	if (indexCards != NULL) {
		free(indexCards);
	}
#if 0
	//ö�ڷ� ã��.
	printf("*******************************FindBySpelling*******************************\n");
	FindBySpelling(&wordCardBinder, "general", &indexes, &count);
	i = 0;
	while (i < count) {
		printf("%s\t%s\t%12s\t\t%s\n", indexes[i]->word.spelling, indexes[i]->word.wordClass,
			indexes[i]->word.meaning, indexes[i]->word.exampleSentence);
		i++;
	}
	if (indexes != NULL) {
		free(indexes);
	}
	//������ ã��.
	printf("*******************************FindByMeaning********************************\n");
	FindByMeaning(&wordCardBinder, "M.angry", &indexes, &count);
	i = 0;
	while (i < count) {
		printf("%s\t%s\t%12s\t\t%s\n", indexes[i]->word.meaning, indexes[i]->word.wordClass,
			indexes[i]->word.spelling, indexes[i]->word.exampleSentence);
		i++;
	}
#endif
	//����
	printf("\n********************************TakeOut*************************************\n");
	FindByMeaning(&wordCardBinder, "M.angry", &indexes, &count);
	index = indexes[0];
	wordCard = TakeOut(&wordCardBinder, index);
	link = IndexCardBook_TakeOut(&indexCardBook, wordCard.word.spelling[0], index);
	printf("INDEX: %c, CAPACITY: %d, LENGTH: %d\n", link->alphabet, link->capacity, link->length);
	i = 0;
	while (i < link->length) {
		index = IndexCard_GetAt(link, i);
		printf("- %s\n", index->word.spelling);
		i++;
	}
	printf("----------------------------------------------------------------------------\n");
	if (indexes != NULL) {
		free(indexes);
	}
	//����ī�带 ã�´�.
	printf("\n*******************************FindIndexCard********************************\n");
	link = IndexCardBook_Find(&indexCardBook, 'g');
	printf("INDEX: %c, CAPACITY: %d, LENGTH: %d\n", link->alphabet, link->capacity, link->length);
	i = 0;
	while (i < link->length) {
		index = IndexCard_GetAt(link, i);
		printf("- %s\n", index->word.spelling);
		i++;
	}
	printf("----------------------------------------------------------------------------\n");
	//����ī�带 ����.
	printf("\n******************************TakeOutIndexCard******************************\n");
	while (link != NULL && link->length > 0) {
		index = IndexCard_GetAt(link, 0);
		wordCard = TakeOut(&wordCardBinder, index);
		link = IndexCardBook_TakeOut(&indexCardBook, wordCard.word.spelling[0], index);
	}
	MakeList(&indexCardBook, &indexCards, &count);
	i = 0;
	while (i < count) {
		link = &(indexCards[i]);
		printf("INDEX: %c, CAPACITY: %d, LENGTH: %d\n", link->alphabet, link->capacity, link->length);
		j = 0;
		while (j < link->length) {
			index = IndexCard_GetAt(link, j);
			printf("- %s\n", index->word.spelling);
			j++;
		}
		printf("----------------------------------------------------------------------------\n");
		i++;
	}
	if (indexCards != NULL) {
		free(indexCards);
	}
#if 0
	//ó������ �̵��Ѵ�
	printf("************************************First***********************************\n");
	index = WordCardBinder_First(&wordCardBinder);
	printf("%s\t%s\t%12s\t\t%s\n", index->word.spelling, index->word.wordClass, index->word.meaning, index->word.exampleSentence);
	//�������� �̵��Ѵ�
	printf("************************************Next************************************\n");
	index = WordCardBinder_Next(&wordCardBinder);
	printf("%s\t%s\t%12s\t\t%s\n", index->word.spelling, index->word.wordClass, index->word.meaning, index->word.exampleSentence);
	//���������� �̵��Ѵ�
	printf("************************************Last************************************\n");
	index = WordCardBinder_Last(&wordCardBinder);
	printf("%s\t%s\t%12s\t\t%s\n", index->word.spelling, index->word.wordClass, index->word.meaning, index->word.exampleSentence);
	//�������� �̵��Ѵ�
	printf("***********************************Previous*********************************\n");
	index = WordCardBinder_Previous(&wordCardBinder);
	printf("%s\t%s\t%12s\t\t%s\n", index->word.spelling, index->word.wordClass, index->word.meaning, index->word.exampleSentence);
	//�����Ѵ�.
	Save(&wordCardBinder);
#endif
	//���ִ�.
	WordCardBinder_Destroy(&wordCardBinder);
	IndexCardBook_Destroy(&indexCardBook);

	return 0;
}
#endif
/************************************************************************************************
��    Ī: WordCardBinder_Create
��    ��: �ܾ�ī��ö�� �����.
��    ��: ����
��    ��: ����
*************************************************************************************************/
void WordCardBinder_Create(WordCardBinder* wordCardBinder) {
	Create(&(wordCardBinder->wordCards));
	wordCardBinder->length = 0;
	wordCardBinder->current = NULL;
}
/************************************************************************************************
��    Ī: Load
��    ��: ��ũ���Ϸκ��� �ܾ�ī��ö�� �Է¹޴´�
��    ��: ����
��    ��: ����
*************************************************************************************************/
Long Load(WordCardBinder* wordCardBinder, WordCardBinder* memorizedWords) {
	WordCard wordCard;
	Node* node;
	size_t flag;
	FILE* wordCards;

	wordCards = fopen("WordCardBinder.dat", "rb");
	if (wordCards != NULL) {
		flag = fread(&wordCard, sizeof(WordCard), 1, wordCards);
		while (!feof(wordCards) && flag != 0) {
			node = AppendFromTail(&(wordCardBinder->wordCards), &wordCard, sizeof(WordCard));
			wordCardBinder->current = (WordCard*)(node + 1);
			wordCardBinder->length++;
			flag = fread(&wordCard, sizeof(WordCard), 1, wordCards);
		}
		fclose(wordCards);
	}
	wordCards = fopen("MemorizedWords.dat", "rb");
	if (wordCards != NULL) {
		flag = fread(&wordCard, sizeof(WordCard), 1, wordCards);
		while (!feof(wordCards) && flag != 0) {
			node = AppendFromTail(&(memorizedWords->wordCards), &wordCard, sizeof(WordCard));
			memorizedWords->current = (WordCard*)(node + 1);
			memorizedWords->length++;
			flag = fread(&wordCard, sizeof(WordCard), 1, wordCards);
		}
		fclose(wordCards);
	}
	return wordCardBinder->length;
}
/************************************************************************************************
��    Ī: TakeIn
��    ��: �ܾ�ī��ö�� �ܾ�ī�带 �����.
��    ��: �ܾ�ī��
��    ��: ��ġ
*************************************************************************************************/
WordCard* TakeIn(WordCardBinder* wordCardBinder, WordCard wordCard) {
	Node* node;

	node = AppendFromTail(&(wordCardBinder->wordCards), &wordCard, sizeof(WordCard));
	wordCardBinder->length++;
	wordCardBinder->current = (WordCard*)(node + 1);

	return wordCardBinder->current;
}
/************************************************************************************************
��    Ī: FindBySpellig
��    ��: �ܾ�ī��ö���� ö�ڷ� ã�´�.
��    ��: ö��
��    ��: ��ġ��, ����
*************************************************************************************************/
void FindBySpelling(WordCardBinder* wordCardBinder, char(*spelling), WordCard** (*indexes), Long* count) {
	Node*(*nodes);
	Long i;

	LinearSearchDuplicate(&(wordCardBinder->wordCards), spelling, &nodes, count, CompareSpellings);
	*indexes = (WordCard * (*))calloc(wordCardBinder->length, sizeof(WordCard*));
	i = 0;
	while (i < *count) {
		(*indexes)[i] = (WordCard*)(nodes[i] + 1);
		i++;
	}
	if (nodes != NULL) {
		free(nodes);
	}
}
/************************************************************************************************
��    Ī: FindByMeaning
��    ��: �ܾ�ī��ö���� ������ ã�´�.
��    ��: ��
��    ��: ��ġ��, ����
*************************************************************************************************/
void FindByMeaning(WordCardBinder* wordCardBinder, char(*meaning), WordCard** (*indexes), Long* count) {
	Node* (*nodes);
	Long i;

	LinearSearchDuplicate(&(wordCardBinder->wordCards), meaning, &nodes, count, CompareMeanings);
	*indexes = (WordCard * (*))calloc(wordCardBinder->length, sizeof(WordCard*));
	i = 0;
	while (i < *count) {
		(*indexes)[i] = (WordCard*)(nodes[i] + 1);
		i++;
	}
	if (nodes != NULL) {
		free(nodes);
	}
}
/************************************************************************************************
��    Ī: TakeOut
��    ��: �ܾ�ī��ö���� �ܾ�ī�带 ����.
��    ��: ��ġ
��    ��: �ܾ�ī��
*************************************************************************************************/
WordCard TakeOut(WordCardBinder* wordCardBinder, WordCard* index) {
	Node* node;
	WordCard wordCard;
	
	node = LinearSearchUnique(&(wordCardBinder->wordCards), index, CompareWordCards);
	GetAt(&(wordCardBinder->wordCards), node, &wordCard, sizeof(WordCard));
	Delete(&(wordCardBinder->wordCards), node);
	wordCardBinder->length--;
	if (wordCardBinder->length > 0) {
		wordCardBinder->current = (WordCard*)(wordCardBinder->wordCards.current + 1);
	}
	else {
		wordCardBinder->current = NULL;
	}
	return wordCard;
}
/************************************************************************************************
��    Ī: CheckWordClass
��    ��: �ܾ�ī��ö���� ��� ä���Ѵ�.
��    ��: ��ġ, ǰ��
��    ��: ���
*************************************************************************************************/
Long CheckWordClass(WordCardBinder* wordCardBinder, char(*wordClass)) {
	Long result = -1;
	WordCard* index;

	index = wordCardBinder->current;
	if (strcmp(index->word.wordClass, wordClass) == 0) {
		result = 0;
	}
	return result;
}
/************************************************************************************************
��    Ī: CheckMeaning
��    ��: �ܾ�ī��ö���� ��� ä���Ѵ�.
��    ��: ��ġ, ��
��    ��: ���
*************************************************************************************************/
Long CheckMeaning(WordCardBinder* wordCardBinder, char(*meaning)) {
	Long result = -1;
	WordCard* index;

	index = wordCardBinder->current;
	if (strcmp(index->word.meaning, meaning) == 0) {
		result = 0;
	}
	return result;
}
/************************************************************************************************
��    Ī: CheckExampleSentence
��    ��: �ܾ�ī��ö���� ��� ä���Ѵ�.
��    ��: ��ġ, ����
��    ��: ���
*************************************************************************************************/
Long CheckExampleSentence(WordCardBinder* wordCardBinder, char(*exampleSentence)) {
	Long result = -1;
	WordCard* index;

	index = wordCardBinder->current;
	if (strcmp(index->word.exampleSentence, exampleSentence) == 0) {
		result = 0;
	}
	return result;
}

/************************************************************************************************
��    Ī: WordCardBinder_Move
��    ��: �ܾ�ī��ö���� �̵����� ��ġ�� �̵��Ѵ�.
��    ��: ��ġ
��    ��: ��ġ
*************************************************************************************************/
WordCard* WordCardBinder_Move(WordCardBinder* wordCardBinder, WordCard* index) {
	Node* node;

	node = LinearSearchUnique(&(wordCardBinder->wordCards), index, CompareWordCards);
	Move(&(wordCardBinder->wordCards), node);
	wordCardBinder->current = (WordCard*)(node + 1);

	return wordCardBinder->current;
}
/************************************************************************************************
��    Ī: WordCardBinder_First
��    ��: �ܾ�ī��ö���� ó������ �̵��Ѵ�.
��    ��: ����
��    ��: ��ġ
*************************************************************************************************/
WordCard* WordCardBinder_First(WordCardBinder* wordCardBinder) {
	Node* node;

	node = First(&(wordCardBinder->wordCards));
	wordCardBinder->current = (WordCard*)(node + 1);

	return wordCardBinder->current;
}
/************************************************************************************************
��    Ī: WordCardBinder_Previous
��    ��: �ܾ�ī��ö���� �������� �̵��Ѵ�.
��    ��: ����
��    ��: ��ġ
*************************************************************************************************/
WordCard* WordCardBinder_Previous(WordCardBinder* wordCardBinder) {
	Node* node;

	node = Previous(&(wordCardBinder->wordCards));
	wordCardBinder->current = (WordCard*)(node + 1);

	return wordCardBinder->current;
}
/************************************************************************************************
��    Ī: WordCardBinder_Next
��    ��: �ܾ�ī��ö���� �������� �̵��Ѵ�.
��    ��: ����
��    ��: ��ġ
*************************************************************************************************/
WordCard* WordCardBinder_Next(WordCardBinder* wordCardBinder) {
	Node* node;

	node = Next(&(wordCardBinder->wordCards));
	wordCardBinder->current = (WordCard*)(node + 1);

	return wordCardBinder->current;
}
/************************************************************************************************
��    Ī: WordCardBinder_Last
��    ��: �ܾ�ī��ö���� ���������� �̵��Ѵ�.
��    ��: ����
��    ��: ��ġ
*************************************************************************************************/
WordCard* WordCardBinder_Last(WordCardBinder* wordCardBinder) {
	Node* node;

	node = Last(&(wordCardBinder->wordCards));
	wordCardBinder->current = (WordCard*)(node + 1);

	return wordCardBinder->current;
}
/************************************************************************************************
��    Ī: Save
��    ��: �ܾ�ī��ö�� ��ũ���Ϸ� ����Ѵ�.
��    ��: ����
��    ��: ����
*************************************************************************************************/
Long Save(WordCardBinder* wordCardBinder, WordCardBinder* memorizedWords) {
	Node* node;
	Node* previous = NULL;
	WordCard wordCard;
	FILE* wordCards;

	wordCards = fopen("WordCardBinder.dat", "wb");
	if (wordCards != NULL) {
		node = First(&(wordCardBinder->wordCards));
		while (node != NULL && node != previous) {
			GetAt(&(wordCardBinder->wordCards), node, &wordCard, sizeof(WordCard));
			fwrite(&wordCard, sizeof(WordCard), 1, wordCards);
			previous = node;
			node = Next(&(wordCardBinder->wordCards));
		}
		fclose(wordCards);
	}
	wordCards = fopen("MemorizedWords.dat", "wb");
	if (wordCards != NULL) {
		node = First(&(memorizedWords->wordCards));
		while (node != NULL && node != previous) {
			GetAt(&(memorizedWords->wordCards), node, &wordCard, sizeof(WordCard));
			fwrite(&wordCard, sizeof(WordCard), 1, wordCards);
			previous = node;
			node = Next(&(memorizedWords->wordCards));
		}
		fclose(wordCards);
	}
	return wordCardBinder->length;
}
/************************************************************************************************
��    Ī: WordCardBinder_Destroy
��    ��: �ܾ�ī��ö�� ���ش�.
��    ��: ����
��    ��: ����
*************************************************************************************************/
void WordCardBinder_Destroy(WordCardBinder* wordCardBinder) {
	Destroy(&(wordCardBinder->wordCards));
}


//�Լ��������� ������ ���� �Լ�
int CompareSpellings(void* one, void* other) {
	WordCard* one_ = (WordCard*)one;
	char(*other_) = (char(*))other;

	return strcmp(one_->word.spelling, other_);
}
int CompareMeanings(void* one, void* other) {
	WordCard* one_ = (WordCard*)one;
	char(*other_) = (char(*))other;

	return strcmp(one_->word.meaning, other_);
}
int CompareWordCards(void* one, void* other) {
	Long ret;

	if (one == other) {
		ret = 0;
	}
	else {
		ret = -1;
	}
	return ret;
}