// Blackjack_Client_Connection.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Blackjack_Client.h"
#include "Blackjack_Client_Connection.h"
#include "ClientSocket.h"
#include "afxdialogex.h"


// CBlackjack_Client_Connection ��ȭ �����Դϴ�.

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


// CBlackjack_Client_Connection �޽��� ó�����Դϴ�.



void CBlackjack_Client_Connection::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	pApp = (CBlackjack_ClientApp*)AfxGetApp();

	pApp->m_pClientSocket = new CClientSocket;
	pApp->m_pClientSocket->Create();

	UpdateData(TRUE);

	CString str; 
	str.Format(_T("[��] ����(%s:%d)�� �����մϴ�.....\r\n"), m_strAddress, m_nPort);
	m_ctrlEdit.ReplaceSel(str);
	this->UpdateData(TRUE);

	bool success = pApp->m_pClientSocket->Connect(m_strAddress, m_nPort);
	if (success)
	{
		Sleep(950);
		m_ctrlEdit.ReplaceSel(_T("������ ���� �Ǿ����ϴ�.\r\n"));
		Sleep(1600);
		CDialog::OnOK();
	}
	else
	{
		m_ctrlEdit.ReplaceSel(_T("[error] ������ �������� ���߽��ϴ�.\r\n"));
	}


}
