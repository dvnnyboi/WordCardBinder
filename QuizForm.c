//QuizForm.c
/************************************************************************************************
���ϸ�Ī: QuizForm.c
��    ��: �ܾ�ī��ö ���� ������ ����
�� �� ��: �ӴٴϿ�
�ۼ�����: 2022/07/21
*************************************************************************************************/
#include "QuizForm.h"
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
��    Ī: QuizFormProc
��    ��: ���������� ���ν����Լ�
*************************************************************************************************/
BOOL CALLBACK QuizFormProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	BOOL ret;

	switch (message) {
	case WM_INITDIALOG:
		ret = QuizForm_OnInitDialog(hWnd, wParam, lParam);
		break;
	case WM_COMMAND:
		ret = QuizForm_OnCommand(hWnd, wParam, lParam);
		break;
	case WM_CLOSE:
		ret = QuizForm_OnClose(hWnd, wParam, lParam);
		break;
	default:
		ret = FALSE;
		break;
	}
	return ret;
}
/************************************************************************************************
��    Ī: QuizForm_OnInitDialog
��    ��: �����찡 ������ �� �ó�����.
*************************************************************************************************/
BOOL QuizForm_OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	HWND hWordCardBinderFormWnd;
	WordCardBinder* wordCardBinder;
	WordCard* index;
	TCHAR correctRate[8];

	//1.1. �ܾ�ī��ö �����츦 ã�´�.
	hWordCardBinderFormWnd = FindWindow("#32770", "�ܾ�ī��ö");
	//1.2. �ܾ�ī��ö �������� �ܾ�ī��ö���� ó������ �̵��Ѵ�.
	wordCardBinder = (WordCardBinder*)GetWindowLong(hWordCardBinderFormWnd, GWL_USERDATA);
	index = WordCardBinder_First(wordCardBinder);
	//1.3. ö�ڿ� ������� ����.
	EnableWindow(GetDlgItem(hWnd, IDC_EDIT_MEANING), FALSE);
	EnableWindow(GetDlgItem(hWnd, IDC_EDIT_EXAMPLESENTENCE), FALSE);
	if (wordCardBinder->length > 0) {
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.spelling);
		//sprintf(correctRate, "%.1f %", index->evaluation.correctRate);
		//SendMessage(GetDlgItem(hWnd, IDC_EDIT_CORRECTRATE), WM_SETTEXT, (WPARAM)0, (LPARAM)correctRate);
	}
	else {
		EnableWindow(GetDlgItem(hWnd, IDC_EDIT_WORDCLASS), FALSE);
		EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_GRADEQUIZ), FALSE);
	}
	return TRUE;
}
/************************************************************************************************
��    Ī: QuizForm_OnCommand
��    ��: WM_COMMAND �޼��� �߻� �� �ó�����.
*************************************************************************************************/
BOOL QuizForm_OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BOOL ret;

	switch (LOWORD(wParam)) {
	case IDC_BUTTON_GRADEQUIZ:
		ret = QuizForm_OnGradeQuizButtonClicked(hWnd, wParam, lParam);
		break;
	case IDC_EDIT_WORDCLASS:
		ret = QuizForm_OnWordClassKillFocus(hWnd, wParam, lParam);
		break;
	case IDC_EDIT_MEANING:
		ret = QuizForm_OnMeaningKillFocus(hWnd, wParam, lParam);
		break;
	case IDC_EDIT_EXAMPLESENTENCE:
		ret = QuizForm_OnExampleSentenceKillFocus(hWnd, wParam, lParam);
		break;
	case IDOK:
		ret = QuizForm_OnEnterKey(hWnd, wParam, lParam);
		break;
	default:
		ret = FALSE;
		break;
	}
	return ret;
}

