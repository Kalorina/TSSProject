
// TSS_cviko1Dlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "TSS_cviko1.h"
#include "TSS_cviko1Dlg.h"
#include "afxdialogex.h"
#include <stdio.h>
#include <thread>
#include <chrono> // for sleep fucntions
#include <gdiplus.h> 
using namespace Gdiplus;
#include "HistogramCalculation.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

ULONG_PTR gdiplusToken;

void CStaticImage::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStuct)
{
	GetParent()->SendMessage(WM_DRAW_IMAGE, (WPARAM)lpDrawItemStuct);
};

void CStaticHist::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStuct)
{
	GetParent()->SendMessage(WM_DRAW_HISTOGRAM, (WPARAM)lpDrawItemStuct);
};


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTSScviko1Dlg dialog


CTSScviko1Dlg::CTSScviko1Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TSS_CVIKO1_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME); 
}

void CTSScviko1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FILE_LIST, m_fileList);
	DDX_Control(pDX, IDC_STATIC_HISTOGRAM, m_staticHistogram);
	DDX_Control(pDX, IDC_STATIC_IMAGE, m_staticImage);
}

BEGIN_MESSAGE_MAP(CTSScviko1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_FILE_LIST, &CTSScviko1Dlg::OnLvnItemchangedFileList)
	ON_COMMAND(ID_FILE_OPEN32771, &CTSScviko1Dlg::OnFileOpen32771)
	ON_COMMAND(ID_FILE_CLOSE32772, &CTSScviko1Dlg::OnFileClose32772)
	//Messages
	ON_MESSAGE(WM_DRAW_IMAGE, OnDrawImage)
	ON_MESSAGE(WM_DRAW_HISTOGRAM, OnDrawHist)
	ON_MESSAGE(WM_HISTOGRAMCALCUCALTION_DONE, HistogramCalculationDone)

	ON_WM_SIZE()
	ON_WM_DRAWITEM()

	ON_NOTIFY(LVN_ITEMCHANGED, IDC_FILE_LIST, &CTSScviko1Dlg::OnLvnItemchangedFileList) // Linking the LVN_ITEMCHANGED event

	ON_STN_CLICKED(IDC_STATIC_IMAGE, &CTSScviko1Dlg::OnStnClickedStaticImage)
	ON_COMMAND(ID_HISTOGRAM_RED, &CTSScviko1Dlg::OnHistogramRed)
	ON_COMMAND(ID_HISTOGRAM_GREEN, &CTSScviko1Dlg::OnHistogramGreen)
	ON_COMMAND(ID_HISTOGRAM_BLUE, &CTSScviko1Dlg::OnHistogramBlue)
	ON_COMMAND(ID_EXITAPP_EXIT, &CTSScviko1Dlg::OnExitappExit)
	ON_COMMAND(ID_DIRECTION_UP, &CTSScviko1Dlg::OnDirectionUp)
	ON_COMMAND(ID_DIRECTION_DOWN, &CTSScviko1Dlg::OnDirectionDown)
	ON_COMMAND(ID_DIRECTION_LEFT, &CTSScviko1Dlg::OnDirectionLeft)
	ON_COMMAND(ID_DIRECTION_RIGHT, &CTSScviko1Dlg::OnDirectionRight)
	ON_COMMAND(ID_IMAGE_BRIGHTER, &CTSScviko1Dlg::OnImageBrighter)
	ON_COMMAND(ID_IMAGE_DARKER, &CTSScviko1Dlg::OnImageDarker)
END_MESSAGE_MAP()



// CTSScviko1Dlg message handlers

