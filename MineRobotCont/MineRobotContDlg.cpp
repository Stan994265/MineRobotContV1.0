
// MineRobotContDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MineRobotCont.h"
#include "MineRobotContDlg.h"
#include "afxdialogex.h"
#include <Mmsystem.h>
#include "Common/UNetwork.h"
#include "SocketDefine.h"

#include <MMSystem.h>
#include <sstream>  
#pragma comment(lib, "Winmm.lib")
//////////////////////////laser////////////////////////////////////////////////////////////
#include "afxdialogex.h"
#include "iostream"
//#include "Baocun.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <math.h>

///////////////////--------////MOTOR////------////////////////////////////////////////////////
#define USE_CAN

#include <cstdio>
#include <cstdlib>

#include "CML.h"

#if defined( USE_CAN )
#include "can_ixxat_v3.h"

#endif
////////////////////////////////
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define USE_CAN

#include <cstdio>
#include <cstdlib>

#include "CML.h"

#if defined( USE_CAN )
#include "can_ixxat_v3.h"

#endif
///////////////////////////////////////
int mode;
double m_v;
double m_lf;
 int count=0;
 int angle_init; 
 double actual_pos_init[3];
////////////////////////////////
 using CML::uunit;
using CML::Point;
using CML::Amp;
using CML::HALT_MODE;
using CML::QUICK_STOP_MODE;
using CML::int32;
using CML::int16;
using CML::IxxatCANV3;
using CML:: CanOpen;
  using CML::Linkage;
  using CML::ProfileConfigVel;
  using CML::ProfileConfigTrap;
  using CML::AmpSettings;
  using CML::Error;
  using CML::HomeConfig;
  using CML::MtrInfo;
  using CML::CHM_NONE;
 using CML::CHM_HARDSTOP_ONDX_POS;
  using CML::HALT_ABRUPT;

//CML_NAMESPACE_USE();
#define AMPCT 6
Point<6> pos;
HALT_MODE haltmode;
QUICK_STOP_MODE qsmode;
int32 canBPS = 1000000;           
const char *canDevice = "CAN0";          
int16 canNodeID = 1;               
/////////////////////////////// 
IxxatCANV3 hw( "CAN0" );    
 CanOpen net;
Linkage link;
ProfileConfigVel pcv;
ProfileConfigTrap pct;
 Amp amp[6];
 AmpSettings set;
 const Error *err ;
 HomeConfig hcfg;
 ///////////////////////////////////COMPASS////////////////////////////////////////////////////////////////////
 unsigned char ucComNo[2] ={0,0};
bool    m_ThreadRun;
int  BauteRate;
/////////////////////////////////////laser/////////////////////////////////////////////////////////////////////////
enum {
  Timeout = 800,
  LineLength = 16 + 64 + 1 + 1 + 1,
};

typedef struct {
  enum {
    MODL = 0,		//!< Sensor Model
    DMIN,			//!< Min detection range [mm]
    DMAX,			//!< Man detection range [mm]
    ARES,			//!< Angular resolution (division of 360degree)
    AMIN,			//!< Min Measurement step
    AMAX,			//!< Max Measurement step
    AFRT,			//!< Front Step 
    SCAN,			//!< Standard scan speed
  };
  std::string model;		//!< Obtained Sensor Model,  MODL
  long distance_min;		//!< Obtained DMIN 
  long distance_max;		//!< Obtained DMAX 
  int area_total;		//!< Obtained ARES 
  int area_min;			//!< Obtained AMIN 
  int area_max;			//!< Obtained AMAX 
  int area_front;		//!< Obtained AFRT 
  int scan_rpm;			//!< Obtained SCAN 

  int first;			//!< Scan Start position
  int last;			//!< Scan end position
  int max_size;			//!< Max. size of data
  long last_timestamp; //!< Time stamp of the last obtained data
} urg_state_t;

static HANDLE HComm = INVALID_HANDLE_VALUE;
static char* ErrorMessage = "no error.";
static long date[800];
static long laser[500];
///////////////////////////////////////////////////////////////////////////////////////////////////////

CvCapture* capture;
CRect rect;
CRect rect1;
CDC *pDC;
HDC hDC;
CWnd *pwnd;

//CvVideoWriter* writer = 0;

CMineRobotContDlg *g_pContDlgData;

UINT PTimerId=-1;
void PASCAL TimerProcPredictDis(UINT wTimerID, UINT msg,DWORD dwUser,DWORD dwl,DWORD dw2);

UINT VTimerId=-1;
void PASCAL TimerProcVedio(UINT wTimerID, UINT msg,DWORD dwUser,DWORD dwl,DWORD dw2);


UINT RTimerId=-1;
void PASCAL TimerProcReceiveSes(UINT wTimerID, UINT msg,DWORD dwUser,DWORD dwl,DWORD dw2);
///////////////////////////////////////////MOTOR////////////////////////////////////////////////////
UINT MTimerId=-1;
void PASCAL TimerProcmove(UINT wTimerID, UINT msg,DWORD dwUser,DWORD dwl,DWORD dw2);
//////////////////////////////////
UINT MCTimerId=-1;
void PASCAL TimerProccmove(UINT wTimerID, UINT msg,DWORD dwUser,DWORD dwl,DWORD dw2);

CWinThread* g_pVedioDisplayThread=NULL;

UINT _VedioDisplayThread(void *param);

CWinThread* g_pSendCommandThread=NULL;

UINT _SendCommandThread(void *param);




//#define blocksize 28800
//#define blocksize 9600
#define blocksize 1440
//#define blocksize 720

struct recvbuf//包格式  
{  
	char buf[blocksize];//存放数据的变量  
	int flag;//标志  

};  
struct recvbuf data;  



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


// CMineRobotContDlg 对话框




CMineRobotContDlg::CMineRobotContDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMineRobotContDlg::IDD, pParent)
	, m_TargetIP(_T(""))
	, m_TargetPort(0)
	////////////////////////////////
	,m_c_arrayLength(500)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	g_pContDlgData=this;

	m_ImageRGB=NULL;
	m_ImageGray=NULL;

	m_bCntSensor = FALSE;

	m_nPCount=0; //预测仿真通讯记数

	//m_ImageGray = cvCreateImage(cvSize(640, 480),IPL_DEPTH_8U, 1);//我的摄像头是640x480的，如不同则自行修改 

	m_ImageRGB = cvCreateImage(cvSize(480, 360), 8, 3);//我的摄像头是640x480的，如不同则自行修改
	m_ImageGray = cvCreateImage(cvSize(480, 360), 8, 1);//我的摄像头是640x480的，如不同则自行修改

	m_TargetIP = _T("192.168.254.120");
	m_TargetPort = 10025;
	m_385 = 0.0;
	m_pitch = 0.0;
	m_roll = 0.0;
	m_yaw = 0.0;

	contral_v = 0.0;
	contral_w = 0.0;
	contral_r = 0.0;
	m_n1 = 0.0;
	m_n2 = 0.0;
}

void CMineRobotContDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_HOST_NAME, m_EditHostName);
	DDX_Control(pDX, IDC_EDIT2, m_EditHostIP);
	DDX_Text(pDX, IDC_EDIT_TARGET_IP, m_TargetIP);
	DDX_Text(pDX, IDC_EDIT_TARGET_PORT, m_TargetPort);
	DDV_MinMaxInt(pDX, m_TargetPort, 0, 99999);
	DDX_Control(pDX, IDC_SLIDER1, m_ctrlSlider);
	DDX_Control(pDX, IDC_SLIDER_LF, m_ctrlSlider_Lf);
	DDX_Control(pDX, IDC_COMBO_BAUDRATE, m_baudRate_cb);
	DDX_Control(pDX, IDC_COMBO_COMLIST, m_comList);
	DDX_Control(pDX, IDC_LIST2, m_list);
	DDX_Text(pDX, IDC_EDIT_385, m_385);
	DDX_Text(pDX, IDC_EDIT_PITCH, m_pitch);
	DDX_Text(pDX, IDC_EDIT_ROLL, m_roll);
	DDX_Text(pDX, IDC_EDIT_YAW, m_yaw);
	DDX_Control(pDX, IDC_CHECK_LF, m_llf);
	DDX_Control(pDX, IDC_CHECK_LR, m_llr);
	DDX_Control(pDX, IDC_CHECK_RF, m_rrf);
	DDX_Control(pDX, IDC_CHECK_RR, m_rrr);
	DDX_Control(pDX, IDC_LIST_MOTORINFO, m_motor_info);
	DDX_Text(pDX, IDC_EDIT_V, contral_v);
	DDX_Text(pDX, IDC_EDIT_W, contral_w);
	DDX_Text(pDX, IDC_EDIT_R, contral_r);
	DDX_Control(pDX, IDC_CHECK_LM, m_lm);
	DDX_Control(pDX, IDC_CHECK_RM, m_rm);
	DDX_Text(pDX, IDC_EDIT_n1, m_n1);
	DDX_Text(pDX, IDC_EDIT_n2, m_n2);
}

