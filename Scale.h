#pragma once


// Scale dialog

class Scale : public CDialogEx
{
	DECLARE_DYNAMIC(Scale)

public:
	Scale(CWnd* pParent = nullptr);   // standard constructor
	virtual ~Scale();

// Dialog Data
	enum { IDD = IDD_DIALOG_SCALE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int iScale;
	int Slider;
};
