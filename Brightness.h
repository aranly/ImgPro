#pragma once


// Brightness dialog

class Brightness : public CDialogEx
{
	DECLARE_DYNAMIC(Brightness)

public:
	Brightness(CWnd* pParent = NULL);   // standard constructor
	virtual ~Brightness();

// Dialog Data
	enum { IDD = IDD_DIALOG_BRIGHTNESS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	//afx_msg void OnNMCustomdrawBrightness(NMHDR *pNMHDR, LRESULT *pResult);
	int iBrightness;
};