BEGIN_MESSAGE_MAP(CMineRobotContDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_OPENCAMER, &CMineRobotContDlg::OnBnClickedOpencamer)
	ON_BN_CLICKED(IDC_CLOSECAMER, &CMineRobotContDlg::OnBnClickedClosecamer)
	ON_BN_CLICKED(IDC_SEND_VEDIO, &CMineRobotContDlg::OnClickedSendVedio)
	ON_BN_CLICKED(IDC_STOP_VEDIO, &CMineRobotContDlg::OnBnClickedStopVedio)
	ON_BN_CLICKED(IDC_SENSOR, &CMineRobotContDlg::OnBnClickedSensor)
	ON_BN_CLICKED(IDC_BUTTON3, &CMineRobotContDlg::OnBnClickedCommand)
	ON_BN_CLICKED(IDC_MOTOR_INIT, &CMineRobotContDlg::OnBnClickedMotorInit)
	ON_BN_CLICKED(IDC_MOTOR_FORWARD, &CMineRobotContDlg::OnBnClickedMotorForward)
	ON_BN_CLICKED(IDC_MOTOR_REAR, &CMineRobotContDlg::OnBnClickedMotorRear)
	ON_BN_CLICKED(IDC_MOTOR_LEFT, &CMineRobotContDlg::OnBnClickedMotorLeft)
	ON_BN_CLICKED(IDC_MOTOR_RIGHT, &CMineRobotContDlg::OnBnClickedMotorRight)
	ON_BN_CLICKED(IDC_START_MTIMER, &CMineRobotContDlg::OnBnClickedStartMtimer)
	ON_BN_CLICKED(IDC_STOP_MTIMER, &CMineRobotContDlg::OnBnClickedStopMtimer)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_CVEL, &CMineRobotContDlg::OnNMCustomdrawSliderCvel)
	ON_BN_CLICKED(IDC_BUTTON_OPENCOM, &CMineRobotContDlg::OnBnClickedButtonOpencom)
	ON_BN_CLICKED(IDC_BUTTON_CLOSECOM, &CMineRobotContDlg::OnBnClickedButtonClosecom)
	ON_BN_CLICKED(IDC_BUTTON_CALIBRATION, &CMineRobotContDlg::OnBnClickedButtonCalibration)
	ON_BN_CLICKED(IDC_BUTTON_ENDCALI, &CMineRobotContDlg::OnBnClickedButtonEndcali)
	ON_BN_CLICKED(IDC_BUTTON_OPENLASER, &CMineRobotContDlg::OnBnClickedButtonOpenlaser)
	ON_BN_CLICKED(IDC_BUTTON_CLOSELASER, &CMineRobotContDlg::OnBnClickedButtonCloselaser)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_GTG, &CMineRobotContDlg::OnBnClickedButtonGtg)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_LF, &CMineRobotContDlg::OnNMCustomdrawSliderLf)
	
	
	ON_BN_CLICKED(IDC_GTG_STOP, &CMineRobotContDlg::OnBnClickedGtgStop)
	ON_BN_CLICKED(IDC_BUTTON_PICTURE_START, &CMineRobotContDlg::OnBnClickedButtonPictureStart)
	ON_BN_CLICKED(IDC_BUTTON_PICTURE_STOP, &CMineRobotContDlg::OnBnClickedButtonPictureStop)
	ON_BN_CLICKED(IDC_BUTTON_Cstrat, &CMineRobotContDlg::OnBnClickedButtonCstrat)
	ON_BN_CLICKED(IDC_BUTTON_Cstop, &CMineRobotContDlg::OnBnClickedButtonCstop)
END_MESSAGE_MAP()


// CMineRobotContDlg 消息处理程序

BOOL CMineRobotContDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
///////////////////////////////////
	 m_ctrlSlider.SetRange(0,5000);
	 m_ctrlSlider.SetPos(0); 
	 m_ctrlSlider_Lf.SetRange(0,270);
	 m_ctrlSlider_Lf.SetPos(0); 
//////////////////////////////////
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
///////////////////////////////////////////CAMERA/////////////////////////////////////////
	pwnd = GetDlgItem(IDC_ShowImage);
	//pwnd->MoveWindow(35,30,352,288);
	pDC =pwnd->GetDC();
	hDC= pDC->GetSafeHdc();
	pwnd->GetClientRect(&rect1);

	//
	char hostName[100];
	char hostIP[50];
	if (UNetwork::GetHostInfo(hostIP, hostName)==TRUE)
	{
		m_EditHostName.SetWindowText(hostName);
		m_EditHostIP.SetWindowText(hostIP);
	}

/*	mUDPManager.SetLocalPort(10026);
	if (mUDPManager.CreateReceiver())
	{
		mUDPManager.StartReceiving();
	}*/

	if (mUDPManager.CreateSender()==FALSE)
	{
		AfxMessageBox("error socket init");
	}
	////////////COMPASS/////////////////////////////////////////////
	GetSystemPorts();

	m_baudRate_cb.AddString("9600");

	m_comList.SetCurSel(0);
	m_baudRate_cb.SetCurSel(0);
	//////////////////PICTURE///////////////////////////////////////////////////////////////////////////////////
	GetDlgItem(IDC_STATIC_Tab)->GetWindowRect(rect);
	GetDlgItem(IDC_STATIC_Tab)->ShowWindow(SW_HIDE);
	ScreenToClient(rect);
	m_tab.Create(CMFCTabCtrl::STYLE_3D_ONENOTE,//控件样式，如最上面所演示 
		rect,//控件区域
		this,//控件的父窗口指针
		1,//控件ID
		CMFCTabCtrl::LOCATION_TOP);//标签位置	
	m_tab.AutoSizeWindow(TRUE);//可以让对话框随tab显示区域而缩放，同时出发对话框的OnSize消息

	m_TSChartCtrl.Create(&m_tab,rect,1);
	m_HSChartCtrl.Create(&m_tab,rect,2);
	CChartAxis *pAxis = NULL;
	CChartAxis *pTAxis = NULL;
	//初始化坐标
	pAxis = m_HSChartCtrl.CreateStandardAxis(CChartCtrl::BottomAxis);
	pAxis->SetAutomatic(true);
	pAxis = m_HSChartCtrl.CreateStandardAxis(CChartCtrl::LeftAxis);
	pAxis->SetAutomatic(true);
	pTAxis = m_TSChartCtrl.CreateStandardAxis(CChartCtrl::BottomAxis);
	pTAxis->SetAutomatic(true);
	pTAxis = m_TSChartCtrl.CreateStandardAxis(CChartCtrl::LeftAxis);
	pTAxis->SetAutomatic(true);
	m_pHLineSerie0 = m_HSChartCtrl.CreateLineSerie();
	m_pHLineSerie1 = m_HSChartCtrl.CreateLineSerie();
	m_pTLineSerie0 = m_TSChartCtrl.CreateLineSerie();
	m_pTLineSerie1 = m_TSChartCtrl.CreateLineSerie();

	m_pHLineSerie0->SetName(_T("LeftMotor"));
	m_pHLineSerie1->SetName(_T("RightMotor"));

	m_pHLineSerie0->SetColor(RGB(0, 0,255));
	m_pHLineSerie1->SetColor(RGB(255, 0, 0));

	m_pTLineSerie0->SetName(_T("LeftMotor"));
	m_pTLineSerie1->SetName(_T("RightMotor"));
	
	m_pTLineSerie0->SetColor(RGB(0, 0,255));
	m_pTLineSerie1->SetColor(RGB(255, 0, 0));


	m_tab.AddTab(&m_TSChartCtrl,_T("ActualPositipn"));
	m_tab.AddTab(&m_HSChartCtrl,_T("ActualVelocity"));
	m_tab.SetActiveTab(0);//设置显示第一页；     
	m_tab.ShowWindow(SW_SHOWNORMAL);
	CRect TabBRect,TabTRect;
	m_tab.GetWindowRect(rect);
	ScreenToClient(rect);
	m_tab.GetTabArea(TabTRect,TabBRect);
	rect.top+=TabTRect.Height();


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMineRobotContDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMineRobotContDlg::OnPaint()
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
HCURSOR CMineRobotContDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMineRobotContDlg::OnBnClickedOpencamer()
{
	// TODO: 在此添加控件通知处理程序代码

	if(!capture)
	{
		capture = cvCaptureFromCAM(0);
		//AfxMessageBox(_T("OK"));
	}

	if (!capture)
	{
		AfxMessageBox(_T("无法打开摄像头"));
		return;
	}

	//writer=cvCreateVideoWriter("MyVideo.avi",CV_FOURCC('x','v','I','D'),25,cvSize(640,480)); //保存视频;

	//

	//

	if(g_pVedioDisplayThread == NULL)
		g_pVedioDisplayThread = ::AfxBeginThread(_VedioDisplayThread,this);
	else
	{
		DWORD code;
		GetExitCodeThread(g_pVedioDisplayThread,&code);
		if(code == STILL_ACTIVE)
			TerminateThread(g_pVedioDisplayThread,0);

		g_pVedioDisplayThread = NULL;
	}

}


