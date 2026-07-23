#pragma once

#include "WaxGl.h"
#include <afxhtml.h>


class CAvViewX;

/// Die Klasse DWFView bildet eine generische Klasse zur Anzeige von Grafikdaten im 
/// DWF Format und über das ActiveX Control von Voloview bzw. AutodeskViewer.
class WAXGLDLL DWFView : 
public CView  
{
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNCREATE(DWFView)
private:
	/// DWF Viewercontrol erstellen
	static UINT createDwfViewer(LPVOID waxgl);

	friend class WaxGl;

protected:
	/// Dynamische Erstellung verwendet geschützten Konstruktor
	DWFView();           
	

public:
	/// DWF Filename setzen. Damit wird das Control initialisiert. Nach der 
	/// Initialisierung wird die Datei angezeigt.
	void showFile(const CString& fname);

	/// Setzt die View Koordinaten der Grafikansicht auf Null, damit die View auf 
	/// maximale Koordinaten gesetzt werden

	/// Wax Schnittstelle zur Grafikanzeige per ActiveX Control
	WaxGl* waxGl;

	/// DWF Filename
	CString dwfFilename;

protected:
	virtual void OnDraw(CDC* pDC);      ///  Überschrieben zum Zeichnen dieser Ansicht
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	public:

protected:
	virtual ~DWFView();

	 BOOL OnEraseBkgnd( CDC* pDC );

	 void OnSize(UINT nType, int cx, int cy);
	
	
};

