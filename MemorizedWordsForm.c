//MemorizedWordsForm.c
/************************************************************************************************
파일명칭: MemorizedWordsForm.c
기    능: 단어카드철 암기완료카드 윈도우
작 성 자: 임다니엘
작성일자: 2022/07/25
*************************************************************************************************/
#include "MemorizedWordsForm.h"
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
명    칭: MemorizedWordsFormProc
기    능: 암기완료카드 윈도우 프로시져함수
*************************************************************************************************/
BOOL CALLBACK MemorizedWordsFormProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	BOOL ret;

	switch (message) {
	case WM_INITDIALOG:
		ret = MemorizedWordsForm_OnInitDialog(hWnd, wParam, lParam);
		break;
	case WM_COMMAND:
		ret = MemorizedWordsForm_OnCommand(hWnd, wParam, lParam);
		break;
	case WM_CLOSE:
		ret = MemorizedWordsForm_OnClose(hWnd, wParam, lParam);
		break;
	default:
		ret = FALSE;
		break;
	}
	return ret;
}
/************************************************************************************************
명    칭: MemorizedWordsForm_OnInitDialog
기    능: 윈도우가 생성될 때 시나리오.
*************************************************************************************************/
BOOL MemorizedWordsForm_OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	HWND hWordCardBinderFormWnd;
	WordCardBinder* memorizedWords;
	WordCard* index;
	TCHAR correctRate[8];

	//1.1. 단어카드철 윈도우를 찾는다.
	hWordCardBinderFormWnd = FindWindow("#32770", "단어카드철");
	memorizedWords = (WordCardBinder*)GetProp(hWordCardBinderFormWnd, "PROP_MEMORIZEDWORDS");
	if (memorizedWords->length > 0) {
		//1.2. 단어카드철 윈도우의 암기완료 카드철에서 처음으로 이동한다.
		index = WordCardBinder_First(memorizedWords);
		//1.3. 단어에 쓴다.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.spelling);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_WORDCLASS), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.wordClass);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.meaning);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_EXAMPLESENTENCE), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.exampleSentence);
		//sprintf(correctRate, "%.1f %", index->evaluation.correctRate);
		//SendMessage(GetDlgItem(hWnd, IDC_EDIT_CORRECTRATE), WM_SETTEXT, (WPARAM)0, (LPARAM)correctRate);
	}
	else {
		EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_FIRST), FALSE);
		EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_PREVIOUS), FALSE);
		EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_NEXT), FALSE);
		EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_LAST), FALSE);
		EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_TAKEIN), FALSE);
		EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_TAKEINALL), FALSE);
	}
	return TRUE;
}
/************************************************************************************************
명    칭: MemorizedWordsForm_OnCommand
기    능: WM_COMMAND 메세지 발생 시 시나리오.
*************************************************************************************************/
BOOL MemorizedWordsForm_OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BOOL ret;

	switch (LOWORD(wParam)) {
	case IDC_BUTTON_TAKEIN:
		ret = MemorizedWordsForm_OnTakeInButtonClicked(hWnd, wParam, lParam);
		break;
	case IDC_BUTTON_TAKEINALL:
		ret = MemorizedWordsForm_OnTakeInAllButtonClicked(hWnd, wParam, lParam);
		break;
	case IDC_BUTTON_FIRST:
		ret = MemorizedWordsForm_OnFirstButtonClicked(hWnd, wParam, lParam);
		break;
	case IDC_BUTTON_PREVIOUS:
		ret = MemorizedWordsForm_OnPreviousButtonClicked(hWnd, wParam, lParam);
		break;
	case IDC_BUTTON_NEXT:
		ret = MemorizedWordsForm_OnNextButtonClicked(hWnd, wParam, lParam);
		break;
	case IDC_BUTTON_LAST:
		ret = MemorizedWordsForm_OnLastButtonClicked(hWnd, wParam, lParam);
		break;
	default:
		ret = FALSE;
		break;
	}
	return ret;
}
/************************************************************************************************
명    칭: MemorizedWordsForm_OnTakeInButtonClicked
기    능: 끼우기 버튼을 클릭했을 때 시나리오.
*************************************************************************************************/
BOOL MemorizedWordsForm_OnTakeInButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	HWND hWordCardBinderFormWnd;
	WordCardBinder* wordCardBinder;
	WordCardBinder* memorizedWords;
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
	TCHAR alphabet[2];
	TCHAR compare[2];
	TCHAR spelling[16];
	TCHAR compareSpelling[16];
	TCHAR correctRate[8];
	Long count;
	Long indexCount;
	Long location;
	Long firstLocation;
	Long i;
	Long j;
	Long k;

	if (HIWORD(wParam) == BN_CLICKED) {
		//2.1. 단어카드철 윈도우를 찾는다.
		hWordCardBinderFormWnd = FindWindow("#32770", "단어카드철");
		wordCardBinder = (WordCardBinder*)GetWindowLong(hWordCardBinderFormWnd, GWL_USERDATA);
		memorizedWords = (WordCardBinder*)GetProp(hWordCardBinderFormWnd, "PROP_MEMORIZEDWORDS");
		indexCardBook = (IndexCardBook*)GetProp(hWordCardBinderFormWnd, "PROP_INDEXCARDBOOK");
		//2.2. 단어카드철 윈도우의 암기완료 카드철에서 뺀다.
		wordCard = TakeOut(memorizedWords, memorizedWords->current);
		//2.3. 단어카드철 윈도우의 단어카드철에서 끼운다.
		index = TakeIn(wordCardBinder, wordCard);
		//2.4. 단어카드철 윈도우의 색인카드집에서 끼운다.
		link = IndexCardBook_TakeIn(indexCardBook, index);
		//2.5. 단어카드철 윈도우의 색인카드집에서 정리한다.
		IndexCardBook_Arrange(indexCardBook);
		//2.6. 단어카드철 윈도우에서 색인카드 목록을 만든다.
		MakeList(indexCardBook, &indexCards, &count);
		//2.7. 단어카드철 윈도우의 트리 뷰 컨트롤에서 모든 항목을 지운다.
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK), TVM_DELETEITEM, (WPARAM)0, (LPARAM)TVI_ROOT);
		//2.8. 단어카드철 윈도우의 트리 뷰 컨트롤에 루트 항목을 추가한다.
		item.mask = TVIF_TEXT | TVIF_CHILDREN;
		item.pszText = "알파벳";
		item.cChildren = 1;
		insertItem.hParent = TVI_ROOT;
		insertItem.hInsertAfter = TVI_FIRST;
		insertItem.item = item;
		hRootItem = (HTREEITEM)SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK),
			TVM_INSERTITEM, (WPARAM)0, (LPARAM)&insertItem);
		//2.9. 개수만큼 반복한다.
		i = 0;
		while (i < count) {
			//2.9.1. 단어카드철 윈도우의 트리 뷰 컨트롤에 알파벳 항목을 추가한다.
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
			//2.9.2. 단어 개수만큼 반복한다.
			j = 0;
			while (j < link->length) {
				//2.9.2.1. 단어카드철 윈도우의 트리 뷰 컨트롤에 단어 항목을 추가한다.
				index = IndexCard_GetAt(link, j);
				item.pszText = index->word.spelling;
				item.cChildren = 1;
				insertItem.hParent = hIndexCardItem;
				insertItem.hInsertAfter = hWordItem;
				insertItem.item = item;
				hWordItem = (HTREEITEM)SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK),
					TVM_INSERTITEM, (WPARAM)0, (LPARAM)&insertItem);
				//2.9.2.2. 단어카드철 윈도우의 단어카드철에서 철자로 찾는다.
				wordCardBinder = (WordCardBinder*)GetWindowLong(hWordCardBinderFormWnd, GWL_USERDATA);
				FindBySpelling(wordCardBinder, index->word.spelling, &indexes, &indexCount);
				//2.9.2.3. 찾은 개수만큼 반복한다.
				k = 0;
				while (k < indexCount) {
					//2.9.2.3.1. 단어카드철 윈도우의 트리 뷰 컨트롤에 품사 항목을 추가한다.
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
		//2.10. 단어카드철 윈도우의 단어카드철에서 마지막으로 이동한다.
		index = WordCardBinder_Last(wordCardBinder);
		//2.11. 단어카드철 윈도우의 단어에 쓴다.
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_EDIT_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.spelling);
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_EDIT_WORDCLASS), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.wordClass);
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_EDIT_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.meaning);
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_EDIT_EXAMPLESENTENCE), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.exampleSentence);
		//sprintf(correctRate, "%.1f", index->evaluation.correctRate);
		//SendMessage(GetDlgItem(hWnd, IDC_EDIT_CORRECTRATE), WM_SETTEXT, (WPARAM)0, (LPARAM)correctRate);
		//2.12. 단어카드철 윈도우의 색인카드집에서 찾는다
		link = IndexCardBook_Find(indexCardBook, index->word.spelling[0]);
		//2.13. 단어카드철 윈도우의 색인카드에서 찾는다.
		location = IndexCard_Find(link, index);
		firstLocation = IndexCard_FindFirstTime(link, index->word.spelling);
		//2.14. 단어카드철 윈도우의 트리 뷰 컨트롤에서 루트 항목을 펼친다.
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hRootItem);
		//2.15. 단어카드철 윈도우의 트리 뷰 컨트롤에서 알파벳 항목을 찾는다.
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
		//2.16. 단어카드철 윈도우의 트리 뷰 컨트롤에서 알파벳 항목을 펼친다.
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hIndexCardItem);
		//2.17. 단어카드철 윈도우의 트리 뷰 컨트롤의 단어 항목을 찾는다.
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
		//2.18. 단어카드철 윈도우의 트리 뷰 컨트롤에서 단어 항목을 펼친다.
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hWordItem);
		//2.19. 단어카드철 윈도우 트리 뷰 컨트롤의 단어 항목에서 이동한다.
		hWordClassItem = (HTREEITEM)SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK),
			TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hWordItem);
		i = 0;
		while (hWordClassItem != NULL && i < (location - firstLocation)) {
			hWordClassItem = (HTREEITEM)SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK),
				TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hWordClassItem);
			i++;
		}
		//2.20. 단어카드철 윈도우의 트리 뷰 컨트롤에서 품사 항목을 선택한다.
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
		//2.21. 암기완료 카드 윈도우를 닫는다.
		EndDialog(hWnd, 0);
	}
	return TRUE;
}
/************************************************************************************************
명    칭: MemorizedWordsForm_OnTakeInAllButtonClicked
기    능: 모두끼우기 버튼을 클릭했을 때 시나리오.
*************************************************************************************************/
BOOL MemorizedWordsForm_OnTakeInAllButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	HWND hWordCardBinderFormWnd;
	WordCardBinder* wordCardBinder;
	WordCardBinder* memorizedWords;
	WordCard* index;
	WordCard* previous = NULL;
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
	TCHAR alphabet[2];
	TCHAR compare[2];
	TCHAR spelling[16];
	TCHAR compareSpelling[16];
	TCHAR correctRate[8];
	Long count;
	Long indexCount;
	Long location;
	Long firstLocation;
	Long i;
	Long j;
	Long k;

	if (HIWORD(wParam) == BN_CLICKED) {
		//3.1. 단어카드철 윈도우를 찾는다.
		hWordCardBinderFormWnd = FindWindow("#32770", "단어카드철");
		wordCardBinder = (WordCardBinder*)GetWindowLong(hWordCardBinderFormWnd, GWL_USERDATA);
		memorizedWords = (WordCardBinder*)GetProp(hWordCardBinderFormWnd, "PROP_MEMORIZEDWORDS");
		indexCardBook = (IndexCardBook*)GetProp(hWordCardBinderFormWnd, "PROP_INDEXCARDBOOK");
		//3.2. 개수만큼 반복한다.
		index = WordCardBinder_First(memorizedWords);
		while (index != NULL && index != previous) {
			previous = index;
			//3.2.1. 단어카드철 윈도우의 완료카드철에서 뺀다.
			wordCard = TakeOut(memorizedWords, index);
			//3.2.2. 단어카드철 윈도우의 단어카드철에서 끼운다.
			index = TakeIn(wordCardBinder, wordCard);
			//3.2.3. 단어카드철 윈도우의 색인카드집에서 끼운다.
			IndexCardBook_TakeIn(indexCardBook, index);
			if (memorizedWords->length > 0) {
				index = WordCardBinder_First(memorizedWords);
			}
			else {
				index = NULL;
			}
		}
		//3.3. 단어카드철 윈도우의 색인카드집에서 정리한다.
		IndexCardBook_Arrange(indexCardBook);
		//3.4. 단어카드철 윈도우에서 색인카드 목록을 만든다.
		MakeList(indexCardBook, &indexCards, &count);
		//3.5. 단어카드철 윈도우의 트리 뷰 컨트롤에서 모든 항목을 지운다.
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK), TVM_DELETEITEM, (WPARAM)0, (LPARAM)TVI_ROOT);
		//3.6. 단어카드철 윈도우의 트리 뷰 컨트롤에 루트 항목을 추가한다.
		item.mask = TVIF_TEXT | TVIF_CHILDREN;
		item.pszText = "알파벳";
		item.cChildren = 1;
		insertItem.hParent = TVI_ROOT;
		insertItem.hInsertAfter = TVI_FIRST;
		insertItem.item = item;
		hRootItem = (HTREEITEM)SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK),
			TVM_INSERTITEM, (WPARAM)0, (LPARAM)&insertItem);
		//3.7. 개수만큼 반복한다.
		i = 0;
		while (i < count) {
			//3.7.1. 단어카드철 윈도우의 트리 뷰 컨트롤에 알파벳 항목을 추가한다.
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
			//3.7.2. 단어 개수만큼 반복한다.
			j = 0;
			while (j < link->length) {
				//3.7.2.1. 단어카드철 윈도우의 트리 뷰 컨트롤에 단어 항목을 추가한다.
				index = IndexCard_GetAt(link, j);
				item.pszText = index->word.spelling;
				item.cChildren = 1;
				insertItem.hParent = hIndexCardItem;
				insertItem.hInsertAfter = hWordItem;
				insertItem.item = item;
				hWordItem = (HTREEITEM)SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK),
					TVM_INSERTITEM, (WPARAM)0, (LPARAM)&insertItem);
				//3.7.2.2. 단어카드철 윈도우의 단어카드철에서 철자로 찾는다.
				wordCardBinder = (WordCardBinder*)GetWindowLong(hWordCardBinderFormWnd, GWL_USERDATA);
				FindBySpelling(wordCardBinder, index->word.spelling, &indexes, &indexCount);
				//3.7.2.3. 찾은 개수만큼 반복한다.
				k = 0;
				while (k < indexCount) {
					//3.7.2.3.1. 단어카드철 윈도우의 트리 뷰 컨트롤에 품사 항목을 추가한다.
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
		//3.8. 단어카드철 윈도우의 단어카드철에서 마지막으로 이동한다.
		index = WordCardBinder_Last(wordCardBinder);
		//3.9. 단어카드철 윈도우의 단어에 쓴다.
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_EDIT_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.spelling);
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_EDIT_WORDCLASS), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.wordClass);
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_EDIT_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.meaning);
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_EDIT_EXAMPLESENTENCE), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.exampleSentence);
		//sprintf(correctRate, "%.1f", index->evaluation.correctRate);
		//SendMessage(GetDlgItem(hWnd, IDC_EDIT_CORRECTRATE), WM_SETTEXT, (WPARAM)0, (LPARAM)correctRate);
		//3.10. 단어카드철 윈도우의 색인카드집에서 찾는다
		link = IndexCardBook_Find(indexCardBook, index->word.spelling[0]);
		//3.11. 단어카드철 윈도우의 색인카드에서 찾는다.
		location = IndexCard_Find(link, index);
		firstLocation = IndexCard_FindFirstTime(link, index->word.spelling);
		//3.12. 단어카드철 윈도우의 트리 뷰 컨트롤에서 루트 항목을 펼친다.
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hRootItem);
		//3.13. 단어카드철 윈도우의 트리 뷰 컨트롤에서 알파벳 항목을 찾는다.
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
		//3.14. 단어카드철 윈도우의 트리 뷰 컨트롤에서 알파벳 항목을 펼친다.
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hIndexCardItem);
		//3.15. 단어카드철 윈도우의 트리 뷰 컨트롤의 단어 항목을 찾는다.
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
		//3.16. 단어카드철 윈도우의 트리 뷰 컨트롤에서 단어 항목을 펼친다.
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hWordItem);
		//3.17. 단어카드철 윈도우 트리 뷰 컨트롤의 단어 항목에서 이동한다.
		hWordClassItem = (HTREEITEM)SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK),
			TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hWordItem);
		i = 0;
		while (hWordClassItem != NULL && i < (location - firstLocation)) {
			hWordClassItem = (HTREEITEM)SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK),
				TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hWordClassItem);
			i++;
		}
		//3.18. 단어카드철 윈도우의 트리 뷰 컨트롤에서 품사 항목을 선택한다.
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
		//3.19. 암기완료 카드 윈도우를 닫는다.
		EndDialog(hWnd, 0);
	}

	return TRUE;
}
/************************************************************************************************
명    칭: MemorizedWordsForm_OnFirstButtonClicked
기    능: 처음 버튼을 클릭했을 때 시나리오.
*************************************************************************************************/
BOOL MemorizedWordsForm_OnFirstButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	HWND hWordCardBinderFormWnd;
	WordCardBinder* memorizedWords;
	WordCard* index;
	TCHAR correctRate[8];

	if (HIWORD(wParam) == BN_CLICKED) {
		//4.1. 단어카드철 윈도우를 찾는다.
		hWordCardBinderFormWnd = FindWindow("#32770", "단어카드철");
		memorizedWords = (WordCardBinder*)GetProp(hWordCardBinderFormWnd, "PROP_MEMORIZEDWORDS");
		//4.2. 단어카드철 윈도우의 암기완료 카드철에서 처음으로 이동한다.
		index = WordCardBinder_First(memorizedWords);
		//4.3. 단어에 쓴다.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.spelling);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_WORDCLASS), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.wordClass);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.meaning);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_EXAMPLESENTENCE), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.exampleSentence);
		//sprintf(correctRate, "%.1f %", index->evaluation.correctRate);
		//SendMessage(GetDlgItem(hWnd, IDC_EDIT_CORRECTRATE), WM_SETTEXT, (WPARAM)0, (LPARAM)correctRate);
	}
	return TRUE;
}
/************************************************************************************************
명    칭: MemorizedWordsForm_OnPreviousButtonClicked
기    능: 이전 버튼을 클릭했을 때 시나리오.
*************************************************************************************************/
BOOL MemorizedWordsForm_OnPreviousButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	HWND hWordCardBinderFormWnd;
	WordCardBinder* memorizedWords;
	WordCard* index;
	TCHAR correctRate[8];

	if (HIWORD(wParam) == BN_CLICKED) {
		//5.1. 단어카드철 윈도우를 찾는다.
		hWordCardBinderFormWnd = FindWindow("#32770", "단어카드철");
		memorizedWords = (WordCardBinder*)GetProp(hWordCardBinderFormWnd, "PROP_MEMORIZEDWORDS");
		//5.2. 단어카드철 윈도우의 암기완료 카드철에서 이전으로 이동한다.
		index = WordCardBinder_Previous(memorizedWords);
		//5.3. 단어에 쓴다.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.spelling);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_WORDCLASS), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.wordClass);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.meaning);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_EXAMPLESENTENCE), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.exampleSentence);
		//sprintf(correctRate, "%.1f %", index->evaluation.correctRate);
		//SendMessage(GetDlgItem(hWnd, IDC_EDIT_CORRECTRATE), WM_SETTEXT, (WPARAM)0, (LPARAM)correctRate);
	}
	return TRUE;
}
/************************************************************************************************
명    칭: MemorizedWordsForm_OnNextButtonClicked
기    능: 다음 버튼을 클릭했을 때 시나리오.
*************************************************************************************************/
BOOL MemorizedWordsForm_OnNextButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	HWND hWordCardBinderFormWnd;
	WordCardBinder* memorizedWords;
	WordCard* index;
	TCHAR correctRate[8];

	if (HIWORD(wParam) == BN_CLICKED) {
		//6.1. 단어카드철 윈도우를 찾는다.
		hWordCardBinderFormWnd = FindWindow("#32770", "단어카드철");
		memorizedWords = (WordCardBinder*)GetProp(hWordCardBinderFormWnd, "PROP_MEMORIZEDWORDS");
		//6.2. 단어카드철 윈도우의 암기완료 카드철에서 다음으로 이동한다.
		index = WordCardBinder_Next(memorizedWords);
		//6.3. 단어에 쓴다.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.spelling);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_WORDCLASS), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.wordClass);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.meaning);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_EXAMPLESENTENCE), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.exampleSentence);
		//sprintf(correctRate, "%.1f %", index->evaluation.correctRate);
		//SendMessage(GetDlgItem(hWnd, IDC_EDIT_CORRECTRATE), WM_SETTEXT, (WPARAM)0, (LPARAM)correctRate);
	}
	return TRUE;
}
/************************************************************************************************
명    칭: MemorizedWordsForm_OnLastButtonClicked
기    능: 마지막 버튼을 클릭했을 때 시나리오.
*************************************************************************************************/
BOOL MemorizedWordsForm_OnLastButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	HWND hWordCardBinderFormWnd;
	WordCardBinder* memorizedWords;
	WordCard* index;
	TCHAR correctRate[8];

	if (HIWORD(wParam) == BN_CLICKED) {
		//7.1. 단어카드철 윈도우를 찾는다.
		hWordCardBinderFormWnd = FindWindow("#32770", "단어카드철");
		memorizedWords = (WordCardBinder*)GetProp(hWordCardBinderFormWnd, "PROP_MEMORIZEDWORDS");
		//7.2. 단어카드철 윈도우의 암기완료 카드철에서 마지막으로 이동한다.
		index = WordCardBinder_Last(memorizedWords);
		//7.3. 단어에 쓴다.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.spelling);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_WORDCLASS), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.wordClass);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.meaning);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_EXAMPLESENTENCE), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.exampleSentence);
		//sprintf(correctRate, "%.1f %", index->evaluation.correctRate);
		//SendMessage(GetDlgItem(hWnd, IDC_EDIT_CORRECTRATE), WM_SETTEXT, (WPARAM)0, (LPARAM)correctRate);
	}
	return TRUE;
}
/************************************************************************************************
명    칭: MemorizedWordsForm_OnClose
기    능: 닫기 버튼을 클릭했을 때 시나리오.
*************************************************************************************************/
BOOL MemorizedWordsForm_OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	EndDialog(hWnd, 0);

	return TRUE;
}