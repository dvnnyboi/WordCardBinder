//WordCardBinderForm.h
/************************************************************************************************
파일명칭: WordCardBinderForm.h
기    능: 단어카드철 메인윈도우 함수 선언
작 성 자: 임다니엘
작성일자: 2022/07/15
*************************************************************************************************/
#ifndef _WORDCARDBINDERFORM_H
#define _WORDCARDBINDERFORM_H
#include <Windows.h>

BOOL CALLBACK WordCardBinderFormProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL WordCardBinderForm_OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL WordCardBinderForm_OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL WordCardBinderForm_OnTakeInButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL WordCardBinderForm_OnFindButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL WordCardBinderForm_OnTakeOutButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL WordCardBinderForm_OnArrangeButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL WordCardBinderForm_OnFirstButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL WordCardBinderForm_OnPreviousButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL WordCardBinderForm_OnNextButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL WordCardBinderForm_OnLastButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL WordCardBinderForm_OnQuizButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL WordCardBinderForm_OnMemorizedWordsButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL WordCardBinderForm_OnNotify(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL WordCardBinderForm_OnWordItemDoubleClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL WordCardBinderForm_OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam);

#endif //_WORDCARDBINDERFORM_H
