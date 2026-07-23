
// xHMTLtestsuiteDoc.cpp: Implementierung der Klasse CxHMTLtestsuiteDoc
//

#include "stdafx.h"
#include "xHMTLtestsuite.h"

#include "xHMTLtestsuiteDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CxHMTLtestsuiteDoc

IMPLEMENT_DYNCREATE(CxHMTLtestsuiteDoc, CDocument)

BEGIN_MESSAGE_MAP(CxHMTLtestsuiteDoc, CDocument)
END_MESSAGE_MAP()


// CxHMTLtestsuiteDoc-Erstellung/Zerstörung

CxHMTLtestsuiteDoc::CxHMTLtestsuiteDoc()
{
	// TODO: Hier Code für One-Time-Konstruktion einfügen

}

CxHMTLtestsuiteDoc::~CxHMTLtestsuiteDoc()
{
}

BOOL CxHMTLtestsuiteDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: Hier Code zur Reinitialisierung einfügen
	// (SDI-Dokumente verwenden dieses Dokument)

	return TRUE;
}




// CxHMTLtestsuiteDoc-Serialisierung

void CxHMTLtestsuiteDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: Hier Code zum Speichern einfügen
	}
	else
	{
		// TODO: Hier Code zum Laden einfügen
	}
}


// CxHMTLtestsuiteDoc-Diagnose

#ifdef _DEBUG
void CxHMTLtestsuiteDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CxHMTLtestsuiteDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CxHMTLtestsuiteDoc-Befehle
