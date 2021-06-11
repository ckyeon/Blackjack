#pragma once
#include "afxwin.h"

class CClientSocket;
class CBlackjack_ClientApp;

// CBlackjack_Client_Login 대화 상자입니다.

class CBlackjack_Client_Login : public CDialog
{
	DECLARE_DYNAMIC(CBlackjack_Client_Login)

public:
	CBlackjack_Client_Login(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CBlackjack_Client_Login();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BLACKJACK_CLIENT_LOGIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CBlackjack_ClientApp* pApp;
	CString m_strUsername;
	CString m_strPassword;
	CEdit m_ctrlEdit;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
};