BOOL CTSScviko1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	m_menu = GetMenu();


	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	GetClientRect(&m_rect);
	m_fileList.GetWindowRect(&m_rectFileList);
	m_staticImage.GetWindowRect(&m_rectStaticImage);
	m_staticHistogram.GetWindowRect(&m_rectStaticHistogram);
	// GetWindowRect(&m_rect);
	// Presunutie height and width values from window to dialog values
	ScreenToClient(&m_rectFileList);
	ScreenToClient(&m_rectStaticImage);
	ScreenToClient(&m_rectStaticHistogram);

	// Initialize GDI+
	GdiplusStartupInput gdiplusStartupInput;
	if (GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr) != Ok) {
		AfxMessageBox(_T("Failed to initialize GDI+"));
		return FALSE; // Return FALSE to exit if GDI+ initialization fails
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTSScviko1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTSScviko1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTSScviko1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// handler function
void CTSScviko1Dlg::OnLvnItemchangedFileList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if (m_RedChecked || m_GreenChecked || m_BlueChecked)
	{
		StartHistogramCalculationForSelectedImage();
	}

	m_staticImage.Invalidate(FALSE); 
	m_staticHistogram.Invalidate(FALSE); 

	*pResult = 0;
}

void CTSScviko1Dlg::StartHistogramCalculationForSelectedImage()
{
	int selectedIndex = m_fileList.GetNextItem(-1, LVNI_SELECTED);

	if (selectedIndex != -1 && selectedIndex < m_imageList.size())
	{
		if ((m_RedChecked || m_GreenChecked || m_BlueChecked) &&
			!m_imageList[selectedIndex].isHistCalculating &&
			!m_imageList[selectedIndex].isHistCalculated)
		{
			m_imageList[selectedIndex].isHistCalculating = true;

			std::thread thread_histogram([this, selectedIndex]() {

				auto selected = m_imageList[selectedIndex];
				CalculateHistogram(m_imageList[selectedIndex]);
				selected.isHistCalculated = true;
				PostMessage(WM_HISTOGRAMCALCUCALTION_DONE);
				});

			thread_histogram.detach();
		}
	}
}

void CalculateHistogram(Img& img)
{
	std::this_thread::sleep_for(std::chrono::seconds(1));
	// Simulate delay

	if (!img.bitmap) return;

	Gdiplus::Bitmap* bitmap = static_cast<Gdiplus::Bitmap*>(img.bitmap);

	int width = bitmap->GetWidth();
	int height = bitmap->GetHeight();

	// Lock the bitmap for direct memory access
	Gdiplus::Rect rect(0, 0, width, height);
	BitmapData bitmapData;

	// 32bppARGB format -> each pixel has 4 bytes (ARGB)
	if (bitmap->LockBits(&rect, ImageLockModeRead, PixelFormat32bppARGB, &bitmapData) == Ok)
	{

		BYTE* pixels = static_cast<BYTE*>(bitmapData.Scan0);
		std::vector<BYTE> pixelData(pixels, pixels + height * bitmapData.Stride);

		CalculateHistogramFromPixelData(pixelData, width, height, img.redChannel, img.greenChannel, img.blueChannel);

		// Unlock the bitmap after processing
		bitmap->UnlockBits(&bitmapData);
	}
}

void CTSScviko1Dlg::DisplayListControl()
{
	m_fileList.DeleteAllItems();

	for (int i = 0; i < m_imageList.size(); ++i)
	{
		m_fileList.InsertItem(i, m_imageList[i].filename);
	}

	if (m_imageList.size() > 0)
	{
		m_fileList.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
		
		//m_staticImage.Invalidate(FALSE);
		Invalidate(FALSE);
	}
}