UINT _VedioDisplayThread(LPVOID param)
{
	while(TRUE)
	{
		g_pContDlgData->VedioDisplay();

	}
	
	return 0;
}


BOOL g_bSendVedioFlg=FALSE;


void PASCAL TimerProcVedio(UINT wTimerID, UINT msg,DWORD dwUser,DWORD dwl,DWORD dw2)
{
	//	CTaskPlanDlg* pDlg = (CTaskPlanDlg*)dwUser;

//	g_pContDlgData->VedioDisplay();

	if (g_bSendVedioFlg)
	{
		g_pContDlgData->SendVedio();
	}

}


void CMineRobotContDlg::VedioDisplay()
{
	IplImage* m_Image=NULL;
	m_Image=cvQueryFrame(capture);

	cvResize(m_Image,m_ImageRGB,2);

//	m_ImageRGB->height;
//	m_ImageRGB->width;
	
//	cvCvtColor(m_ImageRGB, m_ImageGray, CV_BGR2GRAY); 

	CvvImage m_CvvImage;
	m_CvvImage.CopyOf(m_ImageRGB,1);	

    m_CvvImage.DrawToHDC(hDC, &rect1);

//    cvWriteFrame(writer,m_ImageRGB);

}


void CMineRobotContDlg::OnBnClickedClosecamer()
{
	// TODO: 在此添加控件通知处理程序代码
	

	//关闭线程

	DWORD code;
	GetExitCodeThread(g_pVedioDisplayThread,&code);
	if(code == STILL_ACTIVE)
		TerminateThread(g_pVedioDisplayThread,0);

	g_pVedioDisplayThread = NULL;


 //   cvReleaseCapture(&capture);
//	cvReleaseImage(&m_ImageRGB);
	//cvReleaseVideoWriter(&writer);
	
	//
	CDC MemDC; 
	CBitmap m_Bitmap1;
	m_Bitmap1.LoadBitmap(IDB_BITMAP1); 
	MemDC.CreateCompatibleDC(NULL);
	MemDC.SelectObject(&m_Bitmap1);
	pDC->StretchBlt(rect.left,rect.top,rect.Width(),rect.Height(),&MemDC,0,0,48,48,SRCCOPY); 
	
}


void CMineRobotContDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

    timeKillEvent(VTimerId);
	timeKillEvent(PTimerId);
	timeKillEvent(RTimerId);

	

	cvReleaseCapture(&capture);
//	cvReleaseVideoWriter(&writer);
//	cvReleaseImage( &m_ImageRGB); 
//	cvReleaseImage( &m_ImageGray); 
	
	DWORD code1;
	GetExitCodeThread(g_pVedioDisplayThread,&code1);
	if(code1 == STILL_ACTIVE)
		TerminateThread(g_pVedioDisplayThread,0);

	g_pVedioDisplayThread = NULL;


	DWORD code2;
	GetExitCodeThread(g_pSendCommandThread,&code2);
	if(code2 == STILL_ACTIVE)
		TerminateThread(g_pSendCommandThread,0);

	g_pVedioDisplayThread = NULL;



	CDialogEx::OnCancel();
}

void CMineRobotContDlg::OnClickedSendVedio()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(TRUE);
	mUDPManager.SetTargetIP(m_TargetIP);
	mUDPManager.SetTargetPort((WORD)m_TargetPort);

	g_bSendVedioFlg=TRUE;
	VTimerId = timeSetEvent(25,1,(LPTIMECALLBACK)TimerProcVedio,0,TIME_PERIODIC);

}


void CMineRobotContDlg::OnBnClickedStopVedio()
{
	// TODO: 在此添加控件通知处理程序代码
	g_bSendVedioFlg=FALSE;
}


void CMineRobotContDlg::SendVedio(void)
{

	if(m_ImageRGB)  
	{
	//	char* img=m_ImageRGB->imageData;                        //指向该帧的数据矩阵  
		char* img=m_ImageGray->imageData;  

		for(int i=0;i<120;i++)                    //480*360= 172800/120=1440
		{  
			for(int k=0;k<blocksize;k++)  
			{ 
				data.buf[k]=img[i*blocksize+k];       
			}  
			if(i==119)                         //标识一帧中最后一个数据包             
			{  
				data.flag=2;   
			}  
			else  
			{  
				data.flag=1;  
			}  
			mUDPManager.Send((char*)(&data),sizeof(data));
		}      


	}
}

BOOL g_bRepConFlg=FALSE;

