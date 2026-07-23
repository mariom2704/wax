#include "stdafx.h"


#include "resource.h"
#include "ServerDbPa.h"
#include <afxcoll.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(ServerDbPa, ServerDBOpen)
END_MESSAGE_MAP()

ServerDbPa::~ServerDbPa()
{
}

ServerDbPa::ServerDbPa() //: ServerDBOpen(Status::Preisanfrage)
{
	m_lpszTemplateName = MAKEINTRESOURCE(IDD_LOAD_PA);
	panr = 0;
	this->sortPreisanfrage = true;
}

BOOL ServerDbPa::DestroyWindow()
{
	return CWnd::DestroyWindow();
}

const CString ServerDbPa::createSQLSelect() const
{
	CString sql;
	sql.Format("preisanfrageNr = %i", panr);
	return sql;
}

const CString ServerDbPa::createSQLSort(CString tablename) const
{
	return "preisanfragePos";
}

BOOL ServerDbPa::OnInitDialog()
{
	if (!CDialog::OnInitDialog())
		return false;

	
	// Unsichtbare Vorgabe muss gesetzt sein, damit die Basisklasse nicht aussteigt.
	((CButton*)(GetDlgItem(IDC_SORT_WA)))->SetCheck(1); 
	createProgressWnd();
	return true;
}

void ServerDbPa::DoDataExchange(CDataExchange* pDX)
{
	CWnd::DoDataExchange(pDX);

	
	DDX_Text(pDX, IDC_EDIT_PA, panr);
}

bool ServerDbPa::serialize_project(CObList* project)
{
	
	return ServerDBOpen::serialize_project(project);
}
