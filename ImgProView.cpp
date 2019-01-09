
// ImgProView.cpp : implementation of the CImgProView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "ImgPro.h"
#endif

#include "ImgProDoc.h"
#include "ImgProView.h"
#include "MBData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CImgProView

IMPLEMENT_DYNCREATE(CImgProView, CView)

BEGIN_MESSAGE_MAP(CImgProView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_TRANSFORM_VERTICALFLIP, &CImgProView::OnTransformVerticalflip)
	ON_COMMAND(ID_TRANSFORM_HORIZONTALFLIP, &CImgProView::OnTransformHorizontalflip)
	ON_COMMAND(ID_CONVERT_YUV, &CImgProView::OnConvertYuv)
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	END_MESSAGE_MAP()

// CImgProView construction/destruction

CImgProView::CImgProView()
{
	// TODO: add construction code here
	iIfFirst = 1;
	iXStart = -1;
	iYStart = -1;
}

CImgProView::~CImgProView()
{
}

BOOL CImgProView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CImgProView drawing

void CImgProView::OnDraw(CDC* pDC)
{
	CImgProDoc* pDoc = GetDocument();
	RECT clientRect;

	GetClientRect(&clientRect);

	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	if (pDoc->bImageLoaded) {
		if (pDoc->iMode == IMAGE_MODE_DISPLAY)
		{
			pDoc->imagecontext.display.ConvertToRGBA(pDoc->imagecontext.image);
			if (pDoc->bImageUpdated)
			{
				pDoc->bImageUpdated = false;
				MemDC.DeleteDC();
				MemDC.CreateCompatibleDC(pDC);

				bitmap.DeleteObject();
				bitmap.CreateCompatibleBitmap(pDC, pDoc->imagecontext.display.width, pDoc->imagecontext.display.height);
				MemDC.SelectObject(&bitmap);

			}
			bitmap.SetBitmapBits(pDoc->imagecontext.display.width * pDoc->imagecontext.display.height * 4,
				pDoc->imagecontext.display.pucData[0]);
			pDC->BitBlt(0, 0, pDoc->imagecontext.display.width, pDoc->imagecontext.display.height, &MemDC, 0, 0, SRCCOPY);
			if (iXStart >= 0 && iYStart >= 0)
			{
				CPen myPen(PS_SOLID, 1, RGB(255, 100, 100));
				CPen* pOldPen = pDC->SelectObject(&myPen);

				pDC->SelectObject(&Pen);
				pDC->MoveTo(iXStart, iYStart);
				pDC->LineTo(iXStart + 15, iYStart);
				pDC->LineTo(iXStart + 15, iYStart + 15);
				pDC->LineTo(iXStart, iYStart + 15);
				pDC->LineTo(iXStart, iYStart);
			}
		}
		if (pDoc->iMode == IMAGE_MODE_HISTOGRAM) {
			HistogramInit(256, &(pDoc->imagecontext.histogram));
			pDoc->imagecontext.image.CalHistogram(&(pDoc->imagecontext.histogram));
			int height = (clientRect.bottom - clientRect.top) / 3 - 100;

			pDC->SelectObject(&Pen);
			for (int i = 0; i < 256; i++)
			{
				pDC->MoveTo(40 + i, 40);
				pDC->LineTo(40 + i, 40 + pDoc->imagecontext.histogram.piBins[0][i] * height / pDoc->imagecontext.histogram.piMaxOfBins[0]);

				pDC->MoveTo(40 + i, height + 140);
				pDC->LineTo(40 + i, height + 140 + pDoc->imagecontext.histogram.piBins[1][i] * height / pDoc->imagecontext.histogram.piMaxOfBins[1]);

				pDC->MoveTo(40 + i, height * 2 + 240);
				pDC->LineTo(40 + i, height * 2 + 240 + pDoc->imagecontext.histogram.piBins[2][i] * height / pDoc->imagecontext.histogram.piMaxOfBins[2]);
			}
		}
		if (pDoc->iMode == IMAGE_MODE_QRCODE) {
			pDoc->imagecontext.display.ConvertToRGBA(pDoc->imagecontext.image);
			if (pDoc->bImageUpdated)
			{
				pDoc->bImageUpdated = false;
				MemDC.DeleteDC();
				MemDC.CreateCompatibleDC(pDC);

				bitmap.DeleteObject();
				bitmap.CreateCompatibleBitmap(pDC, pDoc->imagecontext.display.width, pDoc->imagecontext.display.height);
				MemDC.SelectObject(&bitmap);

			}
			bitmap.SetBitmapBits(pDoc->imagecontext.display.width * pDoc->imagecontext.display.height * 4,
				pDoc->imagecontext.display.pucData[0]);
			pDC->BitBlt(0, 0, pDoc->imagecontext.display.width, pDoc->imagecontext.display.height, &MemDC, 0, 0, SRCCOPY);
			CPen myPen(PS_SOLID, 1, RGB(255, 100, 100));
			CPen* pOldPen = pDC->SelectObject(&myPen);
			pDC->MoveTo(pDoc->qrCode.pPoints[0].iX, pDoc->qrCode.pPoints[0].iY);
			pDC->LineTo(pDoc->qrCode.pPoints[1].iX, pDoc->qrCode.pPoints[1].iY);
			pDC->LineTo(pDoc->qrCode.pPoints[2].iX, pDoc->qrCode.pPoints[2].iY);
			pDC->LineTo(pDoc->qrCode.pPoints[3].iX, pDoc->qrCode.pPoints[3].iY);
			pDC->LineTo(pDoc->qrCode.pPoints[0].iX, pDoc->qrCode.pPoints[0].iY);
		}
	}
}


