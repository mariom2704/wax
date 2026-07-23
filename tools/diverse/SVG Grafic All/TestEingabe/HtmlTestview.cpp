// HtmlTestview.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "SplitterWindowTutorial.h"
#include "HtmlTestview.h"


// CJScriptViewTemplate

IMPLEMENT_DYNCREATE(CJScriptViewTemplate, CHtmlView)

CJScriptViewTemplate::CJScriptViewTemplate()
{

}

CJScriptViewTemplate::~CJScriptViewTemplate()
{
}

void CJScriptViewTemplate::DoDataExchange(CDataExchange* pDX)
{
	CHtmlView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CJScriptViewTemplate, CHtmlView)
END_MESSAGE_MAP()


// CJScriptViewTemplate-Diagnose

#ifdef _DEBUG
void CJScriptViewTemplate::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CJScriptViewTemplate::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}
#endif //_DEBUG


// CJScriptViewTemplate-Meldungshandler


void CJScriptViewTemplate::OnInitialUpdate()
{
	SetSilent(TRUE);
}
