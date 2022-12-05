//TakingInForm.h
/************************************************************************************************
파일명칭: TakingInForm.h
기    능: 단어카드철 끼우기 윈도우 함수 선언
작 성 자: 임다니엘
작성일자: 2022/07/15
*************************************************************************************************/
#ifndef _TAKINGINFORM_H
#define _TAKINGINFORM_H
#include <Windows.h>

BOOL CALLBACK TakingInFormProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL TakingInForm_OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL TakingInForm_OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL TakingInForm_OnTakeInButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL TakingInForm_OnSpellingKillFocus(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL TakingInForm_OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam);


#endif //_TAKINGINFORM_H