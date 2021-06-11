
// Blackjack_ServerDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "Blackjack_Server.h"
#include "Blackjack_ServerDlg.h"
#include "ListenSocket.h"
#include "ChildSocket.h"
#include "afxdialogex.h"
#include <mysql.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CBlackjack_ServerDlg 대화 상자



CBlackjack_ServerDlg::CBlackjack_ServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_BLACKJACK_SERVER_DIALOG, pParent)
	, m_nPlayerCnt(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_strUsername = _T("");
}

void CBlackjack_ServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_ctrlEdit);
}

BEGIN_MESSAGE_MAP(CBlackjack_ServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CBlackjack_ServerDlg 메시지 처리기

BOOL CBlackjack_ServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	m_pListenSocket = new CListenSocket;
	if (m_pListenSocket->Create(8160, SOCK_STREAM))
	{
		if (m_pListenSocket->Listen())
		{
			m_ctrlEdit.ReplaceSel(_T("[서버] Port(8160)이 Listen 소켓으로 열렸습니다.\r\n"));
		}
		else
		{
			AfxMessageBox(_T("ERROR: Failed to LISTEN."));
		}
	}
	else
	{
		AfxMessageBox(_T("ERROR: Failed to create a listen socket."));
	}

	mysql_init(&m_mysql);
	MYSQL *conn = mysql_real_connect(&m_mysql, "localhost", "user1", "pass01", "db1", 3306, (char *)NULL, 0);
	if (conn == NULL)
	{
		AfxMessageBox((LPCTSTR)mysql_error(&m_mysql));
		::PostQuitMessage(0);
		return FALSE;
	}

	this->m_ctrlEdit.ReplaceSel(_T("MySQL 서버와 연결되었습니다.\r\n"));
	mysql_set_character_set(&m_mysql, "euckr");

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CBlackjack_ServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CBlackjack_ServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CBlackjack_ServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



BOOL CBlackjack_ServerDlg::DestroyWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	POSITION pos = m_pListenSocket->m_pChildSocketList.GetHeadPosition();
	CChildSocket* pChild = NULL;

	while (pos != NULL)
	{
		pChild = (CChildSocket*)(m_pListenSocket->m_pChildSocketList.GetNext(pos));
		if (pChild != NULL)
		{
			pChild->ShutDown();
			pChild->Close();
			delete pChild;
		}
	}

	m_pListenSocket->ShutDown();
	m_pListenSocket->Close();
	delete m_pListenSocket;

	mysql_close(&m_mysql);

	return CDialogEx::DestroyWindow();
}
