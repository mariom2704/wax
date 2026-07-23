#pragma once

namespace ServerInterface
{
	class WaLoader;
}

/**
Server Datenbank öffnen
Öffnen der Serverdatenbank und erstellen einer temporären Projektdatei.
*/
class WAXINT ServerDBOpen : public CDialog  
{
	DECLARE_MESSAGE_MAP()
public:
	/// Konstruktor
	ServerDBOpen(Status::STATUS status = Status::NichtGesetzt);


	/// Destruktor
	virtual ~ServerDBOpen();

	virtual BOOL ResetWaNr(WaxPart* pt){ return TRUE;};


	void set_status(Status::STATUS val);

	/// Zugriff auf den Dateinamen des erstellten Projektes
	const CString& get_fname() const;

	virtual void DoDataExchange(CDataExchange* pDX);

	virtual BOOL DestroyWindow();

	/// Dialogelemente initialisieren
	virtual BOOL OnInitDialog();

	/// Import starten
	virtual void OnOK();

protected:
	/// Speichert das Projekt
	virtual bool serialize_project(CObList* project);

	const virtual CString createSQLSort(CString tablename) const;

	const virtual CString createSQLSelect() const;

	///  Initialisierung von neu geladenen Objekten
	virtual void initObject(WaxPart *pt);

	ServerInterface::WaLoader* waloader;

	LRESULT OnLoadNext( WPARAM wparam, LPARAM lparam);
	
	CDialog freigabefortschritt;

	void createProgressWnd();

	void setProgressText(const CString& str);

	void initProgress(int size);

	void stepProgress();

	bool sortPreisanfrage;
	
	//Status der Aufträge, die eingelesen werden sollen
	Status::STATUS status;

	BOOL m_ReadGUIDFile;

	protected:
	BOOL m_ReadOnlyRecordsWithGUID;

private:
	
	

	/// Stornierte Aufträge ebenfalls laden
	bool load_storniert;

	/// Sortiert die Liste mit Werkaufträgen nach Sachbearbeitern
	CObList* doSachbearbeiterSort(CObList* list);

	/// Sortiert die Werkaufträge nach Klassen
	CObList* doClassSort(CObList* list);

	/// Sortiert die Werkaufträge nach Niederlassungen
	CObList* doNlSort(CObList* list);

	///Nach Preisanfragen sortieren
	CObList* doPreisanfrageSort(CObList* list);

	void initItems();

	/// Durchsucht die Liste mit Werkaufträgen nach Sammelaufträgen
	void createSammelauftrag(CObList& list);

	/// Dateiname des generierten Projektfiles
	CString fname;

	/// Erstellt ein neues Projekt aus der Datenbank
	bool makeProject( );

	/// Lädt aus der Tabelle Werkauftrag einen Datensatz, erstellt ein 
	/// WaxPart und setzt die Leitkartendaten
	WaxPart* loadObject(int thewaid);
	
	/// Objekte sortieren nach NL, WANR usw.
	CObList* sortProject(CObList * list  );
	
	/// Checkbox zum Enablen/Disablen des 'Freigabedatum von' DateControls. Ein 
	/// freigeschaltetes Freigabedatum geht dann in die SQL Select Anweisung ein.
	void OnClickedEnable_freigabe_von();
	
	/// Checkbox zum Enablen/Disablen des 'Freigabedatum bis' DateControls.
	void OnClickedEnable_freigabe_bis();
	
	/// Indicates the user clicked a button
	void OnClickedSelect_new();
	
	/// Indicates the user clicked a button
	void OnClickedSelect_egal();
	
	/// Indicates the user clicked a button
	 void OnClickedSelect_inbearbeitung();
	
	 /// Indicates the user clicked a button
	 void OnClickedSelect_wa();
};
