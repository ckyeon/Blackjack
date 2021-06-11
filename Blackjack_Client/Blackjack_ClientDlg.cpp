
// Blackjack_ClientDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "Blackjack_Client.h"
#include "Blackjack_ClientDlg.h"
#include "ClientSocket.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.


class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
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


// CBlackjack_ClientDlg ��ȭ ����



CBlackjack_ClientDlg::CBlackjack_ClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_BLACKJACK_CLIENT_DIALOG, pParent)
	, m_strMessage(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBlackjack_ClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_ctrlEdit);
	DDX_Text(pDX, IDC_EDIT2, m_strMessage);
	DDX_Control(pDX, IDC_PLAYER1, m_ctrlEdit_P1);
	DDX_Control(pDX, IDC_PLAYER2, m_ctrlEdit_P2);
	DDX_Control(pDX, IDC_DEALER, m_ctrlEdit_DE);
	DDX_Control(pDX, IDC_RANKING, m_ctrlEdit_RANK);
	DDX_Control(pDX, IDC_EDIT4, m_ctrlEdit_userName);
}

BEGIN_MESSAGE_MAP(CBlackjack_ClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CBlackjack_ClientDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_HIT, &CBlackjack_ClientDlg::OnBnClickedHit)
	ON_BN_CLICKED(IDC_STAY, &CBlackjack_ClientDlg::OnBnClickedStay)
	ON_BN_CLICKED(IDC_READY, &CBlackjack_ClientDlg::OnBnClickedReady)
END_MESSAGE_MAP()


// CBlackjack_ClientDlg �޽��� ó����

BOOL CBlackjack_ClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
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

	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CBlackjack_ClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CBlackjack_ClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CBlackjack_ClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CBlackjack_ClientDlg::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	pApp = (CBlackjack_ClientApp*)AfxGetApp();

	this->UpdateData(TRUE);
	m_strMessage.Append(_T("\r\n"));
	pApp->m_pClientSocket->Send(m_strMessage, m_strMessage.GetLength());
	m_strMessage = _T("");
	this->UpdateData(FALSE);

	// CDialogEx::OnOK();
}


void CBlackjack_ClientDlg::OnBnClickedHit()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	pApp = (CBlackjack_ClientApp*)AfxGetApp();

	CString hit;
	hit.Format(_T("_HIT"));
	pApp->m_pClientSocket->Send(hit, hit.GetLength());

	GetDlgItem(IDC_HIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_STAY)->EnableWindow(FALSE);
}


void CBlackjack_ClientDlg::OnBnClickedStay()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	pApp = (CBlackjack_ClientApp*)AfxGetApp();

	CString stay;
	stay.Format(_T("_STAY"));
	pApp->m_pClientSocket->Send(stay, stay.GetLength());

	GetDlgItem(IDC_HIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_STAY)->EnableWindow(FALSE);
}


void CBlackjack_ClientDlg::OnBnClickedReady()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	pApp = (CBlackjack_ClientApp*)AfxGetApp();

	if (readyFlag == false)
	{
		readyFlag = true;

		CString ready;
		ready.Format(_T("_READY\r\n"));
		pApp->m_pClientSocket->Send(ready, ready.GetLength());

		SetDlgItemText(IDC_READY, _T("CANCLE"));
	}
	else
	{
		readyFlag = false;

		CString cancel;
		cancel.Format(_T("_CANCEL\r\n"));
		pApp->m_pClientSocket->Send(cancel, cancel.GetLength());

		SetDlgItemText(IDC_READY, _T("READY"));
	}
}
