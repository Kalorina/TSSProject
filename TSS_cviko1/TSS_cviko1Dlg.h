
// TSS_cviko1Dlg.h : header file
//

#include <vector> 
#include <iostream>
using namespace std;

#pragma once

enum class BrightnessEffect {
	None,
	Brighter,
	Darker
};

enum class Direction {
	None,
	Up,
	Down,
	Left,
	Right
};

struct BitmapEffect
{
	Gdiplus::Image* bitmap_effect;
	BrightnessEffect brightness = BrightnessEffect::None;	// Brighter/Darker effect
	Direction direction = Direction::None;	// Direction of the effect
	bool isEffectApplied = false;

	BitmapEffect()
		: bitmap_effect(nullptr),
		brightness(BrightnessEffect::None),
		direction(Direction::None) {}

	bool IsCombination(BrightnessEffect targetBrightness, Direction targetDirection) const
	{
		return (brightness == targetBrightness && direction == targetDirection);
	}
};

struct Img
{
	CString filename;
	CString filepath;
	Gdiplus::Image* bitmap;

	// caching 8 possible combinations (brighter/darker + one direction)
	std::vector<BitmapEffect> v_bitmap_effects;

	std::vector<int> redChannel;
	std::vector<int> greenChannel;
	std::vector<int> blueChannel;

	// Histrogram 
	bool isHistCalculated = false;
	bool isHistCalculating = false;

	// for thread
	// adjusting brightness
	bool isEffectCalculationg = false;
	bool isEffectCalculated = false;

	// Find an effect by its combination
	BitmapEffect* FindEffect(BrightnessEffect brightness, Direction direction)
	{
		for (BitmapEffect& effect : v_bitmap_effects)
		{
			if (effect.IsCombination(brightness, direction))
				return &effect;
		}
		return nullptr; // Not found
	}
};

enum
{
	WM_DRAW_IMAGE = WM_USER + 1,
	WM_DRAW_HISTOGRAM, //s deklaraciou -> WM_DRAW_HISTOGRAM = WM_USER + 2;
	WM_HISTOGRAMCALCUCALTION_DONE,
	WM_ADJUSTIMAGEBRITHNESS
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

	void StartAdjustingImageBrightness();
	void StartHistogramCalculationForSelectedImage();

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

	bool m_DirectionUp = FALSE;
	bool m_DirectionDown = FALSE;
	bool m_DirectionLeft = FALSE;
	bool m_DirectionRight = FALSE;
	bool m_Brighter = FALSE;
	bool m_Darker = FALSE;

	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;


	//helpers
	void DisplayListControl();

	afx_msg void OnFileOpen32771();
	afx_msg void OnFileClose32772();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	void DrawHistogramChannel(Gdiplus::Graphics* gr, const std::vector<int>& channel, Gdiplus::Pen& pen, float xScale, int maxHeight, int height);
	void ReturntoOriginalBitmap();
	void InitializeEffects(Img& img);

	//Messages
	afx_msg LRESULT OnDrawImage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDrawHist(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT HistogramCalculationDone(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT AdjustImageBrightnessDone(WPARAM wParam, LPARAM lParam);
	afx_msg void OnStnClickedStaticImage();
	afx_msg void OnHistogramRed();
	afx_msg void OnHistogramGreen();
	afx_msg void OnHistogramBlue();
	afx_msg void OnExitappExit();
	afx_msg void OnDirectionUp();
	afx_msg void OnDirectionDown();
	afx_msg void OnDirectionLeft();
	afx_msg void OnDirectionRight();
	afx_msg void OnImageBrighter();
	afx_msg void OnImageDarker();
	afx_msg void OnImageOriginal();
};

void CalculateHistogram(Img& img);
void AdjustImageBrightness(BitmapEffect* effect);