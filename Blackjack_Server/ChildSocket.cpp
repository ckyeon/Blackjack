// ChildSocket.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Blackjack_Server.h"
#include "Blackjack_ServerDlg.h"
#include "ChildSocket.h"
#include "ListenSocket.h"


// CChildSocket

enum shape { spade, club, dia, heart};
enum num {ace = 1, jack = 11 , queen = 12, king = 13};

// 카드 구조체
typedef struct CARD {
	int number;
	char shape;
}Card;

// 모든 카드
Card card_all[52] = { NULL };
int card_index = 0;

CString strIPAddress; UINT uPortNumber;
CString m_strUsernames[100] = { NULL };
CString m_strPlayuser[2] = { NULL };
int m_nPlayuserPort[100] = { 0 };
int gameReadyCnt = 0;
int selectCnt = 0;
int dealerCardCnt = 0;
int playUserCnt[2] = { 0 };

CChildSocket::CChildSocket()
{
}

CChildSocket::~CChildSocket()
{
}


// CChildSocket 멤버 함수


void CChildSocket::OnClose(int nErrorCode)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CBlackjack_ServerDlg* pDlg = (CBlackjack_ServerDlg*)AfxGetMainWnd();

	POSITION pos = m_pListenSocket->m_pChildSocketList.Find(this);
	m_pListenSocket->m_pChildSocketList.RemoveAt(pos);

	CString strSocketName; UINT uPort;
	this->GetPeerName(strSocketName, uPort);

	pDlg->m_nPlayerCnt--;

	for (int i = 0; i < 100; i++)
	{
		m_strUsernames[i] = _T("");
	}
	for (int i = 0; i < 2; i++)
	{
		m_strPlayuser[i] = _T("");
	}

	CString str; str.Format(_T("[%s:%u] 연결 종료\r\n"), strSocketName, uPort);
	pDlg->m_ctrlEdit.ReplaceSel(str);
	
	this->ShutDown();
	this->Close();
	delete this;

	CSocket::OnClose(nErrorCode);
}

