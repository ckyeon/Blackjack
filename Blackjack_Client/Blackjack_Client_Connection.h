#pragma once
#include "afxwin.h"

class CClientSocket;
// CBlackjack_Client_Connection ��ȭ �����Դϴ�.

class CBlackjack_Client_Connection : public CDialog
{
	DECLARE_DYNAMIC(CBlackjack_Client_Connection)

public:
	CBlackjack_Client_Connection(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CBlackjack_Client_Connection();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BLACKJACK_CLIENT_CONNECTION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CString m_strAddress;
	int m_nPort;
//	afx_msg void OnEnChangeEdit3();
	CEdit m_ctrlEdit;
	CBlackjack_ClientApp* pApp;
	afx_msg void OnBnClickedOk();
};
