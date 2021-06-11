
// Blackjack_Client.h : PROJECT_NAME 응용 프로그램에 대한 주 헤더 파일입니다.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'stdafx.h'를 포함합니다."
#endif

#include "resource.h"		// 주 기호입니다.
#include "Blackjack_ClientDlg.h"

class CClientSocket;
class CBlackjack_Client_Connection;
class CBlackjack_Client_Login;

// CBlackjack_ClientApp:
// 이 클래스의 구현에 대해서는 Blackjack_Client.cpp을 참조하십시오.
//

class CBlackjack_ClientApp : public CWinApp
{
public:
	CBlackjack_ClientApp();

// 재정의입니다.
public:
	virtual BOOL InitInstance();

// 구현입니다.

	DECLARE_MESSAGE_MAP()
public:
	CClientSocket* m_pClientSocket;
	bool correct_ID;
	bool correct_PW;
	CString m_strUsername;
	CBlackjack_Client_Login* pLdlg;
};

extern CBlackjack_ClientApp theApp;