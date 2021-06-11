#pragma once
#include "afxwin.h"

class CClientSocket;
class CBlackjack_ClientApp;

// CBlackjack_Client_Login ��ȭ �����Դϴ�.

class CBlackjack_Client_Login : public CDialog
{
	DECLARE_DYNAMIC(CBlackjack_Client_Login)

public:
	CBlackjack_Client_Login(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CBlackjack_Client_Login();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BLACKJACK_CLIENT_LOGIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CBlackjack_ClientApp* pApp;
	CString m_strUsername;
	CString m_strPassword;
	CEdit m_ctrlEdit;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
};
