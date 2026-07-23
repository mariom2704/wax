#pragma once


class WaxPart;
class Rohrleitung;

#define CB_NOTIFIERS(id) \
	ON_EN_KILLFOCUS(id, doUpdate) \
	ON_CBN_KILLFOCUS(id, doUpdate) \
	ON_CBN_SELCHANGE(id, doUpdate)


///  Hier bitte die Userdefined Messages einbauen, damit nicht aus Versehen zwei gleiche 
///  definiert werden
#define WM_DNUPDATE  (WM_USER+814)
#define WM_COMMITUPDATE (WM_USER+815)


typedef struct _menu_Items
	{
		int id;
		string name;
		
	} menu_items;

#define ADD_MENU_ITEM(aa) { ##aa, ""#aa""},

/**
Basisklasse zur Bearbeitung eines WaxParts
Diese Klasse implementiert die Basisfunktionalität einer Bearbeitungsklasse für 
ein WaxPart. Wichtigste Aufgabe ist die Erstellung des Dialogs über dlgid. 
Diese ID muss in einer abgeleiteten Klasse gesetzt sein, damit die 
Dialogerstellung funktionieren kann.
*/
class WAXPRJDLL WaxPartForm : 
public CDialog  
{
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNCREATE(WaxPartForm)
public:
	/// virtuelle Methode, nach einer Prüfung zurückliefert, ob der Tab eine 
	/// Kennzeichnung erhalten soll. Siehe PrjAnhangForm.
	virtual int get_highlight() const;

	
		
	/// Prüft die Werkaufträge auf Änderungen und setzt das Flag, wenn nötig. Wird 
	/// vom Destruktor und TabCtrlView aufgerufen.
	void check_changes();

	virtual ~WaxPartForm();
	/// Plausibilitätscheck, wird aufgerufen, bevor UpdateData(TRUE) aufgerufen 
	/// wird.
	virtual bool check_plausis() ;

	WaxPart* get_data() ;

	const WaxPart* get_data() const;

	inline void set_data(WaxPart* value);

	/// Zugriff auf das Documentobjekt
	 CDocument* get_document() const;

	inline void set_document(CDocument* value);

	/// Name des Dialogs zur Anzeige im Tab
	const CString get_name(bool left = true) const;

	/// Fenster/Dialog erzeugen
	void createForm(CWnd *pParent, WaxPart* pData, CDocument *pDocument);

	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);


	virtual BOOL OnInitDialog();

	/// Initialisiert den CRC zu Prüfung auf Änderungen
	void initCrc();

	/// Setzen des DIN String je nach ausgewählter DN
	static int rohrquali_autoselect(CComboBox* dn, CComboBox* din);
		virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	/// Zugriff auf die Rohrleitung, CNotSupportedException im Fehlerfall
	Rohrleitung* get_rohrleitung();

	BOOL OnEraseBkgnd(CDC* pDC);


	virtual void DoDataExchange(CDataExchange* pDX);    ///  DDX/DDV-Unterstützung
		
	virtual void PreSubclassWindow();

	/// wie vor
	int rohrquali_autoselect(UINT id_dn, UINT id_din);

	/// Prüfung auf korrekte Initialisierung
	bool validWerkauftrag() const;

	/// Flag für die Zwischenspeicherung des AV Flags. 
	bool av;

	bool dn2zoll;

	/// Postet eine Updatemessage, die bewirkt, dass commitUpdate aufgerufen wird. 
	/// Dadurch wird erreicht, dass bei Comboboxen erst die internen Daten 
	/// aktualisiert werden und dann erst ein UpdateData erfolgt.
	void doUpdate();

	/// wie vor
	void do_update_rohr_dn();


	/// Sorgt für ein Auslesen der Dialogelemente und Neuerzeugen der Grafik, wenn 
	/// checkPlausis true liefert.
	virtual void commitUpdate();

	/// Standardmethode zum Lesen und Setzen der Anzahl
	void ddx_anzahl(CDataExchange* pDX, UINT id);

	/// Standardmethode zum Lesen und Setzen der Positionsnummer
	void ddx_posnr(CDataExchange * pDX, UINT id);

	/// Geschützter Konstruktor
	WaxPartForm(UINT idd = 0, const CString& chapter = "", CWnd* pParent = NULL); 

	virtual INT_PTR OnToolHitTest( CPoint point, TOOLINFO* pTI ) const;
	
	/// Setzt disableCheckPlausis auf true
	void disableCheck();

	BOOL DisableAllControls();

	void set_disableCheckPlausis(bool wert);

	/// Initialisierung einer TV_INSERTSTRUCT Struktur zum Einfügen in 
	/// einen Treecontrol.
	TV_INSERTSTRUCT makeInsertStruct(HTREEITEM lastitem, UINT image, const CString& titel, BOOL hasChildren, void* code) ;


	virtual void OnOK();

	/// Setzt eine neue Rohrdn, ändert den Text im Din-Feld bei Bedarf und liefert die neue dn zurück
	int selchange_rohr_dn(int iddn, int iddin);

	void setWindowsText(int id, const string& txtId);
	void setWindowsTextWithColon(int id, const string& txtId);

	virtual void createPopupMenu(CPoint point, menu_items items[]);

private:
	/// Wird in makeInsertStruct für die Initialisierung eines Treeitems benutzt.
	char *treeItemText;

	/// checkChanges liefert false, wenn dieser Wert true ist. Damit kann ein 
	/// doppelter Aufruf von checkChanges verhindert werden.
	bool disableCheckPlausis;

	WaxPart* data;

	/// Dokument
	CDocument* document;

	/// Dialogid aus Dll Resourcen
	UINT dlgid;
	
	/// Name des Dialogs
	CString name;
	
	///  aktueller CRC32
	unsigned int crc;
};




inline void WaxPartForm::set_document(CDocument* value)
{
	document = value;
}


inline void WaxPartForm::set_data(WaxPart* value)
{
	data = value;
}