// CImgProView printing

BOOL CImgProView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CImgProView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CImgProView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CImgProView diagnostics

#ifdef _DEBUG
void CImgProView::AssertValid() const
{
	CView::AssertValid();
}

void CImgProView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CImgProDoc* CImgProView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImgProDoc)));
	return (CImgProDoc*)m_pDocument;
}
#endif //_DEBUG


// CImgProView message handlers


void CImgProView::OnTransformVerticalflip()
{
	CImgProDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	pDoc->imagecontext.image.FlipX();
	Invalidate(true);
}


void CImgProView::OnTransformHorizontalflip()
{
	CImgProDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	pDoc->imagecontext.image.FlipY();
	Invalidate(true);
}


void CImgProView::OnConvertYuv()
{
	CImgProDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	pDoc->imagecontext.image.FlipY();
	pDoc->imagecontext.image.RGBtoYUV();
	Invalidate(true);
}

void CImgProView::OnMouseMove(UINT nFlags, CPoint point) {
	CImgProDoc* pDoc = GetDocument();
	MyImage *pImage = &(pDoc->imagecontext.image);
	if (point.x >= 0 && point.x < pImage->width && point.y >= 0 && point.y < pImage->height){
		char message[50];
		switch (pImage->format)
		{
		case RGB:
			sprintf(message, "x=%d, y=%d, R=%d, G=%d, B=%d",
				point.x, point.y,
				pImage->pucData[0][(pImage->height + pImage->offsetY - 1 - point.y) * pImage->xStride + (point.x + pImage->offsetX) * 3 + 2],
				pImage->pucData[0][(pImage->height + pImage->offsetY - 1 - point.y) * pImage->xStride + (point.x + pImage->offsetX) * 3 + 1],
				pImage->pucData[0][(pImage->height + pImage->offsetY - 1 - point.y) * pImage->xStride + (point.x + pImage->offsetX) * 3]
			);
			break;
		case YUV:
			sprintf(message, "x=%d, y=%d, Y=%d, U=%d, V=%d",
				point.x, point.y,
				pImage->pucData[0][(point.y + pImage->offsetY) * pImage->xStride + point.x + pImage->offsetX],
				pImage->pucData[1][(point.y + pImage->offsetY) * pImage->xStride + point.x + pImage->offsetX],
				pImage->pucData[2][(point.y + pImage->offsetY) * pImage->xStride + point.x + pImage->offsetX]
			);
			break;
		}
		((CFrameWnd *)GetParent())->SetMessageText(message);
	}
	CView::OnMouseMove(nFlags, point);
}

void CImgProView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	int i, j;
	MBData dlg;
	iXStart = point.x / 16 * 16;
	iYStart = point.y / 16 * 16;
	char pcMBID[120];
	char pcMBData0[2000], pcMBData1[2000], pcMBData2[2000];
	CImgProDoc* pDoc = GetDocument();
	MyImage *pImage = &(pDoc->imagecontext.image);
	Invalidate(FALSE);

	if (pImage->format == YUV)
	{
		for (i = 0; i < 16; i++)
		{
			for (j = 0; j < 16; j++)
			{
				sprintf(&(pcMBData0[i * 16 * 3 + j * 3]), "%.2x ",
					(unsigned char)(pImage->pucData[0][(iYStart + i + pImage->offsetY) * pImage->xStride + iXStart + j + pImage->offsetX]));
			}
			sprintf(&(pcMBData0[i * 16 * 3 + 15 * 3 + 2]), "\n");
		}
		for (i = 0; i < 16; i++)
		{
			for (j = 0; j < 16; j++)
			{
				sprintf(&(pcMBData1[i * 16 * 3 + j * 3]), "%.2x ",
					(unsigned char)(pImage->pucData[1][(iYStart + i + pImage->offsetY) * pImage->xStride + iXStart + j + pImage->offsetX]));
			}
			sprintf(&(pcMBData1[i * 16 * 3 + 15 * 3 + 2]), "\n");
		}
		for (i = 0; i < 16; i++)
		{
			for (j = 0; j < 16; j++)
			{
				sprintf(&(pcMBData2[i * 16 * 3 + j * 3]), "%.2x ",
					(unsigned char)(pImage->pucData[2][(iYStart + i + pImage->offsetY) * pImage->xStride + iXStart + j + pImage->offsetX]));
			}
			sprintf(&(pcMBData2[i * 16 * 3 + 15 * 3 + 2]), "\n");
		}
	}
	dlg.MBData0 = pcMBData0;
	dlg.MBData1 = pcMBData1;
	dlg.MBData2 = pcMBData2;

	sprintf(pcMBID, "MB: X=%d, Y=%d", iXStart, iYStart);
	dlg.MBDATA3 = pcMBID;

	dlg.DoModal();
	CView::OnRButtonDown(nFlags, point);
}






