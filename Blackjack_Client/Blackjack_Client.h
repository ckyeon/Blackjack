
// Blackjack_Client.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.
#include "Blackjack_ClientDlg.h"

class CClientSocket;
class CBlackjack_Client_Connection;
class CBlackjack_Client_Login;

// CBlackjack_ClientApp:
// �� Ŭ������ ������ ���ؼ��� Blackjack_Client.cpp�� �����Ͻʽÿ�.
//

class CBlackjack_ClientApp : public CWinApp
{
public:
	CBlackjack_ClientApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CClientSocket* m_pClientSocket;
	bool correct_ID;
	bool correct_PW;
	CString m_strUsername;
	CBlackjack_Client_Login* pLdlg;
};

extern CBlackjack_ClientApp theApp;