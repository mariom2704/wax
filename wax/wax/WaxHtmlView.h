#pragma once

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include <afxcoll.h>

#include "HtmlJScriptCoreView.h"

#include <afxhtml.h>

/**
Ableitung von CHtmlView zur Anzeige von Html Druckausgaben
Die Druckausgaben in Wax bestehen aus generierten Html Dateien. Durch 
diese View können die Dateien angezeigt und ausgedruckt werden.
Für die Anzeige der DWF Dateien sind im Moment Voloview und Zing über 
Systemparameter wählbar.
*/
class WaxSplitterWnd;
class CChildFrame;
class WaxHtmlView : public CHtmlJScriptCoreView  
{
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNCREATE(WaxHtmlView)

private:
	/// print document
	void onPrintDirect();


protected:
	/// Geschützter Konstruktor wird von dynamischer Erstellung verwendet
	WaxHtmlView();           

	/// Destruktor
	virtual ~WaxHtmlView();

	/// Gibt alle Messages an LeftView weiter, damit z.B. das Drucken richtig 
	/// aufgerufen wird.
	void HandleMsgToLeftView(UINT cmd);

	/// Flag, ob View schon initialisiert wurde
	bool m_bInitialUpdate;

	

public:
	
	
	
	void SetInitializeTab(bool bInitialUpdate)
	{
		m_bInitialUpdate = bInitialUpdate;
	}

	virtual void OnUpdate(CView* pSender, LPARAM /*lHint*/, CObject* /*pHint*/);

	/// Erzeugt aus der filelist eine Html Gesamtdatei und liefert den Dateinamen 
	/// zurück
	//CString createAllPages();

	/// Setzt alle Einzelseiten zu einer grossen Html Datei zusammen und zeigt sie 
	/// an.
	// void onShowAllPages();

	/// Erzeugt die Druckausgabedatei(en)
	CString createHtmlFile(CString zielstring = "");

	

	

	public:
	/// OnInitialUpdate wird von der MFC leider nicht aufgerufen, wenn die View in 
	/// einem Splitterwindow erzeugt wird. Deshalb wird die Methode in 
	/// DoDataExchange aufgerufen, wenn initialUpdate false ist.
	virtual void OnInitialUpdate();
	
	protected:
	/// Datenaustausch und Initialisierung
	virtual void DoDataExchange(CDataExchange* pDX);    ///  DDX/DDV-Unterstützung

};
