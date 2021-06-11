#pragma once

class CListenSocket;
class CBlackjack_ServerDlg;

// CChildSocket 명령 대상입니다.

class CChildSocket : public CSocket
{
public:
	CChildSocket();
	virtual ~CChildSocket();
	CListenSocket* m_pListenSocket;
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	void start_Blackjack();
	void filldeck();
	void Shuffle();
	CString getShape(int shape);
	CString getNumber(int number);
	void updateRanking();
	int getCardValue(int cardnumber);
	void blackjack_Progress();
};


