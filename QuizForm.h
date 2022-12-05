//QuizForm.h
/************************************************************************************************
파일명칭: QuizForm.h
기    능: 단어카드철 퀴즈 윈도우 함수 선언
작 성 자: 임다니엘
작성일자: 2022/07/22
*************************************************************************************************/
#ifndef _QUIZFORM_H
#define _QUIZFORM_H
#include <Windows.h>

BOOL CALLBACK QuizFormProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL QuizForm_OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL QuizForm_OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL QuizForm_OnGradeQuizButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL QuizForm_OnEnterKey(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL QuizForm_OnWordClassKillFocus(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL QuizForm_OnMeaningKillFocus(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL QuizForm_OnExampleSentenceKillFocus(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL QuizForm_OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam);

#endif //_QUIZFORM_H