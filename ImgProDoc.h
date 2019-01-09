
// ImgProDoc.h : interface of the CImgProDoc class
//
#include "Utility.h"
#include "MyImage.h"
#include "Brightness.h"
#include "Contrast.h"
#include "Scale.h"
#include "ImageContext.h"
#include "Rotate.h"
#include "CustomBinarization.h"
#include "QRCode.h"
#pragma once


class CImgProDoc : public CDocument
{
protected: // create from serialization only
	CImgProDoc();
	DECLARE_DYNCREATE(CImgProDoc)

// Attributes
public:
	ImageContext imagecontext;
	QRCode qrCode;
// Operations
public:
	int iMode;
	bool bImageLoaded;
	bool bImageUpdated;
	void CalHistogram();

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CImgProDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	afx_msg void OnFileOpen();
	afx_msg void OnFileSaveAs();
	afx_msg void OnEnhancementBrightness();
	afx_msg void OnEnhancementContrast();
	afx_msg void OnEnhanceGreyscale();
	afx_msg void OnEnhanceBlur();
	afx_msg void OnEnhancementSharpen();
	afx_msg void OnEnhanceKernel();
	afx_msg void OnTransformScale();
	afx_msg void OnTransformRotate();
	afx_msg void OnEnhanceBinarization();
	afx_msg void OnEnhanceErode();
	afx_msg void OnEnhanceDilate();
	afx_msg void OnEnhanceGlobalbinarization();
	afx_msg void OnEnhanceCustombinarization();
	afx_msg void OnEnhanceSobeledgedetection();
	afx_msg void OnEnhanceOtsubinarization();
	afx_msg void OnModeHistogram();
	afx_msg void OnEnhanceSauvolabinarization();
	afx_msg void OnEnhanceNiblackbinarization();
	afx_msg void OnModeQrcode();
	afx_msg void OnEnhanceZxingbinarization();
	afx_msg void OnModeDefault();
	afx_msg void OnEnhanceCannyedgedetection();
	afx_msg void OnEnhanceSegmentation();
};
