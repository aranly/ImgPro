// Brightness.cpp : implementation file
//

#include "stdafx.h"
#include "ImgPro.h"
#include "Brightness.h"
#include "afxdialogex.h"


// Brightness dialog

IMPLEMENT_DYNAMIC(Brightness, CDialogEx)

Brightness::Brightness(CWnd* pParent /*=NULL*/)
	: CDialogEx(Brightness::IDD, pParent)
	, iBrightness(0)
{

}

Brightness::~Brightness()
{
}

void Brightness::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Slider(pDX, IDC_BRIGHTNESS, iBrightness);
	DDV_MinMaxInt(pDX, iBrightness, 0, 100);
}


BEGIN_MESSAGE_MAP(Brightness, CDialogEx)

	//ON_NOTIFY(NM_CUSTOMDRAW, IDC_BRIGHTNESS, &Brightness::OnNMCustomdrawBrightness)
END_MESSAGE_MAP()


// Brightness message handlers



/*
void Brightness::OnNMCustomdrawBrightness(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}
*/