
// MFCApplication1Dlg.cpp: 实现文件
//

#include "stdafx.h"
#include "MFCApplication1.h"
#include "MFCApplication1Dlg.h"
#include "afxdialogex.h"
#include<iostream>
#include"Windows.h"
#include<queue>
#include<assert.h>
#include"winuser.h"
#define MAXNUM 10
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
using namespace std;
//线程
HANDLE SdoorEmpty;//表示门是否为可通过状态，可通过为1，不可通过为0
queue<int>startque;
queue<int>endque;
int InNum = 0;//记录多少个人进入教室
bool StartExam = false;//判断是否开始考试
bool finish = false;
int outnum = 0;
bool examing = false;
int nnend, nnstart;
int timer = 70;
//绘图
int stepX, stepY;//每次移动改变的步数
int a[10][5];//对应位置a[i][number] i表示对应座位 number表示移动步骤对应已走过步数
int b[10];//表示目前移动处于的状态
int b1[10];
int time1 = 50;//进入时间
int nstart=-1;//记录哪个学生开始进入；
int nend=-1;//记录哪个学生开始出场；
int backque=-1;
bool isbackque=false;
bool IsFull1 = 0;
bool IsEmpty1 = 0;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCApplication1Dlg 对话框



CMFCApplication1Dlg::CMFCApplication1Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPPLICATION1_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCApplication1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCApplication1Dlg::OnBnClickedButton1)
//	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON3, &CMFCApplication1Dlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON2, &CMFCApplication1Dlg::OnBnClickedButton2)
	ON_STN_CLICKED(SHOW, &CMFCApplication1Dlg::OnStnClickedShow)
	ON_BN_CLICKED(IDC_BUTTON5, &CMFCApplication1Dlg::OnBnClickedButton5)
END_MESSAGE_MAP()


// CMFCApplication1Dlg 消息处理程序

