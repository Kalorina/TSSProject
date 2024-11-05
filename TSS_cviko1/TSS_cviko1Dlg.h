
// TSS_cviko1Dlg.h : header file
//

#include <vector> 
#include <iostream>
using namespace std;

#pragma once

// Define the Img structure
struct Img
{
	CString filename;
	CString filepath;
	Gdiplus::Image* bitmap;

	std::vector<int> redChannel;
	std::vector<int> greenChannel;
	std::vector<int> blueChannel;
};

// enum -> enumarate
enum
{
	WM_DRAW_IMAGE = WM_USER + 1,
	WM_DRAW_HISTOGRAM //s deklaraciou -> WM_DRAW_HISTOGRAM = WM_USER + 2;
};

class CStaticImage : public CStatic
{
public:
	// Overridable (for owner draw only)
	//lp -> long pointer
	void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStuct) override;
};

class CStaticHist : public CStatic
{
public:
	// Overridable (for owner draw only)
	//lp -> long pointer
	void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStuct) override;
};

// CTSScviko1Dlg dialog
class CTSScviko1Dlg : public CDialogEx
{
// Construction
public:
	CTSScviko1Dlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TSS_CVIKO1_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLvnItemchangedFileList(NMHDR* pNMHDR, LRESULT* pResult);

	CRect m_rect;
	CMenu* m_menu;

	CRect m_rectFileList;
	CRect m_rectStaticHistogram;
	CRect m_rectStaticImage;

	CListCtrl m_fileList;
	CStaticHist m_staticHistogram;
	CStaticImage m_staticImage;

	std::vector<Img> m_imageList;

	bool m_RedChecked = FALSE;
	bool m_GreenChecked = FALSE;
	bool m_BlueChecked = FALSE;

	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;


	//helpers
	void DisplayListControl();


	afx_msg void OnFileOpen32771();
	afx_msg void OnFileClose32772();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	
	//Messages
	afx_msg LRESULT OnDrawImage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDrawHist(WPARAM wParam, LPARAM lParam);
	afx_msg void OnStnClickedStaticImage();
	afx_msg void OnHistogramRed();
	afx_msg void OnHistogramGreen();
	afx_msg void OnHistogramBlue();
	afx_msg void OnUpdateHistogramRed(CCmdUI* pCmdUI);
	afx_msg void OnExitappExit();
};

void CalculateHistogram(Img& img);
