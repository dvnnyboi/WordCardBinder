//WordCardBinderForm.c
/************************************************************************************************
���ϸ�Ī: WordCardBinderForm.c
��    ��: �ܾ�ī��ö ����������
�� �� ��: �ӴٴϿ�
�ۼ�����: 2022/07/15
*************************************************************************************************/
#include "WordCardBinderForm.h"
#include "TakingInForm.h"
#include "FindingForm.h"
#include "TakingOutForm.h"
#include "QuizForm.h"
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


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	int response;

	response = DialogBox(hInstance, MAKEINTRESOURCE(IDD_WORDCARDBINDERFORM), NULL, WordCardBinderFormProc);

	return response;
}
/************************************************************************************************
��    Ī: WordCardBinderFormProc
��    ��: ���������� ���ν����Լ�
*************************************************************************************************/
BOOL CALLBACK WordCardBinderFormProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	BOOL ret;

	switch (message) {
	case WM_INITDIALOG:
		ret = WordCardBinderForm_OnInitDialog(hWnd, wParam, lParam);
		break;
	case WM_COMMAND:
		ret = WordCardBinderForm_OnCommand(hWnd, wParam, lParam);
		break;
	case WM_NOTIFY:
		ret = WordCardBinderForm_OnNotify(hWnd, wParam, lParam);
		break;
	case WM_CLOSE:
		ret = WordCardBinderForm_OnClose(hWnd, wParam, lParam);
		break;
	default:
		ret = FALSE;
		break;
	}
	return ret;
}
/************************************************************************************************
��    Ī: WordCardBinderForm_OnInitDialog
��    ��: �����찡 ������ �� �ó�����
*************************************************************************************************/
BOOL WordCardBinderForm_OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	WordCardBinder* wordCardBinder;
	WordCardBinder* memorizedWords;
	WordCard* index;
	WordCard* (*indexes);
	WordCard* previous = NULL;
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
	
	//1.1. �ܾ�ī��ö�� �����.
	wordCardBinder = (WordCardBinder*)malloc(sizeof(WordCardBinder));
	WordCardBinder_Create(wordCardBinder);
	SetWindowLong(hWnd, GWL_USERDATA, (LONG)wordCardBinder);
	// �ϱ�Ϸ� ī��ö�� �����.
	memorizedWords = (WordCardBinder*)malloc(sizeof(WordCardBinder));
	WordCardBinder_Create(memorizedWords);
	SetProp(hWnd, "PROP_MEMORIZEDWORDS", (HANDLE)memorizedWords);
	//1.2. ����ī������ �����
	indexCardBook = (IndexCardBook*)malloc(sizeof(IndexCardBook));
	IndexCardBook_Create(indexCardBook);
	SetProp(hWnd, "PROP_INDEXCARDBOOK", (HANDLE)indexCardBook);
	//1.3. Ʈ�� �� ��Ʈ�ѿ� ��Ʈ �׸��� �߰��Ѵ�.
	item.mask = TVIF_TEXT | TVIF_CHILDREN;
	item.pszText = "���ĺ�";
	item.cChildren = 1;
	insertItem.hParent = TVI_ROOT;
	insertItem.hInsertAfter = TVI_FIRST;
	insertItem.item = item;
	hRootItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_INSERTITEM, (WPARAM)0, (LPARAM)&insertItem);
	//1.4. �ܾ�ī��ö���� �����Ѵ�
	count = Load(wordCardBinder, memorizedWords);
	if (count > 0) {
		//1.5. ������ŭ �ݺ��Ѵ�.
		index = WordCardBinder_First(wordCardBinder);
		while (index != previous) {
			//1.5.1. ����ī�������� �����.
			IndexCardBook_TakeIn(indexCardBook, index);
			previous = index;
			index = WordCardBinder_Next(wordCardBinder);
		}
		//1.6. ����ī������ �����Ѵ�.
		IndexCardBook_Arrange(indexCardBook);
		//1.7. ����ī�� ����� �����.
		MakeList(indexCardBook, &indexCards, &count);
		//1.8. ������ŭ �ݺ��Ѵ�.
		i = 0;
		while (i < count) {
			//1.8.1. Ʈ�� �� ��Ʈ�ѿ� ���ĺ� �׸��� �߰��Ѵ�.
			link = &(indexCards[i]);
			alphabet[0] = link->alphabet;
			alphabet[1] = '\0';
			item.pszText = alphabet;
			item.cChildren = 1;
			insertItem.hParent = hRootItem;
			insertItem.hInsertAfter = hIndexCardItem;
			insertItem.item = item;
			hIndexCardItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_INSERTITEM, (WPARAM)0, (LPARAM)&insertItem);
			//1.8.2. �ܾ� ������ŭ �ݺ��Ѵ�.
			j = 0;
			while (j < link->length) {
				//1.8.2.1. Ʈ�� �� ��Ʈ�ѿ� �ܾ� �׸��� �߰��Ѵ�.
				index = IndexCard_GetAt(link, j);
				item.pszText = index->word.spelling;
				item.cChildren = 1;
				insertItem.hParent = hIndexCardItem;
				insertItem.hInsertAfter = hWordItem;
				insertItem.item = item;
				hWordItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_INSERTITEM, (WPARAM)0, (LPARAM)&insertItem);
				//1.8.2.2. �ܾ�ī��ö���� ö�ڷ� ã�´�.
				FindBySpelling(wordCardBinder, index->word.spelling, &indexes, &indexCount);
				//1.8.2.3. ã�� ������ŭ �ݺ��Ѵ�.
				k = 0;
				while (k < indexCount) {
					//1.8.2.3.1. ǰ�� �׸��� �߰��Ѵ�.
					index = IndexCard_GetAt(link, j + k);
					item.pszText = index->word.wordClass;
					item.cChildren = 0;
					insertItem.hParent = hWordItem;
					insertItem.hInsertAfter = hWordClassItem;
					insertItem.item = item;
					hWordClassItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_INSERTITEM, (WPARAM)0, (LPARAM)&insertItem);
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
		//1.9. �ܾ�ī��ö���� ó������ �̵��Ѵ�.
		index = WordCardBinder_First(wordCardBinder);
		//1.10. �ܾ ����.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.spelling);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_WORDCLASS), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.wordClass);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.meaning);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_EXAMPLESENTENCE), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.exampleSentence);
		//1.11. ����ī�������� ã�´�
		link = IndexCardBook_Find(indexCardBook, index->word.spelling[0]);
		//1.12. ����ī�忡�� ã�´�.
		location = IndexCard_Find(link, index);
		firstLocation = IndexCard_FindFirstTime(link, index->word.spelling);
		//1.13. Ʈ�� �� ��Ʈ�ѿ��� ��Ʈ �׸��� ��ģ��.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hRootItem);
		//1.14. Ʈ�� �� ��Ʈ�ѿ��� ���ĺ� �׸��� ã�´�.
		compare[0] = link->alphabet;
		compare[1] = '\0';
		hIndexCardItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hRootItem);
		item.mask = TVIF_TEXT | TVIF_HANDLE;
		item.hItem = hIndexCardItem;
		item.pszText = alphabet;
		item.cchTextMax = 2;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETITEM, (WPARAM)0, (LPARAM)&item);
		while (hIndexCardItem != NULL && strcmp(compare, alphabet) != 0) {
			hIndexCardItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETNEXTITEM,
				(WPARAM)TVGN_NEXT, (LPARAM)hIndexCardItem);
			item.hItem = hIndexCardItem;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETITEM, (WPARAM)0, (LPARAM)&item);
		}
		//1.15. Ʈ�� �� ��Ʈ�ѿ��� ���ĺ� �׸��� ��ģ��.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hIndexCardItem);
		//1.16. Ʈ�� �� ��Ʈ���� �ܾ� �׸��� ã�´�.
		strcpy(spelling, index->word.spelling);
		hWordItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hIndexCardItem);
		item.mask = TVIF_TEXT | TVIF_HANDLE;
		item.hItem = hWordItem;
		item.pszText = compareSpelling;
		item.cchTextMax = 16;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETITEM, (WPARAM)0, (LPARAM)&item);
		while (hWordItem != NULL && strcmp(spelling, compareSpelling) != 0) {
			hWordItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETNEXTITEM,
				(WPARAM)TVGN_NEXT, (LPARAM)hWordItem);
			item.hItem = hWordItem;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETITEM, (WPARAM)0, (LPARAM)&item);
		}
		//1.17. Ʈ�� �� ��Ʈ�ѿ��� �ܾ� �׸��� ��ģ��.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hWordItem);
		//1.18. Ʈ�� �� ��Ʈ���� ǰ�� �׸񿡼� �̵��Ѵ�.
		hWordClassItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hWordItem);
		i = 0;
		while (hWordClassItem != NULL && i < (location - firstLocation)) {
			hWordClassItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hWordClassItem);
			i++;
		}
		//1.19. Ʈ�� �� ��Ʈ�ѿ��� ǰ�� �׸��� �����Ѵ�.
		item.mask = TVIF_STATE | TVIF_HANDLE;
		item.hItem = hWordClassItem;
		item.stateMask = TVIS_SELECTED;
		item.state = TVIS_SELECTED;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_SETITEM, (WPARAM)0, (LPARAM)&item);
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_SELECTITEM, (WPARAM)TVGN_CARET, (LPARAM)hWordClassItem);
		EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_FIRST), TRUE);
		EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_PREVIOUS), TRUE);
		EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_NEXT), TRUE);
		EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_LAST), TRUE);
		EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_FIND), TRUE);
		EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_ARRANGE), TRUE);
		EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_TAKEOUT), TRUE);
		EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_QUIZ), TRUE);
	}
	return TRUE;
}
/************************************************************************************************
��    Ī: WordCardBinderForm_OnCommand
��    ��: WM_COMMAND �޼��� �߻� �� �ó�����
*************************************************************************************************/
BOOL WordCardBinderForm_OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BOOL ret;

	switch (LOWORD(wParam)) {
	case IDC_BUTTON_TAKEIN:
		ret = WordCardBinderForm_OnTakeInButtonClicked(hWnd, wParam, lParam);
		break;
	case IDC_BUTTON_FIND:
		ret = WordCardBinderForm_OnFindButtonClicked(hWnd, wParam, lParam);
		break;
	case IDC_BUTTON_TAKEOUT:
		ret = WordCardBinderForm_OnTakeOutButtonClicked(hWnd, wParam, lParam);
		break;
	case IDC_BUTTON_ARRANGE:
		ret = WordCardBinderForm_OnArrangeButtonClicked(hWnd, wParam, lParam);
		break;
	case IDC_BUTTON_QUIZ:
		ret = WordCardBinderForm_OnQuizButtonClicked(hWnd, wParam, lParam);
		break;
	case IDC_BUTTON_MEMORIZEDWORDS:
		ret = WordCardBinderForm_OnMemorizedWordsButtonClicked(hWnd, wParam, lParam);
		break;
	case IDC_BUTTON_FIRST:
		ret = WordCardBinderForm_OnFirstButtonClicked(hWnd, wParam, lParam);
		break;
	case IDC_BUTTON_PREVIOUS:
		ret = WordCardBinderForm_OnPreviousButtonClicked(hWnd, wParam, lParam);
		break;
	case IDC_BUTTON_NEXT:
		ret = WordCardBinderForm_OnNextButtonClicked(hWnd, wParam, lParam);
		break;
	case IDC_BUTTON_LAST:
		ret = WordCardBinderForm_OnLastButtonClicked(hWnd, wParam, lParam);
		break;
	default:
		ret = FALSE;
		break;
	}
	return ret;
}
/************************************************************************************************
��    Ī: WordCardBinderForm_OnTakeInButtonClicked
��    ��: ����� ��ư�� Ŭ������ �� �ó�����
*************************************************************************************************/
BOOL WordCardBinderForm_OnTakeInButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	//2.1. ����� �����츦 ����Ѵ�.
	if (HIWORD(wParam) == BN_CLICKED) {
		DialogBox((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDD_TAKINGINFORM),
			NULL, TakingInFormProc);
	}
	return TRUE;
}
/************************************************************************************************
��    Ī: WordCardBinderForm_OnFindButtonClicked
��    ��: ã�� ��ư�� Ŭ������ �� �ó�����
*************************************************************************************************/
BOOL WordCardBinderForm_OnFindButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	//3.1. ã�� �����츦 ����Ѵ�.
	if (HIWORD(wParam) == BN_CLICKED) {
		DialogBox((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDD_FINDINGFORM),
			NULL, FindingFormProc);
	}
	return TRUE;
}
/************************************************************************************************
��    Ī: WordCardBinderForm_OnTakeOutButtonClicked
��    ��: ���� ��ư�� Ŭ������ �� �ó�����
*************************************************************************************************/
BOOL WordCardBinderForm_OnTakeOutButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	//4.1. ���� �����츦 ����Ѵ�.
	if (HIWORD(wParam) == BN_CLICKED) {
		DialogBox((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDD_TAKINGOUTFORM),
			NULL, TakingOutFormProc);
	}
	return TRUE;
}
/************************************************************************************************
��    Ī: WordCardBinderForm_OnArrangeButtonClicked
��    ��: �����ϱ� ��ư�� Ŭ������ �� �ó�����
*************************************************************************************************/
BOOL WordCardBinderForm_OnArrangeButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	WordCardBinder* wordCardBinder;
	WordCard* index;
	WordCard* (*indexes);
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
	
	if (HIWORD(wParam) == BN_CLICKED) {
		//5.1. ����ī������ �����Ѵ�.
		indexCardBook = (IndexCardBook*)GetProp(hWnd, "PROP_INDEXCARDBOOK");
		IndexCardBook_Arrange(indexCardBook);
		//5.2. ����ī�� ����� �����.
		MakeList(indexCardBook, &indexCards, &count);
		//5.3. Ʈ�� �� ��Ʈ���� ��� �׸��� �����.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_DELETEITEM, (WPARAM)0, (LPARAM)TVI_ROOT);
		//5.4. Ʈ�� �� ��Ʈ�ѿ� ��Ʈ �׸��� �߰��Ѵ�.
		item.mask = TVIF_TEXT | TVIF_CHILDREN;
		item.pszText = "���ĺ�";
		item.cChildren = 1;
		insertItem.hParent = TVI_ROOT;
		insertItem.hInsertAfter = TVI_FIRST;
		insertItem.item = item;
		hRootItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_INSERTITEM, (WPARAM)0, (LPARAM)&insertItem);
		//5.5. ������ŭ �ݺ��Ѵ�.
		i = 0;
		while (i < count) {
			//5.5.1. Ʈ�� �� ��Ʈ�ѿ� ���ĺ� �׸��� �߰��Ѵ�.
			link = &(indexCards[i]);
			alphabet[0] = link->alphabet;
			alphabet[1] = '\0';
			item.pszText = alphabet;
			item.cChildren = 1;
			insertItem.hParent = hRootItem;
			insertItem.hInsertAfter = hIndexCardItem;
			insertItem.item = item;
			hIndexCardItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_INSERTITEM, (WPARAM)0, (LPARAM)&insertItem);
			//5.5.2. �ܾ� ������ŭ �ݺ��Ѵ�.
			j = 0;
			while (j < link->length) {
				//5.5.2.1 Ʈ�� �� ��Ʈ�ѿ� �ܾ� �׸��� �߰��Ѵ�.
				index = IndexCard_GetAt(link, j);
				item.pszText = index->word.spelling;
				item.cChildren = 1;
				insertItem.hParent = hIndexCardItem;
				insertItem.hInsertAfter = hWordItem;
				insertItem.item = item;
				hWordItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_INSERTITEM, (WPARAM)0, (LPARAM)&insertItem);
				//5.5.2.2. �ܾ�ī��ö���� ö�ڷ� ã�´�.
				wordCardBinder = (WordCardBinder*)GetWindowLong(hWnd, GWL_USERDATA);
				FindBySpelling(wordCardBinder, index->word.spelling, &indexes, &indexCount);
				//5.5.2.3. ã�� ������ŭ �ݺ��Ѵ�.
				k = 0;
				while (k < indexCount) {
					//5.5.2.3.1. Ʈ�� �� ��Ʈ�ѿ� ǰ�� �׸��� �߰��Ѵ�.
					index = IndexCard_GetAt(link, j + k);
					item.pszText = index->word.wordClass;
					item.cChildren = 0;
					insertItem.hParent = hWordItem;
					insertItem.hInsertAfter = hWordClassItem;
					insertItem.item = item;
					hWordClassItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_INSERTITEM, (WPARAM)0, (LPARAM)&insertItem);
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
		//5.6. �ܾ�ī��ö���� ó������ �̵��Ѵ�.
		wordCardBinder = (WordCardBinder*)GetWindowLong(hWnd, GWL_USERDATA);
		index = WordCardBinder_First(wordCardBinder);
		//5.7. �ܾ ����.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.spelling);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_WORDCLASS), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.wordClass);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.meaning);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_EXAMPLESENTENCE), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.exampleSentence);
		//sprintf(correctRate, "%.1f", index->evaluation.correctRate);
		//SendMessage(GetDlgItem(hWnd, IDC_EDIT_CORRECTRATE), WM_SETTEXT, (WPARAM)0, (LPARAM)correctRate);
		//5.8. ����ī�������� ã�´�
		link = IndexCardBook_Find(indexCardBook, index->word.spelling[0]);
		//5.9. ����ī�忡�� ã�´�.
		location = IndexCard_Find(link, index);
		firstLocation = IndexCard_FindFirstTime(link, index->word.spelling);
		//5.10. Ʈ�� �� ��Ʈ�ѿ��� ��Ʈ �׸��� ��ģ��.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hRootItem);
		//5.11. Ʈ�� �� ��Ʈ�ѿ��� ���ĺ� �׸��� ã�´�.
		compare[0] = link->alphabet;
		compare[1] = '\0';
		hIndexCardItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hRootItem);
		item.mask = TVIF_TEXT | TVIF_HANDLE;
		item.hItem = hIndexCardItem;
		item.pszText = alphabet;
		item.cchTextMax = 2;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETITEM, (WPARAM)0, (LPARAM)&item);
		while (hIndexCardItem != NULL && strcmp(compare, alphabet) != 0) {
			hIndexCardItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETNEXTITEM,
				(WPARAM)TVGN_NEXT, (LPARAM)hIndexCardItem);
			item.hItem = hIndexCardItem;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETITEM, (WPARAM)0, (LPARAM)&item);
		}
		//5.12. Ʈ�� �� ��Ʈ�ѿ��� ���ĺ� �׸��� ��ģ��.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hIndexCardItem);
		//5.13. Ʈ�� �� ��Ʈ���� �ܾ� �׸��� ã�´�.
		strcpy(spelling, index->word.spelling);
		hWordItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hIndexCardItem);
		item.mask = TVIF_TEXT | TVIF_HANDLE;
		item.hItem = hWordItem;
		item.pszText = compareSpelling;
		item.cchTextMax = 16;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETITEM, (WPARAM)0, (LPARAM)&item);
		while (hWordItem != NULL && strcmp(spelling, compareSpelling) != 0) {
			hWordItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETNEXTITEM,
				(WPARAM)TVGN_NEXT, (LPARAM)hWordItem);
			item.hItem = hWordItem;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETITEM, (WPARAM)0, (LPARAM)&item);
		}
		//5.14. Ʈ�� �� ��Ʈ�ѿ��� �ܾ� �׸��� ��ģ��.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hWordItem);
		//5.15. Ʈ�� �� ��Ʈ���� �ܾ� �׸񿡼� �̵��Ѵ�.
		hWordClassItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hWordItem);
		i = 0;
		while (hWordClassItem != NULL && i < (location - firstLocation)) {
			hWordClassItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hWordClassItem);
			i++;
		}
		//5.16. Ʈ�� �� ��Ʈ�ѿ��� ǰ�� �׸��� �����Ѵ�.
		item.mask = TVIF_STATE | TVIF_HANDLE;
		item.hItem = hWordClassItem;
		item.stateMask = TVIS_SELECTED;
		item.state = TVIS_SELECTED;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_SETITEM, (WPARAM)0, (LPARAM)&item);
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_SELECTITEM, (WPARAM)TVGN_CARET, (LPARAM)hWordClassItem);
	}
	return TRUE;
}
/************************************************************************************************
��    Ī: WordCardBinderForm_OnQuizButtonClicked
��    ��: ���� ��ư�� Ŭ������ �� �ó�����
*************************************************************************************************/
BOOL WordCardBinderForm_OnQuizButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	// ���� �����츦 ����Ѵ�.
	if (HIWORD(wParam) == BN_CLICKED) {
		DialogBox((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDD_QUIZFORM),
			NULL, QuizFormProc);
	}
	return TRUE;
}
/************************************************************************************************
��    Ī: WordCardBinderForm_OnMemorizedWordsButtonClicked
��    ��: �ϱ�Ϸ�ī�� ��ư�� Ŭ������ �� �ó�����
*************************************************************************************************/
BOOL WordCardBinderForm_OnMemorizedWordsButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	// �ϱ�Ϸ�ī�� �����츦 ����Ѵ�.
	if (HIWORD(wParam) == BN_CLICKED) {
		DialogBox((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDD_MEMORIZEDWORDSFORM),
			NULL, MemorizedWordsFormProc);
	}
	return TRUE;
}
/************************************************************************************************
��    Ī: WordCardBinderForm_OnFirstButtonClicked
��    ��: ó�� ��ư�� Ŭ������ �� �ó�����
*************************************************************************************************/
BOOL WordCardBinderForm_OnFirstButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	WordCardBinder* wordCardBinder;
	WordCard* index;
	IndexCardBook* indexCardBook;
	IndexCard* link;
	TVITEM item;
	HTREEITEM hRootItem;
	HTREEITEM hIndexCardItem;
	HTREEITEM hWordItem;
	HTREEITEM hWordClassItem;
	Long firstLocation;
	Long location;
	Long i;
	Long j;
	TCHAR alphabet[2];
	TCHAR compare[2];
	TCHAR spelling[16];
	TCHAR compareSpelling[16];
	TCHAR correctRate[8];

	if (HIWORD(wParam) == BN_CLICKED) {
		//6.1. �ܾ�ī��ö���� ó������ �̵��Ѵ�.
		wordCardBinder = (WordCardBinder*)GetWindowLong(hWnd, GWL_USERDATA);
		index = WordCardBinder_First(wordCardBinder);
		//6.2. �ܾ ����.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.spelling);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_WORDCLASS), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.wordClass);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.meaning);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_EXAMPLESENTENCE), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.exampleSentence);
		//sprintf(correctRate, "%.1f", index->evaluation.correctRate);
		//SendMessage(GetDlgItem(hWnd, IDC_EDIT_CORRECTRATE), WM_SETTEXT, (WPARAM)0, (LPARAM)correctRate);
		//6.3. ����ī�������� ã�´�
		indexCardBook = (IndexCardBook*)GetProp(hWnd, "PROP_INDEXCARDBOOK");
		link = IndexCardBook_Find(indexCardBook, index->word.spelling[0]);
		//6.4. ����ī�忡�� ã�´�.
		location = IndexCard_Find(link, index);
		firstLocation = IndexCard_FindFirstTime(link, index->word.spelling);
		//6.5. Ʈ�� �� ��Ʈ�ѿ��� ���ĺ� �׸��� ã�´�.
		compare[0] = link->alphabet;
		compare[1] = '\0';
		hRootItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETNEXTITEM, (WPARAM)TVGN_ROOT, (LPARAM)0);
		hIndexCardItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hRootItem);
		item.mask = TVIF_TEXT | TVIF_HANDLE;
		item.hItem = hIndexCardItem;
		item.pszText = alphabet;
		item.cchTextMax = 2;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETITEM, (WPARAM)0, (LPARAM)&item);
		while (hIndexCardItem != NULL && strcmp(compare, alphabet) != 0) {
			hIndexCardItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETNEXTITEM,
				(WPARAM)TVGN_NEXT, (LPARAM)hIndexCardItem);
			item.hItem = hIndexCardItem;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETITEM, (WPARAM)0, (LPARAM)&item);
		}
		//6.6. Ʈ�� �� ��Ʈ�ѿ��� ���ĺ� �׸��� ��ģ��.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hIndexCardItem);
		//6.7. Ʈ�� �� ��Ʈ���� �ܾ� �׸��� ã�´�.
		strcpy(spelling, index->word.spelling);
		hWordItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hIndexCardItem);
		item.mask = TVIF_TEXT | TVIF_HANDLE;
		item.hItem = hWordItem;
		item.pszText = compareSpelling;
		item.cchTextMax = 16;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETITEM, (WPARAM)0, (LPARAM)&item);
		while (hWordItem != NULL && strcmp(spelling, compareSpelling) != 0) {
			hWordItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETNEXTITEM,
				(WPARAM)TVGN_NEXT, (LPARAM)hWordItem);
			item.hItem = hWordItem;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETITEM, (WPARAM)0, (LPARAM)&item);
		}
		//6.8. Ʈ�� �� ��Ʈ�ѿ��� �ܾ� �׸��� ��ģ��.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hWordItem);
		//6.9. Ʈ�� �� ��Ʈ���� �ܾ� �׸񿡼� �̵��Ѵ�.
		hWordClassItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hWordItem);
		i = 0;
		while (hWordClassItem != NULL && i < (location - firstLocation)) {
			hWordClassItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hWordClassItem);
			i++;
		}
		//6.10. Ʈ�� �� ��Ʈ�ѿ��� ǰ�� �׸��� �����Ѵ�.
		item.mask = TVIF_STATE | TVIF_HANDLE;
		item.hItem = hWordClassItem;
		item.stateMask = TVIS_SELECTED;
		item.state = TVIS_SELECTED;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_SETITEM, (WPARAM)0, (LPARAM)&item);
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_SELECTITEM, (WPARAM)TVGN_CARET, (LPARAM)hWordClassItem);
	}
	return TRUE;
}
/************************************************************************************************
��    Ī: WordCardBinderForm_OnPreviousButtonClicked
��    ��: ���� ��ư�� Ŭ������ �� �ó�����
*************************************************************************************************/
BOOL WordCardBinderForm_OnPreviousButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	WordCardBinder* wordCardBinder;
	WordCard* index;
	IndexCardBook* indexCardBook;
	IndexCard* link;
	TVITEM item;
	HTREEITEM hRootItem;
	HTREEITEM hIndexCardItem;
	HTREEITEM hWordItem;
	HTREEITEM hWordClassItem;
	Long firstLocation;
	Long location;
	Long i;
	Long j;
	TCHAR alphabet[2];
	TCHAR compare[2];
	TCHAR spelling[16];
	TCHAR compareSpelling[16];
	TCHAR correctRate[8];

	if (HIWORD(wParam) == BN_CLICKED) {
		//7.1. �ܾ�ī��ö���� �������� �̵��Ѵ�.
		wordCardBinder = (WordCardBinder*)GetWindowLong(hWnd, GWL_USERDATA);
		index = WordCardBinder_Previous(wordCardBinder);
		//7.2. �ܾ ����.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.spelling);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_WORDCLASS), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.wordClass);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.meaning);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_EXAMPLESENTENCE), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.exampleSentence);
		//sprintf(correctRate, "%.1f", index->evaluation.correctRate);
		//SendMessage(GetDlgItem(hWnd, IDC_EDIT_CORRECTRATE), WM_SETTEXT, (WPARAM)0, (LPARAM)correctRate);
		//7.3. ����ī�������� ã�´�.
		indexCardBook = (IndexCardBook*)GetProp(hWnd, "PROP_INDEXCARDBOOK");
		link = IndexCardBook_Find(indexCardBook, index->word.spelling[0]);
		//7.4. ����ī�忡�� ã�´�.
		location = IndexCard_Find(link, index);
		firstLocation = IndexCard_FindFirstTime(link, index->word.spelling);
		//7.5. Ʈ�� �� ��Ʈ�ѿ��� ���ĺ� �׸��� ã�´�.
		compare[0] = link->alphabet;
		compare[1] = '\0';
		hRootItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETNEXTITEM, (WPARAM)TVGN_ROOT, (LPARAM)0);
		hIndexCardItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hRootItem);
		item.mask = TVIF_TEXT | TVIF_HANDLE;
		item.hItem = hIndexCardItem;
		item.pszText = alphabet;
		item.cchTextMax = 2;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETITEM, (WPARAM)0, (LPARAM)&item);
		while (hIndexCardItem != NULL && strcmp(compare, alphabet) != 0) {
			hIndexCardItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETNEXTITEM,
				(WPARAM)TVGN_NEXT, (LPARAM)hIndexCardItem);
			item.hItem = hIndexCardItem;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETITEM, (WPARAM)0, (LPARAM)&item);
		}
		//7.6. Ʈ�� �� ��Ʈ�ѿ��� ���ĺ� �׸��� ��ģ��.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hIndexCardItem);
		//7.7. Ʈ�� �� ��Ʈ���� �ܾ� �׸��� ã�´�.
		strcpy(spelling, index->word.spelling);
		hWordItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hIndexCardItem);
		item.mask = TVIF_TEXT | TVIF_HANDLE;
		item.hItem = hWordItem;
		item.pszText = compareSpelling;
		item.cchTextMax = 16;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETITEM, (WPARAM)0, (LPARAM)&item);
		while (hWordItem != NULL && strcmp(spelling, compareSpelling) != 0) {
			hWordItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETNEXTITEM,
				(WPARAM)TVGN_NEXT, (LPARAM)hWordItem);
			item.hItem = hWordItem;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETITEM, (WPARAM)0, (LPARAM)&item);
		}
		//7.8. Ʈ�� �� ��Ʈ�ѿ��� �ܾ� �׸��� ��ģ��.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hWordItem);
		//7.9. Ʈ�� �� ��Ʈ���� �ܾ� �׸񿡼� �̵��Ѵ�.
		hWordClassItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hWordItem);
		i = 0;
		while (hWordClassItem != NULL && i < (location - firstLocation)) {
			hWordClassItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hWordClassItem);
			i++;
		}
		//7.10. Ʈ�� �� ��Ʈ�ѿ��� ǰ�� �׸��� �����Ѵ�.
		item.mask = TVIF_STATE | TVIF_HANDLE;
		item.hItem = hWordClassItem;
		item.stateMask = TVIS_SELECTED;
		item.state = TVIS_SELECTED;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_SETITEM, (WPARAM)0, (LPARAM)&item);
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_SELECTITEM, (WPARAM)TVGN_CARET, (LPARAM)hWordClassItem);
	}
	return TRUE;
}
/************************************************************************************************
��    Ī: WordCardBinderForm_OnNextButtonClicked
��    ��: ���� ��ư�� Ŭ������ �� �ó�����
*************************************************************************************************/
BOOL WordCardBinderForm_OnNextButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	WordCardBinder* wordCardBinder;
	WordCard* index;
	IndexCardBook* indexCardBook;
	IndexCard* link;
	TVITEM item;
	HTREEITEM hRootItem;
	HTREEITEM hIndexCardItem;
	HTREEITEM hWordItem;
	HTREEITEM hWordClassItem;
	Long firstLocation;
	Long location;
	Long i;
	Long j;
	TCHAR alphabet[2];
	TCHAR compare[2];
	TCHAR spelling[16];
	TCHAR compareSpelling[16];
	TCHAR correctRate[8];

	if (HIWORD(wParam) == BN_CLICKED) {
		//8.1. �ܾ�ī��ö���� �������� �̵��Ѵ�.
		wordCardBinder = (WordCardBinder*)GetWindowLong(hWnd, GWL_USERDATA);
		index = WordCardBinder_Next(wordCardBinder);
		//8.2. �ܾ ����.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.spelling);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_WORDCLASS), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.wordClass);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.meaning);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_EXAMPLESENTENCE), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.exampleSentence);
		//sprintf(correctRate, "%.1f", index->evaluation.correctRate);
		//SendMessage(GetDlgItem(hWnd, IDC_EDIT_CORRECTRATE), WM_SETTEXT, (WPARAM)0, (LPARAM)correctRate);
		//8.3. ����ī�������� ã�´�.
		indexCardBook = (IndexCardBook*)GetProp(hWnd, "PROP_INDEXCARDBOOK");
		link = IndexCardBook_Find(indexCardBook, index->word.spelling[0]);
		//8.4. ����ī�忡�� ã�´�.
		location = IndexCard_Find(link, index);
		firstLocation = IndexCard_FindFirstTime(link, index->word.spelling);
		//8.5. Ʈ�� �� ��Ʈ�ѿ��� ���ĺ� �׸��� ã�´�.
		compare[0] = link->alphabet;
		compare[1] = '\0';
		hRootItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETNEXTITEM, (WPARAM)TVGN_ROOT, (LPARAM)0);
		hIndexCardItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hRootItem);
		item.mask = TVIF_TEXT | TVIF_HANDLE;
		item.hItem = hIndexCardItem;
		item.pszText = alphabet;
		item.cchTextMax = 2;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETITEM, (WPARAM)0, (LPARAM)&item);
		while (hIndexCardItem != NULL && strcmp(compare, alphabet) != 0) {
			hIndexCardItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETNEXTITEM,
				(WPARAM)TVGN_NEXT, (LPARAM)hIndexCardItem);
			item.hItem = hIndexCardItem;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETITEM, (WPARAM)0, (LPARAM)&item);
		}
		//8.6. Ʈ�� �� ��Ʈ�ѿ��� ���ĺ� �׸��� ��ģ��.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hIndexCardItem);
		//8.7. Ʈ�� �� ��Ʈ���� �ܾ� �׸��� ã�´�.
		strcpy(spelling, index->word.spelling);
		hWordItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hIndexCardItem);
		item.mask = TVIF_TEXT | TVIF_HANDLE;
		item.hItem = hWordItem;
		item.pszText = compareSpelling;
		item.cchTextMax = 16;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETITEM, (WPARAM)0, (LPARAM)&item);
		while (hWordItem != NULL && strcmp(spelling, compareSpelling) != 0) {
			hWordItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETNEXTITEM,
				(WPARAM)TVGN_NEXT, (LPARAM)hWordItem);
			item.hItem = hWordItem;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETITEM, (WPARAM)0, (LPARAM)&item);
		}
		//8.8. Ʈ�� �� ��Ʈ�ѿ��� �ܾ� �׸��� ��ģ��.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hWordItem);
		//8.9. Ʈ�� �� ��Ʈ���� �ܾ� �׸񿡼� �̵��Ѵ�.
		hWordClassItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hWordItem);
		i = 0;
		while (hWordClassItem != NULL && i < (location - firstLocation)) {
			hWordClassItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hWordClassItem);
			i++;
		}
		//8.10. Ʈ�� �� ��Ʈ�ѿ��� ǰ�� �׸��� �����Ѵ�.
		item.mask = TVIF_STATE | TVIF_HANDLE;
		item.hItem = hWordClassItem;
		item.stateMask = TVIS_SELECTED;
		item.state = TVIS_SELECTED;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_SETITEM, (WPARAM)0, (LPARAM)&item);
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_SELECTITEM, (WPARAM)TVGN_CARET, (LPARAM)hWordClassItem);
	}
	return TRUE;
}
/************************************************************************************************
��    Ī: WordCardBinderForm_OnLastButtonClicked
��    ��: ������ ��ư�� Ŭ������ �� �ó�����
*************************************************************************************************/
BOOL WordCardBinderForm_OnLastButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	WordCardBinder* wordCardBinder;
	WordCard* index;
	IndexCardBook* indexCardBook;
	IndexCard* link;
	TVITEM item;
	HTREEITEM hRootItem;
	HTREEITEM hIndexCardItem;
	HTREEITEM hWordItem;
	HTREEITEM hWordClassItem;
	Long firstLocation;
	Long location;
	Long i;
	Long j;
	TCHAR alphabet[2];
	TCHAR compare[2];
	TCHAR spelling[16];
	TCHAR compareSpelling[16];
	TCHAR correctRate[8];

	if (HIWORD(wParam) == BN_CLICKED) {
		//9.1. �ܾ�ī��ö���� ���������� �̵��Ѵ�.
		wordCardBinder = (WordCardBinder*)GetWindowLong(hWnd, GWL_USERDATA);
		index = WordCardBinder_Last(wordCardBinder);
		//9.2. �ܾ ����.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.spelling);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_WORDCLASS), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.wordClass);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.meaning);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_EXAMPLESENTENCE), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.exampleSentence);
		//sprintf(correctRate, "%.1f", index->evaluation.correctRate);
		//SendMessage(GetDlgItem(hWnd, IDC_EDIT_CORRECTRATE), WM_SETTEXT, (WPARAM)0, (LPARAM)correctRate);
		//9.3. ����ī�������� ã�´�.
		indexCardBook = (IndexCardBook*)GetProp(hWnd, "PROP_INDEXCARDBOOK");
		link = IndexCardBook_Find(indexCardBook, index->word.spelling[0]);
		//9.4. ����ī�忡�� ã�´�.
		location = IndexCard_Find(link, index);
		firstLocation = IndexCard_FindFirstTime(link, index->word.spelling);
		//9.5. Ʈ�� �� ��Ʈ�ѿ��� ���ĺ� �׸��� ã�´�.
		compare[0] = link->alphabet;
		compare[1] = '\0';
		hRootItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETNEXTITEM, (WPARAM)TVGN_ROOT, (LPARAM)0);
		hIndexCardItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hRootItem);
		item.mask = TVIF_TEXT | TVIF_HANDLE;
		item.hItem = hIndexCardItem;
		item.pszText = alphabet;
		item.cchTextMax = 2;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETITEM, (WPARAM)0, (LPARAM)&item);
		while (hIndexCardItem != NULL && strcmp(compare, alphabet) != 0) {
			hIndexCardItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETNEXTITEM,
				(WPARAM)TVGN_NEXT, (LPARAM)hIndexCardItem);
			item.hItem = hIndexCardItem;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETITEM, (WPARAM)0, (LPARAM)&item);
		}
		//9.6. Ʈ�� �� ��Ʈ�ѿ��� ���ĺ� �׸��� ��ģ��.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hIndexCardItem);
		//9.7. Ʈ�� �� ��Ʈ���� �ܾ� �׸��� ã�´�.
		strcpy(spelling, index->word.spelling);
		hWordItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hIndexCardItem);
		item.mask = TVIF_TEXT | TVIF_HANDLE;
		item.hItem = hWordItem;
		item.pszText = compareSpelling;
		item.cchTextMax = 16;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETITEM, (WPARAM)0, (LPARAM)&item);
		while (hWordItem != NULL && strcmp(spelling, compareSpelling) != 0) {
			hWordItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETNEXTITEM,
				(WPARAM)TVGN_NEXT, (LPARAM)hWordItem);
			item.hItem = hWordItem;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETITEM, (WPARAM)0, (LPARAM)&item);
		}
		//9.8. Ʈ�� �� ��Ʈ�ѿ��� �ܾ� �׸��� ��ģ��.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hWordItem);
		//9.9. Ʈ�� �� ��Ʈ���� �ܾ� �׸񿡼� �̵��Ѵ�.
		hWordClassItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hWordItem);
		i = 0;
		while (hWordClassItem != NULL && i < (location - firstLocation)) {
			hWordClassItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hWordClassItem);
			i++;
		}
		//9.10. Ʈ�� �� ��Ʈ�ѿ��� ǰ�� �׸��� �����Ѵ�.
		item.mask = TVIF_STATE | TVIF_HANDLE;
		item.hItem = hWordClassItem;
		item.stateMask = TVIS_SELECTED;
		item.state = TVIS_SELECTED;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_SETITEM, (WPARAM)0, (LPARAM)&item);
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_SELECTITEM, (WPARAM)TVGN_CARET, (LPARAM)hWordClassItem);
	}
	return TRUE;
}
/************************************************************************************************
��    Ī: WordCardBinderForm_OnNotify
��    ��: WM_NOTIFY �޼��� �߻� �� �ó�����
*************************************************************************************************/
BOOL WordCardBinderForm_OnNotify(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BOOL ret;
	switch (((LPNMHDR)lParam)->idFrom) {
	case IDC_TREE_INDEXCARDBOOK:
		ret = WordCardBinderForm_OnWordItemDoubleClicked(hWnd, wParam, lParam);
		break;
	default:
		ret = FALSE;
		break;
	}
	return ret;
}
/************************************************************************************************
��    Ī: WordCardBinderForm_OnWordItemDoubleClicked
��    ��: �ܾ� �׸��� ����Ŭ�� ���� �� �ó�����
*************************************************************************************************/
BOOL WordCardBinderForm_OnWordItemDoubleClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	WordCardBinder* wordCardBinder;
	WordCard* index;
	IndexCardBook* indexCardBook;
	IndexCard* link;
	TVITEM item;
	HTREEITEM hIndexCardItem;
	HTREEITEM hWordItem;
	HTREEITEM hWordClassItem;
	HTREEITEM hSelectedItem;
	Long firstLocation;
	Long location;
	TCHAR alphabet[2];
	TCHAR spelling[16];
	TCHAR correctRate[8];

	if (((LPNMHDR)lParam)->code == NM_DBLCLK){
		//10.1. Ʈ�� �� ��Ʈ�ѿ��� ���õ� �׸��� �д´�.
		hSelectedItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETNEXTITEM, (WPARAM)TVGN_CARET, (LPARAM)0);
		//10.2. Ʈ�� �� ��Ʈ�ѿ��� �ܾ� �׸��� �д´�.
		hWordItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETNEXTITEM, (WPARAM)TVGN_PARENT, (LPARAM)hSelectedItem);
		item.mask = TVIF_TEXT | TVIF_HANDLE;
		item.hItem = hWordItem;
		item.pszText = spelling;
		item.cchTextMax = 16;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETITEM, (WPARAM)0, (LPARAM)&item);
		//10.3. Ʈ�� �� ��Ʈ���� �ܾ� �׸񿡼� �̵��Ѵ�.
		hWordClassItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hWordItem);
		location = 0;
		while (hWordClassItem != NULL && hWordClassItem != hSelectedItem) {
			hWordClassItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hWordClassItem);
			location++;
		}
		//10.4. Ʈ�� �� ��Ʈ�ѿ��� ��ȣ �׸��� �д´�.
		hIndexCardItem = (HTREEITEM)SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETNEXTITEM, (WPARAM)TVGN_PARENT, (LPARAM)hWordItem);
		item.mask = TVIF_TEXT | TVIF_HANDLE;
		item.hItem = hIndexCardItem;
		item.pszText = alphabet;
		item.cchTextMax = 2;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXCARDBOOK), TVM_GETITEM, (WPARAM)0, (LPARAM)&item);
		//10.4. ����ī�������� ã�´�.
		indexCardBook = (IndexCardBook*)GetProp(hWnd, "PROP_INDEXCARDBOOK");
		link = IndexCardBook_Find(indexCardBook, alphabet[0]);
		//10.5. ����ī�忡�� ã�´�.
		firstLocation = IndexCard_FindFirstTime(link, spelling);
		index = IndexCard_GetAt(link, (firstLocation + location));
		//10.6. �ܾ�ī��ö���� �̵��Ѵ�.
		wordCardBinder = (WordCardBinder*)GetWindowLong(hWnd, GWL_USERDATA);
		WordCardBinder_Move(wordCardBinder, index);
		//10.7. ���� �ܾ ����,
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
��    Ī: WordCardBinderForm_OnClose
��    ��: �ݱ� ��ư�� Ŭ������ �� �ó�����
*************************************************************************************************/
BOOL WordCardBinderForm_OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	WordCardBinder* wordCardBinder;
	WordCardBinder* memorizedWords;
	IndexCardBook* indexCardBook;

	wordCardBinder = (WordCardBinder*)GetWindowLong(hWnd, GWL_USERDATA);
	memorizedWords = (WordCardBinder*)GetProp(hWnd, "PROP_MEMORIZEDWORDS");
	if (wordCardBinder != NULL) {
		//11.1. �ܾ�ī��ö���� �����Ѵ�.
		Save(wordCardBinder, memorizedWords);
		//11.2. �ܾ�ī��ö�� ���ش�.
		WordCardBinder_Destroy(wordCardBinder);
		free(wordCardBinder);
	}
	if (memorizedWords != NULL) {
		WordCardBinder_Destroy(memorizedWords);
		free(memorizedWords);
		RemoveProp(hWnd, "PROP_MEMORIZEDWORDS");
	}
	//11.3. ����ī������ ���ش�.
	indexCardBook = (IndexCardBook*)GetProp(hWnd, "PROP_INDEXCARDBOOK");
	if (indexCardBook != NULL) {
		IndexCardBook_Destroy(indexCardBook);
		free(indexCardBook);
		RemoveProp(hWnd, "PROP_INDEXCARDBOOK");
	}
	//11.4. �����츦 �ݴ´�.
	EndDialog(hWnd, 0);

	return TRUE;
}