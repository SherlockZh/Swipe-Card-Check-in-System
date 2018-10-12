
// ManageDlg.cpp : ʵ���ļ�
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


// CManageDlg �Ի���
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


// CManageDlg ��Ϣ�������

BOOL CManageDlg::OnInitDialog()
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


	this->GetWindowRect(&m_dlgRect);

	//��ȡ���洰�ڵĴ�С  
	CWnd*  pDestopWnd = GetDesktopWindow();
	pDestopWnd->GetWindowRect(&m_destopRect);

	//��ʼ���Ի����λ�úʹ�С��λ�ã��������ģ���С��0��  
	MoveWindow(
		(m_destopRect.Width() - m_dlgRect.Width()) / 2,
		(m_destopRect.Height() - m_dlgRect.Height()) / 2,
		0,
		0);

	//���ö�ʱ������������5s��  
	this->SetTimer(1, 5, NULL);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CManageDlg::OnPaint()
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

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
	CRect dlgRect;         //����Ի��򴰿ڵ�ǰ��С  
	static int offsetX = 7;//���¶Ի��򴰿�״̬ʱ��X����  
	static int offsetY = 5;//���¶Ի��򴰿�״̬ʱ��Y����  

	switch (nIDEvent)
	{
	case 1:
		while (1)
		{
			//��ȡ�Ի��򴰿ڵ�ǰ�Ĵ�С  
			this->GetWindowRect(&dlgRect);
			//���¶Ի����λ�úʹ�С�����ã�offsetX,offsetY��  
			MoveWindow(
				(-offsetX + m_destopRect.Width() - dlgRect.Width()) / 2,
				(-offsetY + m_destopRect.Height() - dlgRect.Height()) / 2,
				dlgRect.Width() + offsetX,
				dlgRect.Height() + offsetY);

			//X����ﵽ�Ի��򴰿�Ԥ��ֵ��ֹͣ����  
			if (dlgRect.Width() >= m_dlgRect.Width())   offsetX = 0;
			//Y����ﵽ�Ի��򴰿�Ԥ��ֵ��ֹͣ����  
			if (dlgRect.Height() >= m_dlgRect.Height()) offsetY = 0;
			//X��Y���ﵽ�Ի��򴰿�Ԥ��ֵ���رն�ʱ��  
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
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ

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