void CTSScviko1Dlg::OnFileOpen32771()
{
	// TODO: Add your command handler code here
	// into struct of image add GDI+ Image* 

	CString	filename;
	CString filepath;

	TCHAR fileTypeFilters[] = _T("Files (*.bmp;*.png;*.jpg)|*.bmp;*.png;*.jpg||");

	CFileDialog dlg(TRUE, _T(""), _T(""), OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT, fileTypeFilters);

	if (dlg.DoModal() == IDOK)
	{
		POSITION pos = dlg.GetStartPosition();

		while (pos != NULL)
		{
			CString filepath = dlg.GetNextPathName(pos);
			// Get just the file name from the full path
			CString filename = filepath.Mid(filepath.ReverseFind('\\') + 1);

			bool imageExists = false; // Flag to check if the image already exists

			for (const auto& img : m_imageList)
			{
				if (img.filepath == filepath && img.filename == filename)
				{
					imageExists = true; 
					CString message;
					message.Format(_T("The file \"%s\" is already opened."), filename);
					AfxMessageBox(message);
					break; 
				}
			}

			if (!imageExists)
			{
				Img image; 
				image.filename = filename;
				image.filepath = filepath; 
				image.bitmap = Gdiplus::Image::FromFile(filepath);
				m_imageList.push_back(image);
				
				DisplayListControl();
			}

		}
	}
	else
		return;
}

void CTSScviko1Dlg::OnFileClose32772()
{
	POSITION pos = m_fileList.GetFirstSelectedItemPosition();

	// Check if any item is selected
	if (pos != NULL)
	{
		int selectedIndex = m_fileList.GetNextSelectedItem(pos);

		CString selectedFile = m_fileList.GetItemText(selectedIndex, 0);

		CString message;
		message.Format(_T("Are you sure you want to close the file: %s?"), selectedFile);
		int response = AfxMessageBox(message, MB_YESNO | MB_ICONQUESTION);

		// If the user clicked 'Yes', proceed to close the file
		if (response == IDYES)
		{
			CString filenameToDelete = m_fileList.GetItemText(selectedIndex, 0);

			bool imageFound = false;

			for (size_t i = 0; i < m_imageList.size(); ++i)
			{
				if (m_imageList[i].filename.CompareNoCase(filenameToDelete) == 0)
				{
					m_imageList.erase(m_imageList.begin() + i);
					break; // Exit the loop once the item is found and deleted
				}
			}

			DisplayListControl();
			Invalidate(TRUE); 
		}
	}
	else
	{
		// No item is selected
		AfxMessageBox(_T("No image selected to close."));
	}
}

void CTSScviko1Dlg::OnSize(UINT nType, int cx, int cy)
{
	/*CDialogEx::OnSize(nType, cx, cy);

	int nDiffY = cy - m_rect.Height();
	int nDiffX = cx - m_rect.Width();

	if (::IsWindow(m_fileList) && ::IsWindow(m_staticImage) && ::IsWindow(m_staticHistogram)) {

		m_fileList.SetWindowPos(nullptr, 0, 0, m_rectFileList.Width(), m_rectFileList.Height() + nDiffY, SWP_NOMOVE);

		m_staticImage.SetWindowPos(nullptr, 0, 0, m_rectStaticImage.Width() + nDiffX, m_rectStaticImage.Height() + nDiffY, SWP_NOMOVE);

		int histDiff = m_rectFileList.Height() - m_rectStaticHistogram.Height();
		int histRight = cy - m_rectStaticHistogram.Height() - m_rectFileList.Height();

		m_staticHistogram.SetWindowPos(nullptr, m_rectFileList.Width(), m_rectFileList.Height() + nDiffY, m_rectFileList.Width() + nDiffX, m_rectFileList.Height() + nDiffY + m_rectStaticHistogram.Height(), SWP_NOMOVE);
	}

	Invalidate(TRUE);
	*/

	CDialogEx::OnSize(nType, cx, cy);

	int nDiffY = cy - m_rect.Height();
	int nDiffX = cx - m_rect.Width();

	if (::IsWindow(m_fileList) && ::IsWindow(m_staticImage) && ::IsWindow(m_staticHistogram))
	{
		m_fileList.SetWindowPos(nullptr, 0, 0, m_rectFileList.Width(), m_rectFileList.Height() + nDiffY, SWP_NOMOVE);
		m_staticImage.SetWindowPos(nullptr, 0, 0, m_rectStaticImage.Width() + nDiffX, m_rectStaticImage.Height() + nDiffY, SWP_NOMOVE);
		
		m_staticHistogram.SetWindowPos(nullptr, m_rectStaticHistogram.left, m_rectStaticHistogram.top + nDiffY, m_rectStaticHistogram.Width(), m_rectStaticHistogram.Height(), SWP_NOSIZE);
	}

	Invalidate(TRUE);

}

