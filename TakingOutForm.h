//TakingOutForm.h
/************************************************************************************************
���ϸ�Ī: TakingOutForm.h
��    ��: �ܾ�ī��ö ���� ������ �Լ� ����
�� �� ��: �ӴٴϿ�
�ۼ�����: 2022/07/15
*************************************************************************************************/
#ifndef _TAKINGOUTFORM_H
#define _TAKINGOUTFORM_H
#include <Windows.h>

BOOL CALLBACK TakingOutFormProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL TakingOutForm_OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL TakingOutForm_OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam);

#endif //_TAKINGOUTFORM_H