//FindingForm.c
/************************************************************************************************
���ϸ�Ī: FindingForm.c
��    ��: �ܾ�ī��ö ã�� ������
�� �� ��: �ӴٴϿ�
�ۼ�����: 2022/07/15
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
��    Ī: FindingFormProc
��    ��: ã�� ������ ���ν����Լ�.
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
��    Ī: FindingForm_OnInitDialog
��    ��: �����찡 ������ �� �ó�����.
*************************************************************************************************/
BOOL FindingForm_OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	//1.1. ö�� ���� ��ư�� �����Ѵ�.
	SendMessage(GetDlgItem(hWnd, IDC_RADIO_SPELLING), BM_SETCHECK, (WPARAM)BST_CHECKED, (LPARAM)0);
	//1.2. ���� ��Ȱ��ȭ �Ѵ�.
	EnableWindow(GetDlgItem(hWnd, IDC_EDIT_KEYMEANING), FALSE);

	return TRUE;
}
/************************************************************************************************
��    Ī: FindingForm_OnCommand
��    ��: WM_COMMAND �޼��� �߻� �� �ó�����.
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
��    Ī: FindingForm_OnSpellingRadioClicked
��    ��: ö�� ������ư�� Ŭ������ �� �ó�����.
*************************************************************************************************/
BOOL FindingForm_OnSpellingRadioClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	if (HIWORD(wParam) == BN_CLICKED) {
		EnableWindow(GetDlgItem(hWnd, IDC_EDIT_KEYSPELLING), TRUE);
		SetFocus(GetDlgItem(hWnd, IDC_EDIT_KEYSPELLING));
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_KEYSPELLING), WM_ENABLE, (WPARAM)TRUE, (LPARAM)0);
		//2.1. �� ������ư�� ���������Ѵ�.
		SendMessage(GetDlgItem(hWnd, IDC_RADIO_MEANING), BM_SETCHECK, (WPARAM)BST_UNCHECKED, (LPARAM)0);
		//2.2. ���� �����.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_KEYMEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		//2.3. ���� ��Ȱ��ȭ �Ѵ�.
		EnableWindow(GetDlgItem(hWnd, IDC_EDIT_KEYMEANING), FALSE);
	}
	return TRUE;
}
/************************************************************************************************
��    Ī: FindingForm_OnMeaningRadioClicked
��    ��: �� ������ư�� Ŭ������ �� �ó�����.
*************************************************************************************************/
BOOL FindingForm_OnMeaningRadioClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	if (HIWORD(wParam) == BN_CLICKED) {
		EnableWindow(GetDlgItem(hWnd, IDC_EDIT_KEYMEANING), TRUE);
		SetFocus(GetDlgItem(hWnd, IDC_EDIT_KEYMEANING));
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_KEYMEANING), WM_ENABLE, (WPARAM)TRUE, (LPARAM)0);
		//3.1. ö�� ������ư�� ���������Ѵ�.
		SendMessage(GetDlgItem(hWnd, IDC_RADIO_SPELLING), BM_SETCHECK, (WPARAM)BST_UNCHECKED, (LPARAM)0);
		//3.2. ö�ڸ� �����.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_KEYSPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
		//3.3. ö�ڸ� ��Ȱ��ȭ �Ѵ�.
		EnableWindow(GetDlgItem(hWnd, IDC_EDIT_KEYSPELLING), FALSE);
	}
	return TRUE;
}
/************************************************************************************************
��    Ī: FindingForm_OnFindButtonClicked
��    ��: ã�� ��ư�� Ŭ������ �� �ó�����.
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
		//4.1. ������ �д´�.
		spellingRadio = SendMessage(GetDlgItem(hWnd, IDC_RADIO_SPELLING), BM_GETCHECK, 0, 0);
		meaningRadio = SendMessage(GetDlgItem(hWnd, IDC_RADIO_MEANING), BM_GETCHECK, 0, 0);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_KEYSPELLING), WM_GETTEXT, (WPARAM)16, (LPARAM)spelling);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_KEYMEANING), WM_GETTEXT, (WPARAM)16, (LPARAM)meaning);
		//4.2. �ܾ�ī��ö �����츦 ã�´�.
		hWordCardBinderFormWnd = FindWindow("#32770", "�ܾ�ī��ö");
		wordCardBinder = (WordCardBinder*)GetWindowLong(hWordCardBinderFormWnd, GWL_USERDATA);
		indexes = (WordCard * (*))GetWindowLong(hWnd, GWL_USERDATA);
		if (indexes != NULL) {
			free(indexes);
		}
		//4.3. ö�� ������ư�� ���õǾ� ������.
		if (spellingRadio == BST_CHECKED) {
			//4.3.1. �ܾ�ī��ö �������� �ܾ�ī��ö���� ö�ڷ� ã�´�.
			FindBySpelling(wordCardBinder, spelling, &indexes, &count);
		}
		//4.4. �� ������ư�� ���õǾ� ������,
		else if (meaningRadio == BST_CHECKED) {
			//4.4.1. �ܾ�ī��ö �������� �ܾ�ī��ö���� ������ ã�´�.
			FindByMeaning(wordCardBinder, meaning, &indexes, &count);
		}
		//4.5. �ܾ ������
		SetWindowLong(hWnd, GWL_USERDATA, (LONG)indexes);
		if (count > 0) {
			SetProp(hWnd, "PROP_COUNT", (HANDLE)count);
			SetProp(hWnd, "PROP_CURRENT", (HANDLE)i);
			//4.5.1. ã�� �ܾ�� �� ù��°�� �ܾ ����.
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
��    Ī: FindingForm_OnSelectButtonClicked
��    ��: ���� ��ư�� Ŭ������ �� �ó�����.
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
		//5.1. �ܾ�ī��ö �����츦 ã�´�.
		hWordCardBinderFormWnd = FindWindow("#32770", "�ܾ�ī��ö");
		//5.2. �ܾ�ī��ö �������� �ܾ�ī��ö���� �̵��Ѵ�.
		wordCardBinder = (WordCardBinder*)GetWindowLong(hWordCardBinderFormWnd, GWL_USERDATA);
		indexes = (WordCard * (*))GetWindowLong(hWnd, GWL_USERDATA);
		i = (Long)GetProp(hWnd, "PROP_CURRENT");
		index = WordCardBinder_Move(wordCardBinder, indexes[i]);
		//5.3. �ܾ�ī��ö �����쿡 ����.
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_EDIT_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.spelling);
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_EDIT_WORDCLASS), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.wordClass);
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_EDIT_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.meaning);
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_EDIT_EXAMPLESENTENCE), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.exampleSentence);
		//sprintf(correctRate, "%.1f", index->evaluation.correctRate);
		//SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_EDIT_CORRECTRATE), WM_SETTEXT, (WPARAM)0, (LPARAM)correctRate);
		//5.4. �ܾ�ī��ö �������� ����ī�������� ã�´�.
		indexCardBook = (IndexCardBook*)GetProp(hWordCardBinderFormWnd, "PROP_INDEXCARDBOOK");
		link = IndexCardBook_Find(indexCardBook, index->word.spelling[0]);
		//5.5. �ܾ�ī��ö �������� ����ī�忡�� ã�´�.
		location = IndexCard_Find(link, index);
		firstLocation = IndexCard_FindFirstTime(link, index->word.spelling);
		//5.6. �ܾ�ī��ö �������� Ʈ�� �� ��Ʈ�ѿ��� ��Ʈ �׸��� ��ģ��.
		hRootItem = (HTREEITEM)SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK),
			TVM_GETNEXTITEM, (WPARAM)TVGN_ROOT, (LPARAM)0);
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hRootItem);
		//5.7. �ܾ�ī��ö �������� Ʈ�� �� ��Ʈ�ѿ��� ���ĺ� �׸��� ã�´�.
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
		//5.8. �ܾ�ī��ö �������� Ʈ�� �� ��Ʈ�ѿ��� ���ĺ� �׸��� ��ģ��.
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hIndexCardItem);
		//5.9. �ܾ�ī��ö �������� Ʈ�� �� ��Ʈ���� �ܾ� �׸��� ã�´�.
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
		//5.10. �ܾ�ī��ö �������� Ʈ�� �� ��Ʈ�ѿ��� �ܾ� �׸��� ��ģ��.
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hWordItem);
		//5.11. �ܾ�ī��ö ������ Ʈ�� �� ��Ʈ���� �ܾ� �׸񿡼� �̵��Ѵ�.
		hWordClassItem = (HTREEITEM)SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK),
			TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hWordItem);
		i = 0;
		while (hWordClassItem != NULL && i < (location - firstLocation)) {
			hWordClassItem = (HTREEITEM)SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK),
				TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hWordClassItem);
			i++;
		}
		//5.12. �ܾ�ī��ö �������� Ʈ�� �� ��Ʈ�ѿ��� ǰ�� �׸��� �����Ѵ�.
		item.mask = TVIF_STATE | TVIF_HANDLE;
		item.hItem = hWordClassItem;
		item.stateMask = TVIS_SELECTED;
		item.state = TVIS_SELECTED;
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK), TVM_SETITEM, (WPARAM)0, (LPARAM)&item);
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK), TVM_SELECTITEM, (WPARAM)TVGN_CARET, (LPARAM)hWordClassItem);

		//5.13. ã�� �����츦 �ݴ´�.
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
��    Ī: FindingForm_OnFirstButtonClicked
��    ��: ó�� ��ư�� Ŭ������ �� �ó�����.
*************************************************************************************************/
BOOL FindingForm_OnFirstButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	WordCardBinder* wordCardBinder;
	WordCard* (*indexes);
	WordCard* index;
	Long i;
	TCHAR correctRate[8];

	if (HIWORD(wParam) == BN_CLICKED) {
		//6.1. ã�� �ܾ�� �� ó������ �̵��Ѵ�.
		indexes = (WordCard * (*))GetWindowLong(hWnd, GWL_USERDATA);
		i = 0;
		index = indexes[i];
		SetProp(hWnd, "PROP_CURRENT", (HANDLE)i);
		//6.2. �ܾ ����.
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
��    Ī: FindingForm_OnPreviousButtonClicked
��    ��: ���� ��ư�� Ŭ������ �� �ó�����.
*************************************************************************************************/
BOOL FindingForm_OnPreviousButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	WordCardBinder* wordCardBinder;
	WordCard* (*indexes);
	WordCard* index;
	Long i;
	TCHAR correctRate[8];

	if (HIWORD(wParam) == BN_CLICKED) {
		//7.1. ã�� �ܾ�� �� �������� �̵��Ѵ�.
		indexes = (WordCard * (*))GetWindowLong(hWnd, GWL_USERDATA);
		i = (Long)GetProp(hWnd, "PROP_CURRENT");
		i--;
		if (i < 0) {
			i = 0;
		}
		index = indexes[i];
		SetProp(hWnd, "PROP_CURRENT", (HANDLE)i);
		//7.2. �ܾ ����.
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
��    Ī: FindingForm_OnNextButtonClicked
��    ��: ���� ��ư�� Ŭ������ �� �ó�����.
*************************************************************************************************/
BOOL FindingForm_OnNextButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	WordCardBinder* wordCardBinder;
	WordCard* (*indexes);
	WordCard* index;
	Long i;
	Long count;
	TCHAR correctRate[8];

	if (HIWORD(wParam) == BN_CLICKED) {
		//8.1. ã�� �ܾ�� �� �������� �̵��Ѵ�.
		indexes = (WordCard * (*))GetWindowLong(hWnd, GWL_USERDATA);
		i = (Long)GetProp(hWnd, "PROP_CURRENT");
		count = (Long)GetProp(hWnd, "PROP_COUNT");
		i++;
		if (i >= count) {
			i = count - 1;
		}
		index = indexes[i];
		SetProp(hWnd, "PROP_CURRENT", (HANDLE)i);
		//8.2. �ܾ ����.
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
��    Ī: FindingForm_OnLastButtonClicked
��    ��: ������ ��ư�� Ŭ������ �� �ó�����.
*************************************************************************************************/
BOOL FindingForm_OnLastButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	WordCardBinder* wordCardBinder;
	WordCard* (*indexes);
	WordCard* index;
	Long i;
	Long count;
	TCHAR correctRate[8];

	if (HIWORD(wParam) == BN_CLICKED) {
		//9.1. ã�� �ܾ�� �� ���������� �̵��Ѵ�.
		indexes = (WordCard * (*))GetWindowLong(hWnd, GWL_USERDATA);
		count = (Long)GetProp(hWnd, "PROP_COUNT");
		i = count - 1;
		index = indexes[i];
		SetProp(hWnd, "PROP_CURRENT", (HANDLE)i);
		//9.2. �ܾ ����.
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
��    Ī: FindingForm_OnClose
��    ��: �ݱ� ��ư�� Ŭ������ �� �ó�����.
*************************************************************************************************/
BOOL FindingForm_OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	WordCard* (*indexes);

	indexes = (WordCard * (*))GetWindowLong(hWnd, GWL_USERDATA);
	if (indexes != NULL) {
		free(indexes);
	}
	RemoveProp(hWnd, "PROP_COUNT");
	RemoveProp(hWnd, "PROP_CURRENT");
	//10.1. ã�� �����츦 �ݴ´�.
	EndDialog(hWnd, 0);

	return TRUE;
}