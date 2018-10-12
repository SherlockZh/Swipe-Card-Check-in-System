
// CardBetaDlg.h : ͷ�ļ�
//

#pragma once
//#include "afxcmn.h"
#include "UDPSocket.h"
#include "resource.h"
#include "Show.h"

#define BLOCK0_EN  0x01
#define BLOCK1_EN  0x02
#define BLOCK2_EN  0x04
#define NEEDSERIAL 0x08
#define EXTERNKEY  0x10
#define NEEDHALT   0x20



// CCardBetaDlg �Ի���
class CCardBetaDlg : public CDialogEx
{
// ����
public:
	CCardBetaDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CARDBETA_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CIPAddressCtrl m_IPAddress_Host;
	CIPAddressCtrl m_IPAddress_Remot;
	int m_HostPort;
	int m_RemotePort;
	CUDPSocket* m_pSocket;
	static CCardBetaDlg * GetDialog();
	void OnReceive(int nErrorCode);
	void OnSend(int nErrorCode);
	CString m_SendData;
	CString m_ReceiveData;
	CString id;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void ReadCard();
	bool FindCard();
	void Ring();
	afx_msg void OnBnClickedBtnInit();

	CShow * show1;
	NOTIFYICONDATA NotifyIcon;
	afx_msg LRESULT OnNotifyIcon(WPARAM wParam, LPARAM IParam);
	void MinDialog();
};

