
// MineRobotCont.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMineRobotContApp:
// �йش����ʵ�֣������ MineRobotCont.cpp
//

class CMineRobotContApp : public CWinApp
{
public:
	CMineRobotContApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMineRobotContApp theApp;