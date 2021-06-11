#pragma once

class CBlackjack_ClientApp;
class CBlackjack_Client_Login;
// CClientSocket 명령 대상입니다.

class CClientSocket : public CSocket
{
public:
	CClientSocket();
	virtual ~CClientSocket();
	virtual void OnReceive(int nErrorCode);
	CBlackjack_ClientApp* pApp;
	CBlackjack_ClientDlg* pDlg;
	int m_Cnt;
//	bool setNick;
//	bool setName;
	bool setName;
};


