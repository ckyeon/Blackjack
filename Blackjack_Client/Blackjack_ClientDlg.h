
// Blackjack_ClientDlg.h : 헤더 파일
//

#pragma once
#include "afxwin.h"

class CClientSocket;
class CBlackjack_ClientApp;

// CBlackjack_ClientDlg 대화 상자
class CBlackjack_ClientDlg : public CDialogEx
{
// 생성입니다.
public:
	CBlackjack_ClientDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BLACKJACK_CLIENT_DIALOG };
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