void CTSScviko1Dlg::DrawHistogramChannel(Gdiplus::Graphics* gr, const std::vector<int>& channel, Gdiplus::Pen& pen, float xScale, int maxHeight, int height) {

	Gdiplus::PointF* Points = new Gdiplus::PointF[channel.size()];
	for (int i = 0; i < channel.size(); i++) {
		float scaledX = i * xScale;  // Scale X-coordinate
		float barHeight = static_cast<float>(channel[i]) / maxHeight * height;
		Points[i] = Gdiplus::PointF(scaledX, height - barHeight); // Invert Y-axis
	}
	gr->DrawCurve(&pen, Points, channel.size());
	delete[] Points;
}

LRESULT CTSScviko1Dlg::OnDrawImage(WPARAM wParam, LPARAM lParam)
{
	LPDRAWITEMSTRUCT st = (LPDRAWITEMSTRUCT)wParam;
	Gdiplus::Graphics gr(st->hDC);

	if (m_imageList.empty())
		return -1;

	int selectedIndex = m_fileList.GetNextItem(-1, LVNI_SELECTED);

	if (selectedIndex == -1 || selectedIndex >= m_imageList.size())
		return S_OK;

	Gdiplus::Image* pImage = m_imageList[selectedIndex].bitmap;

	if (pImage && pImage->GetLastStatus() == Gdiplus::Ok)
	{
		CRect rect;
		m_staticImage.GetClientRect(&rect);

		float imageAspectRatio = static_cast<float>(pImage->GetWidth()) / static_cast<float>(pImage->GetHeight());
		float controlAspectRatio = static_cast<float>(rect.Width()) / static_cast<float>(rect.Height());

		float scaleFactor;
		if (imageAspectRatio > controlAspectRatio)
		{
			scaleFactor = static_cast<float>(rect.Width()) / pImage->GetWidth();
		}
		else
		{
			scaleFactor = static_cast<float>(rect.Height()) / pImage->GetHeight();
		}

		int scaledWidth = static_cast<int>(pImage->GetWidth() * scaleFactor);
		int scaledHeight = static_cast<int>(pImage->GetHeight() * scaleFactor);

		int xPos = (rect.Width() - scaledWidth) / 2;   // Center horizontally
		int yPos = (rect.Height() - scaledHeight) / 2; // Center vertically

		gr.Clear(Gdiplus::Color::White);

		gr.DrawImage(pImage, xPos, yPos, scaledWidth, scaledHeight);
	}

	return S_OK;
}

