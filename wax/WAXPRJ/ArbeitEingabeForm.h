#pragma once

#include "WaxPartForm.h"
#include "..\rohrkonst\Element.h"
#include <mxcontrols/listctrl.h>

class Arbeitsfolge;

/// Eingabe von Arbeitsfolgen
/// AV spezifische Eingabemaske. Wird in der AV Version bei allen Werkaufträgen 
/// angezeigt, um den Arbeitsplan veränderbar zu machen.
class WAXPRJDLL ArbeitEingabeForm : public WaxPartForm  
{
	DECLARE_DYNCREATE(ArbeitEingabeForm)
	DECLARE_MESSAGE_MAP()

public:
	/// Anordnung der Spalten
	enum COLUMNS 
	{
		POSNR = 0,

		ANZ = 1,

		TEXT = 2,

		ARBPL = 3,

		PREIS = 4,

		ZEIT = 5,

		RUESTZEIT = 6,

		CODE = 7
	};

	/// Konstruktor
	ArbeitEingabeForm();

	/// Destruktor
	virtual ~ArbeitEingabeForm();

	virtual void updateSpecialAF(Arbeitsfolge* afo);


	/// Dialog initialisieren
	virtual BOOL OnInitDialog();
	/// Dialogdatenaustausch
	virtual void DoDataExchange(CDataExchange* pDX);
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	/// Initialisierung der Materialansicht (Spalten und Bezeichnungen)
	virtual void initList();
	
	bool m_bClearArbeitsfolgen;

	/// Wird aufgerufen, nachdem das Fenster in der Grösse geändert wurde
	void OnSize(UINT nType, int cx, int cy);
	/// Zeigt an, dass der Button Delete geklickt wurde
	void onClickedDelete();
	/// Zeigt an, dass der Button Edit geklickt wurde
	void onClickedEdit();
	/// Zeigt an, dass der Button Insert geklickt wurde
	void onClickedInsert();
	/// Zeigt an, dass im Baum doppelgeklickt wurde
	void onDblclkTree(NMHDR* pNMHDR, LRESULT* pResult);
	/// Zeigt an, dass der Benutzer die Editierung des Itemlabels beendet hat
	void onEndlabeleditList(NMHDR* pNMHDR, LRESULT* pResult);
	/// Zeigt an, dass im Baum Alt+Return gedrückt wurde
	void onReturnTree(NMHDR* pNMHDR, LRESULT* pResult);
	/// Called when CWnd is to be hidden or shown.
	void OnShowWindow(BOOL bShow, UINT nStatus);
	/// Indicates that one of the columns was clicked
	void onColumnclickList(NMHDR* pNMHDR, LRESULT* pResult);
	/// Indicates that the user has double-clicked the left mouse button in the 
	/// control
	void onDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	/// Indicates the user clicked a button
	void onClickedAuto();
	/// Indicates the user clicked a button
	void onClickedNew();
	/// Indicates that the user has clicked the right mouse button in the control
	void onRclickList(NMHDR* pNMHDR, LRESULT* pResult);
	void onClickedUp();
	void onClickedDown();
		
	/// Laden von Altwax Arbeitsplänen
	void onApax();
	
	/// Laden eines Arbeitsplans
	void onApaxLoad();
	
	/// Speichern eines Arbeitsplans
	void onApaxSave();
	
	void OnDestroy( );
	
private:
	/// Fensterelemente wurden initialisiert
	bool istInitialisiert;

	/// Initialisierung des Treecontrols mit den Arbeitsfolgen der Tabelle 
	/// Arbeitsfolgen aus den Stammdaten
	void treeInsertApAfos();

	/// Arbeitsfolge in Arbeitsplan einfügen
	void insertAFO();

	/// Vergleichsfunktion für die Sortierung der Liste nach 
	/// Positionsnummern
	static int CALLBACK ArbeitEingabeForm::CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

	void saveAfos(Arbeitsplan& a);

	void writeAfos(Arbeitsplan& a);

	/// Rechter Mausclick auf Liste kreiert ein Popupmenue
	//void createPopupMenu(CPoint point, int menu_items[]);

	/// Zeigt den Datei laden/speichern Dialog an und liefert den ausgewählten 
	/// Dateinamen zurück.
	CString getFileName(bool load, bool oldwax = false);

	/// Speichert den aktuellen Inhalt der Arbeitsfolgen, Materialliste und Sokos 
	/// in eine Datei
	bool storeApax(const CString & file);

	/// Lädt einen Wax+ Arbeitsplan
	bool loadApax(const CString& file);

protected:
	virtual bool editAfo();

	/// Löscht einen Eintrag in der Liste inklusive Arbeitsfolge, die dem Eintrag 
	/// über SetItemData() zugeordnet wurde.
	bool del_listentry(int index);

	void del_list();

	void write_list();
	
	/// Die Liste zeigt das eingefügte Material an
	ListCtrl list;
	
	/// Der Baum zeigt das Material aus den Stammdaten an
	CTreeCtrl tree;

	/// Afo lesen
	bool readArbeitsfolge(int index, Arbeitsfolge &afo);

	/// Afo schreiben
	void writeArbeitsfolge(const Arbeitsfolge& af, int index);

};
