#pragma once


// Rotate dialog

class Rotate : public CDialogEx
{
	DECLARE_DYNAMIC(Rotate)

public:
	Rotate(CWnd* pParent = nullptr);   // standard constructor
	virtual ~Rotate();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ROTATE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int Degree;
};
