// MBData.cpp : implementation file
//

#include "stdafx.h"
#include "ImgPro.h"
#include "MBData.h"
#include "afxdialogex.h"


// MBData dialog

IMPLEMENT_DYNAMIC(MBData, CDialogEx)

MBData::MBData(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_MBDATA, pParent)
	, MBData0(_T(""))
	, MBData1(_T(""))
	, MBData2(_T(""))
	, MBDATA3(_T(""))
{

}

MBData::~MBData()
{
}

void MBData::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_MBDATA0, MBData0);
	DDX_Text(pDX, IDC_MBDATA1, MBData1);
	DDX_Text(pDX, IDC_MBDATA2, MBData2);
	DDX_Text(pDX, IDC_MBDATA3, MBDATA3);
}


BEGIN_MESSAGE_MAP(MBData, CDialogEx)
	ON_STN_CLICKED(IDC_MBDATA2, &MBData::OnStnClickedMbdata2)
END_MESSAGE_MAP()


// MBData message handlers


void MBData::OnStnClickedMbdata2()
{
	// TODO: Add your control notification handler code here
}