BOOL CMFCApplication1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// TODO: 在此添加额外的初始化代码
	stepX = 0;
	stepY = 0;

	//SetTimer(0, 100, NULL);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCApplication1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMFCApplication1Dlg::OnPaint()
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
HCURSOR CMFCApplication1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
void stu_desk(HDC hdc, double left, double top, double right, double bottom)
{

	HPEN curpen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));   //创建画笔
	HPEN oldpen = (HPEN)SelectObject(hdc, curpen);          //选择画笔
	HBRUSH curbrush = CreateSolidBrush(RGB(184, 134, 11));   //创建画刷
	HBRUSH oldbrush = (HBRUSH)SelectObject(hdc, curbrush);  //选择画刷
	SetTextColor(hdc, RGB(255, 0, 0));                      //设置文字颜色
	SetBkMode(hdc, TRANSPARENT);                            //设置文字背景颜色，当前透明

	double x = left, y = top;
	double x45 = (right - left) / 110 * 45;
	double x110 = right - left;
	double x100 = (right - left) / 110 * 100;
	double x10 = (right - left) / 110 * 10;
	double x80 = (right - left) / 110 * 80;
	double x3 = (right - left) / 110 * 3;
	double x5 = x10 / 2;
	double x70 = x10 * 7;
	double x40 = x10 * 4;
	double x20 = x10 * 2;
	RoundRect(hdc, x + x45, y, x + x110, y + x100, x10, x10);
	RoundRect(hdc, x, y + x10, x + x10, y + x80, x3, x3);
	RoundRect(hdc, x + x10, y + x20, x + x40, y + x70, x5, x5);


	SelectObject(hdc, oldpen);   //恢复原先画笔
	DeleteObject(curpen);        //释放创建的画笔
	SelectObject(hdc, oldbrush); //恢复原先画刷
	DeleteObject(curbrush);
}
void teacher_desk(HDC hdc, double left, double top, double right, double bottom)
{

	HPEN curpen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));   //创建画笔
	HPEN oldpen = (HPEN)SelectObject(hdc, curpen);          //选择画笔
	HBRUSH curbrush = CreateSolidBrush(RGB(184, 134, 11));   //创建画刷
	HBRUSH oldbrush = (HBRUSH)SelectObject(hdc, curbrush);  //选择画刷
	SetTextColor(hdc, RGB(255, 0, 0));                      //设置文字颜色
	SetBkMode(hdc, TRANSPARENT);                            //设置文字背景颜色，当前透明
	
	RoundRect(hdc, left, top*1.5, right, bottom*0.4, 5, 5);

	SelectObject(hdc, oldpen);   //恢复原先画笔
	DeleteObject(curpen);        //释放创建的画笔
	SelectObject(hdc, oldbrush); //恢复原先画刷
	DeleteObject(curbrush);
}
void student(HDC hdc, double left, double top, double right, double bottom)
{
	double mx = (right + left) / 2;
	double my = (top + bottom) / 2;
	double x7 = (right - left) / 80 * 7;
	double y7 = (bottom - top) / 80 * 7;
	double x10 = (right - left) / 80 * 10;
	double y10 = (bottom - top) / 80 * 10;
	double x8 = (right - left) / 80 * 8;
	double headx = mx;
	double heady = my - y10*2;
	Ellipse(hdc, headx - x7, heady + y7, headx + x7, heady - y7);
	MoveToEx(hdc, headx, heady + y7, NULL);
	LineTo(hdc, mx, my + y10*2);
	LineTo(hdc, mx - x8, my + y10*4);
	MoveToEx(hdc, mx, my + y10*2, NULL);
	LineTo(hdc, mx + x8, my + y10*4);
	MoveToEx(hdc, mx, my - y10, NULL);
	LineTo(hdc, mx - x8, my + y10);
	MoveToEx(hdc, mx, my - y10, NULL);
	LineTo(hdc, mx + x8, my + y10);

}
void MyDrawer(HDC hdc)
{
	CRect rect;
	double Rest1, Rest2;
	::GetClientRect(WindowFromDC(hdc), &rect);//获取的是以其自身的最左上角的点为坐标原点，此窗口的位置
	Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom*0.9);//画一个矩形

	Rest1 = rect.right - rect.left;
	Rest2 = rect.bottom*0.9 - rect.top;//获取界面边框

	double x = 0;
	double y = Rest2 / 42 * 33;

	stepX = Rest1 / 40;
	stepY = (-1) * Rest2 / 42;

	//HPEN curpen = CreatePen(PS_SOLID, 2, RGB(0, 0, 255));   //创建画笔
	//HPEN oldpen = (HPEN)SelectObject(hdc, curpen);          //选择画笔
	//HBRUSH curbrush = CreateSolidBrush(RGB(255, 255, 0));   //创建画刷
	//HBRUSH oldbrush = (HBRUSH)SelectObject(hdc, curbrush);  //选择画刷
	//SetTextColor(hdc, RGB(255, 0, 0));                      //设置文字颜色
	//SetBkMode(hdc, TRANSPARENT);                            //设置文字背景颜色，当前透明

	MoveToEx(hdc, 0, Rest2 / 42 * 33, NULL);            //绘制线条，移动到起点再绘制
	LineTo(hdc, Rest1 / 40 * 28, Rest2 / 42 * 33);//划分教室区域
	MoveToEx(hdc, 0, Rest2 / 42 * 33, NULL);            //把起点移至线的左端

	stu_desk(hdc, x + Rest1 / 40, y - Rest2 / 42 * 16, x + Rest1 / 40 * 6, y - Rest2 / 42 * 22);//目前每个框长110 宽100
	stu_desk(hdc, x + Rest1 / 40, y - Rest2 / 42 * 24, x + Rest1 / 40 * 6, y - Rest2 / 42 * 30);
	stu_desk(hdc, x + Rest1 / 40, y - Rest2 / 42 * 32, x + Rest1 / 40 * 6, y - Rest2 / 42 * 38);
	stu_desk(hdc, x + Rest1 / 40 * 12, y - Rest2 / 42 * 16, x + Rest1 / 40 * 17, y - Rest2 / 42 * 22);
	stu_desk (hdc, x + Rest1 / 40 * 12, y - Rest2 / 42 * 24, x + Rest1 / 40 * 17, y - Rest2 / 42 * 30);
	stu_desk(hdc, x + Rest1 / 40 * 12, y - Rest2 / 42 * 32, x + Rest1 / 40 * 17, y - Rest2 / 42 * 38);
	stu_desk(hdc, x + Rest1 / 40 * 23, y - Rest2 / 42 * 16, x + Rest1 / 40 * 28, y - Rest2 / 42 * 22);
	stu_desk(hdc, x + Rest1 / 40 * 23, y - Rest2 / 42 * 24, x + Rest1 / 40 * 28, y - Rest2 / 42 * 30);
	stu_desk(hdc, x + Rest1 / 40 * 23, y - Rest2 / 42 * 32, x + Rest1 / 40 * 28, y - Rest2 / 42 * 38);
	teacher_desk(hdc, x + Rest1 / 40 * 34, y - Rest2 / 42 * 17, x + Rest1 / 40 * 39, y - Rest2 / 42 * 24);

	for (int i = 0; i < 10; i++) {
		if (i != 0) {
			if (b1[i] == -2) {
				student(hdc, x + Rest1 / 40 * (i * 2 + 1), y + Rest2 / 42, x + Rest1 / 40 * (6 + i * 2), y + Rest2 / 42 * 7);
				TextOut(hdc, x + Rest1 / 40 * (i * 2 + 3), y + Rest2 / 42, _T("学生"), 2);
			}
		}
		else {
			if (b1[i] == -2) {
				student(hdc, x + Rest1 / 40 * (i * 2 + 1), y + Rest2 / 42, x + Rest1 / 40 * (6 + i * 2), y + Rest2 / 42 * 7);
				TextOut(hdc, x + Rest1 / 40 * (i * 2 + 3), y + Rest2 / 42, _T("老师"), 2);
			}
		}
	}
	
	//TextOut(hdc, x, y, _T("学生"), 2);

//	SelectObject(hdc, oldpen);   //恢复原先画笔
//	DeleteObject(curpen);        //释放创建的画笔
//	SelectObject(hdc, oldbrush); //恢复原先画刷
//	DeleteObject(curbrush);      //释放创建的画刷
}


