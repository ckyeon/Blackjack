// Blackjack_Client_Connection.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Blackjack_Client.h"
#include "Blackjack_Client_Connection.h"
#include "ClientSocket.h"
#include "afxdialogex.h"


// CBlackjack_Client_Connection 대화 상자입니다.

IMPLEMENT_DYNAMIC(CBlackjack_Client_Connection, CDialog)

CBlackjack_Client_Connection::CBlackjack_Client_Connection(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_BLACKJACK_CLIENT_CONNECTION, pParent)
	, m_strAddress(_T("127.0.0.1"))
	, m_nPort(8160)
{
}

CBlackjack_Client_Connection::~CBlackjack_Client_Connection()
{
}

void CBlackjack_Client_Connection::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strAddress);
	DDX_Text(pDX, IDC_EDIT2, m_nPort);
	DDX_Control(pDX, IDC_EDIT3, m_ctrlEdit);
}


BEGIN_MESSAGE_MAP(CBlackjack_Client_Connection, CDialog)
//	ON_EN_CHANGE(IDC_EDIT3, &CBlackjack_Client_Connection::OnEnChangeEdit3)
	ON_BN_CLICKED(IDOK, &CBlackjack_Client_Connection::OnBnClickedOk)
END_MESSAGE_MAP()


// CBlackjack_Client_Connection 메시지 처리기입니다.



void CBlackjack_Client_Connection::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	pApp = (CBlackjack_ClientApp*)AfxGetApp();

	pApp->m_pClientSocket = new CClientSocket;
	pApp->m_pClientSocket->Create();

	UpdateData(TRUE);

	CString str; 
	str.Format(_T("[나] 서버(%s:%d)와 연결합니다.....\r\n"), m_strAddress, m_nPort);
	m_ctrlEdit.ReplaceSel(str);
	this->UpdateData(TRUE);

	bool success = pApp->m_pClientSocket->Connect(m_strAddress, m_nPort);
	if (success)
	{
		Sleep(950);
		m_ctrlEdit.ReplaceSel(_T("서버와 연결 되었습니다.\r\n"));
		Sleep(1600);
		CDialog::OnOK();
	}
	else
	{
		m_ctrlEdit.ReplaceSel(_T("[error] 서버와 연결하지 못했습니다.\r\n"));
	}


}