void CChildSocket::OnReceive(int nErrorCode)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CBlackjack_ServerDlg* pDlg = (CBlackjack_ServerDlg*)AfxGetMainWnd();


	GetPeerName(strIPAddress, uPortNumber);

	TCHAR szBuff[1024];
	int nRead = Receive(szBuff, sizeof(szBuff));
	CString szBuffs = szBuff;

	if (szBuffs.Find("select * from member where") != -1)
	{
		szBuff[nRead] = _T('\0');

		CString m_strUsername;

		int start_userName = szBuffs.Find('\'');
		int last_userName = szBuffs.Find('\'', start_userName + 1);
		for (int i = start_userName + 1; i < last_userName; i++)
		{
			m_strUsername.Append((CString)szBuff[i]);
		}

		int status = mysql_query(&(pDlg->m_mysql), szBuff);
		MYSQL_RES *result = mysql_store_result(&(pDlg->m_mysql));
		int nRowCount = mysql_num_rows(result);
		if (nRowCount > 0 && pDlg->m_nPlayerCnt < 2)
		{
			MYSQL_ROW row = mysql_fetch_row(result);

			m_strUsernames[uPortNumber % 100] = m_strUsername;
			m_nPlayuserPort[uPortNumber % 100] = pDlg->m_nPlayerCnt;
			m_strPlayuser[pDlg->m_nPlayerCnt++] = m_strUsername;

			CString str = m_strUsernames[uPortNumber % 100] + _T("_LOGIN_SUCCESS");
			m_pListenSocket->Broadcast(str);

			Sleep(3000);
			CString clear;
			clear.Format(_T("RANK_CLEAR_%s"), m_strUsernames[uPortNumber % 100]);
			m_pListenSocket->Broadcast(clear);
			Sleep(500);

			int status = mysql_query(&pDlg->m_mysql, _T("select id, point from member order by point desc"));
			MYSQL_RES *result = mysql_store_result(&pDlg->m_mysql);

			if (result)
			{
				int nFieldCount = mysql_num_fields(result);
				MYSQL_ROW row;

				for (int i = 0; i < 5; i++)
				{
					row = mysql_fetch_row(result);
					if (row != NULL)
					{
						CString str; str.Format(_T("%d."), i + 1);
						for (int j = 0; j < nFieldCount; j++)
						{
							str.Append(_T(row[j]));
							str.Append(_T(" "));
						}
						str.Append(_T("\r\n_RANKING_") + m_strUsernames[uPortNumber % 100] + _T("check"));

						m_pListenSocket->Broadcast(str);
						Sleep(500);
					}
				}
				mysql_free_result(result);
			}
		}
		else
		{
			CString str = _T("LOGIN_FAILED_") + m_strUsername;
			m_pListenSocket->Broadcast(str);
		}

		CSocket::OnReceive(nErrorCode);
	}
	else if (szBuffs.Find("insert into member (id,pw) values") != -1)
	{
		szBuff[nRead] = _T('\0');
		int status = mysql_query(&pDlg->m_mysql, szBuff);
		CString str = _T("COMPLETE_RESISTER");
		m_pListenSocket->Broadcast(str);

		CSocket::OnReceive(nErrorCode);
	}
	else if (szBuffs.Find("_READY") != -1 && gameReadyCnt < 2)
	{
		CString ready = m_strUsernames[uPortNumber % 100] + _T("_READY");
		m_pListenSocket->Broadcast(ready);

		if (gameReadyCnt < 2)
			gameReadyCnt++;

		if (gameReadyCnt == 2)
		{
			Sleep(1000);

			start_Blackjack();
		}

		CSocket::OnReceive(nErrorCode);
	}
	else if (szBuffs.Find("_CANCEL") != -1 && gameReadyCnt < 2)
	{
		CString cancel = m_strUsernames[uPortNumber % 100] + _T("_CANCEL");
		m_pListenSocket->Broadcast(cancel);

		if (gameReadyCnt > 0)
			gameReadyCnt--;

		CSocket::OnReceive(nErrorCode);
	}
	else if (szBuffs.Find("_HIT") != -1)
	{
		CString CardShape;
		CString CardNumber;
		CString PlayerCard;

		CString str;
		str.Format(_T("[%s] HIT\r\n"), m_strUsernames[uPortNumber % 100]);
		m_pListenSocket->Broadcast(str);

		Sleep(500);

		CardShape = getShape(card_all[card_index].shape);
		CardNumber = getNumber(card_all[card_index].number);
		playUserCnt[m_nPlayuserPort[uPortNumber % 100]] += getCardValue(card_all[card_index].number);
		card_index++;

		PlayerCard.Format(_T("%s_%s\r\n_PLAYER_CARD(%s)\r\n"), CardShape, CardNumber, m_strUsernames[uPortNumber % 100]);
		m_pListenSocket->Broadcast(PlayerCard);

		Sleep(500);
		selectCnt++;

		if (selectCnt == 2)
		{
			blackjack_Progress();
		}
			
		CSocket::OnReceive(nErrorCode);
	}
	else if (szBuffs.Find("_STAY") != -1)
	{
		CString str;
		str.Format(_T("[%s] STAY\r\n"), m_strUsernames[uPortNumber % 100]);
		m_pListenSocket->Broadcast(str);

		Sleep(500);
		selectCnt++;

		
		if (selectCnt == 2)
		{
			blackjack_Progress();
		}

		CSocket::OnReceive(nErrorCode);
	}
	else 
	{
		szBuff[nRead] = _T('\0');
	
		CString str; str.Format(_T("[%s] %s"), m_strUsernames[uPortNumber % 100], szBuff);
		pDlg->m_ctrlEdit.ReplaceSel(str);
		
		m_pListenSocket->Broadcast(str);
		CSocket::OnReceive(nErrorCode);
	}
}

