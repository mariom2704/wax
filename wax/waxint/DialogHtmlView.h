#pragma once

#include "resource.h"
#include "SimpleBrowser.h"

// DialogHtmlView-Dialogfeld

class DialogHtmlView : public CDialog
{
	DECLARE_DYNAMIC(DialogHtmlView)

public:
	DialogHtmlView(CWnd* pParent = NULL);   // Standardkonstruktor
	virtual ~DialogHtmlView();

// Dialogfelddaten
	enum { IDD = IDD_HTMLVIEW_PRINT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung

	virtual bool printHtmlView();

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	void set_url(const CString &url);
private:
	SimpleBrowser	m_browser;
	CString			m_url;

public:
	 void OnBnClickedOk();
};
