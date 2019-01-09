// Rotate.cpp : implementation file
//

#include "stdafx.h"
#include "ImgPro.h"
#include "Rotate.h"
#include "afxdialogex.h"


// Rotate dialog

IMPLEMENT_DYNAMIC(Rotate, CDialogEx)

Rotate::Rotate(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_ROTATE, pParent)
	, Degree(0)
{

}

Rotate::~Rotate()
{
}

void Rotate::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, Degree);
}


BEGIN_MESSAGE_MAP(Rotate, CDialogEx)
END_MESSAGE_MAP()


// Rotate message handlers
