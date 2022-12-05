//MemorizedWordsForm.h
/************************************************************************************************
파일명칭: MemorizedWordsForm.h
기    능: 단어카드철 암기완료카드 윈도우 함수 선언
작 성 자: 임다니엘
작성일자: 2022/07/25
*************************************************************************************************/
#ifndef _MEMORIZEDWORDSFORM_H
#define _MEMORIZEDWORDSFORM_H
#include <Windows.h>

BOOL CALLBACK MemorizedWordsFormProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL MemorizedWordsForm_OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL MemorizedWordsForm_OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL MemorizedWordsForm_OnTakeInButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL MemorizedWordsForm_OnTakeInAllButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL MemorizedWordsForm_OnFirstButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL MemorizedWordsForm_OnPreviousButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL MemorizedWordsForm_OnNextButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL MemorizedWordsForm_OnLastButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL MemorizedWordsForm_OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam);


#endif //_MEMORIZEDWORDSFORM_H