void CMineRobotContDlg::OnBnClickedSensor()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bCntSensor = !m_bCntSensor;

	if(m_bCntSensor)
	{
		if (PTimerId==-1)
		{
			//UINT PREDICTIVE_Port = PREDICTIVE_PORT; 

			UINT PREDICTIVE_Port = 8001; 

			char *pPREDICTIVE_IP = TELEOPERATION_IP;

			int retConSer = m_ClientSocketPredictive.ConnectServer(pPREDICTIVE_IP,PREDICTIVE_Port);
			m_nPCount=0;

			if (retConSer==0)
			{
		//		PTimerId = timeSetEvent(40,1,(LPTIMECALLBACK)TimerProcPredictDis,0,TIME_PERIODIC);

				RTimerId = timeSetEvent(250,1,(LPTIMECALLBACK)TimerProcReceiveSes,0,TIME_PERIODIC);

				if (RTimerId==NULL)
				{
					//MessageBox("TimerId error");
				}
			}
		}


		g_bRepConFlg=TRUE;
		SetDlgItemText(IDC_SENSOR,"Disconnect");

	//	m_nPredictivStatus = 0;
	}
	else
	{
		SetDlgItemText(IDC_SENSOR,"Sensor Data");
		g_bRepConFlg=FALSE;
	}


}


 void PASCAL TimerProcPredictDis(UINT wTimerID, UINT msg,DWORD dwUser,DWORD dwl,DWORD dw2)
{
	//	CTaskPlanDlg* pDlg = (CTaskPlanDlg*)dwUser;
	//g_pContDlgData->SendSensorData();
 }


 void PASCAL TimerProcReceiveSes(UINT wTimerID, UINT msg,DWORD dwUser,DWORD dwl,DWORD dw2)
 {
	 //	CTaskPlanDlg* pDlg = (CTaskPlanDlg*)dwUser;
	 g_pContDlgData->SendSensorData();

 }

 void CMineRobotContDlg::SendSensorData(void)
 {
	 if (g_bRepConFlg)
	 {
		 m_nPCount++;

		 sprintf(cSendbufferp,"%d,%f,%f,%f,%f,%f,%f\n",&m_nPCount,odometer,angle_motor,-actual_pos[2],actual_pos[3],actual_pos[4],-actual_pos[5]);
		 //里程，角度，右前，左前，右后，左后
		// sprintf(cSendbufferp,"%d,%f\n",m_nPCount,actual_pos[2]);

		 if(m_nPCount >= 10000)
			 m_nPCount = 0;

		 CString strSendData;

		 strSendData.Format("%d，%f,%f,%f,%f,%f,%f\n",&m_nPCount,odometer,angle_motor,-actual_pos[2],actual_pos[3],actual_pos[4],-actual_pos[5]);
		// strSendData.Format("%d,%f\n",m_nPCount,actual_pos[2]);

		 SetDlgItemText(IDC_EDIT4,strSendData);

		 int retSendData= m_ClientSocketPredictive.SendData(cSendbufferp);
	 }

 }

 void CMineRobotContDlg::OnBnClickedCommand()
 {
	 // TODO: 在此添加控件通知处理程序代码

	 UINT COMMAND_Port = 8003; 

	 char *pCOMMAND_IP = TELEOPERATION_IP;

	 int retConSer = m_ClientSocketCommand.ConnectServer(pCOMMAND_IP,COMMAND_Port);

	 if(g_pSendCommandThread == NULL&&retConSer==0)
		 g_pSendCommandThread = ::AfxBeginThread(_SendCommandThread,this);

 }


 UINT _SendCommandThread(LPVOID param)
 {
	 while (TRUE)
	 {

		 g_pContDlgData->ReceiveData();

	 }
	 return 0;
 }


 void CMineRobotContDlg::ReceiveData(void)
 {
	 int ret1;
	 float a=0;
	 float b=0.0;

	 ret1=m_ClientSocketCommand.ReceiveData(buffcmd);

	 sscanf(buffcmd, "%f,%f\n",&a,&b);

	 if(a==9990)
	     mode=0;

	 if(a==9991)
		 mode=3;
	 if(a==9992)
		 mode=2;
	 if(a==9993)
		 mode=1;

	 m_v=b;
	 

	 CString strSendData;

	 strSendData.Format("%0.1f,%0.2f\n",a,b);

	 SetDlgItemText(IDC_EDIT_COMMAND,strSendData);

 }

 ////////////////////////////////////////////////////////MOTOR//////////////////////////////////////////////////////////////////////////
 void PASCAL TimerProcmove(UINT wTimerID, UINT msg,DWORD dwUser,DWORD dwl,DWORD dw2)
{
	
	switch(mode)
	{
	case 0:
		pcv.dir=0;
		pcv.vel=m_v;
		pcv.acc=20;
		pcv.dec=20;
		amp[0].DoMove(pcv);
		pcv.dir=-1;
		amp[1].DoMove(pcv);
		break;
	case 1:
		pcv.dir=-1;
		pcv.vel=m_v;
		pcv.acc=20;
		pcv.dec=20;
		amp[0].DoMove(pcv);
		pcv.dir=0;
		amp[1].DoMove(pcv);
		break;
	case 2:
		pcv.dir=-1;
		pcv.vel=m_v;
		pcv.acc=20;
		pcv.dec=20;
		amp[0].DoMove(pcv);
		pcv.dir=-1;
		amp[1].DoMove(pcv);
		break;
	case 3:
		pcv.dir=0;
		pcv.vel=m_v;
		pcv.acc=20;
		pcv.dec=20;
		amp[0].DoMove(pcv);
		pcv.dir=0;
		amp[1].DoMove(pcv);
		break;

	}
	/////////////////////////////////////////////////////////////////
	g_pContDlgData->BaiBiMove();
	g_pContDlgData->GetMotorPositionActual();
	

}

 void PASCAL TimerProccmove(UINT wTimerID, UINT msg,DWORD dwUser,DWORD dwl,DWORD dw2)
{
	

	
	g_pContDlgData->GetMotorPositionActual();
	g_pContDlgData->Contral_Move();

}


 void CMineRobotContDlg::Contral_Move(void)
 {
	  if(m_rm.GetCheck()==1)
	 {
		
	contral_v=GetDlgItemInt(IDC_EDIT_V);
	contral_r=GetDlgItemInt(IDC_EDIT_R);
	contral_w=GetDlgItemInt(IDC_EDIT_W);
      
		m_n1=(2*contral_v+contral_r*contral_w)/(4*3.1415926*55)*126*2;
		m_n2=(2*contral_v-contral_r*contral_w)/(4*3.1415926*55)*126*2;
        pcv.dir=0;
		pcv.vel=m_n1;
		pcv.acc=20;
		pcv.dec=20;
		amp[0].DoMove(pcv);
		pcv.dir=-1;
		pcv.vel=-m_n2;
		pcv.acc=20;
		pcv.dec=20;
		amp[1].DoMove(pcv);
	
	 }
	 if(m_lm.GetCheck()==1)
	 {
	
	contral_v=GetDlgItemInt(IDC_EDIT_V);
	contral_r=GetDlgItemInt(IDC_EDIT_R);
	contral_w=GetDlgItemInt(IDC_EDIT_W);
		
		m_n1=(2*contral_v+contral_r*contral_w)/(4*3.1415926*55)*126*2;
		m_n2=(2*contral_v-contral_r*contral_w)/(4*3.1415926*55)*126*2;
        pcv.dir=-1;
		pcv.vel=m_n1;
		pcv.acc=20;
		pcv.dec=20;
		amp[1].DoMove(pcv);
		pcv.dir=0;
		pcv.vel=-m_n2;
		pcv.acc=20;
		pcv.dec=20;
		amp[0].DoMove(pcv);
	 }
 }
 void CMineRobotContDlg::BaiBiMove(void)
{
	//CMineRobotContDlg mrcd;
	if(m_llf.GetCheck()==1)
	{
		pct.pos=m_lf*236/360*2;
		pct.vel=20;
		pct.acc=20;
		pct.dec=20;
		amp[3].DoMove(pct);
	}
	if(m_rrf.GetCheck()==1)
	{
		pct.pos=-m_lf*236/360*2;
		pct.vel=20;
		pct.acc=20;
		pct.dec=20;
		amp[2].DoMove(pct);
	}
	if(m_llr.GetCheck()==1)
	{
		pct.pos=-m_lf*236/360*2;
		pct.vel=20;
		pct.acc=20;
		pct.dec=20;
		amp[5].DoMove(pct);
	}
	if(m_rrr.GetCheck()==1)
	{
		pct.pos=m_lf*236/360*2;
		pct.vel=20;
		pct.acc=20;
		pct.dec=20;
		amp[4].DoMove(pct);
	}

	///////////////////////////////////////////////////////////////////////////////////
	
				
}

 void CMineRobotContDlg::GetMotorPositionActual()

