#pragma once
#include "afxwin.h"

class CClientSocket;
// CBlackjack_Client_Connection 대화 상자입니다.

class CBlackjack_Client_Connection : public CDialog
{
	DECLARE_DYNAMIC(CBlackjack_Client_Connection)

public:
	CBlackjack_Client_Connection(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CBlackjack_Client_Connection();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BLACKJACK_CLIENT_CONNECTION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString m_strAddress;
	int m_nPort;
//	afx_msg void OnEnChangeEdit3();
	CEdit m_ctrlEdit;
	CBlackjack_ClientApp* pApp;
	afx_msg void OnBnClickedOk();
};
