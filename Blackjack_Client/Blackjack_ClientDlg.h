
// Blackjack_ClientDlg.h : ��� ����
//

#pragma once
#include "afxwin.h"

class CClientSocket;
class CBlackjack_ClientApp;

// CBlackjack_ClientDlg ��ȭ ����
class CBlackjack_ClientDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CBlackjack_ClientDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BLACKJACK_CLIENT_DIALOG };
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
	afx_msg void OnBnClickedOk();
	CBlackjack_ClientApp* pApp;
	CEdit m_ctrlEdit;
	CString m_strMessage;
	afx_msg void OnBnClickedHit();
	afx_msg void OnBnClickedStay();
	afx_msg void OnBnClickedReady();
	CEdit m_ctrlEdit_P1;
	CEdit m_ctrlEdit_P2;
	CEdit m_ctrlEdit_DE;
	CEdit m_ctrlEdit_RANK;
	bool readyFlag;
	CEdit m_ctrlEdit_userName;
};
