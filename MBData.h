#pragma once


// MBData dialog

class MBData : public CDialogEx
{
	DECLARE_DYNAMIC(MBData)

public:
	MBData(CWnd* pParent = nullptr);   // standard constructor
	virtual ~MBData();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MBDATA };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString MBData0;
	CString MBData1;
	CString MBData2;
	afx_msg void OnStnClickedMbdata2();
	CString MBDATA3;
};
