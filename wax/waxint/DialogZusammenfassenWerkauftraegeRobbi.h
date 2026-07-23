#pragma once
#include "resource.h"
#include "..\rohrkonst\WerkauftragListe.h"
#include "..\rohrkonst\Werkauftrag.h"
#include "GpUebergabeStrangrohrstrasse.h"
#include "afxcmn.h"

#include <mxcontrols\CtrlEnterEdit.h>

// DialogZusammenfassenWerkauftraegeRobbi-Dialogfeld

class DialogZusammenfassenWerkauftraegeRobbi : public CDialog
{
	DECLARE_DYNAMIC(DialogZusammenfassenWerkauftraegeRobbi)

private:
	WerkauftragListe werkauftraegeAnzeige;

	GpUebergabeStrangrohrstrasse	 *m_gpUebergabeStrasse;

	void fillListe();
	bool addWerkauftrag(const CUIntArray &arNewWa);

	void stringToInArray(const CString &str, CUIntArray &ar);

protected:	
	ListCtrl		m_ctrlWerkauftraege;

	CtrlEnterEdit	m_ctrlEdit;

	long			m_waNr;


	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung

	void createPopupMenu(CPoint point,bool isItemSelected);

	void addWaNrFromEditFeld();		//WA Nummer aus dem Editfeld an die Liste mit Werkaufträgen hängen


	//Eingaben überprüfen
	bool checkPlausis();

	enum COLUMNS 
	{
		WANR			= 0,
		BEZ				= 1,
		ANZMUFFEN		= 2,
		DNROHR			= 3,
		ROHRQUALI		= 4,
		MUFFENLAENGE	= 5,
		DNMUFFE			= 6
	};

	void OnPaste();
	void OnDelete();
	 LRESULT OnKeyDownWaEingabe(WPARAM, LPARAM lParam);
	 void OnClickedAddWaNr();
	 void OnBnClickedOk();
	 void OnNMRclickList1(NMHDR *pNMHDR, LRESULT *pResult);

	DECLARE_MESSAGE_MAP()

public:
	DialogZusammenfassenWerkauftraegeRobbi(CWnd* pParent = NULL);   // Standardkonstruktor
	virtual ~DialogZusammenfassenWerkauftraegeRobbi();
	
	virtual BOOL OnInitDialog();

	enum { IDD = IDD_ZUSAMMENFASSEN_WA_ROBBI };
	

	void set_werkauftragsListe(WerkauftragListe *liste);	//Zeiger auf Liste setzen

	void set_gpUebergabeStrStrasse(GpUebergabeStrangrohrstrasse *gpWa);

};
