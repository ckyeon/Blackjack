#pragma once

// CListenSocket ��� ����Դϴ�.
class CBlackjack_ServerDlg;
class CChild_Socket;

class CListenSocket : public CAsyncSocket
{
public:
	CListenSocket();
	virtual ~CListenSocket();
	CPtrList m_pChildSocketList;
	virtual void OnAccept(int nErrorCode);
	void Broadcast(CString strMessage);
//	CString getShape();
};


