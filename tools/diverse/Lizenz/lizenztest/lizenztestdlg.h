// lizenztestdlg.h : Headerdatei
//

#pragma once
#include "atlcomtime.h"
#include "stl.h"


// LizenztestDlg Dialogfeld
class LizenztestDlg : public CDialog
{
// Konstruktion
public:
	LizenztestDlg(CWnd* pParent = NULL);	// Standardkonstruktor

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV-Unterstützung

	/// Key aus den Eingaben erzeugen und anzeigen
	void on_keygen();

	/// Aus dem eingegebenen Key die Daten extrahieren und anzeigen
	void on_keyresolve();

protected:
	HICON m_hIcon;

	// Generierte Funktionen für die Meldungstabellen
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	COleDateTime mAblaufdatum;

	COleDateTime mBegin;

	string mHdkennung;

};
