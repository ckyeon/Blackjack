
// Blackjack_Server.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CBlackjack_ServerApp:
// �� Ŭ������ ������ ���ؼ��� Blackjack_Server.cpp�� �����Ͻʽÿ�.
//

class CBlackjack_ServerApp : public CWinApp
{
public:
	CBlackjack_ServerApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CBlackjack_ServerApp theApp;