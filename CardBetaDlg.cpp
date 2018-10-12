
// CardBetaDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CardBeta.h"
#include "CardBetaDlg.h"
#include "afxdialogex.h"
#include "Show.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_NC (WM_USER+1001)

const int DATA_BUFFER_LENGHT = 1024;

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CCardBetaDlg �Ի���



CCardBetaDlg::CCardBetaDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCardBetaDlg::IDD, pParent)
	, m_HostPort(20000)
	, m_RemotePort(10000)
	, m_pSocket(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCardBetaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS_HOST, m_IPAddress_Host);
	DDX_Control(pDX, IDC_IPADDRESS_REMOT, m_IPAddress_Remot);
	DDX_Text(pDX, IDC_EDT_HOSTPORT, m_HostPort);
	DDX_Text(pDX, IDC_EDT_REMOTPORT, m_RemotePort);
}

BEGIN_MESSAGE_MAP(CCardBetaDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_INIT, &CCardBetaDlg::OnBnClickedBtnInit)
	ON_MESSAGE(WM_NC, &OnNotifyIcon)
END_MESSAGE_MAP()


// CCardBetaDlg ��Ϣ�������

BOOL CCardBetaDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	m_IPAddress_Host.SetAddress(ntohl(inet_addr("192.168.191.1")));
	m_IPAddress_Remot.SetAddress(ntohl(inet_addr("192.168.191.3")));
	UpdateData(FALSE);// ���¿ؼ���ֵ



	SetTimer(1, 1000, NULL);

	UpdateData(FALSE);// ���¿ؼ���ֵ

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CCardBetaDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CCardBetaDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CCardBetaDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


CCardBetaDlg * CCardBetaDlg::GetDialog()
{
	//��ȡ�Ի���ָ��
	CCardBetaDlg *pDl = (CCardBetaDlg *)(AfxGetApp()->m_pMainWnd);
	if (!pDl)
		return NULL;
	return (pDl);
}


void CCardBetaDlg::OnReceive(int nErrorCode)
{
	char receive_buf[DATA_BUFFER_LENGHT];
	UINT m_rPort;
	CString m_rAddr;
	CString m_erro;
	if (m_pSocket != NULL)
	{
		//��ջ�����
		memset(receive_buf, 0, sizeof(receive_buf));

		//������Ϣ
		int nBytes = m_pSocket->ReceiveFrom(receive_buf, DATA_BUFFER_LENGHT,
			m_rAddr, m_rPort, 0);
		if (nBytes == SOCKET_ERROR)
		{
			m_erro.Format(_T("����ReceiveFrom����ʧ�ܣ����������%d"), GetLastError());
			AfxMessageBox(m_erro);
			return;
		}

		//���������Ϣ

//		strcat(receive_buf, (char *)m_rAddr.GetBuffer(m_rAddr.GetLength()));
		char buf[100];
		_itoa(ntohs(m_rPort), buf, 10);

		m_ReceiveData += "\r\n";
		m_ReceiveData += receive_buf;

		show1->m_RecieveData_Show = m_ReceiveData;
		show1->show();
		//ˢ����ʾ
	    UpdateData(0);

		CEdit*pedit = (CEdit*)GetDlgItem(IDC_EDT_RECVFROM);
	}

}


void CCardBetaDlg::OnSend(int nErrorCode)
{
	in_addr buf;
	DWORD m_RemoteIP;
	if (m_SendData.GetLength() == 0)
	{
		return;
	}

	//���ؼ��е�IP��ַ�洢��m_RemoteIP��
	m_IPAddress_Remot.GetAddress(m_RemoteIP);
	buf.S_un.S_addr = htonl(m_RemoteIP);


	//ˢ����ʾ��ʹ����ֵ�仯�Ŀؼ���ֵ������ʾ
	UpdateData(0);

}


void CCardBetaDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	unsigned char(__stdcall *picchalt)();
	HINSTANCE hDllb;
	hDllb = LoadLibrary(_T("OUR_MIFARE.dll"));
	picchalt = (unsigned char(__stdcall *)())GetProcAddress(hDllb, "picchalt");

	DWORD m_RemoteIP;
	m_IPAddress_Remot.GetAddress(m_RemoteIP);
	int m_RemotePort = 10000;

	while (FindCard())
	{    
		ReadCard();
		Ring();
		picchalt();

		m_SendData = id;
		m_pSocket->SendToRemotePC(m_RemoteIP, m_RemotePort, m_SendData);
		FindCard();
	}

	CDialogEx::OnTimer(nIDEvent);
}

bool CCardBetaDlg::FindCard()
{
	unsigned char status;
	unsigned char myserial[4];
	unsigned char(__stdcall *piccrequest)(unsigned char *serial);

	HINSTANCE hDll_b;
	hDll_b = LoadLibrary(_T("OUR_MIFARE.dll"));
	piccrequest = (unsigned char(__stdcall *)(unsigned char *))GetProcAddress(hDll_b, "piccrequest");
	status = piccrequest(myserial);
	if (status == 0)
		return true;
	else
		return false;
}

