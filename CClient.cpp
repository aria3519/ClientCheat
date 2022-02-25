#include "pch.h"
#include "CClient.h"
#include "MFCApplication3Dlg.h"
#include <iostream>
#include <string>




void CClient::OnClose(int nErrorCode)
{
	// ShutDown() �Լ��� Close() �Լ��� �⺻���� �ִ� �Լ���, �׳� �߰��ϸ� �ȴ�.
	ShutDown();
	Close();
	CSocket::OnClose(nErrorCode);
	AfxMessageBox(_T("ERROR : Disconnected From Server!!"));
}


void CClient::OnReceive(int nErrorCode)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	// �������� ���� ������ ó���� ���⼭ �ϸ� �ȴ�.
	int retval;
	char buf[sizeof(TEST)];
	ZeroMemory(buf, sizeof(buf));
	//if (Receive(buf, sizeof(buf) >= size))
	
		retval = Receive(buf, sizeof(buf));
		TEST* recv_packet = (TEST*)buf;
		// Dlg Ŭ������ ���� �ص� �ɹ� ���� ���� �����ͼ� �� �� �ִ�.
		CMFCApplication3Dlg* pMain = (CMFCApplication3Dlg*)AfxGetMainWnd();
		pMain->m_List.AddString(CString(recv_packet->cData1));
		pMain->m_List.AddString(CString(recv_packet->cData2));
		CString str;
		str.Format(_T("%d"), recv_packet->iData);
		pMain->m_List.AddString(str);

		


	
	
	CSocket::OnReceive(nErrorCode);
}
