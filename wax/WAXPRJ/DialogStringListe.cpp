// DialogStringListe.cpp : Implementierungsdatei
//

#include "stdafx.h"


#include "resource.h"

#include "DialogStringListe.h"
#include ".\dialogstringliste.h"

bool DialogStringListe::active = false;

// DialogStringListe-Dialogfeld

IMPLEMENT_DYNAMIC(DialogStringListe, CDialog)
DialogStringListe::DialogStringListe(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_STRINGLISTE, pParent)
{
	active = true;
}

DialogStringListe::~DialogStringListe()
{
	active = false;
}

bool DialogStringListe::is_active()
{
	return active;
}

void DialogStringListe::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ctrlStringliste);
}


BEGIN_MESSAGE_MAP(DialogStringListe, CDialog)
END_MESSAGE_MAP()


// DialogStringListe-Meldungshandler

void DialogStringListe::setWerte(const CStringArray &liste,const CString &ueberschrift)
{
	m_fehlerListe.Copy(liste);

	m_ueberschrift = ueberschrift;
}

BOOL DialogStringListe::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  hier zusätzliche Initialisierung hinzufügen.

	SetWindowText(m_ueberschrift);

	m_ctrlStringliste.InsertColumn(0, "Fehlerbeschreibung", 0, 800);
	//werte für Liste zuweisen
	for (int i=0; i<m_fehlerListe.GetCount(); i++)
	{
		m_ctrlStringliste.InsertItem(i, m_fehlerListe[i]);
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	// AUSNAHME: OCX-Eigenschaftenseite muss FALSE zurückgeben.
}
