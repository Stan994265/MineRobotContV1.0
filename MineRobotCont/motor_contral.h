
// motor_contral.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// Cmotor_contralApp:
// �йش����ʵ�֣������ motor_contral.cpp
//

class Cmotor_contralApp : public CWinApp
{
public:
	Cmotor_contralApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern Cmotor_contralApp theApp;