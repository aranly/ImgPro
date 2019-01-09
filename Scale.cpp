// Scale.cpp : implementation file
//

#include "stdafx.h"
#include "ImgPro.h"
#include "Scale.h"
#include "afxdialogex.h"


// Scale dialog

IMPLEMENT_DYNAMIC(Scale, CDialogEx)

Scale::Scale(CWnd* pParent /*=NULL*/)
	: CDialogEx(Scale::IDD, pParent)
	, Slider(0)
{

}

Scale::~Scale()
{
}

void Scale::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Slider(pDX, IDC_SLIDER1, Slider);
	DDV_MinMaxInt(pDX, Slider, 0, 256);
}


BEGIN_MESSAGE_MAP(Scale, CDialogEx)
END_MESSAGE_MAP()


// Scale message handlers
