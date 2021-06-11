// ListenSocket.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Blackjack_Server.h"
#include "Blackjack_ServerDlg.h"
#include "ChildSocket.h"
#include "ListenSocket.h"


// CListenSocket

CListenSocket::CListenSocket()
{
}

CListenSocket::~CListenSocket()
{
}


// CListenSocket ��� �Լ�


void CListenSocket::OnAccept(int nErrorCode)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	CChildSocket* pChild = new CChildSocket;
	BOOL check = Accept(*pChild);

	if (check == FALSE)
	{
		delete pChild;
		return;
	}

	m_pChildSocketList.AddTail(pChild);

	CString strSocketName; UINT uPort;
	pChild->GetPeerName(strSocketName, uPort);
	CString str; str.Format(_T("[%s:%u] ���� ������ �����մϴ�.\r\n"), strSocketName, uPort);
	
	CBlackjack_ServerDlg* pDlg = (CBlackjack_ServerDlg*)AfxGetMainWnd();
	pDlg->m_ctrlEdit.ReplaceSel(str);

	pChild->Send(str.GetBuffer(), str.GetLength());
	pChild->m_pListenSocket = this;

	CAsyncSocket::OnAccept(nErrorCode);
}


void CListenSocket::Broadcast(CString strMessage)
{
	POSITION pos = m_pChildSocketList.GetHeadPosition();
	while (pos != NULL)
	{
		CChildSocket* pChild = (CChildSocket*)m_pChildSocketList.GetNext(pos);

		if (pChild != NULL)
			pChild->Send(strMessage.GetBuffer(), strMessage.GetLength());
	}
}