void MyDrawer1(HDC hdc, int x1, int y1, int number)
{
	CRect rect;
	int Rest1, Rest2;
	::GetClientRect(WindowFromDC(hdc), &rect);//获取的是以其自身的最左上角的点为坐标原点，此窗口的位置
	Rest1 = rect.right - rect.left;
	Rest2 = rect.bottom*0.9 - rect.top;//获取界面边框

	HPEN curpen = CreatePen(PS_SOLID, 2, RGB(255, 0, 255));   //创建画笔
	HPEN oldpen = (HPEN)SelectObject(hdc, curpen);          //选择画笔
	SetTextColor(hdc, RGB(255, 0, 0));                      //设置文字颜色
	SetBkMode(hdc, TRANSPARENT);                            //设置文字背景颜色，当前透明

	int x = 0;
	int y = Rest2 / 42 * 33;

	MoveToEx(hdc, x, y, NULL);            //固定起点
	if (number == 0) {
		switch (b[number]) {
		case 1:
			student(hdc, x + Rest1 / 40 * 23 + x1 * a[number][0], y + Rest2 / 42 * 8, x + Rest1 / 40 * 28 + x1 * a[number][0], y + Rest2 / 42);
			a[number][0] = a[number][0] + 1;
			if (a[number][0] == 6)
			{
				b[number] = 2;
			}
			break;
		case 2:
			student(hdc, x + Rest1 / 40 * 23 + x1 * a[number][0], y + Rest2 / 42 + y1 * a[number][1], x + Rest1 / 40 * 28 + x1 * a[number][0], y + Rest2 / 42 * 8 + y1 * a[number][1]);
			a[number][1] = a[number][1] + 1;
			if (a[number][1] == 9)
			{
				b[number] = 3;
			}
			break;
		case 3:
			student(hdc, x + Rest1 / 40 * 23 + x1 * (a[number][0] - a[number][2]), y + Rest2 / 42 + y1 * a[number][1], x + Rest1 / 40 * 28 + x1 * (a[number][0] - a[number][2]), y + Rest2 / 42 * 8 + y1 * a[number][1]);
				b[number] = 4;
			break;
		case 4:
			student(hdc, x + Rest1 / 40 * 23 + x1 * (a[number][0] - a[number][2]), y + Rest2 / 42 + y1 * (a[number][1] + a[number][3]), x + Rest1 / 40 * 28 + x1 * (a[number][0] - a[number][2]), y + Rest2 / 42 * 8 + y1 * (a[number][1] + a[number][3]));
			a[number][3] = a[number][3] + 1;
			if (a[number][3] == 16)
			{
				b[number] = 5;
			}
			break;
		case 5:
			student(hdc, x + Rest1 / 40 * 23 + x1 * (a[number][0] - a[number][2] + a[number][4]), y + Rest2 / 42 + y1 * (a[number][1] + a[number][3]), x + Rest1 / 40 * 28 + x1 * (a[number][0] - a[number][2] + a[number][4]), y + Rest2 / 42 * 8 + y1 * (a[number][1] + a[number][3]));
			a[number][4] = a[number][4] + 1;
			if (a[number][4] == 5)
			{
				b[number] = 6;
			}
			break;
		default :
			student(hdc, x + Rest1 / 40 * 23 + x1 * (a[number][0] - a[number][2] + a[number][4]), y + Rest2 / 42 + y1 * (a[number][1] + a[number][3]), x + Rest1 / 40 * 28 + x1 * (a[number][0] - a[number][2] + a[number][4]), y + Rest2 / 42 * 8 + y1 * (a[number][1] + a[number][3]));
			break;
		}
	}
	else
	{
		switch (b[number]) {
		case 1:
			student(hdc, x + Rest1 / 40 * 23 + x1 * a[number][0], y + Rest2 / 42 * 8, x + Rest1 / 40 * 28 + x1 * a[number][0], y + Rest2 / 42);
			a[number][0] = a[number][0] + 1;
			if (a[number][0] == 6)
			{
				b[number] = 2;
			}
			break;
		case 2:
			student(hdc, x + Rest1 / 40 * 23 + x1 * a[number][0], y + Rest2 / 42 + y1 * a[number][1], x + Rest1 / 40 * 28 + x1 * a[number][0], y + Rest2 / 42 * 8 + y1 * a[number][1]);
			a[number][1] = a[number][1] + 1;
			if (a[number][1] == 9)
			{
				b[number] = 3;
			}
			break;
		case 3:
			student(hdc, x + Rest1 / 40 * 23 + x1 * (a[number][0] - a[number][2]), y + Rest2 / 42 + y1 * a[number][1], x + Rest1 / 40 * 28 + x1 * (a[number][0] - a[number][2]), y + Rest2 / 42 * 8 + y1 * a[number][1]);
			if (number > 3)
			{
				a[number][2] = a[number][2] + 1;
			}
			if (a[number][2] == (0 + (number - 1) / 3 * 11))
			{
				b[number] = 4;
			}

			break;
		case 4:
			student(hdc, x + Rest1 / 40 * 23 + x1 * (a[number][0] - a[number][2]), y + Rest2 / 42 + y1 * (a[number][1] + a[number][3]), x + Rest1 / 40 * 28 + x1 * (a[number][0] - a[number][2]), y + Rest2 / 42 * 8 + y1 * (a[number][1] + a[number][3]));
			a[number][3] = a[number][3] + 1;
			if (a[number][3] == (8 + (number - 1) % 3 * 8))
			{
				b[number] = 5;
			}
			break;
		case 5:
			student(hdc, x + Rest1 / 40 * 23 + x1 * (a[number][0] - a[number][2] - a[number][4]), y + Rest2 / 42 + y1 * (a[number][1] + a[number][3]), x + Rest1 / 40 * 28 + x1 * (a[number][0] - a[number][2] - a[number][4]), y + Rest2 / 42 * 8 + y1 * (a[number][1] + a[number][3]));
			a[number][4] = a[number][4] + 1;
			if (a[number][4] == 6)
			{
				b[number] = 6;
			}
			break;
		default :
			student(hdc, x + Rest1 / 40 * 23 + x1 * (a[number][0] - a[number][2] - a[number][4]), y + Rest2 / 42 + y1 * (a[number][1] + a[number][3]), x + Rest1 / 40 * 28 + x1 * (a[number][0] - a[number][2] - a[number][4]), y + Rest2 / 42 * 8 + y1 * (a[number][1] + a[number][3]));
			break;
		}
	}
	SelectObject(hdc, oldpen);   //恢复原先画笔
	DeleteObject(curpen);        //释放创建的画笔
}
void MyDrawer2(HDC hdc, int x1, int y1, int number)
{
		CRect rect;
		int Rest1, Rest2;
		::GetClientRect(WindowFromDC(hdc), &rect);//获取的是以其自身的最左上角的点为坐标原点，此窗口的位置
		Rest1 = rect.right - rect.left;
		Rest2 = rect.bottom*0.9 - rect.top;//获取界面边框

		HPEN curpen = CreatePen(PS_SOLID, 2, RGB(255, 0, 255));   //创建画笔
		HPEN oldpen = (HPEN)SelectObject(hdc, curpen);          //选择画笔
		SetTextColor(hdc, RGB(255, 0, 0));                      //设置文字颜色
		SetBkMode(hdc, TRANSPARENT);                            //设置文字背景颜色，当前透明

		int x = 0;
		int y = Rest2 / 42 * 33;

		MoveToEx(hdc, x, y, NULL);            //固定起点
		if (number == 0) {
			switch (b[number]) {
			case 0:
				//student(hdc, x + Rest1 / 40 * 23, y + Rest2 / 42, x + Rest1 / 40 * 28, y + Rest2 / 42 * 8);
				break;
			case 1:
				student(hdc, x + Rest1 / 40 * 23 + x1 * a[number][0], y + Rest2 / 42 * 8, x + Rest1 / 40 * 28 + x1 * a[number][0], y + Rest2 / 42);
				a[number][0] = a[number][0] + 1;
				if (a[number][0] == 15)
				{
					b[number] = 0;
				}
				break;
			case 2:
				student(hdc, x + Rest1 / 40 * 23 + x1 * a[number][0], y + Rest2 / 42 + y1 * a[number][1], x + Rest1 / 40 * 28 + x1 * a[number][0], y + Rest2 / 42 * 8 + y1 * a[number][1]);
				a[number][1] = a[number][1] - 1;
				if (a[number][1] == 0)
				{
					b[number] = 1;
				}
				break;
			case 3:
				student(hdc, x + Rest1 / 40 * 23 + x1 * (a[number][0] - a[number][2]), y + Rest2 / 42 + y1 * a[number][1], x + Rest1 / 40 * 28 + x1 * (a[number][0] - a[number][2]), y + Rest2 / 42 * 8 + y1 * a[number][1]);
				b[number] = 2;
				break;
			case 4:
				student(hdc, x + Rest1 / 40 * 23 + x1 * (a[number][0] - a[number][2]), y + Rest2 / 42 + y1 * (a[number][1] + a[number][3]), x + Rest1 / 40 * 28 + x1 * (a[number][0] - a[number][2]), y + Rest2 / 42 * 8 + y1 * (a[number][1] + a[number][3]));
				a[number][3] = a[number][3] - 1;
				if (a[number][3] == 0)
				{
					b[number] = 3;
				}
				break;
			case 5:
				student(hdc, x + Rest1 / 40 * 23 + x1 * (a[number][0] - a[number][2] + a[number][4]), y + Rest2 / 42 + y1 * (a[number][1] + a[number][3]), x + Rest1 / 40 * 28 + x1 * (a[number][0] - a[number][2] + a[number][4]), y + Rest2 / 42 * 8 + y1 * (a[number][1] + a[number][3]));
				
				a[number][4] = a[number][4] - 1;
				if (a[number][4] == 0)
				{
					b[number] = 4;
				}
				break;
			default :
				b[number] = 5;
				break;
			}
		}
		else {
			switch (b[number]) {
			case 0:
				student(hdc, x + Rest1 / 40 * 23, y + Rest2 / 42, x + Rest1 / 40 * 28, y + Rest2 / 42 * 8);
				break;
			case 1:
				student(hdc, x + Rest1 / 40 * 23 + x1 * a[number][0], y + Rest2 / 42 * 8, x + Rest1 / 40 * 28 + x1 * a[number][0], y + Rest2 / 42);
				a[number][0] = a[number][0] + 1;
				if (a[number][0] == 15)
				{
					b[number] = 0;
				}
				break;
			case 2:
				student(hdc, x + Rest1 / 40 * 23 + x1 * a[number][0], y + Rest2 / 42 + y1 * a[number][1], x + Rest1 / 40 * 28 + x1 * a[number][0], y + Rest2 / 42 * 8 + y1 * a[number][1]);
				a[number][1] = a[number][1] - 1;
				if (a[number][1] == 0)
				{
					b[number] = 1;
				}
				break;
			case 3:
				student(hdc, x + Rest1 / 40 * 23 + x1 * (a[number][0] - a[number][2]), y + Rest2 / 42 + y1 * a[number][1], x + Rest1 / 40 * 28 + x1 * (a[number][0] - a[number][2]), y + Rest2 / 42 * 8 + y1 * a[number][1]);
				if (number > 3)
				{
					a[number][2] = a[number][2] - 1;
				}
				if (a[number][2] == 0)
				{
					b[number] = 2;
				}

				break;
			case 4:
				student(hdc, x + Rest1 / 40 * 23 + x1 * (a[number][0] - a[number][2]), y + Rest2 / 42 + y1 * (a[number][1] + a[number][3]), x + Rest1 / 40 * 28 + x1 * (a[number][0] - a[number][2]), y + Rest2 / 42 * 8 + y1 * (a[number][1] + a[number][3]));
				a[number][3] = a[number][3] - 1;
				if (a[number][3] == 0)
				{
					b[number] = 3;
				}
				break;
			case 5:
				student(hdc, x + Rest1 / 40 * 23 + x1 * (a[number][0] - a[number][2] - a[number][4]), y + Rest2 / 42 + y1 * (a[number][1] + a[number][3]), x + Rest1 / 40 * 28 + x1 * (a[number][0] - a[number][2] - a[number][4]), y + Rest2 / 42 * 8 + y1 * (a[number][1] + a[number][3]));
				a[number][4] = a[number][4] - 1;
				if (a[number][4] == 0)
				{
					b[number] = 4;
				}
				break;
			default :
				student(hdc, x + Rest1 / 40 * 23 + x1 * (a[number][0] - a[number][2] - a[number][4]), y + Rest2 / 42 + y1 * (a[number][1] + a[number][3]), x + Rest1 / 40 * 28 + x1 * (a[number][0] - a[number][2] - a[number][4]), y + Rest2 / 42 * 8 + y1 * (a[number][1] + a[number][3]));
				b[number] = 5;
				break;
			}
		}
		SelectObject(hdc, oldpen);   //恢复原先画笔
		DeleteObject(curpen);
	}
		
	DWORD WINAPI teacher(LPVOID param)
	{
		HDC hdc = (HDC)param;
		bool isPassed = false;
		while (!isPassed)//如果没还没进入，这这里循环等待
		{
			Sleep(rand() % 1000);
			WaitForSingleObject(SdoorEmpty, INFINITE);//等待到进入信号
			isPassed = true;
			nstart = 0;
			startque.push(nstart);
			Sleep(rand() % time1);
			ReleaseSemaphore(SdoorEmpty, 1, NULL);
			if (++InNum == 10)//如果10个人都进入了则开始考试
			{
				StartExam = true;
			}
		}
		return 0;
	}

	DWORD WINAPI student1(LPVOID param)
	{
		HDC hdc = (HDC)param;
		bool isPassed = false;//判断是否进入教室
		bool FinishExam = false;//判断是否考完
		bool GoOut = false;//判断是否离开教室
		while (1)
		{
			Sleep(rand() % 1000);

			while (!isPassed)
			{
				WaitForSingleObject(SdoorEmpty, INFINITE);
				isPassed = true;
				nstart = 1;
				
				startque.push(nstart);
				Sleep(rand() % time1);
				ReleaseSemaphore(SdoorEmpty, 1, NULL);

				if (++InNum == 10)//如果10个人都进入了则开始考试
				{
					StartExam = true;
				}
			}
			if (StartExam)
			{
				Sleep(rand() % time1);//随机交卷
				FinishExam = true;
				
			}
			while (FinishExam && !GoOut)//离开考场
			{
				WaitForSingleObject(SdoorEmpty, INFINITE);
				GoOut = true;
				nend = 1;
				
				endque.push(nend);
				Sleep(rand() % time1);
				ReleaseSemaphore(SdoorEmpty, 1, NULL);
				if (++outnum == 9)//如果9个人都进入了则开始考试
				{
					finish = true;
					ExitThread(1);
				}
			}
		}
		return 0;
	}

	DWORD WINAPI student2(LPVOID param)
	{
		HDC hdc = (HDC)param;
		bool isPassed = false;//判断是否进入教室
		bool FinishExam = false;//判断是否考完
		bool GoOut = false;//判断是否离开教室
		while (1)
		{
			Sleep(rand() % 1000);
			while (!isPassed)
			{
				WaitForSingleObject(SdoorEmpty, INFINITE);
				isPassed = true;
				nstart = 2;
			
				startque.push(nstart);
				Sleep(rand() % time1);
				ReleaseSemaphore(SdoorEmpty, 1, NULL);

				if (++InNum == 10)//如果10个人都进入了则开始考试
				{
					StartExam = true;
				}
			}
			if (StartExam)
			{
				Sleep(rand() %time1);//随机交卷
				FinishExam = true;
			}
			while (FinishExam && !GoOut)//离开考场
			{
				WaitForSingleObject(SdoorEmpty, INFINITE);
				GoOut = true;
				nend = 2;
				
				endque.push(nend);
				Sleep(rand() % time1);
				ReleaseSemaphore(SdoorEmpty, 1, NULL);
				if (++outnum == 9)//如果9个人都进入了则开始考试
				{
					finish = true;
					ExitThread(2);
				}
			}
		}

		return 0;
	}

	DWORD WINAPI student3(LPVOID param)
	{
		HDC hdc = (HDC)param;
		bool isPassed = false;//判断是否进入教室
		bool FinishExam = false;//判断是否考完
		bool GoOut = false;//判断是否离开教室
		while (1)
		{
			Sleep(rand() % 1000);
			while (!isPassed)
			{
				WaitForSingleObject(SdoorEmpty, INFINITE);
				isPassed = true;
				nstart = 3;
			
				startque.push(nstart);
				Sleep(rand() % time1);
				ReleaseSemaphore(SdoorEmpty, 1, NULL);

				if (++InNum == 10)//如果10个人都进入了则开始考试
				{
					StartExam = true;
				}
			}
			if (StartExam)
			{
				Sleep(rand() %time1);//随机交卷
				FinishExam = true;
			}
			while (FinishExam && !GoOut)//离开考场
			{
				WaitForSingleObject(SdoorEmpty, INFINITE);
				GoOut = true;
				nend = 3;
			
				endque.push(nend);
				Sleep(rand() % time1);
				ReleaseSemaphore(SdoorEmpty, 1, NULL);
				if (++outnum == 9)//如果9个人都进入了则开始考试
				{
					finish = true;
					ExitThread(3);
				}
			}
		}

		return 0;
	}

	DWORD WINAPI student4(LPVOID param)
	{
		HDC hdc = (HDC)param;
		bool isPassed = false;//判断是否进入教室
		bool FinishExam = false;//判断是否考完
		bool GoOut = false;//判断是否离开教室
		while (1)
		{
			Sleep(rand() % 1000);
			while (!isPassed)
			{
				WaitForSingleObject(SdoorEmpty, INFINITE);
				isPassed = true;
				nstart = 4;
			
				startque.push(nstart);
				Sleep(rand() % time1);
				ReleaseSemaphore(SdoorEmpty, 1, NULL);

				if (++InNum == 10)//如果10个人都进入了则开始考试
				{
					StartExam = true;
				}
			}
			if (StartExam)
			{
				Sleep(rand() % time1);//随机交卷
				FinishExam = true;
			}
			while (FinishExam && !GoOut)//离开考场
			{
				WaitForSingleObject(SdoorEmpty, INFINITE);
				GoOut = true;
				nend = 4;
			
				endque.push(nend);
				Sleep(rand() % time1);
				ReleaseSemaphore(SdoorEmpty, 1, NULL);
				if (++outnum == 9)//如果9个人都进入了则开始考试
				{
					finish = true;
					ExitThread(4);
				}
			}
		}

		return 0;
	}

	DWORD WINAPI student5(LPVOID param)
	{
		HDC hdc = (HDC)param;
		bool isPassed = false;//判断是否进入教室
		bool FinishExam = false;//判断是否考完
		bool GoOut = false;//判断是否离开教室
		while (1)
		{
			Sleep(rand() % 1000);
			while (!isPassed)
			{
				WaitForSingleObject(SdoorEmpty, INFINITE);
				isPassed = true;
				nstart = 5;
			
				startque.push(nstart);
				Sleep(rand() % time1);
				ReleaseSemaphore(SdoorEmpty, 1, NULL);

				if (++InNum == 10)//如果10个人都进入了则开始考试
				{
					StartExam = true;
				}
			}
			if (StartExam)
			{
				Sleep(rand() % time1);//随机交卷
				FinishExam = true;
			}
			while (FinishExam && !GoOut)//离开考场
			{
				WaitForSingleObject(SdoorEmpty, INFINITE);
				GoOut = true;
				nend = 5;
			
				endque.push(nend);
				Sleep(rand() % time1);
				ReleaseSemaphore(SdoorEmpty, 1, NULL);
				if (++outnum == 9)//如果9个人都进入了则开始考试
				{
					finish = true;
					ExitThread(5);
				}
			}
		}

		return 0;
	}

	DWORD WINAPI student6(LPVOID param)
	{
		HDC hdc = (HDC)param;
		bool isPassed = false;//判断是否进入教室
		bool FinishExam = false;//判断是否考完
		bool GoOut = false;//判断是否离开教室
		while (1)
		{
			Sleep(rand() % 1000);
			while (!isPassed)
			{
				WaitForSingleObject(SdoorEmpty, INFINITE);
				isPassed = true;
				nstart = 6;
			
				startque.push(nstart);

				Sleep(rand() % time1);
				ReleaseSemaphore(SdoorEmpty, 1, NULL);

				if (++InNum == 10)//如果10个人都进入了则开始考试
				{
					StartExam = true;
				}
			}
			if (StartExam)
			{
				Sleep(rand() % time1);//随机交卷
				FinishExam = true;
			}
			while (FinishExam && !GoOut)//离开考场
			{
				WaitForSingleObject(SdoorEmpty, INFINITE);
				GoOut = true;
				nend = 6;
				
				endque.push(nend);
				Sleep(rand() % time1);
				ReleaseSemaphore(SdoorEmpty, 1, NULL);
				if (++outnum == 9)//如果9个人都进入了则开始考试
				{
					finish = true;
					ExitThread(6);
				}
			}
		}

		return 0;
	}

	DWORD WINAPI student7(LPVOID param)
	{
		HDC hdc = (HDC)param;
		bool isPassed = false;//判断是否进入教室
		bool FinishExam = false;//判断是否考完
		bool GoOut = false;//判断是否离开教室
		while (1)
		{
			Sleep(rand() % 1000);
			while (!isPassed)
			{
				WaitForSingleObject(SdoorEmpty, INFINITE);
				isPassed = true;
				nstart = 7;
				
				startque.push(nstart);
				Sleep(rand() % time1);
				ReleaseSemaphore(SdoorEmpty, 1, NULL);

				if (++InNum == 10)//如果10个人都进入了则开始考试
				{
					StartExam = true;
				}
			}
			if (StartExam)
			{
				Sleep(rand() % time1);//随机交卷
				FinishExam = true;
			}
			while (FinishExam && !GoOut)
			{
				WaitForSingleObject(SdoorEmpty, INFINITE);
				GoOut = true;
				nend = 7;
				
				endque.push(nend);
				Sleep(rand() % time1);
				ReleaseSemaphore(SdoorEmpty, 1, NULL);
				if (++outnum == 9)//如果9个人都进入了则开始考试
				{
					finish = true;
					ExitThread(7);
				}
			}
		}
		return 0;
	}

	DWORD WINAPI student8(LPVOID param)
	{
		HDC hdc = (HDC)param;
		bool isPassed = false;//判断是否进入教室
		bool FinishExam = false;//判断是否考完
		bool GoOut = false;//判断是否离开教室
		while (1)
		{
			Sleep(rand() % 1000);
			while (!isPassed)
			{
				WaitForSingleObject(SdoorEmpty, INFINITE);
				isPassed = true;
				nstart = 8;
				
				startque.push(nstart);
				Sleep(rand() % time1);
				ReleaseSemaphore(SdoorEmpty, 1, NULL);

				if (++InNum == 10)//如果10个人都进入了则开始考试
				{
					StartExam = true;
				}
			}
			if (StartExam)
			{
				Sleep(rand() % time1);//随机交卷
				FinishExam = true;
			}
			while (FinishExam && !GoOut)
			{
				WaitForSingleObject(SdoorEmpty, INFINITE);
				GoOut = true;
				nend = 8;
			
				endque.push(nend);
				Sleep(rand() % time1);
				ReleaseSemaphore(SdoorEmpty, 1, NULL);
				if (++outnum == 9)//如果9个人都进入了则开始考试
				{
					finish = true;
					ExitThread(8);
				}
			}
		}
		return 0;
	}



	DWORD WINAPI student9(LPVOID param)
	{
		HDC hdc = (HDC)param;
		bool isPassed = false;//判断是否进入教室
		bool FinishExam = false;//判断是否考完
		bool GoOut = false;//判断是否离开教室
		while (1)
		{
			Sleep(rand() % 1000);
			while (!isPassed)
			{
				WaitForSingleObject(SdoorEmpty, INFINITE);
				isPassed = true;
				nstart = 9;

				startque.push(nstart);
				Sleep(rand() % time1);
				ReleaseSemaphore(SdoorEmpty, 1, NULL);

				if (++InNum == 10)//如果10个人都进入了则开始考试
				{
					StartExam = true;
				}
			}
			if (StartExam)
			{
				Sleep(rand() % time1);//随机交卷
				FinishExam = true;
			}
			while (FinishExam && !GoOut)
			{
				WaitForSingleObject(SdoorEmpty, INFINITE);
				GoOut = true;
				nend = 9;

				endque.push(nend);
				Sleep(rand() % time1);
				ReleaseSemaphore(SdoorEmpty, 1, NULL);
				if (++outnum == 9)//如果9个人都进入了则开始考试
				{
					finish = true;
					ExitThread(9);
				}
			}
		}
		return 0;
	}