void CChildSocket::start_Blackjack()
{
	gameReadyCnt = 0;

	CString str = _T("게임을 시작합니다.\r\n딜러가 카드를 섞는 중입니다....\r\n_ST");
	m_pListenSocket->Broadcast(str);

	filldeck();
	Shuffle();

	Sleep(1000);

	CString message = _T("카드를 분배합니다.\r\n");
	m_pListenSocket->Broadcast(message);

	Sleep(500);

	CString CardShape;
	CString CardNumber;

	CardShape = getShape(card_all[card_index].shape);
	CardNumber = getNumber(card_all[card_index].number);
	dealerCardCnt += getCardValue(card_all[card_index].number);
	card_index++;

	CString dealerCard;
	dealerCard.Format(_T("%s_%s\r\n_DEALER_CARD"), CardShape, CardNumber);
	m_pListenSocket->Broadcast(dealerCard);

	Sleep(500);

	CString PlayerCard;
	for (int i = 0; i < 2; i++)
	{
		CardShape = getShape(card_all[card_index].shape);
		CardNumber = getNumber(card_all[card_index].number);
		playUserCnt[i] += getCardValue(card_all[card_index].number);
		card_index++;

		PlayerCard.Format(_T("%s_%s\r\n_PLAYER_CARD(%s)"), CardShape, CardNumber, m_strPlayuser[i]);
		m_pListenSocket->Broadcast(PlayerCard);

		Sleep(500);
	}

	for (int i = 0; i < 2; i++)
	{
		CardShape = getShape(card_all[card_index].shape);
		CardNumber = getNumber(card_all[card_index].number);
		playUserCnt[i] += getCardValue(card_all[card_index].number);
		card_index++;

		PlayerCard.Format(_T("%s_%s\r\n_PLAYER_CARD(%s)\r\n"), CardShape, CardNumber, m_strPlayuser[i]);
		m_pListenSocket->Broadcast(PlayerCard);

		Sleep(500);
	}

	message.Format(_T("[HIT] 또는 [STAY]를 선택해 주세요.\r\n"));
	m_pListenSocket->Broadcast(message);

	CSocket::OnReceive(NULL);
}

void CChildSocket::filldeck()
{
	int i = 0;
	for (int j = 0; j < 4; j++)
	{
		for (int k = 0; k < 13; k++)
		{
			card_all[i].number = k;
			card_all[i].shape = j;
			i++;
		}
	}
}

void CChildSocket::Shuffle()
{
	srand(time(NULL));
	for (int i = 0; i < 52; i++)
	{
		int index = rand() % 52;
		Card temp = card_all[index];
		card_all[index] = card_all[i];
		card_all[i] = temp;
	}
}



CString CChildSocket::getShape(int shape)
{
	switch (shape)
	{
	case 0:
		return _T("Spade");
	case 1:
		return _T("Club");
	case 2:
		return _T("Dia");
	case 3:
		return _T("Heart");
	}

	return CString();
}


CString CChildSocket::getNumber(int number)
{
	switch (number)
	{
	case 0:
		return _T("A");
	case 1:
		return _T("2");
	case 2:
		return _T("3");
	case 3:
		return _T("4");
	case 4:
		return _T("5");
	case 5:
		return _T("6");
	case 6:
		return _T("7");
	case 7:
		return _T("8");
	case 8:
		return _T("9");
	case 9:
		return _T("10");
	case 10:
		return _T("J");
	case 11:
		return _T("Q");
	case 12:
		return _T("K");
	}

	return CString();
}


void CChildSocket::updateRanking()
{
	CBlackjack_ServerDlg* pDlg = (CBlackjack_ServerDlg*)AfxGetMainWnd();

	Sleep(500);

	CString clear;
	for (int i = 0; i < 2; i++)
	{
		clear.Format(_T("RANK_CLEAR_%s"), m_strPlayuser[i]);
		m_pListenSocket->Broadcast(clear);
		Sleep(500);
	}

	for (int k = 0; k < 2; k++)
	{
		int status = mysql_query(&pDlg->m_mysql, _T("select id, point from member order by point desc"));
		MYSQL_RES *result = mysql_store_result(&pDlg->m_mysql);

		if (result)
		{
			int nFieldCount = mysql_num_fields(result);
			MYSQL_ROW row;


			for (int i = 0; i < 5; i++)
			{
				row = mysql_fetch_row(result);
				if (row != NULL)
				{
					CString str; str.Format(_T("%d."), i + 1);
					for (int j = 0; j < nFieldCount; j++)
					{
						str.Append(_T(row[j]));
						str.Append(_T(" "));
					}
					str.Append(_T("\r\n_RANKING_") + m_strPlayuser[k] + _T("check"));

					m_pListenSocket->Broadcast(str);
					Sleep(500);
				}
			}
			mysql_free_result(result);
		}
	}
}

