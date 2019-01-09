#pragma once


// Contrast dialog

class Contrast : public CDialogEx
{
	DECLARE_DYNAMIC(Contrast)

public:
	Contrast(CWnd* pParent = NULL);   // standard constructor
	virtual ~Contrast();

// Dialog Data
	enum { IDD = IDD_DIALOG_CONTRAST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int iContrast;
};
