// Blackjack_Client_Login.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Blackjack_Client.h"
#include "Blackjack_Client_Login.h"
#include "ClientSocket.h"
#include "afxdialogex.h"


// CBlackjack_Client_Login 대화 상자입니다.

IMPLEMENT_DYNAMIC(CBlackjack_Client_Login, CDialog)

CBlackjack_Client_Login::CBlackjack_Client_Login(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_BLACKJACK_CLIENT_LOGIN, pParent)
	, m_strUsername(_T(""))
	, m_strPassword(_T(""))
{

}

CBlackjack_Client_Login::~CBlackjack_Client_Login()
{
}

void CBlackjack_Client_Login::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strUsername);
	DDX_Text(pDX, IDC_EDIT2, m_strPassword);
	DDX_Control(pDX, IDC_EDIT3, m_ctrlEdit);
}


BEGIN_MESSAGE_MAP(CBlackjack_Client_Login, CDialog)
	ON_BN_CLICKED(IDOK, &CBlackjack_Client_Login::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CBlackjack_Client_Login::OnBnClickedButton1)
END_MESSAGE_MAP()


// CBlackjack_Client_Login 메시지 처리기입니다.


void CBlackjack_Client_Login::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	pApp = (CBlackjack_ClientApp*)AfxGetApp();

	CString str; str.Format(_T("로그인을 시도합니다....\r\n"));
	m_ctrlEdit.ReplaceSel(str);

	GetDlgItemText(IDC_EDIT1, m_strUsername);
	GetDlgItemText(IDC_EDIT2, m_strPassword);
	
	GetDlgItemText(IDC_EDIT1, pApp->m_strUsername);

	if (pApp->m_pClientSocket == NULL)
	{
		str.Format(_T("NULL입니다.\r\n"));
		m_ctrlEdit.ReplaceSel(str);
	}
	else
	{
		CString query;
		query.Format(_T("select * from member where id='%s' and pw=password('%s')"), m_strUsername, m_strPassword);

		pApp->m_pClientSocket->Send(query, query.GetLength());
		pApp->m_pClientSocket->OnReceive(NULL);

		if (pApp->correct_ID == TRUE && pApp->correct_PW == TRUE)
		{
			str.Format(_T("로그인에 성공했습니다.\r\n"));
			m_ctrlEdit.ReplaceSel(str);
			Sleep(1600);
			CDialog::OnOK();
		}
		else
		{
			CString str;
			str.Format(_T("존재하지 않는 사용자입니다.\r\n회원가입을 진행해 주세요.\r\n"));
			m_ctrlEdit.ReplaceSel(str);
		}
	}
}


void CBlackjack_Client_Login::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	pApp = (CBlackjack_ClientApp*)AfxGetApp();

	GetDlgItemText(IDC_EDIT1, m_strUsername);
	GetDlgItemText(IDC_EDIT2, m_strPassword);

	CString query;
	query.Format(_T("insert into member (id,pw) values ('%s',password('%s'))"), m_strUsername, m_strPassword);

	pApp->m_pClientSocket->Send(query, query.GetLength());
	pApp->m_pClientSocket->OnReceive(NULL);
}
