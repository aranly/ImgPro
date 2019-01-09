#pragma once


// CustomBinarization dialog

class CustomBinarization : public CDialogEx
{
	DECLARE_DYNAMIC(CustomBinarization)

public:
	CustomBinarization(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CustomBinarization();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CUSTOMBINARIZATION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int Threshold;
};
