
// MFCApplication3Dlg.h: 헤더 파일
//

#pragma once
#include "CClient.h"
#define MAX_BUFFER_SIZE 6
#pragma pack(1)
typedef struct
{
	char cData1; // 1byte
	char cData2; // 1byte
	int iData; // 4byte
}TEST;
#pragma pack()


// CMFCApplication3Dlg 대화 상자
class CMFCApplication3Dlg : public CDialogEx
{
// 생성입니다.
public:
	CMFCApplication3Dlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.


// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPLICATION3_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListBox m_List;
	
	afx_msg void OnBnClickedButtonSend();
	CClient m_Client;
};