void CCardBetaDlg::ReadCard()
{
	unsigned char status;//��ŷ���ֵ
	unsigned char myareano;//����
	unsigned char authmode;//�������ͣ���A�����B����
	unsigned char myctrlword;//������
	unsigned char mypicckey[6];//����
	unsigned char mypiccserial[4];//�����к�
	unsigned char mypiccdata[48]; //�����ݻ���

	unsigned char(__stdcall *piccreadex)(unsigned char ctrlword, unsigned char *serial, unsigned char area, unsigned char keyA1B0, unsigned char *picckey, unsigned char *piccdata0_2);
	//�ж϶�̬���Ƿ����

	//��ȡ����
	HINSTANCE hDlla;
	hDlla = LoadLibrary(_T("OUR_MIFARE.dll"));
	piccreadex = (unsigned char(__stdcall *)(unsigned char, unsigned char *, unsigned char, unsigned char, unsigned char *, unsigned char *))GetProcAddress(hDlla, "piccreadex");

	//������ָ��,�����ֵĺ�����鿴����˾��վ�ṩ�Ķ�̬��˵��
	myctrlword = BLOCK0_EN + BLOCK1_EN + BLOCK2_EN + EXTERNKEY;

	//ָ������
	myareano = 8;//ָ��Ϊ��8��
	//��������ģʽ
	authmode = 1;//����0��ʾ��A������֤���Ƽ���A������֤

	//ָ������
	mypicckey[0] = 0xff;
	mypicckey[1] = 0xff;
	mypicckey[2] = 0xff;
	mypicckey[3] = 0xff;
	mypicckey[4] = 0xff;
	mypicckey[5] = 0xff;

	status = piccreadex(myctrlword, mypiccserial, myareano, authmode, mypicckey, mypiccdata);
	//�������趨�ϵ㣬Ȼ��鿴mypiccserial��mypiccdata��
	//������ piccreadex�����ɶ��������кŵ� mypiccserial�����������ݵ�mypiccdata��
	//������Ա�����Լ�����Ҫ����mypiccserial��mypiccdata �е������ˡ�
	//�����غ���


	id.Format(_T("%s"), mypiccdata);

	switch (status)
	{
	case 0:
		break;
	case 8:
		break;
	}
}


void CCardBetaDlg::Ring()
{
	unsigned char(__stdcall *pcdbeep)(unsigned long xms);

	HINSTANCE hDll_c;
	hDll_c = LoadLibrary(_T("OUR_MIFARE.dll"));
	pcdbeep = (unsigned char(__stdcall *)(unsigned long))GetProcAddress(hDll_c, "pcdbeep");
	pcdbeep(50);
}

void CCardBetaDlg::OnBnClickedBtnInit()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	DWORD m_LocalIP;
	// ����׽����Ƿ�Ϊ��
	if (m_pSocket != NULL)
	{
		delete m_pSocket;
		m_pSocket = NULL;
	}
	// �����µ��׽���ָ��
	m_pSocket = new CUDPSocket();
	//�ӶԻ����ȡ����
	UpdateData(1);

	m_IPAddress_Host.GetAddress(m_LocalIP);
	//�󶨱���IP
	if (!m_pSocket->BindLocatePC(m_LocalIP, m_HostPort))
	{
		AfxMessageBox(_T("Socket����ʧ�ܣ�"));
	}


	UpdateData();
	CShow show;
	//show1 = &show;
	//show.DoModal();
	MinDialog();
}


void CCardBetaDlg::MinDialog()
{
	NotifyIcon.cbSize = sizeof(NOTIFYICONDATA);
	NotifyIcon.hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	NotifyIcon.hWnd = m_hWnd;
	lstrcpy(NotifyIcon.szTip, _T("NotifyIcon Test"));
	NotifyIcon.uCallbackMessage = WM_NC;
	NotifyIcon.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	Shell_NotifyIcon(NIM_ADD, &NotifyIcon);
	ShowWindow(SW_HIDE);
}

LRESULT CCardBetaDlg::OnNotifyIcon(WPARAM wParam, LPARAM lParam)
{
	switch (lParam)
	{
	case WM_RBUTTONUP:{
						  LPPOINT lpoint = new tagPOINT;
						  ::GetCursorPos(lpoint);                    // �õ����λ��
						  CMenu menu;
						  menu.CreatePopupMenu();                    // ����һ������ʽ�˵�
						  menu.AppendMenu(MF_STRING, WM_DESTROY, "�ر�");
						  menu.TrackPopupMenu(TPM_LEFTALIGN, lpoint->x, lpoint->y, this);
						  HMENU hmenu = menu.Detach();
						  menu.DestroyMenu();
						  delete lpoint;
						  break;
	};
	case WM_LBUTTONDOWN:
	{
						  this->ShowWindow(SW_SHOWNORMAL);
						  break;
	};

	}

	return true;
}