void CMFCApplication1Dlg::OnBnClickedButton1()
{
	GetDlgItem(SHOW)->SetWindowTextW(_T("before the exam"));
	examing = false;
	isbackque = false;
	 timer = 70;
	//StartExam = false;//判断是否开始考试
	//finish = false;
	 InNum = 0;//记录多少个人进入教室
	 outnum = 0;
	//绘图
	 nstart = -1;//记录哪个学生开始进入；
	 nend = -1;//记录哪个学生开始出场；
	 backque = -1;
	 IsFull1 = 0;
	 IsEmpty1 = 0;
	HDC hdc = GetDC()->GetSafeHdc();//获取窗口的hdc
	for (int i = 0; i < 10; i++)//初始化
	{
		for (int j = 0; j < 5; j++)
		{
			a[i][j] = 0;
		}
	}
	for (int i = 0; i < 10; i++)//初始化 调用即开始移动
	{
		b[i] = 1;
		b1[i] = -2;
	}

	SetTimer(0, timer, NULL);
	SdoorEmpty = CreateSemaphore(NULL, 1, 1, NULL);
	MyDrawer(hdc);//找出基础座位
	//开启线程
	HANDLE hTeacher = CreateThread(NULL, 0, teacher, hdc, 0, NULL);
	HANDLE hStudent1 = CreateThread(NULL, 0, student1, hdc, 0, NULL);
	HANDLE hStudent2 = CreateThread(NULL, 0, student2, hdc, 0, NULL);
	HANDLE hStudent3 = CreateThread(NULL, 0, student3, hdc, 0, NULL);
	HANDLE hStudent4 = CreateThread(NULL, 0, student4, hdc, 0, NULL);
	HANDLE hStudent5 = CreateThread(NULL, 0, student5, hdc, 0, NULL);
	HANDLE hStudent6 = CreateThread(NULL, 0, student6, hdc, 0, NULL);
	HANDLE hStudent7 = CreateThread(NULL, 0, student7, hdc, 0, NULL);
	HANDLE hStudent8 = CreateThread(NULL, 0, student8, hdc, 0, NULL);
	HANDLE hStudent9 = CreateThread(NULL, 0, student9, hdc, 0, NULL);
	
	
}


