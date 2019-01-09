// CustomBinarization.cpp : implementation file
//

#include "stdafx.h"
#include "ImgPro.h"
#include "CustomBinarization.h"
#include "afxdialogex.h"


// CustomBinarization dialog

IMPLEMENT_DYNAMIC(CustomBinarization, CDialogEx)

CustomBinarization::CustomBinarization(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_CUSTOMBINARIZATION, pParent)
	, Threshold(0)
{

}

CustomBinarization::~CustomBinarization()
{
}

void CustomBinarization::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, Threshold);
}


BEGIN_MESSAGE_MAP(CustomBinarization, CDialogEx)
END_MESSAGE_MAP()


// CustomBinarization message handlers
