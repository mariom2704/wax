#pragma once



#include "WaxPartListe.h"
#include <string>
#include <vector>
#include "WaxPartForm.h"
#include "Fortschrittanzeige.h"

class WaxPartForm;
class CObList;
class Werkauftrag;
class Leitkarte;


//class MenuManager;
class IconManager;

/**
Basisklasse für Wax Elemente
Diese Klasse dient als Basis für alle in Wax auftauchenden Datenobjekte und ist 
ein Wrapper für die Rohrkonstklassen. 
Hier werden u.a. folgende Funktionalitäten bereitgestellt:
  - Anzeige eines Objektes und aller Unterobjekte im Baum
  - Serialisierung eines Objektes inklusive aller Unterobjekte
  - Freigabe der WA Daten und Statusaktualisierung
*/
class WAXPRJDLL WaxPart : public CWnd  
{
	DECLARE_MESSAGE_MAP()
private:
	/// Initialisiert das Icon in der Imagelist des Baumes und im Iconmananger.
	void initIcon(CTreeCtrl* tree, IconManager &iconman);

	/// Zeiger auf ein Kontextmenü aus dem MenuManager
	//CMenu* menu;
	CMenu* popUpMenu;

	static string  get_CounterGlobal()
		{
			static UINT uiCounterGlobal; 
			char szBuffer[1000];
			return itoa(uiCounterGlobal++, szBuffer, 10); 
		};

	/// Verwaltung der Kontextmenüs
	//static MenuManager menu_manager;
	bool m_IsAnhangGraficfile;
	/// Erzeugt aus dem Standardicon des Objektes ein modifiziertes (Ausrufezeichen). 
	/// Dieses Icon wird angezeigt, wenn der Werkauftrag nicht freigeben wurde.
	HICON modify_icon(HICON src, UINT overlay_icon);

	/// Unterobjektliste, wird bei Sammelauftrag und Gliederung benutzt
	WaxPartListe subobjects;

	/// Datenobjekt, wird in abgeleiteten Klassen gesetzt
	Werkauftrag* werkauftrag;

	/// Dieses Flag wird gesetzt, wenn ein Objekt im Baum gelöscht wurde. 
	/// Erst beim Speichern wird das Objekt tatsächlich gelöscht.
	bool deleted;
	
	/// Flag, ob Objekte dieser Klasse im Baum verschoben werden können, standardmässig true
	bool dragable;

	/// Versionsnummer, die in <mf .Serialize> abgefragt werden kann
	UINT version;

	/// ID des Icons, das dieses Objekt im Baum anzeigt
	UINT icon;

	bool enable_update;

	/// Wird auf false gesetzt, wenn bereits einmal nach einem Statusupdate gefragt wurde
	bool status_update_test;

	DECLARE_SERIAL(WaxPart)
	
public:

	
	enum KENNUNG 
	{
		/// Kennzeichnet ein einzelnes WaxPart beim Kopieren und Einfügen.
		FILETOC = 1234
	};

	/// Bei Sammelauftrag und Einzelauftrag wird Statusaktualisierung abgefragt, 
	/// wenn der Auftrag freigegeben ist aber noch keine WAnr hat und nicht heute freigegeben wurde 
	virtual bool sollUpdateStatus();

	void set_status_update_test(bool value);

	void saveToFile(CFile* f = NULL);

	/// Lädt ein WaxPart aus einer Datei
	static WaxPart *  loadFromFile(HANDLE mem = NULL  );

	/// Zählt die Subobjekte durch, gelöschte werden ignoriert
	int get_subobject_count() const;

	/// Führt die Synchronisation für alle Unterobjekte durch
	virtual bool doSync(bool checkFreigabe = true);

	/// Prüft, ob dieser Auftrag der erste Auftrag eines Sammelauftrages (parent) ist
	bool isFirstSubOfSammelauftrag();

	/// Liefert den ersten nicht gelöschten Unterauftrag. Falls keiner vorhanden ist, wird NULL zurückgeliefert.
	WaxPart* getFirstSubobject();

	const WaxPart* const getFirstSubobject() const;

	/// Liefert den letzten nicht gelöschten Unterauftrag. Falls keiner vorhanden ist, wird NULL zurückgeliefert.
	WaxPart* getLastSubobject();

	/// Ruft Werkauftrag::reset für alle Unterobjekte auf. Dies ist nötig, wenn z.B 
	/// eine Gliederung aus einer Datei eingefügt wird. Dann müssen die 
	/// Statusinformationen aller Unterobjekte zurückgesetzt werden.
	void resetWa();

	/// Erstellt eine Liste mit geänderten Werkaufträgen.
	void get_changelist(std::vector<const WaxPart*>& lst, const MXDate& refdt) const;

	void updateGrafic(CWnd *window);

	void set_parent(WaxPart* value);

	const WaxPart* get_parent() const;

	/// Konstruktor
	WaxPart(WaxPart *pParent = NULL);

	/// Destruktor löscht alle Unterobjekte aus subobjects
	virtual ~WaxPart();
	
	/// Zuweisungsoperator
	virtual void copy(WaxPart &other);

	/// Liefert eine Referenz auf das Kontextmenü zurück
	virtual CMenu* get_menu() ;

	WaxPartListe& get_subobjects();

	const WaxPartListe& get_subobjects() const;

	const WaxPartListe& get_subobjects_const() const { return subobjects; };


	Werkauftrag* get_werkauftrag() ;

