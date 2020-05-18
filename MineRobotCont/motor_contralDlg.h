
// motor_contralDlg.h : 头文件
//

#pragma once



// Cmotor_contralDlg 对话框
class Cmotor_contralDlg : public CDialogEx
{
// 构造
public:

	Cmotor_contralDlg(CWnd* pParent = NULL);	// 标准构造函数
	CSliderCtrl m_ctrlSlider;
// 对话框数据
	enum { IDD = IDD_MOTOR_CONTRAL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


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
	//afx_msg void OnBnClickedButton8();
	double a;
	double b;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton7();


	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	double c;
	double d;

	double e;
	double f;
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButton10();
	afx_msg void OnBnClickedButton11();
	afx_msg void OnBnClickedButton12();
	afx_msg void OnBnClickedButton13();
	afx_msg void OnBnClickedButton14();
	afx_msg void OnBnClickedButton15();
	afx_msg void OnBnClickedButton16();
	afx_msg void OnBnClickedButton17();
	afx_msg void OnBnClickedButton18();
	afx_msg void OnBnClickedButton19();
	afx_msg void OnBnClickedButton20();
	afx_msg void OnBnClickedButton21();
	afx_msg void OnBnClickedButton22();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton23();
	afx_msg void OnBnClickedButton24();
	
//	double v_1;
//	CEdit m_v;
//	double m_v;
//	double v_1;
	afx_msg void OnBnClickedButton25();
	
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	double v_0;
	double v_1;
	double v_2;
	double v_3;
	double v_4;
	double v_5;
//	CString c_0;
	double c_0;
	double c_1;
	double c_2;
	double c_3;
	double c_4;
	double c_5;
	double v0;
	double v1;
	double v2;
	double v3;
	double v4;
	double v5;
	afx_msg void OnBnClickedButton26();
	double p_0;
	double p_1;
	double p_2;
	double p_3;
	double p_4;
	double p_5;
	afx_msg void OnBnClickedButton27();
	afx_msg void OnBnClickedButton28();
	afx_msg void OnBnClickedButton29();
	afx_msg void OnBnClickedButton30();

	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	double k;
	afx_msg void OnBnClickedButton31();
	afx_msg void OnBnClickedButton32();
	afx_msg void OnBnClickedButton33();
};
