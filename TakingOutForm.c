//TakingOutForm.c
/************************************************************************************************
파일명칭: TakingOutForm.c
기    능: 단어카드철 빼기 윈도우
작 성 자: 임다니엘
작성일자: 2022/07/15
*************************************************************************************************/
#include "TakingOutForm.h"
#include "WordCardBinder.h"
#include "IndexCardBook.h"
#include "IndexCard.h"
#include "resource.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinUser.h>
#include <CommCtrl.h>
#pragma warning (disable:4996)
/************************************************************************************************
명    칭: TakingOutFormProc
기    능: 빼기 윈도우 프로시져함수.
*************************************************************************************************/
BOOL CALLBACK TakingOutFormProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	BOOL ret;

	switch (message) {
	case WM_INITDIALOG:
		ret = TakingOutForm_OnInitDialog(hWnd, wParam, lParam);
		break;
	case WM_CLOSE:
		ret = TakingOutForm_OnClose(hWnd, wParam, lParam);
		break;
	default:
		ret = FALSE;
		break;
	}
	return ret;
}
/************************************************************************************************
명    칭: TakingOutForm_OnInitDialog
기    능: 윈도우가 생성될 때 시나리오.
*************************************************************************************************/
BOOL TakingOutForm_OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	HWND hWordCardBinderFormWnd;
	WordCardBinder* wordCardBinder;
	WordCard* index;
	WordCard wordCard;
	IndexCardBook* indexCardBook;
	IndexCard* link;
	TVITEM item;
	HTREEITEM hRootItem;
	HTREEITEM hIndexCardItem;
	HTREEITEM hWordItem;
	HTREEITEM hSelectedItem;
	HTREEITEM hWordClassItem;
	Long location;
	Long firstLocation;
	Long i;
	TCHAR alphabet[2];
	TCHAR compare[2];
	TCHAR spelling[16];
	TCHAR compareSpelling[16];
	TCHAR correctRate[8];

	//1.1. 단어카드철 윈도우를 찾는다.
	hWordCardBinderFormWnd = FindWindow("#32770", "단어카드철");
	//1.2. 단어카드철 윈도우의 단어카드철에서 뺀다.
	wordCardBinder = (WordCardBinder*)GetWindowLong(hWordCardBinderFormWnd, GWL_USERDATA);
	index = wordCardBinder->current;
	wordCard = TakeOut(wordCardBinder, index);
	//1.3. 단어카드철 윈도우의 색인카드집에서 뺀다.
	indexCardBook = (IndexCardBook*)GetProp(hWordCardBinderFormWnd, "PROP_INDEXCARDBOOK");
	link = IndexCardBook_TakeOut(indexCardBook, wordCard.word.spelling[0], index);
	//1.4. 단어카드철 윈도우의 트리 뷰 컨트롤에서 선택된 항목을 읽는다.
	hSelectedItem = (HTREEITEM)SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK),
		TVM_GETNEXTITEM, (WPARAM)TVGN_CARET, (LPARAM)0);
	//1.5. 단어카드철 윈도우의 트리 뷰 컨트롤에서 단어 항목을 읽는다.
	 hWordItem = (HTREEITEM)SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK),
		TVM_GETNEXTITEM, (WPARAM)TVGN_PARENT, (LPARAM)hSelectedItem);
	//1.6. 단어카드철 윈도우의 트리 뷰 컨트롤에서 알파벳 항목을 읽는다.
	hIndexCardItem = (HTREEITEM)SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK),
		TVM_GETNEXTITEM, (WPARAM)TVGN_PARENT, (LPARAM)hWordItem);
	//1.7. 단어카드철 윈도우의 트리 뷰 컨트롤에서 품사 항목을 지운다.
	SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK),	TVM_DELETEITEM, (WPARAM)0, (LPARAM)hSelectedItem);
	//1.8. 단어카드철 윈도우의 트리 뷰 컨트롤에서 단어 항목이 비었으면,
	hWordClassItem = (HTREEITEM)SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK),
		TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hWordItem);
	if (hWordClassItem == NULL) {
		//1.8.1. 단어카드철 윈도우의 트리 뷰 컨트롤에서 단어 항목을 지운다.
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK), TVM_DELETEITEM, (WPARAM)0, (LPARAM)hWordItem);
	}
	//1.9. 단어카드철 윈도우의 트리 뷰 컨트롤에서 알파벳 항목이 비었으면
	hWordItem = (HTREEITEM)SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK),
		TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hIndexCardItem);
	if (hWordItem == NULL) {
		//1.9.1. 단어카드철 윈도우의 트리 뷰 컨트롤에서 알파벳 항목을 지운다.
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK), TVM_DELETEITEM, (WPARAM)0, (LPARAM)hIndexCardItem);
	}
	index = wordCardBinder->current;
	if (index != NULL) {
		//1.10. 단어카드철 윈도우에 현재 단어를 쓴다.
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_EDIT_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.spelling);
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_EDIT_WORDCLASS), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.wordClass);
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_EDIT_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.meaning);
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_EDIT_EXAMPLESENTENCE), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.exampleSentence);
		//sprintf(correctRate, "%.1f", index->evaluation.correctRate);
		//SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_EDIT_CORRECTRATE), WM_SETTEXT, (WPARAM)0, (LPARAM)correctRate);
		//1.11. 단어카드철 윈도우의 색인카드집에서 찾는다.
		link = IndexCardBook_Find(indexCardBook, index->word.spelling[0]);
		//1.12. 단어카드철 윈도우의 색인카드에서 찾는다.
		location = IndexCard_Find(link, index);
		firstLocation = IndexCard_FindFirstTime(link, index->word.spelling);
		//1.13. 단어카드철 윈도우의 트리 뷰 컨트롤에서 루트 항목을 펼친다.
		hRootItem = (HTREEITEM)SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK),
			TVM_GETNEXTITEM, (WPARAM)TVGN_ROOT, (LPARAM)0);
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hRootItem);
		//1.14. 단어카드철 윈도우의 트리 뷰 컨트롤에서 알파벳 항목을 찾는다.
		compare[0] = link->alphabet;
		compare[1] = '\0';
		hIndexCardItem = (HTREEITEM)SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK),
			TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hRootItem);
		item.mask = TVIF_TEXT | TVIF_HANDLE;
		item.hItem = hIndexCardItem;
		item.pszText = alphabet;
		item.cchTextMax = 2;
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETITEM, (WPARAM)0, (LPARAM)&item);
		while (hIndexCardItem != NULL && strcmp(compare, alphabet) != 0) {
			hIndexCardItem = (HTREEITEM)SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETNEXTITEM,
				(WPARAM)TVGN_NEXT, (LPARAM)hIndexCardItem);
			item.hItem = hIndexCardItem;
			SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETITEM, (WPARAM)0, (LPARAM)&item);
		}
		//1.15. 단어카드철 윈도우의 트리 뷰 컨트롤에서 알파벳 항목을 펼친다.
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hIndexCardItem);
		//1.16. 단어카드철 윈도우의 트리 뷰 컨트롤의 단어 항목을 찾는다.
		strcpy(spelling, index->word.spelling);
		hWordItem = (HTREEITEM)SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK),
			TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hIndexCardItem);
		item.mask = TVIF_TEXT | TVIF_HANDLE;
		item.hItem = hWordItem;
		item.pszText = compareSpelling;
		item.cchTextMax = 16;
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETITEM, (WPARAM)0, (LPARAM)&item);
		while (hWordItem != NULL && strcmp(spelling, compareSpelling) != 0) {
			hWordItem = (HTREEITEM)SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETNEXTITEM,
				(WPARAM)TVGN_NEXT, (LPARAM)hWordItem);
			item.hItem = hWordItem;
			SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETITEM, (WPARAM)0, (LPARAM)&item);
		}
		//1.17. 단어카드철 윈도우의 트리 뷰 컨트롤에서 단어 항목을 펼친다.
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hWordItem);
		//1.18. 단어카드철 윈도우 트리 뷰 컨트롤의 단어 항목에서 이동한다.
		hWordClassItem = (HTREEITEM)SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK),
			TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hWordItem);
		i = 0;
		while (hWordClassItem != NULL && i < (location - firstLocation)) {
			hWordClassItem = (HTREEITEM)SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK),
				TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hWordClassItem);
			i++;
		}
		//1.19. 단어카드철 윈도우의 트리 뷰 컨트롤에서 품사 항목을 선택한다.
		item.mask = TVIF_STATE | TVIF_HANDLE;
		item.hItem = hWordClassItem;
		item.stateMask = TVIS_SELECTED;
		item.state = TVIS_SELECTED;
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK), TVM_SETITEM, (WPARAM)0, (LPARAM)&item);
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK), TVM_SELECTITEM, (WPARAM)TVGN_CARET, (LPARAM)hWordClassItem);
	}
	else {
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_EDIT_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_EDIT_WORDCLASS), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_EDIT_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_EDIT_EXAMPLESENTENCE), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		EnableWindow(GetDlgItem(hWordCardBinderFormWnd, IDC_BUTTON_FIRST), FALSE);
		EnableWindow(GetDlgItem(hWordCardBinderFormWnd, IDC_BUTTON_PREVIOUS), FALSE);
		EnableWindow(GetDlgItem(hWordCardBinderFormWnd, IDC_BUTTON_NEXT), FALSE);
		EnableWindow(GetDlgItem(hWordCardBinderFormWnd, IDC_BUTTON_LAST), FALSE);
		EnableWindow(GetDlgItem(hWordCardBinderFormWnd, IDC_BUTTON_FIND), FALSE);
		EnableWindow(GetDlgItem(hWordCardBinderFormWnd, IDC_BUTTON_ARRANGE), FALSE);
		EnableWindow(GetDlgItem(hWordCardBinderFormWnd, IDC_BUTTON_TAKEOUT), FALSE);
		EnableWindow(GetDlgItem(hWordCardBinderFormWnd, IDC_BUTTON_QUIZ), FALSE);
		//SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_EDIT_CORRECTRATE), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
	}
	
	//1.16. 뺀 단어카드를 쓴다.
	SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)wordCard.word.spelling);
	SendMessage(GetDlgItem(hWnd, IDC_EDIT_WORDCLASS), WM_SETTEXT, (WPARAM)0, (LPARAM)wordCard.word.wordClass);
	SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)wordCard.word.meaning);
	SendMessage(GetDlgItem(hWnd, IDC_EDIT_EXAMPLESENTENCE), WM_SETTEXT, (WPARAM)0, (LPARAM)wordCard.word.exampleSentence);
	//sprintf(correctRate, "%.1f", wordCard.evaluation.correctRate);
	//SendMessage(GetDlgItem(hWnd, IDC_EDIT_CORRECTRATE), WM_SETTEXT, (WPARAM)0, (LPARAM)correctRate);

	return TRUE;
}
/************************************************************************************************
명    칭: TakingOutForm_OnClose
기    능: 닫기 버튼을 클릭했을 때 시나리오.
*************************************************************************************************/
BOOL TakingOutForm_OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	HWND hWordCardBinderFormWnd;
	WordCardBinder* wordCardBinder;
	WordCard* index;
	WordCard* (*indexes);
	WordCard wordCard;
	IndexCardBook* indexCardBook;
	IndexCard* link;
	IndexCard(*indexCards);
	TVINSERTSTRUCT insertItem;
	TVITEM item;
	HTREEITEM hRootItem;
	HTREEITEM hIndexCardItem = (HTREEITEM)TVI_LAST;
	HTREEITEM hWordItem = (HTREEITEM)TVI_LAST;
	HTREEITEM hWordClassItem = (HTREEITEM)TVI_LAST;
	Long count;
	Long indexCount;
	Long location;
	Long firstLocation;
	Long i;
	Long j;
	Long k;
	TCHAR alphabet[2];
	TCHAR compare[2];
	TCHAR spelling[16];
	TCHAR compareSpelling[16];
	TCHAR correctRate[8];
	Long messageBox;

	//2.1. 메세지박스를 출력한다.
	messageBox = MessageBox(hWnd, "끼우시겠습니까?", "닫기", MB_YESNOCANCEL | MB_DEFBUTTON3);
	//2.2. 예 버튼을 클릭했을 때.
	if (messageBox == IDYES) {
		//2.2.1. 단어를 읽는다.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_GETTEXT, (WPARAM)16, (LPARAM)wordCard.word.spelling);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_WORDCLASS), WM_GETTEXT, (WPARAM)16, (LPARAM)wordCard.word.wordClass);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_GETTEXT, (WPARAM)16, (LPARAM)wordCard.word.meaning);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_EXAMPLESENTENCE), WM_GETTEXT, (WPARAM)64, (LPARAM)wordCard.word.exampleSentence);
		//2.2.2. 단어카드철 윈도우를 찾는다.
		hWordCardBinderFormWnd = FindWindow("#32770", "단어카드철");
		//2.2.3. 단어카드철 윈도우의 단어카드철에서 끼운다.
		wordCard.evaluation.quiz = 0;
		wordCard.evaluation.correct = 0;
		wordCard.evaluation.correctRate = 0.0F;
		wordCardBinder = (WordCardBinder*)GetWindowLong(hWordCardBinderFormWnd, GWL_USERDATA);
		index = TakeIn(wordCardBinder, wordCard);
		//2.2.4. 단어카드철 윈도우의 색인카드집에서 끼운다.
		indexCardBook = (IndexCardBook*)GetProp(hWordCardBinderFormWnd, "PROP_INDEXCARDBOOK");
		link = IndexCardBook_TakeIn(indexCardBook, index);
		//2.2.5. 단어카드철 윈도우의 색인카드집에서 정리한다.
		IndexCardBook_Arrange(indexCardBook);
		//2.2.6. 단어카드철 윈도우에서 색인카드 목록을 만든다.
		MakeList(indexCardBook, &indexCards, &count);
		//2.2.7. 단어카드철 윈도우의 트리 뷰 컨트롤에서 모든 항목을 지운다.
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK), TVM_DELETEITEM, (WPARAM)0, (LPARAM)TVI_ROOT);
		//2.2.8. 단어카드철 윈도우의 트리 뷰 컨트롤에 루트 항목을 추가한다.
		item.mask = TVIF_TEXT | TVIF_CHILDREN;
		item.pszText = "알파벳";
		item.cChildren = 1;
		insertItem.hParent = TVI_ROOT;
		insertItem.hInsertAfter = TVI_FIRST;
		insertItem.item = item;
		hRootItem = (HTREEITEM)SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK),
			TVM_INSERTITEM, (WPARAM)0, (LPARAM)&insertItem);
		//2.2.9. 개수만큼 반복한다.
		i = 0;
		while (i < count) {
			//2.2.9.1. 단어카드철 윈도우의 트리 뷰 컨트롤에 알파벳 항목을 추가한다.
			link = &(indexCards[i]);
			alphabet[0] = link->alphabet;
			alphabet[1] = '\0';
			item.pszText = alphabet;
			item.cChildren = 1;
			insertItem.hParent = hRootItem;
			insertItem.hInsertAfter = hIndexCardItem;
			insertItem.item = item;
			hIndexCardItem = (HTREEITEM)SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK),
				TVM_INSERTITEM, (WPARAM)0, (LPARAM)&insertItem);
			//2.2.9.2. 단어 개수만큼 반복한다.
			j = 0;
			while (j < link->length) {
				//2.2.9.2.1. 단어카드철 윈도우의 트리 뷰 컨트롤에 단어 항목을 추가한다.
				index = IndexCard_GetAt(link, j);
				item.pszText = index->word.spelling;
				item.cChildren = 1;
				insertItem.hParent = hIndexCardItem;
				insertItem.hInsertAfter = hWordItem;
				insertItem.item = item;
				hWordItem = (HTREEITEM)SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK),
					TVM_INSERTITEM, (WPARAM)0, (LPARAM)&insertItem);
				//2.2.9.2.2. 단어카드철 윈도우의 단어카드철에서 철자로 찾는다.
				wordCardBinder = (WordCardBinder*)GetWindowLong(hWordCardBinderFormWnd, GWL_USERDATA);
				FindBySpelling(wordCardBinder, index->word.spelling, &indexes, &indexCount);
				//2.2.9.2.3. 찾은 개수만큼 반복한다.
				k = 0;
				while (k < indexCount) {
					//2.2.9.2.3.1. 단어카드철 윈도우의 트리 뷰 컨트롤에 품사 항목을 추가한다.
					index = IndexCard_GetAt(link, j + k);
					item.pszText = index->word.wordClass;
					item.cChildren = 0;
					insertItem.hParent = hWordItem;
					insertItem.hInsertAfter = hWordClassItem;
					insertItem.item = item;
					hWordClassItem = (HTREEITEM)SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK),
						TVM_INSERTITEM, (WPARAM)0, (LPARAM)&insertItem);
					k++;
				}
				if (indexes != NULL) {
					free(indexes);
				}
				j += k;
			}
			i++;
		}
		if (indexCards != NULL) {
			free(indexCards);
		}
		//2.2.10. 단어카드철 윈도우의 단어카드철에서 마지막으로 이동한다.
		index = WordCardBinder_Last(wordCardBinder);
		//2.2.11. 단어카드철 윈도우의 단어에 쓴다.
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_EDIT_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.spelling);
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_EDIT_WORDCLASS), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.wordClass);
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_EDIT_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.meaning);
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_EDIT_EXAMPLESENTENCE), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.exampleSentence);
		//sprintf(correctRate, "%.1f", index->evaluation.correctRate);
		//SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_EDIT_CORRECTRATE), WM_SETTEXT, (WPARAM)0, (LPARAM)correctRate);
		//2.2.12. 단어카드철 윈도우의 색인카드집에서 찾는다
		link = IndexCardBook_Find(indexCardBook, index->word.spelling[0]);
		//2.2.13. 단어카드철 윈도우의 색인카드에서 찾는다.
		location = IndexCard_Find(link, index);
		firstLocation = IndexCard_FindFirstTime(link, index->word.spelling);
		//2.2.14. 단어카드철 윈도우의 트리 뷰 컨트롤에서 루트 항목을 펼친다.
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hRootItem);
		//2.2.15. 단어카드철 윈도우의 트리 뷰 컨트롤에서 알파벳 항목을 찾는다.
		compare[0] = link->alphabet;
		compare[1] = '\0';
		hIndexCardItem = (HTREEITEM)SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK),
			TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hRootItem);
		item.mask = TVIF_TEXT | TVIF_HANDLE;
		item.hItem = hIndexCardItem;
		item.pszText = alphabet;
		item.cchTextMax = 2;
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETITEM, (WPARAM)0, (LPARAM)&item);
		while (hIndexCardItem != NULL && strcmp(compare, alphabet) != 0) {
			hIndexCardItem = (HTREEITEM)SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETNEXTITEM,
				(WPARAM)TVGN_NEXT, (LPARAM)hIndexCardItem);
			item.hItem = hIndexCardItem;
			SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETITEM, (WPARAM)0, (LPARAM)&item);
		}
		//2.2.16. 단어카드철 윈도우의 트리 뷰 컨트롤에서 알파벳 항목을 펼친다.
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hIndexCardItem);
		//2.2.17. 단어카드철 윈도우의 트리 뷰 컨트롤의 단어 항목을 찾는다.
		strcpy(spelling, index->word.spelling);
		hWordItem = (HTREEITEM)SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK),
			TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hIndexCardItem);
		item.mask = TVIF_TEXT | TVIF_HANDLE;
		item.hItem = hWordItem;
		item.pszText = compareSpelling;
		item.cchTextMax = 16;
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETITEM, (WPARAM)0, (LPARAM)&item);
		while (hWordItem != NULL && strcmp(spelling, compareSpelling) != 0) {
			hWordItem = (HTREEITEM)SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETNEXTITEM,
				(WPARAM)TVGN_NEXT, (LPARAM)hWordItem);
			item.hItem = hWordItem;
			SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETITEM, (WPARAM)0, (LPARAM)&item);
		}
		//2.2.18. 단어카드철 윈도우의 트리 뷰 컨트롤에서 단어 항목을 펼친다.
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hWordItem);
		//2.2.19. 단어카드철 윈도우 트리 뷰 컨트롤의 단어 항목에서 이동한다.
		hWordClassItem = (HTREEITEM)SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK),
			TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hWordItem);
		i = 0;
		while (hWordClassItem != NULL && i < (location - firstLocation)) {
			hWordClassItem = (HTREEITEM)SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK),
				TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hWordClassItem);
			i++;
		}
		//2.2.20. 단어카드철 윈도우의 트리 뷰 컨트롤에서 품사 항목을 선택한다.
		item.mask = TVIF_STATE | TVIF_HANDLE;
		item.hItem = hWordClassItem;
		item.stateMask = TVIS_SELECTED;
		item.state = TVIS_SELECTED;
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK), TVM_SETITEM, (WPARAM)0, (LPARAM)&item);
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK), TVM_SELECTITEM, (WPARAM)TVGN_CARET, (LPARAM)hWordClassItem);
		EnableWindow(GetDlgItem(hWordCardBinderFormWnd, IDC_BUTTON_FIRST), TRUE);
		EnableWindow(GetDlgItem(hWordCardBinderFormWnd, IDC_BUTTON_PREVIOUS), TRUE);
		EnableWindow(GetDlgItem(hWordCardBinderFormWnd, IDC_BUTTON_NEXT), TRUE);
		EnableWindow(GetDlgItem(hWordCardBinderFormWnd, IDC_BUTTON_LAST), TRUE);
		EnableWindow(GetDlgItem(hWordCardBinderFormWnd, IDC_BUTTON_FIND), TRUE);
		EnableWindow(GetDlgItem(hWordCardBinderFormWnd, IDC_BUTTON_ARRANGE), TRUE);
		EnableWindow(GetDlgItem(hWordCardBinderFormWnd, IDC_BUTTON_TAKEOUT), TRUE);
		EnableWindow(GetDlgItem(hWordCardBinderFormWnd, IDC_BUTTON_QUIZ), TRUE);
		//2.2.21. 빼기 윈도우를 닫는다.
		EndDialog(hWnd, 0);
	}
	//2.3. 아니오 버튼을 클릭했을 때
	else if (messageBox == IDNO) {
		//2.3.1. 빼기윈도우를 닫는다.
		EndDialog(hWnd, 0);
	}
	return TRUE;
}