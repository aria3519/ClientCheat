#include "pch.h"
#include "CClient.h"
#include "MFCApplication3Dlg.h"
#include <iostream>
#include <string>




void CClient::OnClose(int nErrorCode)
{
	// ShutDown() 함수와 Close() 함수는 기본으로 있는 함수로, 그냥 추가하면 된다.
	ShutDown();
	Close();
	CSocket::OnClose(nErrorCode);
	AfxMessageBox(_T("ERROR : Disconnected From Server!!"));
}


void CClient::OnReceive(int nErrorCode)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	// 서버에서 받은 데이터 처리를 여기서 하면 된다.
	int retval;
	char buf[sizeof(TEST)];
	ZeroMemory(buf, sizeof(buf));
	//if (Receive(buf, sizeof(buf) >= size))
	
		retval = Receive(buf, sizeof(buf));
		TEST* recv_packet = (TEST*)buf;
		// Dlg 클래스에 선언 해둔 맴버 변수 등을 가져와서 쓸 수 있다.
		CMFCApplication3Dlg* pMain = (CMFCApplication3Dlg*)AfxGetMainWnd();
		pMain->m_List.AddString(CString(recv_packet->cData1));
		pMain->m_List.AddString(CString(recv_packet->cData2));
		CString str;
		str.Format(_T("%d"), recv_packet->iData);
		pMain->m_List.AddString(str);

		


	
	
	CSocket::OnReceive(nErrorCode);
}
