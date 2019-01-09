
// ImgProDoc.cpp : implementation of the CImgProDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "ImgPro.h"
#endif

#include "ImgProDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CImgProDoc

IMPLEMENT_DYNCREATE(CImgProDoc, CDocument)

BEGIN_MESSAGE_MAP(CImgProDoc, CDocument)
	ON_COMMAND(ID_FILE_OPEN, &CImgProDoc::OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE_AS, &CImgProDoc::OnFileSaveAs)
	ON_COMMAND(ID_ENHANCEMENT_BRIGHTNESS, &CImgProDoc::OnEnhancementBrightness)
	ON_COMMAND(ID_ENHANCEMENT_CONTRAST, &CImgProDoc::OnEnhancementContrast)
	ON_COMMAND(ID_ENHANCE_GREYSCALE, &CImgProDoc::OnEnhanceGreyscale)
	ON_COMMAND(ID_ENHANCE_BLUR, &CImgProDoc::OnEnhanceBlur)
	ON_COMMAND(ID_ENHANCEMENT_SHARPEN, &CImgProDoc::OnEnhancementSharpen)
	ON_COMMAND(ID_ENHANCE_KERNEL, &CImgProDoc::OnEnhanceKernel)
	ON_COMMAND(ID_TRANSFORM_SCALE, &CImgProDoc::OnTransformScale)
	ON_COMMAND(ID_TRANSFORM_ROTATE, &CImgProDoc::OnTransformRotate)
	ON_COMMAND(ID_ENHANCE_BINARIZATION, &CImgProDoc::OnEnhanceBinarization)
	ON_COMMAND(ID_ENHANCE_ERODE, &CImgProDoc::OnEnhanceErode)
	ON_COMMAND(ID_ENHANCE_DILATE, &CImgProDoc::OnEnhanceDilate)
	ON_COMMAND(ID_ENHANCE_GLOBALBINARIZATION, &CImgProDoc::OnEnhanceGlobalbinarization)
	ON_COMMAND(ID_ENHANCE_CUSTOMBINARIZATION, &CImgProDoc::OnEnhanceCustombinarization)
	ON_COMMAND(ID_ENHANCE_SOBELEDGEDETECTION, &CImgProDoc::OnEnhanceSobeledgedetection)
	ON_COMMAND(ID_ENHANCE_OTSUBINARIZATION, &CImgProDoc::OnEnhanceOtsubinarization)
	ON_COMMAND(ID_MODE_HISTOGRAM, &CImgProDoc::OnModeHistogram)
	ON_COMMAND(ID_ENHANCE_SAUVOLABINARIZATION, &CImgProDoc::OnEnhanceSauvolabinarization)
	ON_COMMAND(ID_ENHANCE_NIBLACKBINARIZATION, &CImgProDoc::OnEnhanceNiblackbinarization)
	ON_COMMAND(ID_MODE_QRCODE, &CImgProDoc::OnModeQrcode)
	ON_COMMAND(ID_ENHANCE_ZXINGBINARIZATION, &CImgProDoc::OnEnhanceZxingbinarization)
	ON_COMMAND(ID_MODE_DEFAULT, &CImgProDoc::OnModeDefault)
	ON_COMMAND(ID_ENHANCE_CANNYEDGEDETECTION, &CImgProDoc::OnEnhanceCannyedgedetection)
	ON_COMMAND(ID_ENHANCE_SEGMENTATION, &CImgProDoc::OnEnhanceSegmentation)
END_MESSAGE_MAP()


// CImgProDoc construction/destruction

CImgProDoc::CImgProDoc()
{
	// TODO: add one-time construction code here
	bImageLoaded = false;
	bImageUpdated = false;
	iMode = IMAGE_MODE_DISPLAY;
}

CImgProDoc::~CImgProDoc()
{
}

BOOL CImgProDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CImgProDoc serialization

void CImgProDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CImgProDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CImgProDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CImgProDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CImgProDoc diagnostics

#ifdef _DEBUG
void CImgProDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CImgProDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CImgProDoc commands


void CImgProDoc::OnFileOpen()
{
	// TODO: Add your command handler code here
	LPSTR fileFilter = "Image File(*.yuv, *.raw, *.raw16, *.bmp)\0*.yuv;*.raw;*.raw16;*.bmp\0\0";
	LPSTR fileDir = "D:\\data";
	CFileDialog fileDlg(TRUE);
	OPENFILENAME& ofn = fileDlg.GetOFN();

	fileDlg.m_ofn.lpstrFilter = fileFilter;
	fileDlg.m_ofn.lpstrInitialDir = fileDir;
	if (fileDlg.DoModal() == IDOK)
	{
		CString filepath = fileDlg.GetPathName();
		char pcFilePath[100];
		sprintf(pcFilePath, "%s", CStringA(filepath));
		//sprintf(pcFilePath, "%s", CW2A(filepath));
		imagecontext.originalImage.LoadImageFromFile(pcFilePath);
		imagecontext.image.Copy(imagecontext.originalImage);
		bImageLoaded = true;
		bImageUpdated = true;
		UpdateAllViews(NULL);

	}
}


