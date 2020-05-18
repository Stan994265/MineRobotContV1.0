
// MineRobotContDlg.h : 头文件
//

#pragma once

#include "cv.h"
#include "highgui.h"
#include "CvvImage.h"
#include "afxwin.h"
#include "Common/CUDPManager.h"
#include "SocketBlockClient.h"
/////////////////COMPASS//////////////////////////////////////////////////////////////////////////
#include "com.h"
#include "afxwin.h"
#include <process.h>
/////////////////////////////////////////////////////////////////////////////////////////////////
#include "ChartClass\ChartCtrl.h"
#include "ChartClass\ChartLineSerie.h"
#include "ChartClass\ChartAxis.h"
#include "ChartClass\ChartTitle.h"
#include "ChartClass\ChartLegend.h"

// CMineRobotContDlg 对话框
class CMineRobotContDlg : public CDialogEx
{
// 构造
public:
	CSliderCtrl m_ctrlSlider;
	CSliderCtrl m_ctrlSlider_Lf;
	CMineRobotContDlg(CWnd* pParent = NULL);	// 标准构造函数

	CUDPManager  mUDPManager;

	IplImage* m_ImageRGB;
	IplImage* m_ImageGray;

	int m_i, m_j;  

	BOOL m_bCntSensor;

	CSocketBlockClient m_ClientSocketPredictive;
	CSocketBlockClient m_ClientSocketCommand;

	int m_nPCount;

	char cSendbufferp[ORDER_BUF_LEN];

	char buffcmd[ORDER_BUF_LEN];


	//char sendData[1000000];

// 对话框数据
	enum { IDD = IDD_MINEROBOTCONT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
//////////////////////////////////////COMPASS/////////////////////////////////////////////////////
	static unsigned int __stdcall  ComProc(void* lpParam);

	int DecodeIMUData(unsigned char chrTemp[]);

	void GetSystemPorts();

	UINT ComId[16];
	int  ComList[16];
	int  ComNum;
	int  ComSelect;

	HANDLE	m_hStopEvent;
	HANDLE	m_hThreadProc;

	bool    bInit;

/////////////////////////////////////////////////////////////////////////////////////////////////////

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOpencamer();
	void VedioDisplay(void);
	afx_msg void OnBnClickedClosecamer();
	virtual void OnCancel();
	CEdit m_EditHostName;
	CEdit m_EditHostIP;
	afx_msg void OnClickedSendVedio();
	CString m_TargetIP;
	int m_TargetPort;
	afx_msg void OnBnClickedStopVedio();
	void SendVedio(void);
	afx_msg void OnBnClickedSensor();
	void SendSensorData(void);
	afx_msg void OnBnClickedCommand();
	void ReceiveData(void);
	afx_msg void OnBnClickedMotorInit();
	afx_msg void OnBnClickedMotorForward();
	afx_msg void OnBnClickedMotorRear();
	afx_msg void OnBnClickedMotorLeft();
	afx_msg void OnBnClickedMotorRight();
	afx_msg void OnBnClickedStartMtimer();
	afx_msg void OnBnClickedStopMtimer();
	afx_msg void OnNMCustomdrawSliderCvel(NMHDR *pNMHDR, LRESULT *pResult);
	CComboBox m_baudRate_cb;
	CComboBox m_comList;
	CListBox m_list;
	afx_msg void OnBnClickedButtonOpencom();
	afx_msg void OnBnClickedButtonClosecom();
	afx_msg void OnBnClickedButtonCalibration();
	afx_msg void OnBnClickedButtonEndcali();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnRead();
	afx_msg void OnBnClickedButtonOpenlaser();
	afx_msg void OnBnClickedButtonCloselaser();
	double m_385;
	double m_pitch;
	double m_roll;
	double m_yaw;
	afx_msg void OnBnClickedButtonGtg();
	afx_msg void OnNMCustomdrawSliderLf(NMHDR *pNMHDR, LRESULT *pResult);
	void BaiBiMove(void);
	void CMineRobotContDlg::Contral_Move(void);
	CButton m_llf;
	CButton m_llr;
	CButton m_rrf;
	CButton m_rrr;
	CListBox m_motor_info;
	void GetMotorPositionActual(void);
	double actual_pos[10];
	double current[3];
	double angle_motor;
	double velocity[3];
	double mvh0,mvh1,mpt0,mpt1;
	double odometer;
	afx_msg void OnBnClickedGtgStop();
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	public:
	CMFCTabCtrl m_tab;
	CChartCtrl m_HSChartCtrl;
	CChartCtrl m_TSChartCtrl;
	CChartLineSerie *m_pHLineSerie0;
	CChartLineSerie *m_pHLineSerie1;
	CChartLineSerie *m_pTLineSerie0;
	CChartLineSerie *m_pTLineSerie1;


	private:
	void drawMoving();
	double m_HightSpeedChartArray0[500];
	double m_HightSpeedChartArray1[500];
	double m_TeeSpeedChartArray0[500];
	double m_TeeSpeedChartArray1[500];
	
	double m_X[500];

    
	unsigned int m_count;
	const size_t m_c_arrayLength;
	void LeftMoveArray(double* ptr,size_t length,double data);

public:
	afx_msg void OnBnClickedButtonPictureStart();
	afx_msg void OnBnClickedButtonPictureStop();
	double contral_v;
	double contral_w;
	double contral_r;
	//int contral_count;
	afx_msg void OnBnClickedButtonCstrat();
	afx_msg void OnBnClickedButtonCstop();
	CButton m_lm;
	CButton m_rm;
	double m_n1;
	double m_n2;
};