int CChildSocket::getCardValue(int cardnumber)
{
	switch (cardnumber)
	{
	case 0:
		return 1;
	case 1:
		return 2;
	case 2:
		return 3;
	case 3:
		return 4;
	case 4:
		return 5;
	case 5:
		return 6;
	case 6:
		return 7;
	case 7:
		return 8;
	case 8:
		return 9;
	case 9:
		return 10;
	case 10:
		return 10;
	case 11:
		return 10;
	case 12:
		return 10;
	}
}


void CChildSocket::blackjack_Progress()
{
	CBlackjack_ServerDlg* pDlg = (CBlackjack_ServerDlg*)AfxGetMainWnd();
	
	CString str;
	CString CardShape;
	CString CardNumber;
	CString PlayerCard;

	int surviveUser = -1;
	bool isBurst[2] = { false };
	bool isLose[2] = { false };
	bool isWin[2] = { false };
	if (playUserCnt[0] > 21 && playUserCnt[1] > 21)	// 둘 다 버스트
	{
		for (int i = 0; i < 2; i++)
		{
			isBurst[i] = true;


			Sleep(1000);
		}
	}
	else if (playUserCnt[0] > 21 || playUserCnt[1] > 21)	// 한 명 버스트
	{
		for (int i = 0; i < 2; i++)
		{
			if (playUserCnt[i] > 21)
			{
				isBurst[i] = true;

				Sleep(500);
			}
			else
			{
				surviveUser = i;
			}
		}

		while (dealerCardCnt <= 17)
		{
			CString CardShape;
			CString Number;
			CString dealerCard;

			CardShape = getShape(card_all[card_index].shape);
			CardNumber = getNumber(card_all[card_index].number);
			dealerCardCnt += getCardValue(card_all[card_index].number);
			card_index++;

			dealerCard.Format(_T("%s_%s\r\n_DEALER_CARD"), CardShape, CardNumber);
			m_pListenSocket->Broadcast(dealerCard);

			Sleep(500);
		}

		if (dealerCardCnt > 21)
		{
			isWin[surviveUser] = true;

			Sleep(500);

			CString query;
			query.Format(_T("UPDATE member SET point=point+1 WHERE id='%s'"), m_strPlayuser[surviveUser]);
			mysql_query(&pDlg->m_mysql, query);
		}
		else
		{
			if (playUserCnt[surviveUser] > dealerCardCnt)
			{
				isWin[surviveUser] = true;

				Sleep(500);

				CString query;
				query.Format(_T("UPDATE member SET point=point+1 WHERE id='%s'"), m_strPlayuser[surviveUser]);
				mysql_query(&pDlg->m_mysql, query);
			}
			else
			{
				isLose[surviveUser] = true;

				Sleep(500);
			}
		}
	}
	else  // 둘 다 살았을 때
	{
		while (dealerCardCnt <= 17)
		{
			CString CardShape;
			CString Number;
			CString dealerCard;

			CardShape = getShape(card_all[card_index].shape);
			CardNumber = getNumber(card_all[card_index].number);
			dealerCardCnt += getCardValue(card_all[card_index].number);
			card_index++;

			dealerCard.Format(_T("%s_%s\r\n_DEALER_CARD"), CardShape, CardNumber);
			m_pListenSocket->Broadcast(dealerCard);

			Sleep(500);
		}

		if (dealerCardCnt > 21)
		{
			for (int i = 0; i < 2; i++)
			{
				isWin[i] = true;

				Sleep(500);

				CString query;
				query.Format(_T("UPDATE member SET point=point+1 WHERE id='%s'"), m_strPlayuser[i]);
				mysql_query(&pDlg->m_mysql, query);
			}
		}
		else if (playUserCnt[0] > 21 || playUserCnt[1] > 21)
		{
			for (int i = 0; i < 2; i++)
			{
				if (playUserCnt[i] > 21)
				{
					isBurst[i] = true;

					Sleep(500);
				}
				else
				{
					if (playUserCnt[i] > dealerCardCnt)
					{
						isWin[i] = true;

						Sleep(500);

						CString query;
						query.Format(_T("UPDATE member SET point=point+1 WHERE id='%s'"), m_strPlayuser[i]);
						mysql_query(&pDlg->m_mysql, query);
					}
					else
					{
						isLose[i] = true;

						Sleep(500);
					}
				}
			}
		}
		else
		{
			if (playUserCnt[0] > dealerCardCnt && playUserCnt[1] > dealerCardCnt)
			{
				if (playUserCnt[0] > playUserCnt[1])
				{
					isWin[0] = true;

					Sleep(500);

					CString query;
					query.Format(_T("UPDATE member SET point=point+1 WHERE id='%s'"), m_strPlayuser[0]);
					mysql_query(&pDlg->m_mysql, query);

					isLose[1] = false;

					Sleep(500);
				}
				else
				{
					isWin[1] = true;

					Sleep(500);

					CString query;
					query.Format(_T("UPDATE member SET point=point+1 WHERE id='%s'"), m_strPlayuser[1]);
					mysql_query(&pDlg->m_mysql, query);

					isLose[0] = true;

					Sleep(500);
				}
			}
			else if (playUserCnt[0] > dealerCardCnt || playUserCnt[1] > dealerCardCnt)
			{
				for (int i = 0; i < 2; i++)
				{
					if (dealerCardCnt > playUserCnt[i])
					{
						isLose[i] = true;

						Sleep(500);
					}
					else if (playUserCnt[0] == playUserCnt[1])	// 둘다 같으면 같이 승리
					{
						for (int i = 0; i < 2; i++)
						{
							isWin[i] = true;
							Sleep(500);

							CString query;
							query.Format(_T("UPDATE member SET point=point+1 WHERE id='%s'"), m_strPlayuser[i]);
							mysql_query(&pDlg->m_mysql, query);
						}
					}
					else
					{
						isWin[i] = true;

						Sleep(500);

						CString query;
						query.Format(_T("UPDATE member SET point=point+1 WHERE id='%s'"), m_strPlayuser[i]);
						mysql_query(&pDlg->m_mysql, query);
					}
				}
			}
			else
			{
				for (int i = 0; i < 2; i++)
				{
					isLose[i] = true;

					Sleep(500);
				}
			}
		}
	}

	CString temp = _T("\r\n");
	m_pListenSocket->Broadcast(temp);
	Sleep(500);

	for (int i = 0; i < 2; i++)
	{
		if (isBurst[i])
		{
			Sleep(500);
			str.Format(_T("[%s] Burst\r\n"), m_strPlayuser[i]);
			m_pListenSocket->Broadcast(str);
		}
		else if (isLose[i])
		{
			Sleep(500);
			str.Format(_T("[%s] Lose\r\n"), m_strPlayuser[i]);
			m_pListenSocket->Broadcast(str);
		}
		else if (isWin[i])
		{
			Sleep(500);
			str.Format(_T("[%s] Win\r\n"), m_strPlayuser[i]);
			m_pListenSocket->Broadcast(str);
		}
	}

	for (int i = 0; i < 2; i++)
	{
		playUserCnt[i] = 0;

		if (isBurst[i])
		{
			Sleep(500);

			CString result;
			result.Format(_T("LOSE_%s\r\n"), m_strPlayuser[i]);
			m_pListenSocket->Broadcast(result);

			Sleep(500);
		}
		else if (isLose[i])
		{
			Sleep(500);

			CString result;
			result.Format(_T("LOSE_%s\r\n"), m_strPlayuser[i]);
			m_pListenSocket->Broadcast(result);
		}
		else if (isWin[i])
		{
			Sleep(500);

			CString result;
			result.Format(_T("WIN_%s\r\n"), m_strPlayuser[i]);
			m_pListenSocket->Broadcast(result);
		}
	}

	Sleep(1000);

	gameReadyCnt = 0;
	selectCnt = 0;
	dealerCardCnt = 0;

	CString result = _T("GAME_END\r\n");
	m_pListenSocket->Broadcast(result);

	updateRanking();
}
