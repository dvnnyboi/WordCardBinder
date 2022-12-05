//MemorizedWordsForm.c
/************************************************************************************************
���ϸ�Ī: MemorizedWordsForm.c
��    ��: �ܾ�ī��ö �ϱ�Ϸ�ī�� ������
�� �� ��: �ӴٴϿ�
�ۼ�����: 2022/07/25
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
��    Ī: MemorizedWordsFormProc
��    ��: �ϱ�Ϸ�ī�� ������ ���ν����Լ�
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
��    Ī: MemorizedWordsForm_OnInitDialog
��    ��: �����찡 ������ �� �ó�����.
*************************************************************************************************/
BOOL MemorizedWordsForm_OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	HWND hWordCardBinderFormWnd;
	WordCardBinder* memorizedWords;
	WordCard* index;
	TCHAR correctRate[8];

	//1.1. �ܾ�ī��ö �����츦 ã�´�.
	hWordCardBinderFormWnd = FindWindow("#32770", "�ܾ�ī��ö");
	memorizedWords = (WordCardBinder*)GetProp(hWordCardBinderFormWnd, "PROP_MEMORIZEDWORDS");
	if (memorizedWords->length > 0) {
		//1.2. �ܾ�ī��ö �������� �ϱ�Ϸ� ī��ö���� ó������ �̵��Ѵ�.
		index = WordCardBinder_First(memorizedWords);
		//1.3. �ܾ ����.
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
��    Ī: MemorizedWordsForm_OnCommand
��    ��: WM_COMMAND �޼��� �߻� �� �ó�����.
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
��    Ī: MemorizedWordsForm_OnTakeInButtonClicked
��    ��: ����� ��ư�� Ŭ������ �� �ó�����.
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
		//2.1. �ܾ�ī��ö �����츦 ã�´�.
		hWordCardBinderFormWnd = FindWindow("#32770", "�ܾ�ī��ö");
		wordCardBinder = (WordCardBinder*)GetWindowLong(hWordCardBinderFormWnd, GWL_USERDATA);
		memorizedWords = (WordCardBinder*)GetProp(hWordCardBinderFormWnd, "PROP_MEMORIZEDWORDS");
		indexCardBook = (IndexCardBook*)GetProp(hWordCardBinderFormWnd, "PROP_INDEXCARDBOOK");
		//2.2. �ܾ�ī��ö �������� �ϱ�Ϸ� ī��ö���� ����.
		wordCard = TakeOut(memorizedWords, memorizedWords->current);
		//2.3. �ܾ�ī��ö �������� �ܾ�ī��ö���� �����.
		index = TakeIn(wordCardBinder, wordCard);
		//2.4. �ܾ�ī��ö �������� ����ī�������� �����.
		link = IndexCardBook_TakeIn(indexCardBook, index);
		//2.5. �ܾ�ī��ö �������� ����ī�������� �����Ѵ�.
		IndexCardBook_Arrange(indexCardBook);
		//2.6. �ܾ�ī��ö �����쿡�� ����ī�� ����� �����.
		MakeList(indexCardBook, &indexCards, &count);
		//2.7. �ܾ�ī��ö �������� Ʈ�� �� ��Ʈ�ѿ��� ��� �׸��� �����.
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK), TVM_DELETEITEM, (WPARAM)0, (LPARAM)TVI_ROOT);
		//2.8. �ܾ�ī��ö �������� Ʈ�� �� ��Ʈ�ѿ� ��Ʈ �׸��� �߰��Ѵ�.
		item.mask = TVIF_TEXT | TVIF_CHILDREN;
		item.pszText = "���ĺ�";
		item.cChildren = 1;
		insertItem.hParent = TVI_ROOT;
		insertItem.hInsertAfter = TVI_FIRST;
		insertItem.item = item;
		hRootItem = (HTREEITEM)SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK),
			TVM_INSERTITEM, (WPARAM)0, (LPARAM)&insertItem);
		//2.9. ������ŭ �ݺ��Ѵ�.
		i = 0;
		while (i < count) {
			//2.9.1. �ܾ�ī��ö �������� Ʈ�� �� ��Ʈ�ѿ� ���ĺ� �׸��� �߰��Ѵ�.
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
			//2.9.2. �ܾ� ������ŭ �ݺ��Ѵ�.
			j = 0;
			while (j < link->length) {
				//2.9.2.1. �ܾ�ī��ö �������� Ʈ�� �� ��Ʈ�ѿ� �ܾ� �׸��� �߰��Ѵ�.
				index = IndexCard_GetAt(link, j);
				item.pszText = index->word.spelling;
				item.cChildren = 1;
				insertItem.hParent = hIndexCardItem;
				insertItem.hInsertAfter = hWordItem;
				insertItem.item = item;
				hWordItem = (HTREEITEM)SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK),
					TVM_INSERTITEM, (WPARAM)0, (LPARAM)&insertItem);
				//2.9.2.2. �ܾ�ī��ö �������� �ܾ�ī��ö���� ö�ڷ� ã�´�.
				wordCardBinder = (WordCardBinder*)GetWindowLong(hWordCardBinderFormWnd, GWL_USERDATA);
				FindBySpelling(wordCardBinder, index->word.spelling, &indexes, &indexCount);
				//2.9.2.3. ã�� ������ŭ �ݺ��Ѵ�.
				k = 0;
				while (k < indexCount) {
					//2.9.2.3.1. �ܾ�ī��ö �������� Ʈ�� �� ��Ʈ�ѿ� ǰ�� �׸��� �߰��Ѵ�.
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
		//2.10. �ܾ�ī��ö �������� �ܾ�ī��ö���� ���������� �̵��Ѵ�.
		index = WordCardBinder_Last(wordCardBinder);
		//2.11. �ܾ�ī��ö �������� �ܾ ����.
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_EDIT_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.spelling);
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_EDIT_WORDCLASS), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.wordClass);
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_EDIT_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.meaning);
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_EDIT_EXAMPLESENTENCE), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.exampleSentence);
		//sprintf(correctRate, "%.1f", index->evaluation.correctRate);
		//SendMessage(GetDlgItem(hWnd, IDC_EDIT_CORRECTRATE), WM_SETTEXT, (WPARAM)0, (LPARAM)correctRate);
		//2.12. �ܾ�ī��ö �������� ����ī�������� ã�´�
		link = IndexCardBook_Find(indexCardBook, index->word.spelling[0]);
		//2.13. �ܾ�ī��ö �������� ����ī�忡�� ã�´�.
		location = IndexCard_Find(link, index);
		firstLocation = IndexCard_FindFirstTime(link, index->word.spelling);
		//2.14. �ܾ�ī��ö �������� Ʈ�� �� ��Ʈ�ѿ��� ��Ʈ �׸��� ��ģ��.
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hRootItem);
		//2.15. �ܾ�ī��ö �������� Ʈ�� �� ��Ʈ�ѿ��� ���ĺ� �׸��� ã�´�.
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
		//2.16. �ܾ�ī��ö �������� Ʈ�� �� ��Ʈ�ѿ��� ���ĺ� �׸��� ��ģ��.
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hIndexCardItem);
		//2.17. �ܾ�ī��ö �������� Ʈ�� �� ��Ʈ���� �ܾ� �׸��� ã�´�.
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
		//2.18. �ܾ�ī��ö �������� Ʈ�� �� ��Ʈ�ѿ��� �ܾ� �׸��� ��ģ��.
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hWordItem);
		//2.19. �ܾ�ī��ö ������ Ʈ�� �� ��Ʈ���� �ܾ� �׸񿡼� �̵��Ѵ�.
		hWordClassItem = (HTREEITEM)SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK),
			TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hWordItem);
		i = 0;
		while (hWordClassItem != NULL && i < (location - firstLocation)) {
			hWordClassItem = (HTREEITEM)SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK),
				TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hWordClassItem);
			i++;
		}
		//2.20. �ܾ�ī��ö �������� Ʈ�� �� ��Ʈ�ѿ��� ǰ�� �׸��� �����Ѵ�.
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
		//2.21. �ϱ�Ϸ� ī�� �����츦 �ݴ´�.
		EndDialog(hWnd, 0);
	}
	return TRUE;
}
/************************************************************************************************
��    Ī: MemorizedWordsForm_OnTakeInAllButtonClicked
��    ��: ��γ���� ��ư�� Ŭ������ �� �ó�����.
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
		//3.1. �ܾ�ī��ö �����츦 ã�´�.
		hWordCardBinderFormWnd = FindWindow("#32770", "�ܾ�ī��ö");
		wordCardBinder = (WordCardBinder*)GetWindowLong(hWordCardBinderFormWnd, GWL_USERDATA);
		memorizedWords = (WordCardBinder*)GetProp(hWordCardBinderFormWnd, "PROP_MEMORIZEDWORDS");
		indexCardBook = (IndexCardBook*)GetProp(hWordCardBinderFormWnd, "PROP_INDEXCARDBOOK");
		//3.2. ������ŭ �ݺ��Ѵ�.
		index = WordCardBinder_First(memorizedWords);
		while (index != NULL && index != previous) {
			previous = index;
			//3.2.1. �ܾ�ī��ö �������� �Ϸ�ī��ö���� ����.
			wordCard = TakeOut(memorizedWords, index);
			//3.2.2. �ܾ�ī��ö �������� �ܾ�ī��ö���� �����.
			index = TakeIn(wordCardBinder, wordCard);
			//3.2.3. �ܾ�ī��ö �������� ����ī�������� �����.
			IndexCardBook_TakeIn(indexCardBook, index);
			if (memorizedWords->length > 0) {
				index = WordCardBinder_First(memorizedWords);
			}
			else {
				index = NULL;
			}
		}
		//3.3. �ܾ�ī��ö �������� ����ī�������� �����Ѵ�.
		IndexCardBook_Arrange(indexCardBook);
		//3.4. �ܾ�ī��ö �����쿡�� ����ī�� ����� �����.
		MakeList(indexCardBook, &indexCards, &count);
		//3.5. �ܾ�ī��ö �������� Ʈ�� �� ��Ʈ�ѿ��� ��� �׸��� �����.
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK), TVM_DELETEITEM, (WPARAM)0, (LPARAM)TVI_ROOT);
		//3.6. �ܾ�ī��ö �������� Ʈ�� �� ��Ʈ�ѿ� ��Ʈ �׸��� �߰��Ѵ�.
		item.mask = TVIF_TEXT | TVIF_CHILDREN;
		item.pszText = "���ĺ�";
		item.cChildren = 1;
		insertItem.hParent = TVI_ROOT;
		insertItem.hInsertAfter = TVI_FIRST;
		insertItem.item = item;
		hRootItem = (HTREEITEM)SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK),
			TVM_INSERTITEM, (WPARAM)0, (LPARAM)&insertItem);
		//3.7. ������ŭ �ݺ��Ѵ�.
		i = 0;
		while (i < count) {
			//3.7.1. �ܾ�ī��ö �������� Ʈ�� �� ��Ʈ�ѿ� ���ĺ� �׸��� �߰��Ѵ�.
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
			//3.7.2. �ܾ� ������ŭ �ݺ��Ѵ�.
			j = 0;
			while (j < link->length) {
				//3.7.2.1. �ܾ�ī��ö �������� Ʈ�� �� ��Ʈ�ѿ� �ܾ� �׸��� �߰��Ѵ�.
				index = IndexCard_GetAt(link, j);
				item.pszText = index->word.spelling;
				item.cChildren = 1;
				insertItem.hParent = hIndexCardItem;
				insertItem.hInsertAfter = hWordItem;
				insertItem.item = item;
				hWordItem = (HTREEITEM)SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK),
					TVM_INSERTITEM, (WPARAM)0, (LPARAM)&insertItem);
				//3.7.2.2. �ܾ�ī��ö �������� �ܾ�ī��ö���� ö�ڷ� ã�´�.
				wordCardBinder = (WordCardBinder*)GetWindowLong(hWordCardBinderFormWnd, GWL_USERDATA);
				FindBySpelling(wordCardBinder, index->word.spelling, &indexes, &indexCount);
				//3.7.2.3. ã�� ������ŭ �ݺ��Ѵ�.
				k = 0;
				while (k < indexCount) {
					//3.7.2.3.1. �ܾ�ī��ö �������� Ʈ�� �� ��Ʈ�ѿ� ǰ�� �׸��� �߰��Ѵ�.
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
		//3.8. �ܾ�ī��ö �������� �ܾ�ī��ö���� ���������� �̵��Ѵ�.
		index = WordCardBinder_Last(wordCardBinder);
		//3.9. �ܾ�ī��ö �������� �ܾ ����.
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_EDIT_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.spelling);
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_EDIT_WORDCLASS), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.wordClass);
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_EDIT_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.meaning);
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_EDIT_EXAMPLESENTENCE), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.exampleSentence);
		//sprintf(correctRate, "%.1f", index->evaluation.correctRate);
		//SendMessage(GetDlgItem(hWnd, IDC_EDIT_CORRECTRATE), WM_SETTEXT, (WPARAM)0, (LPARAM)correctRate);
		//3.10. �ܾ�ī��ö �������� ����ī�������� ã�´�
		link = IndexCardBook_Find(indexCardBook, index->word.spelling[0]);
		//3.11. �ܾ�ī��ö �������� ����ī�忡�� ã�´�.
		location = IndexCard_Find(link, index);
		firstLocation = IndexCard_FindFirstTime(link, index->word.spelling);
		//3.12. �ܾ�ī��ö �������� Ʈ�� �� ��Ʈ�ѿ��� ��Ʈ �׸��� ��ģ��.
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hRootItem);
		//3.13. �ܾ�ī��ö �������� Ʈ�� �� ��Ʈ�ѿ��� ���ĺ� �׸��� ã�´�.
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
		//3.14. �ܾ�ī��ö �������� Ʈ�� �� ��Ʈ�ѿ��� ���ĺ� �׸��� ��ģ��.
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hIndexCardItem);
		//3.15. �ܾ�ī��ö �������� Ʈ�� �� ��Ʈ���� �ܾ� �׸��� ã�´�.
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
		//3.16. �ܾ�ī��ö �������� Ʈ�� �� ��Ʈ�ѿ��� �ܾ� �׸��� ��ģ��.
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hWordItem);
		//3.17. �ܾ�ī��ö ������ Ʈ�� �� ��Ʈ���� �ܾ� �׸񿡼� �̵��Ѵ�.
		hWordClassItem = (HTREEITEM)SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK),
			TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hWordItem);
		i = 0;
		while (hWordClassItem != NULL && i < (location - firstLocation)) {
			hWordClassItem = (HTREEITEM)SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK),
				TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hWordClassItem);
			i++;
		}
		//3.18. �ܾ�ī��ö �������� Ʈ�� �� ��Ʈ�ѿ��� ǰ�� �׸��� �����Ѵ�.
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
		//3.19. �ϱ�Ϸ� ī�� �����츦 �ݴ´�.
		EndDialog(hWnd, 0);
	}

	return TRUE;
}
/************************************************************************************************
��    Ī: MemorizedWordsForm_OnFirstButtonClicked
��    ��: ó�� ��ư�� Ŭ������ �� �ó�����.
*************************************************************************************************/
BOOL MemorizedWordsForm_OnFirstButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	HWND hWordCardBinderFormWnd;
	WordCardBinder* memorizedWords;
	WordCard* index;
	TCHAR correctRate[8];

	if (HIWORD(wParam) == BN_CLICKED) {
		//4.1. �ܾ�ī��ö �����츦 ã�´�.
		hWordCardBinderFormWnd = FindWindow("#32770", "�ܾ�ī��ö");
		memorizedWords = (WordCardBinder*)GetProp(hWordCardBinderFormWnd, "PROP_MEMORIZEDWORDS");
		//4.2. �ܾ�ī��ö �������� �ϱ�Ϸ� ī��ö���� ó������ �̵��Ѵ�.
		index = WordCardBinder_First(memorizedWords);
		//4.3. �ܾ ����.
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
��    Ī: MemorizedWordsForm_OnPreviousButtonClicked
��    ��: ���� ��ư�� Ŭ������ �� �ó�����.
*************************************************************************************************/
BOOL MemorizedWordsForm_OnPreviousButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	HWND hWordCardBinderFormWnd;
	WordCardBinder* memorizedWords;
	WordCard* index;
	TCHAR correctRate[8];

	if (HIWORD(wParam) == BN_CLICKED) {
		//5.1. �ܾ�ī��ö �����츦 ã�´�.
		hWordCardBinderFormWnd = FindWindow("#32770", "�ܾ�ī��ö");
		memorizedWords = (WordCardBinder*)GetProp(hWordCardBinderFormWnd, "PROP_MEMORIZEDWORDS");
		//5.2. �ܾ�ī��ö �������� �ϱ�Ϸ� ī��ö���� �������� �̵��Ѵ�.
		index = WordCardBinder_Previous(memorizedWords);
		//5.3. �ܾ ����.
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
��    Ī: MemorizedWordsForm_OnNextButtonClicked
��    ��: ���� ��ư�� Ŭ������ �� �ó�����.
*************************************************************************************************/
BOOL MemorizedWordsForm_OnNextButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	HWND hWordCardBinderFormWnd;
	WordCardBinder* memorizedWords;
	WordCard* index;
	TCHAR correctRate[8];

	if (HIWORD(wParam) == BN_CLICKED) {
		//6.1. �ܾ�ī��ö �����츦 ã�´�.
		hWordCardBinderFormWnd = FindWindow("#32770", "�ܾ�ī��ö");
		memorizedWords = (WordCardBinder*)GetProp(hWordCardBinderFormWnd, "PROP_MEMORIZEDWORDS");
		//6.2. �ܾ�ī��ö �������� �ϱ�Ϸ� ī��ö���� �������� �̵��Ѵ�.
		index = WordCardBinder_Next(memorizedWords);
		//6.3. �ܾ ����.
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
��    Ī: MemorizedWordsForm_OnLastButtonClicked
��    ��: ������ ��ư�� Ŭ������ �� �ó�����.
*************************************************************************************************/
BOOL MemorizedWordsForm_OnLastButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	HWND hWordCardBinderFormWnd;
	WordCardBinder* memorizedWords;
	WordCard* index;
	TCHAR correctRate[8];

	if (HIWORD(wParam) == BN_CLICKED) {
		//7.1. �ܾ�ī��ö �����츦 ã�´�.
		hWordCardBinderFormWnd = FindWindow("#32770", "�ܾ�ī��ö");
		memorizedWords = (WordCardBinder*)GetProp(hWordCardBinderFormWnd, "PROP_MEMORIZEDWORDS");
		//7.2. �ܾ�ī��ö �������� �ϱ�Ϸ� ī��ö���� ���������� �̵��Ѵ�.
		index = WordCardBinder_Last(memorizedWords);
		//7.3. �ܾ ����.
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
��    Ī: MemorizedWordsForm_OnClose
��    ��: �ݱ� ��ư�� Ŭ������ �� �ó�����.
*************************************************************************************************/
BOOL MemorizedWordsForm_OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	EndDialog(hWnd, 0);

	return TRUE;
}