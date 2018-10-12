
// CardBetaDlg.cpp : 实现文件
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

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CCardBetaDlg 对话框



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


// CCardBetaDlg 消息处理程序

BOOL CCardBetaDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	m_IPAddress_Host.SetAddress(ntohl(inet_addr("192.168.191.1")));
	m_IPAddress_Remot.SetAddress(ntohl(inet_addr("192.168.191.3")));
	UpdateData(FALSE);// 更新控件的值



	SetTimer(1, 1000, NULL);

	UpdateData(FALSE);// 更新控件的值

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CCardBetaDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CCardBetaDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


CCardBetaDlg * CCardBetaDlg::GetDialog()
{
	//获取对话框指针
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
		//清空缓冲区
		memset(receive_buf, 0, sizeof(receive_buf));

		//接收消息
		int nBytes = m_pSocket->ReceiveFrom(receive_buf, DATA_BUFFER_LENGHT,
			m_rAddr, m_rPort, 0);
		if (nBytes == SOCKET_ERROR)
		{
			m_erro.Format(_T("调用ReceiveFrom函数失败，错误代码是%d"), GetLastError());
			AfxMessageBox(m_erro);
			return;
		}

		//整理接收信息

//		strcat(receive_buf, (char *)m_rAddr.GetBuffer(m_rAddr.GetLength()));
		char buf[100];
		_itoa(ntohs(m_rPort), buf, 10);

		m_ReceiveData += "\r\n";
		m_ReceiveData += receive_buf;

		show1->m_RecieveData_Show = m_ReceiveData;
		show1->show();
		//刷新显示
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

	//将控件中的IP地址存储到m_RemoteIP中
	m_IPAddress_Remot.GetAddress(m_RemoteIP);
	buf.S_un.S_addr = htonl(m_RemoteIP);


	//刷新显示，使得有值变化的控件的值得以显示
	UpdateData(0);

}


void CCardBetaDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
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
	unsigned char status;//存放返回值
	unsigned char myareano;//区号
	unsigned char authmode;//密码类型，用A密码或B密码
	unsigned char myctrlword;//控制字
	unsigned char mypicckey[6];//密码
	unsigned char mypiccserial[4];//卡序列号
	unsigned char mypiccdata[48]; //卡数据缓冲

	unsigned char(__stdcall *piccreadex)(unsigned char ctrlword, unsigned char *serial, unsigned char area, unsigned char keyA1B0, unsigned char *picckey, unsigned char *piccdata0_2);
	//判断动态库是否存在

	//提取函数
	HINSTANCE hDlla;
	hDlla = LoadLibrary(_T("OUR_MIFARE.dll"));
	piccreadex = (unsigned char(__stdcall *)(unsigned char, unsigned char *, unsigned char, unsigned char, unsigned char *, unsigned char *))GetProcAddress(hDlla, "piccreadex");

	//控制字指定,控制字的含义请查看本公司网站提供的动态库说明
	myctrlword = BLOCK0_EN + BLOCK1_EN + BLOCK2_EN + EXTERNKEY;

	//指定区号
	myareano = 8;//指定为第8区
	//批定密码模式
	authmode = 1;//大于0表示用A密码认证，推荐用A密码认证

	//指定密码
	mypicckey[0] = 0xff;
	mypicckey[1] = 0xff;
	mypicckey[2] = 0xff;
	mypicckey[3] = 0xff;
	mypicckey[4] = 0xff;
	mypicckey[5] = 0xff;

	status = piccreadex(myctrlword, mypiccserial, myareano, authmode, mypicckey, mypiccdata);
	//在下面设定断点，然后查看mypiccserial、mypiccdata，
	//调用完 piccreadex函数可读出卡序列号到 mypiccserial，读出卡数据到mypiccdata，
	//开发人员根据自己的需要处理mypiccserial、mypiccdata 中的数据了。
	//处理返回函数


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
	// TODO:  在此添加控件通知处理程序代码
	DWORD m_LocalIP;
	// 检查套接字是否为空
	if (m_pSocket != NULL)
	{
		delete m_pSocket;
		m_pSocket = NULL;
	}
	// 创建新的套接字指针
	m_pSocket = new CUDPSocket();
	//从对话框获取数据
	UpdateData(1);

	m_IPAddress_Host.GetAddress(m_LocalIP);
	//绑定本地IP
	if (!m_pSocket->BindLocatePC(m_LocalIP, m_HostPort))
	{
		AfxMessageBox(_T("Socket创建失败！"));
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
						  ::GetCursorPos(lpoint);                    // 得到鼠标位置
						  CMenu menu;
						  menu.CreatePopupMenu();                    // 声明一个弹出式菜单
						  menu.AppendMenu(MF_STRING, WM_DESTROY, "关闭");
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