//void CMFCApplication1Dlg::OnLButtonDown(UINT nFlags, CPoint point)
//{
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//	//HDC hdc = GetDC()->GetSafeHdc();
//	//MyDrawer(hdc, point.x, point.y);
//
//}


void CMFCApplication1Dlg::OnTimer(UINT_PTR nIDEvent)
{
	
	// TODO: 在此添加消息处理程序代码和/或调用默认值
  	if (nIDEvent == 0&&finish&&StartExam&&outnum>=-1)
	{
		int h = 0;//0的时候横向移动 1的时候纵向移动
		if (!isbackque)
		{
			backque = startque.back();
			isbackque = true;
		}
		HDC hdc = GetDC()->GetSafeHdc();
		//获取窗口尺寸
		MyDrawer(hdc);//找出基础座位
		if (InNum != 0)//如果还有人没进去
		{
			nnstart = startque.front();//按照已经进行竞争 且根据竞争结果排好的顺序 执行动画
			MyDrawer1(hdc, stepX, stepY, nnstart);
			b1[nnstart] = -3;
			if (b[nnstart] == 6)
			{
				b1[nnstart] = nnstart;
				InNum--;
				startque.pop();
			}
		}
		if (InNum == 0 && outnum > 0 && (!examing))
		{
		
			for (int j = 0; j < 5000; j++)
			{
				GetDlgItem(SHOW)->SetWindowTextW(_T("exam taking"));
				for (int i = 0; i < 10; i++)
				{
					if (i != backque)
						MyDrawer1(hdc, stepX, stepY, b1[i]);
					else
						continue;
				}
			}

			examing = true;
		}
		if (examing)
		{
			GetDlgItem(SHOW)->SetWindowTextW(_T("exam is over"));
		}
		if (InNum == 0 && outnum > 0)//如果所有的人都已经进来且不是所有的学生都离开
		{
			nnend = endque.front();//按照已经进行竞争 且根据竞争结果排好的顺序 执行动画
			MyDrawer2(hdc, stepX, stepY, nnend);
			b1[nnend] = -1;//改变状态防止循环执行mydrawer1

			if (b[nnend] == 0)
			{
				outnum--;
				if(!endque.empty())
				endque.pop();
			}
		}
		if (InNum == 0 && outnum == 0)//如果所有的人都已经进来且不是所有的学生都离开,老师离开
		{
			nnend = 0;//按照已经进行竞争 且根据竞争结果排好的顺序 执行动画
			MyDrawer2(hdc, stepX, stepY, nnend);
			b1[nnend] = -1;//改变状态防止循环执行mydrawer1
			if (b[nnend] == 0)
			{
				outnum--;
			}
		/*	if(outnum==0)
			{
				int a = 3;
			}*/
		}
		
		for (int i = 0; i < 10; i++)
		{
			if (b1[i] == i)
			{
				MyDrawer1(hdc, stepX, stepY, b1[i]);
			}
		}	


	}

}


void CMFCApplication1Dlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	exit(0);
}


void CMFCApplication1Dlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	if(timer>=10)
		timer = timer - 10;
	
	SetTimer(0, timer, NULL);
}


void CMFCApplication1Dlg::OnStnClickedShow()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CMFCApplication1Dlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	if (timer <= 150)
		timer = timer+10;

	SetTimer(0, timer, NULL);
}
