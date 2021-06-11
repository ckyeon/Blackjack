
// Blackjack_ServerDlg.h : 헤더 파일
//

#pragma once
#include "afxwin.h"
#include <mysql.h>

class CListenSocket;
class CChildSocket;

// CBlackjack_ServerDlg 대화 상자
class CBlackjack_ServerDlg : public CDialogEx
{
// 생성입니다.
public:
	CBlackjack_ServerDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BLACKJACK_SERVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
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
