//FindingForm.c
/************************************************************************************************
파일명칭: FindingForm.c
기    능: 단어카드철 찾기 윈도우
작 성 자: 임다니엘
작성일자: 2022/07/15
*************************************************************************************************/
#include "FindingForm.h"
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
명    칭: FindingFormProc
기    능: 찾기 윈도우 프로시져함수.
*************************************************************************************************/
BOOL CALLBACK FindingFormProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	BOOL ret;

	switch (message) {
	case WM_INITDIALOG:
		ret = FindingForm_OnInitDialog(hWnd, wParam, lParam);
		break;
	case WM_COMMAND:
		ret = FindingForm_OnCommand(hWnd, wParam, lParam);
		break;
	case WM_CLOSE:
		ret = FindingForm_OnClose(hWnd, wParam, lParam);
		break;
	default:
		ret = FALSE;
		break;
	}
	return ret;
}
/************************************************************************************************
명    칭: FindingForm_OnInitDialog
기    능: 윈도우가 생성될 때 시나리오.
*************************************************************************************************/
BOOL FindingForm_OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	//1.1. 철자 라디오 버튼을 선택한다.
	SendMessage(GetDlgItem(hWnd, IDC_RADIO_SPELLING), BM_SETCHECK, (WPARAM)BST_CHECKED, (LPARAM)0);
	//1.2. 뜻을 비활성화 한다.
	EnableWindow(GetDlgItem(hWnd, IDC_EDIT_KEYMEANING), FALSE);

	return TRUE;
}
/************************************************************************************************
명    칭: FindingForm_OnCommand
기    능: WM_COMMAND 메세지 발생 시 시나리오.
*************************************************************************************************/
BOOL FindingForm_OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BOOL ret;

	switch (LOWORD(wParam)) {
	case IDC_RADIO_SPELLING:
		ret = FindingForm_OnSpellingRadioClicked(hWnd, wParam, lParam);
		break;
	case IDC_RADIO_MEANING:
		ret = FindingForm_OnMeaningRadioClicked(hWnd, wParam, lParam);
		break;
	case IDC_BUTTON_FIND:
		ret = FindingForm_OnFindButtonClicked(hWnd, wParam, lParam);
		break;
	case IDC_BUTTON_SELECT:
		ret = FindingForm_OnSelectButtonClicked(hWnd, wParam, lParam);
		break;
	case IDC_BUTTON_FIRST:
		ret = FindingForm_OnFirstButtonClicked(hWnd, wParam, lParam);
		break;
	case IDC_BUTTON_PREVIOUS:
		ret = FindingForm_OnPreviousButtonClicked(hWnd, wParam, lParam);
		break;
	case IDC_BUTTON_NEXT:
		ret = FindingForm_OnNextButtonClicked(hWnd, wParam, lParam);
		break;
	case IDC_BUTTON_LAST:
		ret = FindingForm_OnLastButtonClicked(hWnd, wParam, lParam);
		break;
	default:
		ret = FALSE;
		break;
	}
	return ret;
}
/************************************************************************************************
명    칭: FindingForm_OnSpellingRadioClicked
기    능: 철자 라디오버튼을 클릭했을 때 시나리오.
*************************************************************************************************/
BOOL FindingForm_OnSpellingRadioClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	if (HIWORD(wParam) == BN_CLICKED) {
		EnableWindow(GetDlgItem(hWnd, IDC_EDIT_KEYSPELLING), TRUE);
		SetFocus(GetDlgItem(hWnd, IDC_EDIT_KEYSPELLING));
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_KEYSPELLING), WM_ENABLE, (WPARAM)TRUE, (LPARAM)0);
		//2.1. 뜻 라디오버튼을 선택해제한다.
		SendMessage(GetDlgItem(hWnd, IDC_RADIO_MEANING), BM_SETCHECK, (WPARAM)BST_UNCHECKED, (LPARAM)0);
		//2.2. 뜻을 지운다.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_KEYMEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		//2.3. 뜻을 비활성화 한다.
		EnableWindow(GetDlgItem(hWnd, IDC_EDIT_KEYMEANING), FALSE);
	}
	return TRUE;
}
/************************************************************************************************
명    칭: FindingForm_OnMeaningRadioClicked
기    능: 뜻 라디오버튼을 클릭했을 때 시나리오.
*************************************************************************************************/
BOOL FindingForm_OnMeaningRadioClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	if (HIWORD(wParam) == BN_CLICKED) {
		EnableWindow(GetDlgItem(hWnd, IDC_EDIT_KEYMEANING), TRUE);
		SetFocus(GetDlgItem(hWnd, IDC_EDIT_KEYMEANING));
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_KEYMEANING), WM_ENABLE, (WPARAM)TRUE, (LPARAM)0);
		//3.1. 철자 라디오버튼을 선택해제한다.
		SendMessage(GetDlgItem(hWnd, IDC_RADIO_SPELLING), BM_SETCHECK, (WPARAM)BST_UNCHECKED, (LPARAM)0);
		//3.2. 철자를 지운다.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_KEYSPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		//3.3. 철자를 비활성화 한다.
		EnableWindow(GetDlgItem(hWnd, IDC_EDIT_KEYSPELLING), FALSE);
	}
	return TRUE;
}
/************************************************************************************************
명    칭: FindingForm_OnFindButtonClicked
기    능: 찾기 버튼을 클릭했을 때 시나리오.
*************************************************************************************************/
BOOL FindingForm_OnFindButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	WordCardBinder* wordCardBinder;
	WordCard* (*indexes) = NULL;
	WordCard* index;
	HWND hWordCardBinderFormWnd;
	Long count;
	Long i = 0;
	Long spellingRadio;
	Long meaningRadio;
	TCHAR spelling[16];
	TCHAR meaning[16];
	TCHAR correctRate[8];

	if (HIWORD(wParam) == BN_CLICKED) {
		//4.1. 조건을 읽는다.
		spellingRadio = SendMessage(GetDlgItem(hWnd, IDC_RADIO_SPELLING), BM_GETCHECK, 0, 0);
		meaningRadio = SendMessage(GetDlgItem(hWnd, IDC_RADIO_MEANING), BM_GETCHECK, 0, 0);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_KEYSPELLING), WM_GETTEXT, (WPARAM)16, (LPARAM)spelling);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_KEYMEANING), WM_GETTEXT, (WPARAM)16, (LPARAM)meaning);
		//4.2. 단어카드철 윈도우를 찾는다.
		hWordCardBinderFormWnd = FindWindow("#32770", "단어카드철");
		wordCardBinder = (WordCardBinder*)GetWindowLong(hWordCardBinderFormWnd, GWL_USERDATA);
		indexes = (WordCard * (*))GetWindowLong(hWnd, GWL_USERDATA);
		if (indexes != NULL) {
			free(indexes);
		}
		//4.3. 철자 라디오버튼이 선택되어 있으면.
		if (spellingRadio == BST_CHECKED) {
			//4.3.1. 단어카드철 윈도우의 단어카드철에서 철자로 찾는다.
			FindBySpelling(wordCardBinder, spelling, &indexes, &count);
		}
		//4.4. 뜻 라디오버튼이 선택되어 있으면,
		else if (meaningRadio == BST_CHECKED) {
			//4.4.1. 단어카드철 윈도우의 단어카드철에서 뜻으로 찾는다.
			FindByMeaning(wordCardBinder, meaning, &indexes, &count);
		}
		//4.5. 단어가 있으면
		SetWindowLong(hWnd, GWL_USERDATA, (LONG)indexes);
		if (count > 0) {
			SetProp(hWnd, "PROP_COUNT", (HANDLE)count);
			SetProp(hWnd, "PROP_CURRENT", (HANDLE)i);
			//4.5.1. 찾은 단어들 중 첫번째를 단어에 쓴다.
			index = indexes[i];
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.spelling);
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_WORDCLASS), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.wordClass);
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.meaning);
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_EXAMPLESENTENCE), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.exampleSentence);
			sprintf(correctRate, "%.1f", index->evaluation.correctRate);
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_CORRECTRATE), WM_SETTEXT, (WPARAM)0, (LPARAM)correctRate);
		}
		else {
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_WORDCLASS), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_EXAMPLESENTENCE), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_CORRECTRATE), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		}
	}
	return TRUE;
}
/************************************************************************************************
명    칭: FindingForm_OnSelectButtonClicked
기    능: 선택 버튼을 클릭했을 때 시나리오.
*************************************************************************************************/
BOOL FindingForm_OnSelectButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	HWND hWordCardBinderFormWnd;
	WordCardBinder* wordCardBinder;
	WordCard* (*indexes) = NULL;
	WordCard* index;
	IndexCardBook* indexCardBook;
	IndexCard* link;
	TVITEM item;
	HTREEITEM hRootItem;
	HTREEITEM hIndexCardItem;
	HTREEITEM hWordItem;
	HTREEITEM hWordClassItem;
	Long count;
	Long indexCount;
	Long location;
	Long firstLocation;
	Long i;
	TCHAR alphabet[2];
	TCHAR compare[2];
	TCHAR spelling[16];
	TCHAR compareSpelling[16];
	TCHAR correctRate[8];

	if (HIWORD(wParam) == BN_CLICKED) {
		//5.1. 단어카드철 윈도우를 찾는다.
		hWordCardBinderFormWnd = FindWindow("#32770", "단어카드철");
		//5.2. 단어카드철 윈도우의 단어카드철에서 이동한다.
		wordCardBinder = (WordCardBinder*)GetWindowLong(hWordCardBinderFormWnd, GWL_USERDATA);
		indexes = (WordCard * (*))GetWindowLong(hWnd, GWL_USERDATA);
		i = (Long)GetProp(hWnd, "PROP_CURRENT");
		index = WordCardBinder_Move(wordCardBinder, indexes[i]);
		//5.3. 단어카드철 윈도우에 쓴다.
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_EDIT_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.spelling);
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_EDIT_WORDCLASS), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.wordClass);
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_EDIT_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.meaning);
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_EDIT_EXAMPLESENTENCE), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.exampleSentence);
		//sprintf(correctRate, "%.1f", index->evaluation.correctRate);
		//SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_EDIT_CORRECTRATE), WM_SETTEXT, (WPARAM)0, (LPARAM)correctRate);
		//5.4. 단어카드철 윈도우의 색인카드집에서 찾는다.
		indexCardBook = (IndexCardBook*)GetProp(hWordCardBinderFormWnd, "PROP_INDEXCARDBOOK");
		link = IndexCardBook_Find(indexCardBook, index->word.spelling[0]);
		//5.5. 단어카드철 윈도우의 색인카드에서 찾는다.
		location = IndexCard_Find(link, index);
		firstLocation = IndexCard_FindFirstTime(link, index->word.spelling);
		//5.6. 단어카드철 윈도우의 트리 뷰 컨트롤에서 루트 항목을 펼친다.
		hRootItem = (HTREEITEM)SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK),
			TVM_GETNEXTITEM, (WPARAM)TVGN_ROOT, (LPARAM)0);
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hRootItem);
		//5.7. 단어카드철 윈도우의 트리 뷰 컨트롤에서 알파벳 항목을 찾는다.
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
		//5.8. 단어카드철 윈도우의 트리 뷰 컨트롤에서 알파벳 항목을 펼친다.
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hIndexCardItem);
		//5.9. 단어카드철 윈도우의 트리 뷰 컨트롤의 단어 항목을 찾는다.
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
		//5.10. 단어카드철 윈도우의 트리 뷰 컨트롤에서 단어 항목을 펼친다.
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hWordItem);
		//5.11. 단어카드철 윈도우 트리 뷰 컨트롤의 단어 항목에서 이동한다.
		hWordClassItem = (HTREEITEM)SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK),
			TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hWordItem);
		i = 0;
		while (hWordClassItem != NULL && i < (location - firstLocation)) {
			hWordClassItem = (HTREEITEM)SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK),
				TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hWordClassItem);
			i++;
		}
		//5.12. 단어카드철 윈도우의 트리 뷰 컨트롤에서 품사 항목을 선택한다.
		item.mask = TVIF_STATE | TVIF_HANDLE;
		item.hItem = hWordClassItem;
		item.stateMask = TVIS_SELECTED;
		item.state = TVIS_SELECTED;
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK), TVM_SETITEM, (WPARAM)0, (LPARAM)&item);
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK), TVM_SELECTITEM, (WPARAM)TVGN_CARET, (LPARAM)hWordClassItem);

		//5.13. 찾기 윈도우를 닫는다.
		if (indexes != NULL) {
			free(indexes);
		}
		RemoveProp(hWnd, "PROP_CURRENT");
		RemoveProp(hWnd, "PROP_COUNT");
		EndDialog(hWnd, 0);
	}
	return TRUE;
}
/************************************************************************************************
명    칭: FindingForm_OnFirstButtonClicked
기    능: 처음 버튼을 클릭했을 때 시나리오.
*************************************************************************************************/
BOOL FindingForm_OnFirstButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	WordCardBinder* wordCardBinder;
	WordCard* (*indexes);
	WordCard* index;
	Long i;
	TCHAR correctRate[8];

	if (HIWORD(wParam) == BN_CLICKED) {
		//6.1. 찾은 단어들 중 처음으로 이동한다.
		indexes = (WordCard * (*))GetWindowLong(hWnd, GWL_USERDATA);
		i = 0;
		index = indexes[i];
		SetProp(hWnd, "PROP_CURRENT", (HANDLE)i);
		//6.2. 단어에 쓴다.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.spelling);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_WORDCLASS), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.wordClass);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.meaning);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_EXAMPLESENTENCE), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.exampleSentence);
		//sprintf(correctRate, "%.1f", index->evaluation.correctRate);
		//SendMessage(GetDlgItem(hWnd, IDC_EDIT_CORRECTRATE), WM_SETTEXT, (WPARAM)0, (LPARAM)correctRate);
	}
	return TRUE;
}
/************************************************************************************************
명    칭: FindingForm_OnPreviousButtonClicked
기    능: 이전 버튼을 클릭했을 때 시나리오.
*************************************************************************************************/
BOOL FindingForm_OnPreviousButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	WordCardBinder* wordCardBinder;
	WordCard* (*indexes);
	WordCard* index;
	Long i;
	TCHAR correctRate[8];

	if (HIWORD(wParam) == BN_CLICKED) {
		//7.1. 찾은 단어들 중 이전으로 이동한다.
		indexes = (WordCard * (*))GetWindowLong(hWnd, GWL_USERDATA);
		i = (Long)GetProp(hWnd, "PROP_CURRENT");
		i--;
		if (i < 0) {
			i = 0;
		}
		index = indexes[i];
		SetProp(hWnd, "PROP_CURRENT", (HANDLE)i);
		//7.2. 단어에 쓴다.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.spelling);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_WORDCLASS), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.wordClass);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.meaning);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_EXAMPLESENTENCE), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.exampleSentence);
		//sprintf(correctRate, "%.1f", index->evaluation.correctRate);
		//SendMessage(GetDlgItem(hWnd, IDC_EDIT_CORRECTRATE), WM_SETTEXT, (WPARAM)0, (LPARAM)correctRate);
	}
	return TRUE;
}
/************************************************************************************************
명    칭: FindingForm_OnNextButtonClicked
기    능: 다음 버튼을 클릭했을 때 시나리오.
*************************************************************************************************/
BOOL FindingForm_OnNextButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	WordCardBinder* wordCardBinder;
	WordCard* (*indexes);
	WordCard* index;
	Long i;
	Long count;
	TCHAR correctRate[8];

	if (HIWORD(wParam) == BN_CLICKED) {
		//8.1. 찾은 단어들 중 다음으로 이동한다.
		indexes = (WordCard * (*))GetWindowLong(hWnd, GWL_USERDATA);
		i = (Long)GetProp(hWnd, "PROP_CURRENT");
		count = (Long)GetProp(hWnd, "PROP_COUNT");
		i++;
		if (i >= count) {
			i = count - 1;
		}
		index = indexes[i];
		SetProp(hWnd, "PROP_CURRENT", (HANDLE)i);
		//8.2. 단어에 쓴다.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.spelling);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_WORDCLASS), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.wordClass);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.meaning);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_EXAMPLESENTENCE), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.exampleSentence);
		//sprintf(correctRate, "%.1f", index->evaluation.correctRate);
		//SendMessage(GetDlgItem(hWnd, IDC_EDIT_CORRECTRATE), WM_SETTEXT, (WPARAM)0, (LPARAM)correctRate);
	}
	return TRUE;
}
/************************************************************************************************
명    칭: FindingForm_OnLastButtonClicked
기    능: 마지막 버튼을 클릭했을 때 시나리오.
*************************************************************************************************/
BOOL FindingForm_OnLastButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	WordCardBinder* wordCardBinder;
	WordCard* (*indexes);
	WordCard* index;
	Long i;
	Long count;
	TCHAR correctRate[8];

	if (HIWORD(wParam) == BN_CLICKED) {
		//9.1. 찾은 단어들 중 마지막으로 이동한다.
		indexes = (WordCard * (*))GetWindowLong(hWnd, GWL_USERDATA);
		count = (Long)GetProp(hWnd, "PROP_COUNT");
		i = count - 1;
		index = indexes[i];
		SetProp(hWnd, "PROP_CURRENT", (HANDLE)i);
		//9.2. 단어에 쓴다.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.spelling);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_WORDCLASS), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.wordClass);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.meaning);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_EXAMPLESENTENCE), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.exampleSentence);
		//sprintf(correctRate, "%.1f", index->evaluation.correctRate);
		//SendMessage(GetDlgItem(hWnd, IDC_EDIT_CORRECTRATE), WM_SETTEXT, (WPARAM)0, (LPARAM)correctRate);
	}
	return TRUE;
}
/************************************************************************************************
명    칭: FindingForm_OnClose
기    능: 닫기 버튼을 클릭했을 때 시나리오.
*************************************************************************************************/
BOOL FindingForm_OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	WordCard* (*indexes);

	indexes = (WordCard * (*))GetWindowLong(hWnd, GWL_USERDATA);
	if (indexes != NULL) {
		free(indexes);
	}
	RemoveProp(hWnd, "PROP_COUNT");
	RemoveProp(hWnd, "PROP_CURRENT");
	//10.1. 찾기 윈도우를 닫는다.
	EndDialog(hWnd, 0);

	return TRUE;
}