{
	
	uunit p0;	
	uunit p1;
    uunit p2;
    uunit p3;
	uunit p4;
	uunit p5;
	amp[0].GetPositionActual(p0);
	amp[1].GetPositionActual(p1);
	amp[2].GetPositionActual(p2);
	amp[3].GetPositionActual(p3);
	amp[4].GetPositionActual(p4);
	amp[5].GetPositionActual(p5);
	actual_pos[0]=p0/126/2;
	actual_pos[1]=p1/126/2;
	/////////////////////////////////////////////////////////////////////////
	mpt0=actual_pos[0];
	mpt1=actual_pos[1];
	////////////////////////////////////////////////////////////////////////
	actual_pos[2]=p2/236*360/2;
	actual_pos[3]=p3/236*360/2;
	actual_pos[4]=p4/236*360/2;
	actual_pos[5]=p5/236*360/2;
	///////////////////////////////////////////////////
	int16 current_0;
	int16 current_1;
	amp[0].GetCurrentActual(current_0);
	amp[1].GetCurrentActual(current_1);
	current[0]=current_0/100.00;
	current[1]=current_1/100.00;
	///////////////////////////////////////////////////
	uunit m_v0;
	amp[0].GetVelocityActual(m_v0);
	velocity[0]=m_v0*60;
	mvh0=velocity[0];
	uunit m_v1;
	amp[1].GetVelocityActual(m_v1);
	velocity[1]=m_v1*60;
	mvh1=velocity[1];
	//////////////////////////////////////////////////
	double angle_pos=actual_pos[0]-actual_pos_init[0]+actual_pos[1]-actual_pos_init[1];
	angle_motor=angle_pos*10.5/27*360/3.281;
	odometer=(actual_pos[0]-actual_pos_init[0]-actual_pos[1]+actual_pos_init[1])*32.986695;
	CString str;
str.Format("Odometer%f Angle%f Current%f %f Pos%f %f %f %f %f %f VelocityActual%f %f",
						odometer,angle_motor,current[0],current[1],actual_pos[0],actual_pos[1],actual_pos[2],actual_pos[3],actual_pos[4],actual_pos[5],velocity[0],velocity[1]);
	if(m_motor_info.GetCount() > 3)
				{
					m_motor_info.DeleteString(0);
					m_motor_info.InsertString(3,str);
				}
				else
				{
					m_motor_info.AddString(str);

				}
	///////////////////////////////////////PICTURE///////////////////////////////////////////////
	int i=0;
	m_HightSpeedChartArray0[i]=mvh0;
	m_HightSpeedChartArray1[i]=-mvh1;
	m_TeeSpeedChartArray0[i]=mpt0;
	m_TeeSpeedChartArray0[i]=-mpt1;
	i++;
	if(i>500) i=0;
}


 void CMineRobotContDlg::OnBnClickedMotorInit()
 {
	  hw.SetBaud( canBPS );
 const Error *err = net.Open( hw ); 
   set.guardTime = 0;
   int i;
   for( i=0; i<6; i++ )
   {
      
      err = amp[i].Init( net, canNodeID+i, set );
    

      MtrInfo mtrInfo;
      err = amp[i].GetMtrInfo( mtrInfo );
     

      err = amp[i].SetCountsPerUnit( mtrInfo.ctsPerRev );
     
   }

   /*err = link.Init( 6, amp );
   if( err )
      printf( "Error: %%s\\n", err->toString() );
   else
      printf( "no error\\n" );*/
   hcfg.method  = CHM_HARDSTOP_ONDX_POS;
   hcfg.offset  = 0;
   for( i=0; i<AMPCT; i++ )
   {
      err = amp[i].GoHome( hcfg );   
   }
  //err = link.WaitMoveDone( -1);    
  AfxMessageBox("MotorInit");
 }


 void CMineRobotContDlg::OnBnClickedMotorForward()
 {
	mode=0;
 }


 void CMineRobotContDlg::OnBnClickedMotorRear()
 {
	 mode=1;
 }


 void CMineRobotContDlg::OnBnClickedMotorLeft()
 {
	  mode=2;
 }


 void CMineRobotContDlg::OnBnClickedMotorRight()
 {
	  mode=3;
 }


 void CMineRobotContDlg::OnBnClickedStartMtimer()
 {
	uunit p0;	
	amp[0].GetPositionActual(p0);
	actual_pos_init[0]=p0/126/2;
	uunit p1;	
	amp[1].GetPositionActual(p1);
	actual_pos_init[1]=p1/126/2;
	
	////////////////////////////////////////////////////
	
	////////////////////////////////////////////////////
	MTimerId = timeSetEvent(10,1,(LPTIMECALLBACK)TimerProcmove,0,TIME_PERIODIC);
 }


 void CMineRobotContDlg::OnBnClickedStopMtimer()
 {
	timeKillEvent(MTimerId);
    amp[0].SetHaltMode(HALT_ABRUPT);
	amp[0].GetHaltMode(haltmode);
	amp[1].SetHaltMode(HALT_ABRUPT);
	amp[1].GetHaltMode(haltmode);
	amp[2].SetHaltMode(HALT_ABRUPT);
	amp[2].GetHaltMode(haltmode);
	amp[3].SetHaltMode(HALT_ABRUPT);
	amp[3].GetHaltMode(haltmode);
	amp[4].SetHaltMode(HALT_ABRUPT);
	amp[4].GetHaltMode(haltmode);
	amp[5].SetHaltMode(HALT_ABRUPT);
	amp[5].GetHaltMode(haltmode);
	amp[0].HaltMove();
	amp[1].HaltMove();
	amp[2].HaltMove();
	amp[3].HaltMove();
	amp[4].HaltMove();
	amp[5].HaltMove();
	 /*hcfg.method  =CHM_HARDSTOP_ONDX_POS;//CHM_NONE;
		

   hcfg.offset  = 0;

  int i;

   for( i=0; i<AMPCT; i++ )
   {
     
      err = amp[i].GoHome( hcfg );
     
   }
    pos[0]=0;
	pos[1]=0;
	pos[2]=0;
	pos[3]=0;
	pos[4]=0;
    pos[5]=0;
	*/
 }


 void CMineRobotContDlg::OnNMCustomdrawSliderCvel(NMHDR *pNMHDR, LRESULT *pResult)
 {
	 LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	 *pResult = 0;
	 double nPos = m_ctrlSlider.GetPos(); 
	    CString mv;
       mv.Format("%f",nPos);
	   m_v=nPos/60;
	   SetDlgItemText(IDC_MOTOR_CVEL,mv);
 }

 void CMineRobotContDlg::OnNMCustomdrawSliderLf(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	double lf = m_ctrlSlider_Lf.GetPos(); 
	    CString LF;
       LF.Format("%f",lf);
	   m_lf=lf;
	   SetDlgItemText(IDC_EDIT_LF,LF);
}
 





 ///////////////////////////////////////////////////////COMPASS/////////////////////////////////////////////////////////////////

 void CMineRobotContDlg::OnBnClickedButtonOpencom()
 {
	 // TODO: 在此添加控件通知处理程序代码
	  int i = m_comList.GetCurSel();
	if (i==-1)
	{
		MessageBox("请选择串口!");
		return;
	}

	if (m_baudRate_cb.GetCurSel() == -1)
	{
		MessageBox("请选择波特率!");
		return;
	}




	CString  baud;

	m_baudRate_cb.GetLBText(m_baudRate_cb.GetCurSel(),baud);

	BauteRate = atoi(baud);

	if(ComSelect == ComList[i])
		return;

	while (m_ThreadRun)
	{
		::SetEvent(m_hStopEvent);
		Sleep(200);

		KillTimer(0); 			
	}

	ComSelect = ComList[i];

	ucComNo[0] = ComSelect;

	m_hStopEvent   = ::CreateEvent(NULL, FALSE, FALSE, NULL);

	unsigned int nThreadID;

	m_hThreadProc = (HANDLE)::_beginthreadex(NULL,
		10,
		ComProc,
		this,
		CREATE_SUSPENDED,
		&nThreadID
		);
	m_ThreadRun = 1;
	Sleep(10);
	ResumeThread( m_hThreadProc );

	bInit = 1;

	SetTimer(0,100,NULL);
 }



 void CMineRobotContDlg::OnBnClickedButtonClosecom()
 {
	 // TODO: 在此添加控件通知处理程序代码
	  while (m_ThreadRun)
	{
		::SetEvent(m_hStopEvent);
		Sleep(200);
 			
	}

	ComSelect = 0;

	ucComNo[0] = 0;

 }
 int CMineRobotContDlg::DecodeIMUData(unsigned char chrTemp[])
{
	unsigned char checksum = 0;

	short roll,pitch;

	unsigned short yaw;

	unsigned short temp;


	pitch = (short)chrTemp[6]*255 + chrTemp[7];

	roll = (short)chrTemp[8]*255 + chrTemp[9];


	yaw = (short)chrTemp[16]*255 + chrTemp[17];


	m_yaw = (double)yaw/100.0;

	m_pitch = (double)pitch/1000.0/3.14159*180;

	m_roll = (double)roll/1000.0/3.14159*180;

	//m_pitch = (double)pitch/100.0;
	//	m_roll = (double)roll/100.0;
	return 1;
}

 signed char OpenCom(void)
{
	static unsigned long ulNo=0;
	signed char cResult= 0;	

	do
	{
		cResult = OpenCOMDevice(ucComNo[0],BauteRate);
	}while(cResult!=0);

	return 0;
}

unsigned int __stdcall  CMineRobotContDlg::ComProc(void* lpParam)
{
	CMineRobotContDlg* pThis = (CMineRobotContDlg*)lpParam;

	char chrBuffer[98304];
	unsigned char chrTemp[1000];
	signed char cResult[2] = {0};
	unsigned short usLength=0,usRxLength=0;

	/*	ucComNo[0] = 2;*/
	OpenCom();


	while(WaitForSingleObject(pThis->m_hStopEvent,400) != WAIT_OBJECT_0)
	{
		usLength = CollectUARTData(ucComNo[0],chrBuffer);
		if (usLength>0)
		{
			usRxLength += usLength;
			while (usRxLength > 50)
			{
				memcpy(chrTemp,chrBuffer,usRxLength);
				if (!((chrTemp[0] == 0xAA) /*&&(chrTemp[1] == 0x52)*/))
				{
					for (int i = 1; i < usRxLength; i++) chrBuffer[i - 1] = chrBuffer[i];
					usRxLength--;
					continue;
				}
				int error =	pThis->DecodeIMUData(chrTemp);

				//pThis->m_list.DeleteString(0);
				CString str;
				if (usRxLength>20)
				{

					str.Format(" 0X%x  0X%x  0X%x  0X%x  0X%x  0X%x   0X%x  0X%x   0X%x   0X%x   0X%x  0X%x  0X%x  0X%x  0X%x  0X%x  0X%x  0X%x   0X%x   0X%x  %f",
						chrTemp[0],chrTemp[1],chrTemp[2],chrTemp[3],chrTemp[4],chrTemp[5],chrTemp[6],chrTemp[7],chrTemp[8],chrTemp[9],chrTemp[10],chrTemp[11],
						chrTemp[12],chrTemp[13],chrTemp[14],chrTemp[15],chrTemp[16],chrTemp[17],chrTemp[18],chrTemp[19],pThis->m_yaw);
				}
				
				if(pThis->m_list.GetCount() > 4)
				{
					pThis->m_list.DeleteString(0);
					pThis->m_list.InsertString(4,str);
				}
				else
				{
					pThis->m_list.AddString(str);

				}




				


				for (int i = 20; i < usRxLength; i++) chrBuffer[i - 20] = chrBuffer[i];
				usRxLength -= 20;
			}
		}

	}

	CloseCOMDevice();

	m_ThreadRun = 0;

	return 0;
}

