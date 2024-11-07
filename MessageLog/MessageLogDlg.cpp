
// MessageLogDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "MessageLog.h"
#include "MessageLogDlg.h"
#include "afxdialogex.h"
#include <string>
#include <sstream>
#include <iostream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CMessageLogDlg dialog



CMessageLogDlg::CMessageLogDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MESSAGELOG_DIALOG, pParent)
	, m_searchword(_T(""))
	, m_searchpath(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMessageLogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_searchword);
	DDX_Text(pDX, IDC_EDIT2, m_searchpath);
}

BEGIN_MESSAGE_MAP(CMessageLogDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_UPDATE_SEARCH_PATH, &CMessageLogDlg::OnUpdateSearchPath)
	ON_BN_CLICKED(IDCANCEL, &CMessageLogDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CMessageLogDlg::OnBnClickedOk)
END_MESSAGE_MAP()

//PIPE
#define SEARCH_PIPE_NAME "\\\\.\\pipe\\SEARCHDEBUGLOG" // Replace with your keyboard pipe name
HANDLE g_Pipe = SEARCH_PIPE_NAME;
DWORD WINAPI ReadFromPipelineThread(LPVOID lpParam)
{
	CMessageLogDlg* pThis = static_cast<CMessageLogDlg*>(lpParam);

	do
	{
		g_Pipe = CreateFile(SEARCH_PIPE_NAME, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
		Sleep(3000);
	} while (g_Pipe == INVALID_HANDLE_VALUE);

	DWORD dwMode = PIPE_READMODE_MESSAGE;
	if (!SetNamedPipeHandleState(
		g_Pipe, // pipe handle
		&dwMode,   // new pipe mode
		NULL,	   // don't set maximum bytes
		NULL)	   // don't set maximum time
		)
	{
		DWORD errorcode = GetLastError();
		char bufferlog[2048];
		sprintf(bufferlog, "SetNamedPipeHandleState is failed, %d", errorcode);
	}

	while (1)
	{
		if (g_Pipe != INVALID_HANDLE_VALUE)
		{
			char buffer[1024];
			DWORD bytesRead;

			while (ReadFile(g_Pipe, buffer, sizeof(buffer) - 1, &bytesRead, NULL) != 0)
			{
				PostMessage(pThis->m_hWnd, WM_UPDATE_SEARCH_PATH, reinterpret_cast<WPARAM>(buffer), static_cast<LPARAM>(static_cast<LONG>(bytesRead)));
			}
		}

		Sleep(1000);
	}

	return 0;
}

bool checkProcessRunning()
{
	HANDLE hMutexOneInstance(::CreateMutex(NULL, TRUE, "{15784090-9F01-3A2F-B1A5-BBEB1D76178F}"));
	bool bAlreadyRunning((::GetLastError() == ERROR_ALREADY_EXISTS));

	if (hMutexOneInstance == NULL || bAlreadyRunning)
	{
		if (hMutexOneInstance)
		{
			::ReleaseMutex(hMutexOneInstance);
			::CloseHandle(hMutexOneInstance);
		}
		return true;
	}
	return false;
}

// CMessageLogDlg message handlers

BOOL CMessageLogDlg::OnInitDialog()
{
	BOOL flag = true;
	flag = checkProcessRunning();
	if (flag == true)
	{
		ExitProcess(-1);
	}

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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	CreateThread(NULL, 0, ReadFromPipelineThread, this, 0, NULL);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMessageLogDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMessageLogDlg::OnPaint()
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
HCURSOR CMessageLogDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// Message handler for updating the UI
LRESULT  CMessageLogDlg::OnUpdateSearchPath(WPARAM wParam, LPARAM lParam) {

	char* buffer = reinterpret_cast<char*>(wParam);
	DWORD length = static_cast<LONG>(lParam);
	char* token;

	char stringbuffer[1024] = {0};
	strncpy(stringbuffer, buffer, length);
	
	token = strtok(stringbuffer, "&");
	if (token != nullptr) {
		ShowWindow(SW_SHOW);

		std::cout << "Part 1: " << token << std::endl; // Output first part
	}
	m_searchword = token;

	token = strtok(nullptr, "&"); // Continue splitting
	if (token != nullptr) {
		std::cout << "Part 2: " << token << std::endl; // Output second part
	}
	m_searchpath = token;

	UpdateData(FALSE); // Update controls with new data
	return 0; // Return a value
}



void CMessageLogDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	//CDialogEx::OnCancel();

	ShowWindow(SW_HIDE);
}


void CMessageLogDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	ShowWindow(SW_HIDE);
}
