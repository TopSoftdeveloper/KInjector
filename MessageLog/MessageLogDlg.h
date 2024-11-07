
// MessageLogDlg.h : header file
//

#pragma once
#define WM_UPDATE_SEARCH_PATH (WM_USER + 1)

// CMessageLogDlg dialog
class CMessageLogDlg : public CDialogEx
{
// Construction
public:
	CMessageLogDlg(CWnd* pParent = nullptr);	// standard constructor
	//void UpdateSearchPath(const std::string& newPath);

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MESSAGELOG_DIALOG };
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
	afx_msg LRESULT  OnUpdateSearchPath(WPARAM wParam, LPARAM lParam);
public:
	CString m_searchword;
	CString m_searchpath;
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
};