void CMineRobotContDlg::GetSystemPorts()
{ 
	HANDLE hCom;
	int  i,num,k;
	CString str;

	num = 0;
	for (i = 1;i <= 32;i++)
	{
		str.Format("\\\\.\\COM%d",i);
		hCom = CreateFile(str,0,0,0,
			OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
		if(INVALID_HANDLE_VALUE != hCom)
		{
			CloseHandle(hCom);
			str = str.Mid(4);

			ComList[num] = i;
			m_comList.InsertString(num,str);
			num++;

			
		}
	}

	ComNum = num;  //记录可用串口数目
}



void CMineRobotContDlg::OnBnClickedButtonCalibration()
{
	if (m_ThreadRun)
	{
		char buff[8];

		buff[0] = 0xd0;

		SendUARTMessageLength(ucComNo[0],buff,1);

	}
}


void CMineRobotContDlg::OnBnClickedButtonEndcali()
{
		if (m_ThreadRun)
	{
		char buff[8];

		buff[0] = 0xd1;

		SendUARTMessageLength(ucComNo[0],buff,1);

	}
}


///////////////////////////////////////////////////LASER///////////////////////////////////////////////////////////////////

static int com_connect(const char* device, long baudrate) 
{
	
	char adjust_device[16];
	sprintf_s(adjust_device, "\\\\.\\%s", device);
	HComm = CreateFile(adjust_device, GENERIC_READ | GENERIC_WRITE, 0,
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	
	if (HComm == INVALID_HANDLE_VALUE) 
	{
		return -1;
	}
	
	// Baud Rate setting
	// !!! Not done
	
	return true;
}

static void com_disconnect(void)
{
	if (HComm != INVALID_HANDLE_VALUE) 
	{
		CloseHandle(HComm);
		HComm = INVALID_HANDLE_VALUE;
	}
}


static int com_send(const char* data, int size)
{
	DWORD n;
	WriteFile(HComm, data, size, &n, NULL);
	return n;
}


static int com_recv(char* data, int max_size, int timeout) 
{
	int filled = 0;
	int readable_size = 0;
	
	// Reading of Concerned data.
	do {
		DWORD dwErrors;
		COMSTAT ComStat;
		ClearCommError(HComm, &dwErrors, &ComStat);
		readable_size = (int)ComStat.cbInQue;
		int read_n = (readable_size > max_size) ? max_size : readable_size;
		
		DWORD n;
		ReadFile(HComm, &data[filled], read_n, &n, NULL);
		filled += n;
		readable_size -= n;
		
		if (filled >= max_size) 
		{
			return filled;
		}
	} while (readable_size > 0);	
	if (timeout > 0)
	{
		// Read data using time out
		COMMTIMEOUTS pcto;
		GetCommTimeouts(HComm, &pcto);
		pcto.ReadIntervalTimeout = 0;
		pcto.ReadTotalTimeoutMultiplier = 0;
		pcto.ReadTotalTimeoutConstant = timeout;
		SetCommTimeouts(HComm, &pcto);
		//Read data one charaters each
		DWORD n;
		while (1) 
		{
			ReadFile(HComm, &data[filled], 1, &n, NULL);
			if (n < 1) 
			{
				return filled;
			}
			filled += n;
			if (filled >= max_size)
			{
				return filled;
			}
		}
	}
	return filled;
}


// Send data(Commands) to URG 
static int urg_sendTag(const char* tag) 
{	
	char send_message[LineLength];
	sprintf_s(send_message, "%s\n", tag);
	int send_size = strlen(send_message);
	com_send(send_message, send_size);	
	return send_size;
}


// Read data (Reply) from URG until the termination 
static int urg_readLine(char *buffer) 
{	
	int i;
	for (i = 0; i < LineLength -1; ++i) 
	{
		char recv_ch;
		int n = com_recv(&recv_ch, 1, Timeout);
		if (n <= 0) 
		{
			if (i == 0) 
			{
				return -1;		// timeout
			}
			break;
		}
		if ((recv_ch == '\r') || (recv_ch == '\n'))
		{
			break;
		}
		buffer[i] = recv_ch;
	}
	buffer[i] = '\0';	
	return i;
}


// Send data (Commands) to URG and wait for reply
static int urg_sendMessage(const char* command, int timeout, int* recv_n) 
{
	int send_size = urg_sendTag(command);
	int recv_size = send_size + 2 + 1 + 2;
	char buffer[LineLength];	
	int n = com_recv(buffer, recv_size, timeout);
	*recv_n = n;	
	if (n < recv_size) 
	{
		// When received size not matched
		return -1;
	}	
	if (strncmp(buffer, command, send_size -1))
	{
		// When command not matched
		return -1;
	}	
	// !!! If possible do calculate check-sum to verify data	
	// Convert the received data to Hex and return data.
	char reply_str[3] = "00";
	reply_str[0] = buffer[send_size];
	reply_str[1] = buffer[send_size + 1];
	return strtol(reply_str, NULL, 16);
}


// Read URG parameters
static int urg_getParameters(urg_state_t* state) 
{
	// Parameter read and prcessing (use)
	urg_sendTag("PP");
	char buffer[LineLength];
	int line_index = 0;
	enum {
		    TagReply = 0,
			DataReply,
			Other,
	};
	int line_length;
	for (; (line_length = urg_readLine(buffer)) > 0; ++line_index) 
	{		
		if (line_index == Other + urg_state_t::MODL)
		{
			buffer[line_length - 2] = '\0';
			state->model = &buffer[5];
		} 
		else if (line_index == Other + urg_state_t::DMIN) 
		{
			state->distance_min = atoi(&buffer[5]);
		} 
		else if (line_index == Other + urg_state_t::DMAX) 
		{
			state->distance_max = atoi(&buffer[5]);
		} 
		else if (line_index == Other + urg_state_t::ARES) 
		{
			state->area_total = atoi(&buffer[5]);
		}
		else if (line_index == Other + urg_state_t::AMIN) 
		{
			state->area_min = atoi(&buffer[5]);
			state->first = state->area_min;
		}
		else if (line_index == Other + urg_state_t::AMAX) 
		{
			state->area_max = atoi(&buffer[5]);
			state->last = state->area_max;
		} 
		else if (line_index == Other + urg_state_t::AFRT)
		{
			state->area_front = atoi(&buffer[5]);
		}
		else if (line_index == Other + urg_state_t::SCAN)
		{
			state->scan_rpm = atoi(&buffer[5]);
		}
	}
	
	if (line_index <= Other + urg_state_t::SCAN)
	{
		return -1;
	}
	// Caluclate size of the data if necessary
	state->max_size = state->area_max +1;
	
	return 0;
}


// Process to connect with URG 
static int urg_connect(urg_state_t* state,
					   const char* port, const long baudrate)
{	
	if (com_connect(port, baudrate) < 0) 
	{
		ErrorMessage = "Cannot connect COM device.";
		return -1;
	}	
	const long try_baudrate[] = { 19200, 115200, 38400 };
	for (size_t i = 0; i < sizeof(try_baudrate)/sizeof(try_baudrate[0]); ++i) 
	{		
		// Change baud rate to detect the compatible baud rate with sensor
		// !!! com_changeBaudrate(try_baudrate[i]);		
		// Change to SCIP2.0 mode
		int recv_n = 0;
		urg_sendMessage("SCIP2.0", Timeout, &recv_n);
		if (recv_n <= 0) 
		{
			// If there is no reply it is considered as baud rate incompatibility, try with different baud rate
			continue;
		}		
		// Change the baud rate if not match the setting
		if (try_baudrate[i] != baudrate) 
		{
			// !!! urg_changeBaudrate(baudrate);
			// !!! The change will be implemented after 1 scan.
			// !!! Thus, Host or PC should wait to send the command			
			// !!! com_changeBaudrate(baudrate);
		}		
		// Read parameter
		if (urg_getParameters(state) < 0) 
		{
			ErrorMessage = "PP command fail.";
			return -1;
		}
		state->last_timestamp = 0;		
		// URG is detected
		return 0;
	}	
	// URG is not detected
	return -1;
}

// Disconnect URG 
static void urg_disconnect(void)
{
	com_disconnect();
}

// Data read using GD-Command
static int urg_captureByGD(const urg_state_t* state)
{
	char send_message[LineLength];
	sprintf_s(send_message, "GD%04d%04d%02d", state->first, state->last, 0);
	return urg_sendTag(send_message);
}


// Data read using MD-Command
static int urg_captureByMD(const urg_state_t* state, int capture_times) 
{	
	char send_message[LineLength];
	sprintf_s(send_message, "MD%04d%04d%02d%01d%02d",
		state->first, state->last, 0, 0, capture_times);
	return urg_sendTag(send_message);
}


// Decode 6 bit data from URG 
static long urg_decode(const char* data, int data_byte)
{
	long value = 0;
	for (int i = 0; i < data_byte; ++i) 
	{
		value <<= 6;
		value &= ~0x3f;
		value |= data[i] - 0x30;
	}
	return value;
}

// Receive URG distance data 
static int urg_addRecvData(const char buffer[], long data[], int* filled) 
{	
	static int remain_byte = 0;
	static char remain_data[3];
	const int data_byte = 3;
	const char* pre_p = buffer;
	const char* p = pre_p;	
	if (remain_byte > 0) 
	{
		memmove(&remain_data[remain_byte], buffer, data_byte - remain_byte);
		data[*filled] = urg_decode(remain_data, data_byte);
		++(*filled);
		pre_p = &buffer[data_byte - remain_byte];
		p = pre_p;
		remain_byte = 0;
	}	
	do {
		++p;
		if ((p - pre_p) >= static_cast<int>(data_byte)) 
		{
			data[*filled] = urg_decode(pre_p, data_byte);
			++(*filled);
			pre_p = p;
		}
	} while (*p != '\0');
	remain_byte = p - pre_p;
	memmove(remain_data, pre_p, remain_byte);
	return 0;
}

// Receive URG data
static int urg_receiveData(urg_state_t* state, long data[], size_t max_size) 
{
	int filled = 0;
	// Fill the positions upto first or min by 19 (non-measurement range)
	for (int j = state->first -1; j >= 0; --j) 
	{
		data[filled++] = 19;
	}	
	char message_type = 'M';
	char buffer[LineLength];
	int line_length;
	for (int i = 0;  (line_length = urg_readLine(buffer))>= 0;++i) 
	{
		// Verify the checksum
		if ((i >= 6) && (line_length == 0)) 
		{		
			// End of data receive
			for (size_t i = filled; i < max_size; ++i) 
			{
				// Fill the position upto data end by 19 (non-measurement range)
				data[filled++] = 19;
			}
			return filled;			
		}
		else if (i == 0) 
		{
			// Judge the message (Command) by first letter of receive data
			if ((buffer[0] != 'M') && (buffer[0] != 'G')) 
			{
				return -1;
			}
			message_type = buffer[0];			
		} 
		else if (! strncmp(buffer, "99b", 3)) 
		{
			// Detect "99b" and assume [time-stamp] and [data] to follow
			i = 4;
		} 
		else if ((i == 1) && (message_type == 'G')) 
		{
			i = 4;
		} 
		else if (i == 4) 
		{
			// "99b" Fixed
			if (strncmp(buffer, "99b", 3)) 
			{
				return -1;
			}
		} 
		else if (i == 5) 
		{
			state->last_timestamp = urg_decode(buffer, 4);
		} 
		else if (i >= 6) 
		{
			// Received Data
			if (line_length > (64 + 1)) 
			{
				line_length = (64 + 1);
			}
			buffer[line_length -1] = '\0';
			int ret = urg_addRecvData(buffer, data, &filled);
			if (ret < 0)
			{
				return ret;
			}
		}
	}
	return -1;
}

void CMineRobotContDlg::OnRead() ////////////////////////串口选择处////////////////////////
{
	// TODO: Add your control notification handler code here
	const char* com_port = "COM17";
	const long com_baudrate = 115200;
	// Connect with URG 
	urg_state_t urg_state;
	int ret = urg_connect(&urg_state, com_port, com_baudrate);
	if (ret < 0) 
	{
		getchar();
		exit(1);
	}	
	int max_size = urg_state.max_size;
	long* data = new long [max_size];
	enum { CaptureTimes = 1 };
	//////////////////////////////////////////////////////////////////////
	int recv_n = 0;
	urg_sendMessage("BM", Timeout, &recv_n);
	for (int m = 0; m < CaptureTimes; ++m) 
	{
		urg_captureByGD(&urg_state);
		int n = urg_receiveData(&urg_state, data, max_size);
		
	}
	
	////////////////////////////////////////////////////////////////////
	for(int k=44;k<726;k++)
	{
		if (data[k]>5000)
			data[k]=5000;
	
		if (data[k]<20)
		    data[k]=(2*data[k-1]+data[k+1])/2;	
		date[k]=data[k];
		
	}
	urg_disconnect();
	delete [] data;
	m_385=date[384];
laser[-90]=date[640];
laser[-60]=date[555];
laser[-45]=date[513];
laser[-30]=date[469];
laser[0]=date[384];
laser[30]=date[299];
laser[45]=date[256];
laser[60]=date[213];
laser[90]=date[127];
	///////////////////////////////////////////////////////////////////////////////////////////////
	/*for(int k=44,j=-120;k<726,j<121;k=k+681/240,j++)
	{
	 if (data[k]>5000)
			data[k]=5000;
	
		if (data[k]==0)
		    data[k]=(2*data[k-1]+data[k+1])/2;	
		//date[k]=data[k];
		laser[j]=data[k];
	}
	urg_disconnect();
	delete [] data;
	m_385=date[385];
	m_385=laser[0];*/

	
}


void CMineRobotContDlg::OnBnClickedButtonOpenlaser()
{
	//SetTimer(1,500,NULL);
	SetTimer(1,100,NULL);
}


void CMineRobotContDlg::OnBnClickedButtonCloselaser()
{
	
	KillTimer(1);
}


////////////////////////////////ONtimer///////////////////////////////////////////////////////////////////

void CMineRobotContDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	if(nIDEvent==0)
	{
	UpdateData(false);
	}
	
	if(nIDEvent==1)
	{
	OnRead();
    
	//Invalidate(true);
	UpdateData(false);
	
	}
	if(nIDEvent==2)
	{
///////////////////stop/////////////////////////////		
/*	int m385;
pcv.dir=0;
pcv.vel=45;
pcv.acc=30;
pcv.dec=30;
amp[0].DoMove(pcv);
pcv.dir=-1;
amp[1].DoMove(pcv);
m385=GetDlgItemInt(IDC_EDIT_385);

if (m385<1100)
{
count=count+1;
}
if (count>1) 
{
   KillTimer(2);
		amp[0].HaltMove();
		amp[1].HaltMove();
		count=0;

}
*////////////////////angle////////////////////////////////////////

/*  int angle=GetDlgItemInt(IDC_EDIT_YAW)-angle_init;
        pcv.dir=0;
		pcv.vel=25;
		pcv.acc=30;
		pcv.dec=30;
		amp[0].DoMove(pcv);
		pcv.dir=0;
		amp[1].DoMove(pcv);
		if (angle>30) 
{
        KillTimer(2);
		amp[0].HaltMove();
		amp[1].HaltMove();
		
}
*//////////////////////////////////////////////////////////////
/*laser[-90]=date[640];
laser[-60]=date[555];
laser[-30]=date[469];
laser[0]=date[384];
laser[30]=date[299];
laser[60]=date[213];
laser[90]=date[128];
*//////////////////////////////////////
		int limit=1000;
		
		if (laser[-90]>limit&&laser[-60]>limit&&laser[-30]>limit&&laser[0]>limit&&laser[30]>limit&&laser[60]>limit&&laser[90]>limit)
		{
		pcv.dir=0;
		pcv.vel=40;
		pcv.acc=20;
		pcv.dec=20;
		amp[0].DoMove(pcv);
		pcv.dir=-1;
		amp[1].DoMove(pcv);
		}
		if((laser[-90]>limit||laser[-60]>limit||laser[-45]>limit||laser[-30]>limit)&&(laser[30]<limit||laser[45]<limit||laser[60]<limit||laser[90]<limit))
		{
		pcv.dir=-1;
		pcv.vel=40;
		pcv.acc=20;
		pcv.dec=20;
		amp[0].DoMove(pcv);
		pcv.dir=-1;
		amp[1].DoMove(pcv);
		/*if ((laser[-90]<limit||laser[-60]<limit||laser[-45]<limit||laser[-30]<limit)&&(laser[30]>limit||laser[45]>limit||laser[60]>limit||laser[90]>limit))
		{
		count=count+1;
		}*/
		}
		if ((laser[-90]<limit||laser[-60]<limit||laser[-45]<limit||laser[-30]<limit)&&(laser[30]>limit||laser[45]>limit||laser[60]>limit||laser[90]>limit))
		{
		pcv.dir=0;
		pcv.vel=40;
		pcv.acc=20;
		pcv.dec=20;
		amp[0].DoMove(pcv);
		pcv.dir=0;
		amp[1].DoMove(pcv);
		/*if((laser[-90]>limit||laser[-60]>limit||laser[-45]>limit||laser[-30]>limit)&&(laser[30]<limit||laser[45]<limit||laser[60]<limit||laser[90]<limit))
		{
		count=count+1;
		}
		*/
		}
		if (laser[-30]<limit&&laser[0]<limit&&laser[30]<limit)
		{
		
		pcv.dir=-1;
		pcv.vel=40;
		pcv.acc=20;
		pcv.dec=20;
		amp[0].DoMove(pcv);
		pcv.dir=0;
		amp[1].DoMove(pcv);
		 Sleep(2000);
		 pcv.dir=0;
		pcv.vel=40;
		pcv.acc=20;
		pcv.dec=20;
		amp[0].DoMove(pcv);
		pcv.dir=0;
		amp[1].DoMove(pcv);
		Sleep(3500);
		
		}
		if ((laser[-90]<limit||laser[-60]<limit||laser[-45]<limit)&&laser[-30]>limit&&laser[0]>limit&&laser[30]>limit&&(laser[45]<limit||laser[60]<limit||laser[90]<limit))
		{
		pcv.dir=-1;
		pcv.vel=40;
		pcv.acc=20;
		pcv.dec=20;
		amp[0].DoMove(pcv);
		pcv.dir=0;
		amp[1].DoMove(pcv);
		 Sleep(2000);
		 pcv.dir=0;
		pcv.vel=40;
		pcv.acc=20;
		pcv.dec=20;
		amp[0].DoMove(pcv);
		pcv.dir=0;
		amp[1].DoMove(pcv);
		Sleep(3500);
		}
	/*	if(count>5)
		{
			
		pcv.dir=-1;
		pcv.vel=40;
		pcv.acc=40;
		pcv.dec=40;
		amp[0].DoMove(pcv);
		pcv.dir=0;
		amp[1].DoMove(pcv);
		 Sleep(4000);
		 pcv.dir=0;
		pcv.vel=40;
		pcv.acc=40;
		pcv.dec=40;
		amp[0].DoMove(pcv);
		pcv.dir=0;
		amp[1].DoMove(pcv);
		Sleep(4000);
		count=0;
		}
	*/	
	}	
	////////////////////////////////////////////PICTURE_TIMER///////////////////////////////////////////////////////////////
		if (3 == nIDEvent)
	{
		++m_count;

		/*m_pHLineSerie0->ClearSerie();
		m_pHLineSerie1->ClearSerie();
		
		m_pTLineSerie0->ClearSerie();
		m_pTLineSerie1->ClearSerie();
		
		LeftMoveArray(m_HightSpeedChartArray0,m_c_arrayLength,dData1);
		LeftMoveArray(m_HightSpeedChartArray1,m_c_arrayLength,randf(9.8,10));
		
		LeftMoveArray(m_TeeSpeedChartArray0,m_c_arrayLength,randf(-0.025,0.015));
		LeftMoveArray(m_TeeSpeedChartArray1,m_c_arrayLength,randf(0.099,0.1));
		
		LeftMoveArray(m_X,m_c_arrayLength,m_count);
		m_pHLineSerie0->AddPoints(m_X,m_HightSpeedChartArray0,m_c_arrayLength);
		m_pHLineSerie1->AddPoints(m_X,m_HightSpeedChartArray1,m_c_arrayLength);
		
		m_pTLineSerie0->AddPoints(m_X,m_TeeSpeedChartArray0,m_c_arrayLength);
		m_pTLineSerie1->AddPoints(m_X,m_TeeSpeedChartArray1,m_c_arrayLength);
		*/
	    drawMoving();
	}


	
	CDialog::OnTimer(nIDEvent);
}
/////////////////////////////////////////////SEARCH//////////////////////////////////////////////////////////

void CMineRobotContDlg::OnBnClickedButtonGtg()
{

	//angle_init=GetDlgItemInt(IDC_EDIT_YAW);
	SetTimer(2,100,NULL);
}


void CMineRobotContDlg::OnBnClickedGtgStop()
{
	// TODO: 在此添加控件通知处理程序代码
	amp[0].SetHaltMode(HALT_ABRUPT);
	amp[0].GetHaltMode(haltmode);
	amp[1].SetHaltMode(HALT_ABRUPT);
	amp[1].GetHaltMode(haltmode);
	amp[0].HaltMove();
	amp[1].HaltMove();
	KillTimer(2);
}
//////////////////////////////////PICTURE//////////////////////////////////////////////////////////////////////



void CMineRobotContDlg::OnBnClickedButtonPictureStart()
{
	// TODO: 在此添加控件通知处理程序代码


    //*dData0=velocity[0];
	//*dData1=velocity[1];
	ZeroMemory(m_TeeSpeedChartArray0,sizeof(double)*m_c_arrayLength);
	ZeroMemory(m_TeeSpeedChartArray1,sizeof(double)*m_c_arrayLength);
	
	ZeroMemory(m_HightSpeedChartArray0,sizeof(double)*m_c_arrayLength);
	ZeroMemory(m_HightSpeedChartArray1,sizeof(double)*m_c_arrayLength);
	
	for (size_t i=0;i<m_c_arrayLength;++i)
	{
		m_X[i] = i;
	}
	m_count = m_c_arrayLength;

	m_pHLineSerie0->ClearSerie();
	m_pHLineSerie1->ClearSerie();
	
	m_pTLineSerie0->ClearSerie();
	m_pTLineSerie1->ClearSerie();
	
	SetTimer(3,0,NULL);
}

void CMineRobotContDlg::LeftMoveArray(double* ptr,size_t length,double data)
{
	for (size_t i=1;i<length;i ++)
	{
		ptr[i-1] = ptr[i];
	}
	ptr[length-1] = data;
}

/*void CMineRobotContDlg::RandArray(double* ptr,size_t length)
{
	for (size_t i=0;i<545;++i)
	{
		ptr[i] = 0;
	}
	for (size_t i=545;i<length;++i)
	{
		ptr[i] = randf(-0.05,0.1);
	}
}*/



void CMineRobotContDlg::OnBnClickedButtonPictureStop()
{
	// TODO: 在此添加控件通知处理程序代码
	KillTimer(3);
}

void CMineRobotContDlg::drawMoving()
{

	m_pHLineSerie0->ClearSerie();
	m_pHLineSerie1->ClearSerie();
	
	m_pTLineSerie0->ClearSerie();
	m_pTLineSerie1->ClearSerie();
	
	LeftMoveArray(m_HightSpeedChartArray0,m_c_arrayLength,mvh0);
	LeftMoveArray(m_HightSpeedChartArray1,m_c_arrayLength,-mvh1);
	LeftMoveArray(m_TeeSpeedChartArray0,m_c_arrayLength,mpt0);
	LeftMoveArray(m_TeeSpeedChartArray1,m_c_arrayLength,-mpt1);

	LeftMoveArray(m_X,m_c_arrayLength,m_count);
	m_pHLineSerie0->AddPoints(m_X,m_HightSpeedChartArray0,m_c_arrayLength);
	m_pHLineSerie1->AddPoints(m_X,m_HightSpeedChartArray1,m_c_arrayLength);

	m_pTLineSerie0->AddPoints(m_X,m_TeeSpeedChartArray0,m_c_arrayLength);
	m_pTLineSerie1->AddPoints(m_X,m_TeeSpeedChartArray1,m_c_arrayLength);

}

void CMineRobotContDlg::OnBnClickedButtonCstrat()
{
	// TODO: 在此添加控件通知处理程序代码
	uunit p0;	
	amp[0].GetPositionActual(p0);
	actual_pos_init[0]=p0/126/2;
	uunit p1;	
	amp[1].GetPositionActual(p1);
	actual_pos_init[1]=p1/126/2;
	UpdateData(true);
	contral_v=GetDlgItemInt(IDC_EDIT_V);
	contral_r=GetDlgItemInt(IDC_EDIT_R);
	contral_w=GetDlgItemInt(IDC_EDIT_W);
	m_n1=(2*contral_v+contral_r*contral_w)/(4*3.1415926*55)*126*2;
    m_n2=(2*contral_v-contral_r*contral_w)/(4*3.1415926*55)*126*2;
    UpdateData(false);

	MCTimerId = timeSetEvent(10,1,(LPTIMECALLBACK)TimerProccmove,0,TIME_PERIODIC);
}


void CMineRobotContDlg::OnBnClickedButtonCstop()
{
	// TODO: 在此添加控件通知处理程序代码
	
	timeKillEvent(MCTimerId);
	amp[0].SetHaltMode(HALT_ABRUPT);
	amp[0].GetHaltMode(haltmode);
	amp[1].SetHaltMode(HALT_ABRUPT);
	amp[1].GetHaltMode(haltmode);
	amp[0].HaltMove();
	amp[1].HaltMove();
	timeKillEvent(MCTimerId);
}