BOOL QuizForm_OnEnterKey(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	TCHAR markWordClass[8];
	TCHAR markMeaning[8];
	TCHAR markExampleSentence[8];
	
	if (HIWORD(wParam) == BN_CLICKED) {
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_MARKWORDCLASS), WM_GETTEXT, (WPARAM)8, (LPARAM)markWordClass);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_MARKMEANING), WM_GETTEXT, (WPARAM)8, (LPARAM)markMeaning);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_MARKEXAMPLE), WM_GETTEXT, (WPARAM)8, (LPARAM)markExampleSentence);
		if (strcmp(markWordClass, "O") != 0) {
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_WORDCLASS), WM_KILLFOCUS, (WPARAM)NULL, 0);
		}
		else if (strcmp(markWordClass, "O") == 0 && strcmp(markMeaning, "O") != 0) {
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_KILLFOCUS, (WPARAM)NULL, 0);
		}
		else if (strcmp(markWordClass, "O") == 0 && strcmp(markMeaning, "O") == 0 && strcmp(markExampleSentence, "O") != 0) {
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_EXAMPLESENTENCE), WM_KILLFOCUS, (WPARAM)NULL, 0);
		}
		else if (strcmp(markWordClass, "O") == 0 && strcmp(markMeaning, "O") == 0 && strcmp(markExampleSentence, "O") == 0) {
			SendMessage(GetDlgItem(hWnd, IDC_BUTTON_GRADEQUIZ), BM_CLICK, 0, 0);
		}
	}
	return TRUE;
}
/************************************************************************************************
��    Ī: QuizForm_OnGradeQuizButtonClicked
��    ��: ä���ϱ� ��ư�� Ŭ������ �� �ó�����.
*************************************************************************************************/
BOOL QuizForm_OnGradeQuizButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	HWND hWordCardBinderFormWnd;
	WordCardBinder* wordCardBinder;
	WordCardBinder* memorizedWords;
	WordCard* index;
	WordCard wordCard;
	IndexCardBook* indexCardBook;
	IndexCard* link;
	TVITEM item;
	HTREEITEM hRootItem;
	HTREEITEM hIndexCardItem;
	HTREEITEM hWordItem;
	HTREEITEM hWordClassItem;
	TCHAR wordClass[16];
	TCHAR meaning[16];
	TCHAR exampleSentence[64];
	TCHAR alphabet[2];
	TCHAR compare[2];
	TCHAR spelling[16];
	TCHAR compareSpelling[16];
	TCHAR correctRate[8];
	Long wordClassResult;
	Long meaningResult;
	Long exampleSentenceResult;
	Long location;
	Long firstLocation;
	Long i;
	Long correctCount = 0;

	if (HIWORD(wParam) == BN_CLICKED) {
		//2.1. �ܾ �д´�.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_WORDCLASS), WM_GETTEXT, (WPARAM)16, (LPARAM)wordClass);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_GETTEXT, (WPARAM)16, (LPARAM)meaning);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_EXAMPLESENTENCE), WM_GETTEXT, (WPARAM)64, (LPARAM)exampleSentence);
		//2.2. �ܾ�ī��ö �����츦 ã�´�.
		hWordCardBinderFormWnd = FindWindow("#32770", "�ܾ�ī��ö");
		wordCardBinder = (WordCardBinder*)GetWindowLong(hWordCardBinderFormWnd, GWL_USERDATA);
		memorizedWords = (WordCardBinder*)GetProp(hWordCardBinderFormWnd, "PROP_MEMORIZEDWORDS");
		//2.3. �ܾ�ī��ö �������� �ܾ�ī��ö���� ǰ�縦 ä���Ѵ�.
		wordClassResult = CheckWordClass(wordCardBinder, wordClass);
		if (wordClassResult == 0) {
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_MARKWORDCLASS), WM_SETTEXT, (WPARAM)0, (LPARAM)"O");
			correctCount++;
		}
		else {
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_MARKWORDCLASS), WM_SETTEXT, (WPARAM)0, (LPARAM)"X");
		}
		//2.4.�ܾ�ī��ö �������� �ܾ�ī��ö���� ���� ä���Ѵ�.
		meaningResult = CheckMeaning(wordCardBinder, meaning);
		if (meaningResult == 0) {
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_MARKMEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)"O");
			correctCount++;
		}
		else {
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_MARKMEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)"X");
		}
		//2.5. �ܾ�ī��ö �������� �ܾ�ī��ö���� ������ ä���Ѵ�.
		exampleSentenceResult = CheckExampleSentence(wordCardBinder, exampleSentence);
		if (exampleSentenceResult == 0) {
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_MARKEXAMPLE), WM_SETTEXT, (WPARAM)0, (LPARAM)"O");
			correctCount++;
		}
		else {
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_MARKEXAMPLE), WM_SETTEXT, (WPARAM)0, (LPARAM)"X");
		}
		index = wordCardBinder->current;
		index->evaluation.quiz++;
		if (correctCount == 3) {
			index->evaluation.correct++;
		}
		index->evaluation.correctRate = index->evaluation.correct / (index->evaluation.quiz * 1.00F) * 100.0;
		//2.6. ������� ����.
		//sprintf(correctRate, "%.1f %", index->evaluation.correctRate);
		//SendMessage(GetDlgItem(hWnd, IDC_EDIT_CORRECTRATE), WM_SETTEXT, (WPARAM)0, (LPARAM)correctRate);
		//2.7. ���� �¾�����
		indexCardBook = (IndexCardBook*)GetProp(hWordCardBinderFormWnd, "PROP_INDEXCARDBOOK");
		if (correctCount == 3) {
			//2.7.1. �ܾ�ī��ö �������� ����ī�������� ã�´�.
			link = IndexCardBook_Find(indexCardBook, index->word.spelling[0]);
			//2.7.2. �ܾ�ī��ö �������� ����ī�忡�� ã�´�.
			location = IndexCard_Find(link, index);
			firstLocation = IndexCard_FindFirstTime(link, index->word.spelling);
			//2.7.3. �ܾ�ī��ö �������� Ʈ�� �� ��Ʈ�ѿ��� ��Ʈ �׸��� ��ģ��.
			hRootItem = (HTREEITEM)SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK),
				TVM_GETNEXTITEM, (WPARAM)TVGN_ROOT, (LPARAM)0);
			SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK),
				TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hRootItem);
			//2.7.4. �ܾ�ī��ö �������� Ʈ�� �� ��Ʈ�ѿ��� ���ĺ� �׸��� ã�´�.
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
			//2.7.5. �ܾ�ī��ö �������� Ʈ�� �� ��Ʈ�ѿ��� ���ĺ� �׸��� ��ģ��.
			SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hIndexCardItem);
			//2.7.6. �ܾ�ī��ö �������� Ʈ�� �� ��Ʈ���� �ܾ� �׸��� ã�´�.
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
			//2.7.7. �ܾ�ī��ö �������� Ʈ�� �� ��Ʈ�ѿ��� �ܾ� �׸��� ��ģ��.
			SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hWordItem);
			//2.7.8. �ܾ�ī��ö ������ Ʈ�� �� ��Ʈ���� �ܾ� �׸񿡼� �̵��Ѵ�.
			hWordClassItem = (HTREEITEM)SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK),
				TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hWordItem);
			i = 0;
			while (hWordClassItem != NULL && i < (location - firstLocation)) {
				hWordClassItem = (HTREEITEM)SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK),
					TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hWordClassItem);
				i++;
			}
			//2.7.9. �ܾ�ī��ö ������ Ʈ�� �� ��Ʈ���� ǰ�� �׸��� �����.
			SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK), TVM_DELETEITEM,
				(WPARAM)0, (LPARAM)hWordClassItem);
			//2.7.10. �ܾ�ī��ö �������� Ʈ�� �� ��Ʈ�ѿ��� �ܾ� �׸��� �������
			hWordClassItem = (HTREEITEM)SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK),
				TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hWordItem);
			if (hWordClassItem == NULL) {
				//2.7.10.1. �ܾ�ī��ö �������� Ʈ�� �� ��Ʈ�ѿ��� �ܾ� �׸��� �����.
				SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK), TVM_DELETEITEM,
					(WPARAM)0, (LPARAM)hWordItem);
			}
			//2.7.11. �ܾ�ī��ö �������� Ʈ�� �� ��Ʈ�ѿ��� ���ĺ� �׸��� �������
			hWordItem = (HTREEITEM)SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK),
				TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hIndexCardItem);
			if (hWordItem == NULL) {
				//2.7.11.1. �ܾ�ī��ö �������� Ʈ�� �� ��Ʈ�ѿ������ĺ��� �׸��� �����.
				SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK), TVM_DELETEITEM,
					(WPARAM)0, (LPARAM)hIndexCardItem);
			}
			//2.7.12. �ܾ�ī��ö �������� �ܾ�ī��ö���� ����.
			wordCard = TakeOut(wordCardBinder, index);
			//2.7.13. �ܾ�ī��ö �������� ����ī�������� ����.
			link = IndexCardBook_TakeOut(indexCardBook, wordCard.word.spelling[0], index);
			//2.7.14. �ܾ�ī��ö �������� �ϱ�Ϸ� ī��ö�� �����.
			TakeIn(memorizedWords, wordCard);
			//2.7.15. ���� �ܾ ����.
			if (wordCardBinder->length > 0) {
				index = wordCardBinder->current;
				SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.spelling);
				SetFocus(GetDlgItem(hWnd, IDC_EDIT_WORDCLASS));
				//sprintf(correctRate, "%.1f %", index->evaluation.correctRate);
				//SendMessage(GetDlgItem(hWnd, IDC_EDIT_CORRECTRATE), WM_SETTEXT, (WPARAM)0, (LPARAM)correctRate);

			}
			else {
				EnableWindow(GetDlgItem(hWnd, IDC_EDIT_WORDCLASS), FALSE);
				EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_GRADEQUIZ), FALSE);
				SendMessage(GetDlgItem(hWnd, IDC_EDIT_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
				SendMessage(GetDlgItem(hWnd, IDC_EDIT_CORRECTRATE), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
			}
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_WORDCLASS), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_EXAMPLESENTENCE), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_MARKWORDCLASS), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_MARKMEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_MARKEXAMPLE), WM_SETTEXT, (WPARAM)0, (LPARAM)"");
			EnableWindow(GetDlgItem(hWnd, IDC_EDIT_MEANING), FALSE);
			EnableWindow(GetDlgItem(hWnd, IDC_EDIT_EXAMPLESENTENCE), FALSE);
		}
	}
	return TRUE;
}
/************************************************************************************************
��    Ī: QuizForm_OnWordClassKillFocus
��    ��: ǰ���� ��Ŀ���� �Ҿ��� �� �ó�����.
*************************************************************************************************/
BOOL QuizForm_OnWordClassKillFocus(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	HWND hWordCardBinderFormWnd;
	WordCardBinder* wordCardBinder;
	TCHAR wordClass[16];
	Long wordClassResult;

	if (HIWORD(wParam) == EN_KILLFOCUS) {
		//4.1. ǰ�縦 �д´�.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_WORDCLASS), WM_GETTEXT, (WPARAM)16, (LPARAM)wordClass);
		//4.2. �ܾ�ī��ö �����츦 ã�´�.
		hWordCardBinderFormWnd = FindWindow("#32770", "�ܾ�ī��ö");
		wordCardBinder = (WordCardBinder*)GetWindowLong(hWordCardBinderFormWnd, GWL_USERDATA);
		//4.3. �ܾ�ī��ö �������� �ܾ�ī��ö���� ǰ�縦 ä���Ѵ�.
		wordClassResult = CheckWordClass(wordCardBinder, wordClass);
		//4.4. ��������
		if (wordClassResult == 0) {
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_MARKWORDCLASS), WM_SETTEXT, (WPARAM)0, (LPARAM)"O");
			//4.4.1. ���� Ȱ��ȭ�Ѵ�.
			EnableWindow(GetDlgItem(hWnd, IDC_EDIT_MEANING), TRUE);
			SetFocus(GetDlgItem(hWnd, IDC_EDIT_MEANING));
		}
		else {
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_MARKWORDCLASS), WM_SETTEXT, (WPARAM)0, (LPARAM)"X");
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_WORDCLASS), WM_SETFOCUS, (WPARAM)NULL, 0);
			//SetFocus(GetDlgItem(hWnd, IDC_EDIT_WORDCLASS));
		}
	}
	return TRUE;
}
/************************************************************************************************
��    Ī: QuizForm_OnMeaningKillFocus
��    ��: ���� ��Ŀ���� �Ҿ��� �� �ó�����.
*************************************************************************************************/
BOOL QuizForm_OnMeaningKillFocus(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	HWND hWordCardBinderFormWnd;
	WordCardBinder* wordCardBinder;
	TCHAR meaning[16];
	Long meaningResult;

	if (HIWORD(wParam) == EN_KILLFOCUS) {
		//5.1. ���� �д´�.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_GETTEXT, (WPARAM)16, (LPARAM)meaning);
		//5.2. �ܾ�ī��ö �����츦 ã�´�.
		hWordCardBinderFormWnd = FindWindow("#32770", "�ܾ�ī��ö");
		wordCardBinder = (WordCardBinder*)GetWindowLong(hWordCardBinderFormWnd, GWL_USERDATA);
		//5.3. �ܾ�ī��ö �������� �ܾ�ī��ö���� ���� ä���Ѵ�.
		meaningResult = CheckMeaning(wordCardBinder, meaning);
		//5.4. ��������
		if (meaningResult == 0) {
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_MARKMEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)"O");
			//5.4.1. ������ Ȱ��ȭ�Ѵ�.
			EnableWindow(GetDlgItem(hWnd, IDC_EDIT_EXAMPLESENTENCE), TRUE);
			SetFocus(GetDlgItem(hWnd, IDC_EDIT_EXAMPLESENTENCE));
		}
		else {
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_MARKMEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)"X");
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_MEANING), WM_SETFOCUS, (WPARAM)NULL, 0);
			//SetFocus(GetDlgItem(hWnd, IDC_EDIT_MEANING));
		}
	}
	return TRUE;
}
/************************************************************************************************
��    Ī: QuizForm_OnExampleSentenceKillFocus
��    ��: ������ ��Ŀ���� �Ҿ��� �� �ó�����.
*************************************************************************************************/
BOOL QuizForm_OnExampleSentenceKillFocus(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	HWND hWordCardBinderFormWnd;
	WordCardBinder* wordCardBinder;
	TCHAR exampleSentence[64];
	Long exampleSentenceResult;

	if (HIWORD(wParam) == EN_KILLFOCUS) {
		//6.1. ������ �д´�.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_EXAMPLESENTENCE), WM_GETTEXT, (WPARAM)64, (LPARAM)exampleSentence);
		//6.2. �ܾ�ī��ö �����츦 ã�´�.
		hWordCardBinderFormWnd = FindWindow("#32770", "�ܾ�ī��ö");
		wordCardBinder = (WordCardBinder*)GetWindowLong(hWordCardBinderFormWnd, GWL_USERDATA);
		//6.3. �ܾ�ī��ö �������� �ܾ�ī��ö���� ������ ä���Ѵ�.
		exampleSentenceResult = CheckExampleSentence(wordCardBinder, exampleSentence);
		//6.4. ��������
		if (exampleSentenceResult == 0) {
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_MARKEXAMPLE), WM_SETTEXT, (WPARAM)0, (LPARAM)"O");
			SendMessage(GetDlgItem(hWnd, IDC_BUTTON_GRADEQUIZ), WM_SETFOCUS, (WPARAM)NULL, 0);
		}
		else {
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_MARKEXAMPLE), WM_SETTEXT, (WPARAM)0, (LPARAM)"X");
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_EXAMPLESENTENCE), WM_SETFOCUS, (WPARAM)NULL, 0);
		}
	}
	return TRUE;
}
/************************************************************************************************
��    Ī: QuizForm_OnClose
��    ��: �ݱ� ��ư�� Ŭ������ �� �ó�����.
*************************************************************************************************/
BOOL QuizForm_OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	HWND hWordCardBinderFormWnd;
	WordCardBinder* wordCardBinder;
	WordCardBinder* memorizedWords;
	WordCard* index;
	WordCard* previous = NULL;
	WordCard* last;
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
	TCHAR mark[5];

	//3.1. �ܾ�ī��ö �����츦 ã�´�.
	hWordCardBinderFormWnd = FindWindow("#32770", "�ܾ�ī��ö");
	wordCardBinder = (WordCardBinder*)GetWindowLong(hWordCardBinderFormWnd, GWL_USERDATA);
	indexCardBook = (IndexCardBook*)GetProp(hWordCardBinderFormWnd, "PROP_INDEXCARDBOOK");
	if(wordCardBinder->length > 0){
		//3.2. �ܾ�ī��ö �������� �ܾ�ī��ö���� ó������ �̵��Ѵ�.
		index = WordCardBinder_First(wordCardBinder);
		//3.3. �ܾ�ī��ö �������� �ܾ ����.
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_EDIT_SPELLING), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.spelling);
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_EDIT_WORDCLASS), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.wordClass);
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_EDIT_MEANING), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.meaning);
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_EDIT_EXAMPLESENTENCE), WM_SETTEXT, (WPARAM)0, (LPARAM)index->word.exampleSentence);
		//sprintf(correctRate, "%.1f %", index->evaluation.correctRate);
		//SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_EDIT_CORRECTRATE), WM_SETTEXT, (WPARAM)0, (LPARAM)correctRate);
		//3.4. �ܾ�ī��ö �������� ����ī�������� ã�´�
		link = IndexCardBook_Find(indexCardBook, index->word.spelling[0]);
		//3.5. �ܾ�ī��ö �������� ����ī�忡�� ã�´�.
		location = IndexCard_Find(link, index);
		firstLocation = IndexCard_FindFirstTime(link, index->word.spelling);
		//3.6. �ܾ�ī��ö �������� Ʈ�� �� ��Ʈ�ѿ��� ��Ʈ �׸��� ��ģ��.
		hRootItem = (HTREEITEM)SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK),
			TVM_GETNEXTITEM, (WPARAM)TVGN_ROOT, (LPARAM)0);
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hRootItem);
		//3.7. �ܾ�ī��ö �������� Ʈ�� �� ��Ʈ�ѿ��� ���ĺ� �׸��� ã�´�.
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
		//3.8. �ܾ�ī��ö �������� Ʈ�� �� ��Ʈ�ѿ��� ���ĺ� �׸��� ��ģ��.
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hIndexCardItem);
		//3.9. �ܾ�ī��ö �������� Ʈ�� �� ��Ʈ���� �ܾ� �׸��� ã�´�.
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
		//3.10. �ܾ�ī��ö �������� Ʈ�� �� ��Ʈ�ѿ��� �ܾ� �׸��� ��ģ��.
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)hWordItem);
		//3.11. �ܾ�ī��ö ������ Ʈ�� �� ��Ʈ���� �ܾ� �׸񿡼� �̵��Ѵ�.
		hWordClassItem = (HTREEITEM)SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK),
			TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD, (LPARAM)hWordItem);
		i = 0;
		while (hWordClassItem != NULL && i < (location - firstLocation)) {
			hWordClassItem = (HTREEITEM)SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK),
				TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT, (LPARAM)hWordClassItem);
			i++;
		}
		//3.12. �ܾ�ī��ö �������� Ʈ�� �� ��Ʈ�ѿ��� ǰ�� �׸��� �����Ѵ�.
		item.mask = TVIF_STATE | TVIF_HANDLE;
		item.hItem = hWordClassItem;
		item.stateMask = TVIS_SELECTED;
		item.state = TVIS_SELECTED;
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK), TVM_SETITEM, (WPARAM)0, (LPARAM)&item);
		SendMessage(GetDlgItem(hWordCardBinderFormWnd, IDC_TREE_INDEXCARDBOOK), TVM_SELECTITEM, (WPARAM)TVGN_CARET, (LPARAM)hWordClassItem);
	}
	else{
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
	//3.13. ���� �����츦 �ݴ´�.
	EndDialog(hWnd, 0);
	return TRUE;
}