LRESULT CTSScviko1Dlg::OnDrawHist(WPARAM wParam, LPARAM lParam)
{
	LPDRAWITEMSTRUCT st = (LPDRAWITEMSTRUCT)wParam;

	auto gr = Gdiplus::Graphics::FromHDC(st->hDC); 

	if (m_imageList.empty())
		return S_OK;

	// If no channels are checked, clear the histogram display and return
	if (!m_RedChecked && !m_GreenChecked && !m_BlueChecked) {
		gr->Clear(Gdiplus::Color(255, 255, 255)); 
		return S_OK;
	}

	int selectedIndex = m_fileList.GetNextItem(-1, LVNI_SELECTED);

	if (selectedIndex == -1 || selectedIndex >= m_imageList.size())
		return S_OK;

	Img& currentImg = m_imageList[selectedIndex];

	Gdiplus::Pen redPen(Gdiplus::Color(255, 255, 0, 0), 2);   
	Gdiplus::Pen greenPen(Gdiplus::Color(255, 0, 255, 0), 2); 
	Gdiplus::Pen bluePen(Gdiplus::Color(255, 0, 0, 255), 2);  
	
	int maxHeight = 0;
	gr->Clear(Gdiplus::Color(255, 255, 255));
	
	// Set drawing dimensions
	int width = st->rcItem.right - st->rcItem.left;
	int height = st->rcItem.bottom - st->rcItem.top;

	// Calculate maximum width value for scaling (X-axis)
	float xScale = static_cast<float>(width) / 256.0f;  // 256 is the number of histogram bins
	
	// the maximum Height value for scaling (Y-axis)
	if (m_RedChecked)
	{	
		for (int i = 0; i < currentImg.redChannel.size(); i++) {
			if (currentImg.redChannel[i] > maxHeight) {
				maxHeight = currentImg.redChannel[i];
			}
		}
	}
	if (m_GreenChecked) 
	{
		for (int i = 0; i < currentImg.greenChannel.size(); i++) {
			if (currentImg.greenChannel[i] > maxHeight) {
				maxHeight = currentImg.greenChannel[i];
			}
		}
	}
	if (m_BlueChecked) 
	{
		for (int i = 0; i < currentImg.blueChannel.size(); i++) {
			if (currentImg.blueChannel[i] > maxHeight) {
				maxHeight = currentImg.blueChannel[i];
			}
		}
	}

	if (m_RedChecked) 
		DrawHistogramChannel(gr, currentImg.redChannel, redPen, xScale, maxHeight, height);
	
	if (m_GreenChecked) 
		DrawHistogramChannel(gr, currentImg.greenChannel, greenPen, xScale, maxHeight, height);
	
	if (m_BlueChecked) 
		DrawHistogramChannel(gr, currentImg.blueChannel, bluePen, xScale, maxHeight, height);

	return S_OK;
}

LRESULT CTSScviko1Dlg::HistogramCalculationDone(WPARAM wParam, LPARAM lParam)
{
	m_staticImage.Invalidate(FALSE); //display image 
	m_staticHistogram.Invalidate(FALSE); //display histogram channels
	
	return S_OK;
}

void CTSScviko1Dlg::OnStnClickedStaticImage()
{
	// TODO: Add your control notification handler code here
}

void CTSScviko1Dlg::OnHistogramRed()
{
	m_RedChecked = !m_RedChecked;
	StartHistogramCalculationForSelectedImage();

	if (m_RedChecked) 
	{
		m_menu->CheckMenuItem(ID_HISTOGRAM_RED, MF_CHECKED | MF_BYCOMMAND);
	}
	else 
	{
		m_menu->CheckMenuItem(ID_HISTOGRAM_RED, MF_UNCHECKED | MF_BYCOMMAND);
	}
	m_staticHistogram.Invalidate(FALSE);
}

void CTSScviko1Dlg::OnHistogramGreen()
{
	m_GreenChecked = !m_GreenChecked;
	StartHistogramCalculationForSelectedImage();
	if (m_GreenChecked) 
	{
		m_menu->CheckMenuItem(ID_HISTOGRAM_GREEN, MF_CHECKED | MF_BYCOMMAND);
	}
	else 
	{
		m_menu->CheckMenuItem(ID_HISTOGRAM_GREEN, MF_UNCHECKED | MF_BYCOMMAND);
	}
	m_staticHistogram.Invalidate(FALSE);
}

void CTSScviko1Dlg::OnHistogramBlue()
{
	m_BlueChecked = !m_BlueChecked;
	StartHistogramCalculationForSelectedImage();
	if (m_BlueChecked) 
	{
		m_menu->CheckMenuItem(ID_HISTOGRAM_BLUE, MF_CHECKED | MF_BYCOMMAND);
	}
	else 
	{
		m_menu->CheckMenuItem(ID_HISTOGRAM_BLUE, MF_UNCHECKED | MF_BYCOMMAND);
	}
	m_staticHistogram.Invalidate(FALSE);
}

