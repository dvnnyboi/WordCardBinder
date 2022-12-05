//FindingForm.h
/************************************************************************************************
파일명칭: FindingForm.h
기    능: 단어카드철 찾기 윈도우 함수 선언
작 성 자: 임다니엘
작성일자: 2022/07/15
*************************************************************************************************/
#ifndef _FINDINGFORM_H
#define _FINDINGFORM_H
#include <Windows.h>

BOOL CALLBACK FindingFormProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL FindingForm_OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL FindingForm_OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL FindingForm_OnSpellingRadioClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL FindingForm_OnMeaningRadioClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL FindingForm_OnFindButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL FindingForm_OnSelectButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL FindingForm_OnFirstButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL FindingForm_OnPreviousButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL FindingForm_OnNextButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL FindingForm_OnLastButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL FindingForm_OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam);


#endif //_FINDINGFORM_H