// Contrast.cpp : implementation file
//

#include "stdafx.h"
#include "ImgPro.h"
#include "Contrast.h"
#include "afxdialogex.h"


// Contrast dialog

IMPLEMENT_DYNAMIC(Contrast, CDialogEx)

Contrast::Contrast(CWnd* pParent /*=NULL*/)
	: CDialogEx(Contrast::IDD, pParent)
	, iContrast(0)
{

}

Contrast::~Contrast()
{
}

void Contrast::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Slider(pDX, IDC_CONTRAST, iContrast);
	DDV_MinMaxInt(pDX, iContrast, 0, 100);
}


BEGIN_MESSAGE_MAP(Contrast, CDialogEx)
END_MESSAGE_MAP()


// Contrast message handlers
