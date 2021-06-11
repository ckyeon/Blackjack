
// Blackjack_ServerDlg.h : ��� ����
//

#pragma once
#include "afxwin.h"
#include <mysql.h>

class CListenSocket;
class CChildSocket;

// CBlackjack_ServerDlg ��ȭ ����
class CBlackjack_ServerDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CBlackjack_ServerDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BLACKJACK_SERVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CEdit m_ctrlEdit;
	CListenSocket* m_pListenSocket;
	virtual BOOL DestroyWindow();
	MYSQL m_mysql;
	CString m_strUsername;
	int m_nPlayerCnt;
};
