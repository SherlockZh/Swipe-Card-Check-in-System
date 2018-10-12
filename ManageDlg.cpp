
// ManageDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Manage.h"
#include "ManageDlg.h"
#include "afxdialogex.h"
#include"MyBtn.h"
#include"MessageBox.h"
#include"SetDLG.h"
#include"AddDlg.h"
#include "Excel9.h"
#include"SearchDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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
public:
//	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
//	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
//	ON_WM_MOUSEHOVER()
//	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CManageDlg 对话框
#include"DBSExportDlg.h"


CManageDlg::CManageDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CManageDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CManageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MIN, m_Min);
	DDX_Control(pDX, IDC_CLOSE, m_Close);
	DDX_Control(pDX, IDC_SEACH, m_seach);
	DDX_Control(pDX, IDC_EXPORT, m_Export);
	DDX_Control(pDX, IDC_SET, m_Set);
	DDX_Control(pDX, IDC_ADD, m_Add);
}

BEGIN_MESSAGE_MAP(CManageDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_NCHITTEST()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_MIN, &CManageDlg::OnBnClickedMin)
	ON_BN_CLICKED(IDC_CLOSE, &CManageDlg::OnBnClickedClose)
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_SEACH, &CManageDlg::OnBnClickedSeach)
	ON_BN_CLICKED(IDC_EXPORT, &CManageDlg::OnBnClickedExport)
	ON_BN_CLICKED(IDC_SET, &CManageDlg::OnBnClickedSet)
	ON_BN_CLICKED(IDC_ADD, &CManageDlg::OnBnClickedAdd)
END_MESSAGE_MAP()


// CManageDlg 消息处理程序

BOOL CManageDlg::OnInitDialog()
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


	this->GetWindowRect(&m_dlgRect);

	//获取桌面窗口的大小  
	CWnd*  pDestopWnd = GetDesktopWindow();
	pDestopWnd->GetWindowRect(&m_destopRect);

	//初始化对话框的位置和大小（位置：桌面中心，大小：0）  
	MoveWindow(
		(m_destopRect.Width() - m_dlgRect.Width()) / 2,
		(m_destopRect.Height() - m_dlgRect.Height()) / 2,
		0,
		0);

	//设置定时器，并启动（5s）  
	this->SetTimer(1, 5, NULL);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CManageDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CManageDlg::OnPaint()
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

		CBitmap bmp;
		bmp.LoadBitmap(IDB_BITMAP5);

		CDC dcCompatible;
		dcCompatible.CreateCompatibleDC(&dc);

		dcCompatible.SelectObject(&bmp);

		//CRect rect;
		GetClientRect(rect);
		dc.BitBlt(0, 0, rect.Width(), rect.Height(), &dcCompatible, 0, 0, SRCCOPY);

	
	}
	else
	{
		CPaintDC dc(this);
		CBitmap bmp;
		bmp.LoadBitmap(IDB_BITMAP5);

		CDC dcCompatible;
		dcCompatible.CreateCompatibleDC(&dc);

		dcCompatible.SelectObject(&bmp);

		CRect rect;
		GetClientRect(rect);

		dc.BitBlt(0, 0, rect.Width(), rect.Height(), &dcCompatible, 0, 0, SRCCOPY);


		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CManageDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



LRESULT CManageDlg::OnNcHitTest(CPoint point)
{
	UINT nHitTest = CDialog::OnNcHitTest(point);
	return (nHitTest == HTCLIENT) ? HTCAPTION : nHitTest;
	return CDialogEx::OnNcHitTest(point);
}


void CManageDlg::OnTimer(UINT_PTR nIDEvent)
{
	CRect dlgRect;         //保存对话框窗口当前大小  
	static int offsetX = 7;//更新对话框窗口状态时的X增量  
	static int offsetY = 5;//更新对话框窗口状态时的Y增量  

	switch (nIDEvent)
	{
	case 1:
		while (1)
		{
			//获取对话框窗口当前的大小  
			this->GetWindowRect(&dlgRect);
			//更新对话框的位置和大小（利用：offsetX,offsetY）  
			MoveWindow(
				(-offsetX + m_destopRect.Width() - dlgRect.Width()) / 2,
				(-offsetY + m_destopRect.Height() - dlgRect.Height()) / 2,
				dlgRect.Width() + offsetX,
				dlgRect.Height() + offsetY);

			//X方向达到对话框窗口预期值，停止增加  
			if (dlgRect.Width() >= m_dlgRect.Width())   offsetX = 0;
			//Y方向达到对话框窗口预期值，停止增加  
			if (dlgRect.Height() >= m_dlgRect.Height()) offsetY = 0;
			//X，Y都达到对话框窗口预期值，关闭定时器  
			if (dlgRect.Width() >= m_dlgRect.Width() && dlgRect.Height() >= m_dlgRect.Height())
			{
				this->KillTimer(nIDEvent);
				return;
			}
			Sleep(5);
		}

	default:
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CManageDlg::OnBnClickedMin()
{
	PostMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
}


void CManageDlg::OnBnClickedClose()
{
	SendMessage(WM_CLOSE);
}


BOOL CManageDlg::OnEraseBkgnd(CDC* pDC)
{
	CBitmap bmp;
	bmp.LoadBitmap(IDB_BITMAP5);

	CDC dcCompatible;
	dcCompatible.CreateCompatibleDC(pDC);

	dcCompatible.SelectObject(&bmp);

	CRect rect;
	GetClientRect(rect);

	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &dcCompatible, 0, 0, SRCCOPY);

	return TRUE;
	return CDialogEx::OnEraseBkgnd(pDC);
}

void CManageDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnMouseMove(nFlags, point);
}


void CManageDlg::OnBnClickedSeach()
{
	CSearchDlg dlg;
	dlg.DoModal();
}


void CManageDlg::OnBnClickedExport()
{
	CDBSExportDlg dlg;
	dlg.DoModal();

}


void CManageDlg::OnBnClickedSet()
{
	CSetDLG dlg;
	IDOK == dlg.DoModal();
}


void CManageDlg::OnBnClickedAdd()
{
	CAddDlg dlg;
	dlg.DoModal();
}