void CTSScviko1Dlg::OnExitappExit()
{
	if (AfxMessageBox(_T("Are you sure you want to exit?"), MB_YESNO | MB_ICONQUESTION) == IDNO)
	{
		return; // User chose not to close the dialog
	}

	// if you're using GDI+, make sure to shut it down
	GdiplusShutdown(gdiplusToken);


	CDialogEx::OnOK();  // or CDialogEx::OnCancel();
}

void CTSScviko1Dlg::OnDirectionUp()
{
	m_DirectionUp = !m_DirectionUp;

	if (m_DirectionUp)
	{
		m_menu->CheckMenuItem(ID_DIRECTION_UP, MF_CHECKED | MF_BYCOMMAND);
	}
	else
	{
		m_menu->CheckMenuItem(ID_DIRECTION_UP, MF_UNCHECKED | MF_BYCOMMAND);
	}
}

void CTSScviko1Dlg::OnDirectionDown()
{
	m_DirectionDown = !m_DirectionDown;

	if (m_DirectionDown)
	{
		m_menu->CheckMenuItem(ID_DIRECTION_DOWN, MF_CHECKED | MF_BYCOMMAND);
	}
	else
	{
		m_menu->CheckMenuItem(ID_DIRECTION_DOWN, MF_UNCHECKED | MF_BYCOMMAND);
	}
}

void CTSScviko1Dlg::OnDirectionLeft()
{
	m_DirectionLeft = !m_DirectionLeft;

	if (m_DirectionLeft)
	{
		m_menu->CheckMenuItem(ID_DIRECTION_LEFT, MF_CHECKED | MF_BYCOMMAND);
	}
	else
	{
		m_menu->CheckMenuItem(ID_DIRECTION_LEFT, MF_UNCHECKED | MF_BYCOMMAND);
	}
}

void CTSScviko1Dlg::OnDirectionRight()
{
	m_DirectionRight = !m_DirectionRight;

	if (m_DirectionRight)
	{
		m_menu->CheckMenuItem(ID_DIRECTION_RIGHT, MF_CHECKED | MF_BYCOMMAND);
	}
	else
	{
		m_menu->CheckMenuItem(ID_DIRECTION_RIGHT, MF_UNCHECKED | MF_BYCOMMAND);
	}
}

void CTSScviko1Dlg::OnImageBrighter()
{
	m_Brighter = !m_Brighter;
	if(m_Brighter) 
		m_Darker = FALSE;

	if (m_Brighter)
	{
		m_menu->CheckMenuItem(ID_IMAGE_BRIGHTER, MF_CHECKED | MF_BYCOMMAND);
	}
	else
	{
		m_menu->CheckMenuItem(ID_IMAGE_BRIGHTER, MF_UNCHECKED | MF_BYCOMMAND);
	}

	if (m_Darker)
	{
		m_menu->CheckMenuItem(ID_IMAGE_DARKER, MF_CHECKED | MF_BYCOMMAND);
	}
	else
	{
		m_menu->CheckMenuItem(ID_IMAGE_DARKER, MF_UNCHECKED | MF_BYCOMMAND);
	}
}

void CTSScviko1Dlg::OnImageDarker()
{
	m_Darker = !m_Darker;
	if (m_Darker)
		m_Brighter = FALSE;

	if (m_Darker)
	{
		m_menu->CheckMenuItem(ID_IMAGE_DARKER, MF_CHECKED | MF_BYCOMMAND);
	}
	else
	{
		m_menu->CheckMenuItem(ID_IMAGE_DARKER, MF_UNCHECKED | MF_BYCOMMAND);
	}
	if (m_Brighter)
	{
		m_menu->CheckMenuItem(ID_IMAGE_BRIGHTER, MF_CHECKED | MF_BYCOMMAND);
	}
	else
	{
		m_menu->CheckMenuItem(ID_IMAGE_BRIGHTER, MF_UNCHECKED | MF_BYCOMMAND);
	}
}
