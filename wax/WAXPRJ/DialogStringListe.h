#pragma once


#include "afxwin.h"


// DialogStringListe-Dialogfeld

class WAXPRJDLL DialogStringListe : public CDialog
{
	DECLARE_DYNAMIC(DialogStringListe)

public:
	DialogStringListe(CWnd* pParent = NULL);   // Standardkonstruktor

	virtual ~DialogStringListe();

// Dialogfelddaten
	//enum { IDD = IDD_STRINGLISTE };


	void setWerte(const CStringArray &liste,const CString &ueberschrift);

	/// Liefert true, wenn ein Objekt aktiv ist
	static bool is_active();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung

	CListCtrl m_ctrlStringliste;

	DECLARE_MESSAGE_MAP()

private:
	CStringArray m_fehlerListe;
	CString		 m_ueberschrift;

	static bool active;

public:
	virtual BOOL OnInitDialog();
};
