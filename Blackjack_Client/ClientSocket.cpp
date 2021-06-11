// ClientSocket.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Blackjack_Client.h"
#include "ClientSocket.h"
#include "Blackjack_Client_Login.h"

// CClientSocket

CClientSocket::CClientSocket()
	: m_Cnt(0)
	, setName(false)
{
}

CClientSocket::~CClientSocket()
{
}


// CClientSocket 멤버 함수


void CClientSocket::OnReceive(int nErrorCode)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	pApp = (CBlackjack_ClientApp*)AfxGetApp();
	pDlg = (CBlackjack_ClientDlg*)AfxGetMainWnd();

	TCHAR szBuff[1024];
	int nRead = Receive(szBuff, sizeof(szBuff));
	CString szBuffs;
	szBuffs.Format("%s", szBuff);

	if (szBuffs.Find("LOGIN_SUCCESS") != -1 && szBuffs.Find((pApp->m_strUsername)) != -1 && pApp->m_strUsername != "")
	{
		pApp->correct_ID = TRUE;
		pApp->correct_PW = TRUE;

		CSocket::OnReceive(nErrorCode);
	}
	else if (szBuffs.Find("WIN_") != -1 && szBuffs.Find((pApp->m_strUsername)) != -1)
	{
		CString result = _T("You Win!\r\n");
		pDlg->m_ctrlEdit.ReplaceSel(result);
	
		CSocket::OnReceive(nErrorCode);
	}
	else if (szBuffs.Find("LOSE_") != -1 && szBuffs.Find((pApp->m_strUsername)) != -1)
	{
		CString result = _T("You Lose...\r\n");
		pDlg->m_ctrlEdit.ReplaceSel(result);
	
		CSocket::OnReceive(nErrorCode);
	}
	else if (szBuffs.Find("WIN_") != -1 || szBuffs.Find("LOSE_") != -1)
	{
		CSocket::OnReceive(nErrorCode);
	}
	else if (szBuffs.Find("GAME_END\r\n") != - 1)
	{
		CString endMessage = _T("\r\n게임이 끝났습니다.\r\n재시작 하려면 Ready를 눌러주세요.\r\n");
		pDlg->m_ctrlEdit.ReplaceSel(endMessage);

		pDlg->readyFlag = false;
		pDlg->SetDlgItemText(IDC_READY, _T("READY"));
		pDlg->GetDlgItem(IDC_READY)->EnableWindow(TRUE);
		pDlg->GetDlgItem(IDC_HIT)->EnableWindow(FALSE);
		pDlg->GetDlgItem(IDC_STAY)->EnableWindow(FALSE);


		pDlg->m_ctrlEdit_DE.SetSel(0, -1, TRUE);
		pDlg->m_ctrlEdit_DE.Clear();

		pDlg->m_ctrlEdit_P1.SetSel(0, -1, TRUE);
		pDlg->m_ctrlEdit_P1.Clear();

		pDlg->m_ctrlEdit_P2.SetSel(0, -1, TRUE);
		pDlg->m_ctrlEdit_P2.Clear();

		CSocket::OnReceive(nErrorCode);
	}
	else if (szBuffs.Find("RANK_CLEAR") != -1 && szBuffs.Find((pApp->m_strUsername)) != -1 && pApp->m_strUsername != "")
	{
		pDlg->m_ctrlEdit_RANK.SetSel(0, -1, TRUE);
		pDlg->m_ctrlEdit_RANK.Clear();
	
		CSocket::OnReceive(nErrorCode);
	}
	else if (szBuffs.Find("_RANKING") != -1 && szBuffs.Find((pApp->m_strUsername) + _T("check")) != -1 && pApp->m_strUsername != "")
	{
		int find = szBuffs.Find("_");
		szBuff[find] = '\0';
		pDlg->m_ctrlEdit_RANK.ReplaceSel(szBuff);

		if (setName == false)
		{
			CString name;
			name.Format(_T("%s\r\n"), pApp->m_strUsername);

			for (int i = 0; i < 2; i++)
			{
				pDlg->m_ctrlEdit_userName.ReplaceSel(name);
			}
			setName = true;
		}
		
		CSocket::OnReceive(nErrorCode);
	}
	else if (szBuffs.Find("_RANKING") != -1 || szBuffs.Find("RANK_CLEAR") != -1)
	{
		CSocket::OnReceive(nErrorCode);
	}
	else if (szBuffs.Find("DEALER_CARD") != -1)
	{
		szBuff[nRead - 12] = '\0';
		pDlg->m_ctrlEdit_DE.ReplaceSel(szBuff);

		CSocket::OnReceive(nErrorCode);
	}
	else if (szBuffs.Find("PLAYER_CARD") != -1 && szBuffs.Find(pApp->m_strUsername) != -1 && pApp->m_strUsername != "")
	{
		int find = szBuffs.Find("_");
		find = szBuffs.Find("_", find + 1);
		szBuff[find] = '\0';
		pDlg->m_ctrlEdit_P1.ReplaceSel(szBuff);

		CSocket::OnReceive(nErrorCode);
	}
	else if (szBuffs.Find("PLAYER_CARD") != -1)
	{
		int find = szBuffs.Find("_");
		find = szBuffs.Find("_", find + 1);
		szBuff[find] = '\0';
		pDlg->m_ctrlEdit_P2.ReplaceSel(szBuff);

		CSocket::OnReceive(nErrorCode);
	}
	else if (szBuffs.Find("LOGIN_SUCCESS") != -1 && pApp->correct_ID == TRUE && pApp->correct_PW == TRUE)
	{
		szBuff[nRead - 14] = _T('\0');

		CString str;
		str.Format(_T("[%s]님이 접속하셨습니다.\r\n"), szBuff);
		pDlg->m_ctrlEdit.ReplaceSel(str);
		CSocket::OnReceive(nErrorCode);
	}
	else if (szBuffs.Find("LOGIN_FAILED") != -1)
	{
		CSocket::OnReceive(nErrorCode);
	}
	else if (szBuffs.Find("COMPLETE_RESISTER") != -1 && pApp->correct_ID == FALSE && pApp->correct_PW == FALSE && pApp->m_strUsername != "")
	{
		CString str = _T("가입에 성공했습니다.\r\n");
		pApp->pLdlg->m_ctrlEdit.ReplaceSel(str);

		CSocket::OnReceive(nErrorCode);
	}
	else if (szBuffs.Find("COMPLETE_RESISTER") != -1)
	{
		CSocket::OnReceive(nErrorCode);
	}
	else if (szBuffs.Find("_READY") != -1)
	{
		szBuff[nRead - 6] = _T('\0');

		CString str;
		str.Format(_T("[%s] Game Ready\r\n"), szBuff);
		pDlg->m_ctrlEdit.ReplaceSel(str);

		CSocket::OnReceive(nErrorCode);
	}
	else if (szBuffs.Find("_CANCEL") != -1)
	{
		szBuff[nRead - 7] = _T('\0');

		CString str;
		str.Format(_T("[%s] Ready Cancel\r\n"), szBuff);
		pDlg->m_ctrlEdit.ReplaceSel(str);
	
		CSocket::OnReceive(nErrorCode);
	}
	else if (szBuffs.Find("_ST") != -1)
	{
		szBuff[nRead - 3] = _T('\0');

		CString str;
		str.Format(_T("%s"), szBuff);
		pDlg->m_ctrlEdit.ReplaceSel(str);
		
		pDlg->SetDlgItemText(IDC_READY, _T("게임 진행 중"));
		pDlg->GetDlgItem(IDC_READY)->EnableWindow(FALSE);

		pDlg->GetDlgItem(IDC_HIT)->EnableWindow(TRUE);
		pDlg->GetDlgItem(IDC_STAY)->EnableWindow(TRUE);

		CSocket::OnReceive(nErrorCode);
	}
	else if (szBuffs.Find("COMPLETE_RESISTER") != -1)
	{
		CSocket::OnReceive(nErrorCode);
	}
	else if (pApp->correct_ID == TRUE && pApp->correct_PW == TRUE)
	{
		switch (nRead)
		{
		case 0: this->Close(); 
			break;
		case SOCKET_ERROR:
			if (GetLastError() != WSAEWOULDBLOCK)
			{
				AfxMessageBox(_T("Error occurred"));
				this->Close();
			}
		default:
			szBuff[nRead] = _T('\0');
			pDlg->m_ctrlEdit.ReplaceSel(szBuff);

			CSocket::OnReceive(nErrorCode);
		}
	}
}
