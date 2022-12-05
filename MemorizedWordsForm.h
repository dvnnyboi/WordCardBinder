//MemorizedWordsForm.h
/************************************************************************************************
���ϸ�Ī: MemorizedWordsForm.h
��    ��: �ܾ�ī��ö �ϱ�Ϸ�ī�� ������ �Լ� ����
�� �� ��: �ӴٴϿ�
�ۼ�����: 2022/07/25
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