void CImgProDoc::OnFileSaveAs()
{
	LPSTR fileFilter = "Image File(*.yuv, *.raw, *.raw16, *.bmp)\0*.yuv;*.raw;*.raw16;*.bmp\0\0";
	LPSTR fileDir = "D:\\data";
	CFileDialog fileDlg(TRUE);
	OPENFILENAME& ofn = fileDlg.GetOFN();
	fileDlg.m_ofn.lpstrFilter = fileFilter;
	fileDlg.m_ofn.lpstrInitialDir = fileDir;
	if (fileDlg.DoModal() == IDOK)
	{
		CString filepath = fileDlg.GetPathName();
		char pcFilePath[100];
		sprintf(pcFilePath, "%s", CStringA(filepath));
		//sprintf(pcFilePath, "%s", (LPCTSTR)filepath);
		imagecontext.image.StoreImageToFile(pcFilePath);

	}
}


void CImgProDoc::OnEnhancementBrightness()
{
	Brightness brightnessDlg;
	brightnessDlg.iBrightness = 50;
	if (brightnessDlg.DoModal() == IDOK)
	{
		imagecontext.image.AdjustBrightness((float)brightnessDlg.iBrightness / 50.);
	}
	UpdateAllViews(NULL);
}


void CImgProDoc::OnEnhancementContrast()
{
	Contrast contrastDlg;
	contrastDlg.iContrast = 50;
	if (contrastDlg.DoModal() == IDOK)
	{
		imagecontext.image.AdjustContrast((float)contrastDlg.iContrast/ 50.);
	}
	UpdateAllViews(NULL);

}


void CImgProDoc::OnEnhanceGreyscale()
{
	imagecontext.image.MakeGreyscale();
	UpdateAllViews(NULL);
}


void CImgProDoc::OnEnhanceBlur()
{
	imagecontext.image.Blur();
	UpdateAllViews(NULL);
}


void CImgProDoc::OnEnhancementSharpen()
{
	imagecontext.image.Sharpen();
	UpdateAllViews(NULL);
}


void CImgProDoc::OnEnhanceKernel()
{
	imagecontext.image.Kernel();
	UpdateAllViews(NULL);
}

void CImgProDoc::OnEnhanceBinarization()
{
	imagecontext.image.CustomBinarization(128);
	UpdateAllViews(NULL);
}

void CImgProDoc::OnEnhanceErode()
{
	imagecontext.image.Erode(5);
	UpdateAllViews(NULL);
}

void CImgProDoc::OnEnhanceDilate()
{
	imagecontext.image.Dilate(5);
	UpdateAllViews(NULL);
}

void CImgProDoc::OnTransformScale()
{
	Scale scaleDlg;
	scaleDlg.Slider = 50;
	if (scaleDlg.DoModal() == IDOK)
	{
		imagecontext.image.Scale(scaleDlg.Slider);
	}
	bImageUpdated = true;
	UpdateAllViews(NULL);
}


void CImgProDoc::OnTransformRotate()
{
	Rotate rotateDlg;
	if (rotateDlg.DoModal() == IDOK)
	{
		imagecontext.image.Rotate(rotateDlg.Degree);
	}
	bImageUpdated = true;
	UpdateAllViews(NULL);

}

void CImgProDoc::CalHistogram()
{
	imagecontext.image.RGBtoYUV();
	HistogramInit(256, &imagecontext.histogram);
	imagecontext.image.CalHistogram(&imagecontext.histogram);
}


void CImgProDoc::OnEnhanceGlobalbinarization()
{
	CalHistogram();
	imagecontext.image.GlobalBinarization(imagecontext.histogram);
	UpdateAllViews(NULL);
}


void CImgProDoc::OnEnhanceCustombinarization()
{
	CustomBinarization cstBinDlg;
	if (cstBinDlg.DoModal() == IDOK)
	{
		imagecontext.image.CustomBinarization(cstBinDlg.Threshold);
	}
	bImageUpdated = true;
	UpdateAllViews(NULL);
}


void CImgProDoc::OnEnhanceSobeledgedetection()
{
	imagecontext.image.SobelVisualization();
	UpdateAllViews(NULL);
}


void CImgProDoc::OnEnhanceOtsubinarization()
{
	CalHistogram();
	imagecontext.image.OtsuBinarization(imagecontext.histogram);
	UpdateAllViews(NULL);
}


void CImgProDoc::OnModeHistogram()
{
	switch (imagecontext.image.format)
	{
	case RGB:
		imagecontext.image.RGBtoYUV();
		break;
	}
	iMode = IMAGE_MODE_HISTOGRAM;
	UpdateAllViews(NULL);
}


void CImgProDoc::OnEnhanceSauvolabinarization()
{
	imagecontext.image.SauvolaBinarization();
	UpdateAllViews(NULL);
}


void CImgProDoc::OnEnhanceNiblackbinarization()
{
	imagecontext.image.NiblackBinarization();
	UpdateAllViews(NULL);
}


void CImgProDoc::OnModeQrcode()
{
	qrCode.Process(imagecontext.image);
	iMode = IMAGE_MODE_QRCODE;
	bImageUpdated = true;
	UpdateAllViews(NULL);
}


void CImgProDoc::OnEnhanceZxingbinarization()
{
	imagecontext.image.ZxingBinarization();
	UpdateAllViews(NULL);
}


void CImgProDoc::OnModeDefault()
{
	switch (imagecontext.image.format)
	{
	case RGB:
		imagecontext.image.RGBtoYUV();
		break;
	}
	iMode = IMAGE_MODE_DISPLAY;
	UpdateAllViews(NULL);
}


void CImgProDoc::OnEnhanceCannyedgedetection()
{
	imagecontext.image.Canny();
	UpdateAllViews(NULL);
}


void CImgProDoc::OnEnhanceSegmentation()
{
	imagecontext.image.Segmentation();
	UpdateAllViews(NULL);
}
