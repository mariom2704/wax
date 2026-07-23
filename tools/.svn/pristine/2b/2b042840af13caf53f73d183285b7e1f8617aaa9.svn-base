#pragma once

#ifdef _WIN32_WCE
#error "CHtmlView wird für Windows CE nicht unterstützt."
#endif 

// CJScriptViewTemplate HTML-Ansicht

class CJScriptViewTemplate : public CHtmlView
{
	DECLARE_DYNCREATE(CJScriptViewTemplate)

protected:
	CJScriptViewTemplate();           // Dynamische Erstellung verwendet geschützten Konstruktor
	virtual ~CJScriptViewTemplate();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
};


