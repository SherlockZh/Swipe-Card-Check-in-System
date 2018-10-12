
// SeverDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Sever.h"
#include "SeverDlg.h"
#include "afxdialogex.h"
#include"InitDlg.h"
#include"Listen.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CSeverDlg �Ի���



CSeverDlg::CSeverDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSeverDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	head = NULL;
}

void CSeverDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSeverDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_SHOWTASK, OnShowTask)
	ON_WM_SIZE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CSeverDlg ��Ϣ�������

BOOL CSeverDlg::OnInitDialog()
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

	AfxSocketInit();

	//������IP�Ͷ˿ں�����
	//���м����׽��ֳ�ʼ��
	m_listen = new CListen;
	m_listen->Create(UINT(10000), SOCK_STREAM, FD_ACCEPT | FD_READ);
	m_listen->Listen(100);

	CStdioFile file;
	file.Open((LPCTSTR)_T("Settings.txt"), CFile::modeReadWrite);
	CString open;
	file.ReadString(open);
	if (open == _T("1"))
	{
		HKEY hKey;
		//�ҵ�ϵͳ�������� 
		LPCTSTR lpRun = "Software\\Microsoft\\Windows\\CurrentVersion\\Run";
		//��������Key 
		long lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpRun, 0, KEY_WRITE, &hKey);
		if (lRet == ERROR_SUCCESS)
		{
			char pFileName[MAX_PATH] = { 0 };
			//�õ����������ȫ·�� 
			DWORD dwRet = GetModuleFileName(NULL, pFileName, MAX_PATH);
			//���һ����Key,������ֵ // �����"getip"��Ӧ�ó������֣����Ӻ�׺.exe��
			lRet = RegSetValueEx(hKey, "Sever", 0, REG_SZ, (BYTE *)pFileName, dwRet);
			//�ر�ע��� 
			RegCloseKey(hKey);
			if (lRet != ERROR_SUCCESS)
			{
				AfxMessageBox("ϵͳ��������,������ϵͳ����");
			}
		}
		file.SeekToBegin();
		file.WriteString(_T("0"));
	}


	m_nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
	m_nid.hWnd = this->m_hWnd;
	m_nid.uID = IDR_MAINFRAME;
	m_nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	m_nid.uCallbackMessage = WM_SHOWTASK;
	m_nid.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	strcpy_s(m_nid.szTip, strlen("�Ի���") + 1, "�Ի���");
	Shell_NotifyIcon(NIM_ADD, &m_nid); // �����������ͼ��

	SetTimer(1, 1, NULL);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CSeverDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSeverDlg::OnPaint()
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
HCURSOR CSeverDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


CSeverDlg* CSeverDlg::GetDlg()
{
	//��ȡ�Ի���ָ��
	CSeverDlg *pDl = (CSeverDlg*)(AfxGetApp()->m_pMainWnd);
	if (!pDl)
		return NULL;
	return (pDl);
}


void CSeverDlg::OnAccept()
{
	LINK_Client *p0 = new LINK_Client;
	p0->next = NULL;
	if (NULL == head)
	{
		head = p0;
	}
	else
	{
		LINK_Client *pt =head;
		while (NULL != pt->next)
			pt = pt->next;
		pt->next = p0;
	}
	m_listen->Accept(p0->client);
	p0->client.AsyncSelect(FD_READ|FD_WRITE|FD_CLOSE);
}


void CSeverDlg::OnReceive(Csocket *socket)
{
	char receive_buf[1024];
	int nBytes = socket->ReceiveFrom(receive_buf, 1024,socket->Addr,socket->Port);
	socket->GetPeerName(socket->Addr, socket->Port);
   	if (nBytes == SOCKET_ERROR)
	{
		return;
	}

	CString ID;
	ID.Format(receive_buf);
	CString str=SearchBDS(ID);

	char send_buf[2];
	strcpy_s(send_buf, str.GetLength() * 2, (char*)str.GetBuffer(str.GetLength()));
	//char receive_buf1[1024]= "aaaaaaa";
	//socket->SendTo(receive_buf1, 1024, socket->Port, socket->Addr);
	socket->Send(send_buf, 1024);
	//int aaa = socket->GetLastError();
}


void CSeverDlg::OnClose(Csocket *socket)
{
	LINK_Client *pt = head, *p0=head;
	if (pt->client == *socket)
	{
		head = pt->next;
		delete pt;
		return;
	}

	while (pt != NULL)
	{
		if (pt->client == *socket)
		{
			p0->next = pt->next;
			delete pt;
			return;
		}
		p0 = pt;
		pt = pt->next;
	}
	return;
}


CString CSeverDlg::SearchBDS(CString ID)
{
	Ado m_ado;
	m_ado.Link(_T("HELLO-PC"), _T("STUDENT"), _T("yes"), _T("yes"));
	CString strSQL;
	strSQL.Format(_T("select * from student where ID='%s'"), ID);
	m_ado.Open(strSQL);

	if (m_ado.IsEOF())
	{
		return _T("0");
	}
	CString Name = m_ado.GetValue(1);
	m_ado.Close();

	CTime t = CTime::GetCurrentTime();
	int year, month, day;
	year = t.GetYear();
	month = t.GetMonth();
	day = t.GetDay();
	CString dayTime,time;
	dayTime.Format(_T("%d/%d/%d"), year, month, day);
	time.Format(_T("%d:%d:%d"), t.GetHour(), t.GetMinute(), t.GetSecond());

	strSQL.Format(_T("select * from carddata1 where ID='%s' and day='%d/%d/%d'"),ID, year, month, day);
	m_ado.Open(strSQL);
	CString str1, str2, str3, str4, str5, str6, str7;
	str1=str2=str3=str4=str5=str6=str7 = _T("\0");
	if (m_ado.IsEOF())
	{
		m_ado.Add(12, ID, Name, _T(""), dayTime, time, str1, str2, str3, str4, str5, str6, str7);
		m_ado.Update();
		return _T("1");
	}

	int index = 5;
	while (_T("")!=m_ado.GetValue(index))
	{
		index++;
		if (index == 12)
			return _T("1");
	}

	m_ado.recordset->Fields->GetItem((long)index)->Value = _variant_t(time);
	m_ado.Update();
	return _T("1");
}


LRESULT CSeverDlg::OnShowTask(WPARAM wParam, LPARAM lParam)
{

	if (wParam != IDR_MAINFRAME) return 1;
	switch (lParam)
	{
	case WM_RBUTTONUP:               //�Ҽ�����ʱ�����˵�
	{
		LPPOINT lpoint = new tagPOINT;
		::GetCursorPos(lpoint);          
		CMenu menu;
		menu.CreatePopupMenu();         
		menu.AppendMenu(MF_STRING, WM_DESTROY, "�˳�");
		menu.TrackPopupMenu(TPM_LEFTALIGN, lpoint->x, lpoint->y, this);
		HMENU hmenu = menu.Detach();
		menu.DestroyMenu();
		delete lpoint;
		break;
	}
	case WM_LBUTTONDBLCLK:                            // ˫������Ĵ���
		this->ShowWindow(SW_SHOWNORMAL);         // ��ʾ������ 
		break;
	}
	return 0;
}

void CSeverDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	if (nType == SIZE_MINIMIZED)
	{
		ShowWindow(SW_HIDE);//������С������������ͼ��
	}
}


void CSeverDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == 1)
	{
		KillTimer(1);
		ShowWindow(SW_HIDE);
	}
}