	const Werkauftrag* get_werkauftrag_const() const { return werkauftrag; };

	void set_werkauftrag(Werkauftrag* value);

	//Pointer Werkauftrag und intern auf NULL setzen
	Werkauftrag* detach_werkauftrag();


	void set_dragable(bool value);

	const bool get_dragable() const;

	const bool get_deleted() const;

	void set_deleted(bool value);

	/// DWF Filename abfragen
	std::string get_grafic_file() ;

	void set_grafic_file(const std::string& value);

	const UINT& get_version() const;

	void set_version(UINT value);

	/// Grafikansichtsklasse zurückliefern, Null wenn keine Grafikansicht möglich ist
	virtual CRuntimeClass* getGraphicView();
	
	/// Bearbeitungsklassen zurückliefern
	virtual void getForms(CObList &RuntimeModules);
	
	/// Unterobjekt erzeugen
	virtual CObject* addChild(WaxPart *newchild = NULL, bool setleitkarte = true);
	
	/// Serialisierung
	virtual void serialize(BinaryArchive& ar);
	
	/// Objekt im Baum anzeigen
	virtual HTREEITEM Show(CTreeCtrl &tree, HTREEITEM parent  , HTREEITEM insafter  , IconManager &iconman);
	
	/// Grafikdatei erzeugen, normalerweise ein DWF File
	virtual void createGraphic();
	
	/// Titel für Baum: entweder Bezeichnung des Werkauftrages oder Defaultwert z.B. 'Verteiler'
	virtual  CString getBezeichnung() const;
			
	/// Status des Werkauftrages vom Waxserver erfragen. Wird aufgerufen, wenn der WA freigegeben wurde
	bool updateStatus();

	/// Prüfung, ob die Leitkarte neu angezeigt werden soll
	bool needDlgUpdate();

	/// Prüft, ob das Objekt gezeigt werden muss. Das ist der Fall, wenn nicht freigegeben wurde.
	virtual bool initVisible() ;

	/// Werkauftrag freigeben
	virtual void on_freigeben();

	virtual bool freigeben(bool sicherheitsabfrage, Fortschrittanzeige *fortschritt, DWORD dbhandle);

	Leitkarte& get_leitkarte();

	/// rekursive Vererbung der 'statischen' Leitkartendaten.
	void setPrjLeitkarte(const Leitkarte& leitk);

	void set_leitkarte( Leitkarte& value);

	void vererbe_leitkarte();

	CString getStatusText()const;


	//prüfen ob WaxPart zu einer Preisanfrage gehört
	virtual bool isPreisanfrage()const;

protected:
	/// Allgemeine Überprüfungen vor der Freigabe
	bool checkFreigabe();

	/// Flag für AV Version, wird einmal gesetzt, damit nicht immer die Systemparameter befragt werden müssen
	bool av;

	/// Menü initialisieren
	virtual void initPopUpMenu();

	/// Fügt die AV Reiter für die Material, Arbeit und Sonderkosteneingabe hinzu. 
	/// Diese Methode muss in abgeleiteten Klassen in getForms() aufgerufen werden.
	void addAVTabs(CObList& runtimeModules) const;


	/// Wird bei der Freigabe gesetzt, damit das Tabcontrol über <mf 
	/// .needDlgUpdate> abfragen kann, ob die Leitkarte (der Status) neu angezeigt werden soll.
	bool needUpdate;

	void set_icon(UINT  value);
	
	/// Zeiger auf das Vaterobjekt
	WaxPart* parent;
	
	/// Name der Grafikdatei
	std::string graficfile;
	std::string  svgFile, DwfAndOtherGraficFiles; 

	CMenu *getPopUpMenu();
	void appendMenuItems(CMenu *menu, menu_items items[]);
	void appendStandardMenuItems(CMenu *menu);


	void nextStepFortschritt(Fortschrittanzeige *fortschritt, Werkauftrag *wa);

public:

	//WM_COMMAND Nachrichten verarbeiten
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	bool update_enabled() const;

	void set_enable_update(bool enable = true);

	///  Rückfrage beim User
	bool enable_delete();
	
	void setDeleted();

	/// Methode, die in abgeleiteten Klassen überschrieben werden kann, um über 
	/// einen Aufruf von synchronize(this) einen Abgleich von Memberattributen zu 
	/// erreichen. Defaultmässig ruft synchronize die synchronize Methode aller 
	/// Objekte in subobjects auf.
	virtual bool synchronize(WaxPart* with);

	/// Liefert den Index des Icons im Baum. Dieser Index wird vom Iconmanager verwaltet
	int get_iconindex(IconManager &iconman) const;

	/// Ist der Menuepunkt Delete zu disablen
	virtual bool initDelete();

	/// Ist das Icon für nicht freigegeben zu modifizieren
	virtual bool is_freigegeben() const;

	//Gibt den Status zurück
	virtual Status::STATUS get_status() const;

	/// Prüft die Eingaben der Leitkarte auf Vollständigkeit
	virtual bool testLeitkarte(bool isPreisanfrage);

	bool get_plausiOk(CString *msg = NULL);

	/// Abfrage, ob Unterobjekte eingefügt werden dürfen
	virtual bool subobj_allowed() const;

	/// Automatisch die Arbeitsfolgen generieren
	virtual void createAutoArbeit();

	/// Automatisch die Sokos generieren
	virtual void createAutoSoko(bool deleteFixedSoko);

	///  Überprüfung auf mehrere Rohrqualitäten
	bool check2rohrquali();
};
