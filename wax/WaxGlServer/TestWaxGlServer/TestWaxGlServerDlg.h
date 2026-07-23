// TestWaxGlServerDlg.h : Headerdatei
//

#pragma once
#include "afxwin.h"
#include "CWebBrowser.h"
#pragma warning(disable: 4005)

using namespace std;

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <deque>
#include <algorithm>
#include <functional>
#include <set>
#include <hash_map>


#ifdef UNICODE
	typedef std::wstring tstring;
	typedef std::wostringstream _tostringstream;
	#define LPCXSTR LPCWSTR
#else
	typedef std::string tstring;
	typedef std::ostringstream _tostringstream;
	#define LPCXSTR LPCSTR
#endif
// TestWaxGlServerDlg-Dialogfeld
class TestWaxGlServerDlg : public CDialog
{
	DECLARE_MESSAGE_MAP()

// Konstruktion
public:
	TestWaxGlServerDlg(CWnd* pParent = NULL);	// Standardkonstruktor

	virtual ~TestWaxGlServerDlg();

// Dialogfelddaten
	enum { IDD = IDD_TESTWAXGLSERVER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV-Unterstützung

	HICON m_hIcon;

	// Generierte Funktionen für die Meldungstabellen
	virtual BOOL OnInitDialog();
	void OnSysCommand(UINT nID, LPARAM lParam);
	void OnPaint();
	HCURSOR OnQueryDragIcon();
	
public:
	CListBox log;

	CComboBox fileformat;

	CWebBrowser viewer;

	void OnBnClickedConvert();

private:
	std::string load_xml(const string& file) const;

	std::string store_file(const string& content) const;

	void add_logstring(const string& str);

	afx_msg void OnBnClickedLoad();

	std::string xmlfile;
	
	std::string get_selected_fileformat() const;
};
