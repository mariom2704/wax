#pragma once

#include <mxcontrols/fileloader.h>
#include <afxcoll.h>
#include "..\waxprj\WaxPartListe.h"


class Fortschrittanzeige;
class Leitkarte;
class WaxPart;
class PrjLeitkarteData;

/**
Wax Dokument
Lädt, speichert und bearbeitet die Objekte eines Projektes
*/
class  CWaxDoc : public CDocument  
{
protected:
	/// Geschützter Konstruktor. Dokument wird nur aus Serialisierung heraus erzeugt.
	CWaxDoc();
	DECLARE_DYNCREATE(CWaxDoc)
	DECLARE_MESSAGE_MAP()

public:
	virtual ~CWaxDoc();
	
	virtual void SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU = TRUE);

	virtual BOOL DoSave(LPCTSTR lpszPathName, BOOL bReplace = TRUE);

	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);

	virtual BOOL IsModified();

	virtual BOOL OnNewDocument();

	/// Inhalt des Dokumentes löschen
	virtual void DeleteContents();

	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);

	/// aktualisiert den Status des gerade ausgewählten Werkauftrages
	void OnStatusAktualisieren();

	/// Liefert das Menü des aktuellen Objektes zurück
	CMenu* GetMenu() const;
	
	/// Setzt aktives Objekt und schaltet die Views um
	bool set_active_object(CObject *object);
	
	/// Liefert aktives Objekt zurück
	WaxPart* get_active_object();
	
	/// Zugriff auf die Liste mit Objekten
	CObArray& get_objects() { return objects; } ;

	//Zugriff auf Liste mit Waxpart Objekten. (Inhalt gleich get_objects)
	WaxPartListe& get_waxPartListe();


	const PrjLeitkarteData *get_prjleitkarte() const;

	/// / Autosave ausführen
	void doAutosave(bool disable = false);

	virtual void UpdateAllViews(CView* pSender, LPARAM lHint = 0L, CObject* pHint = NULL);

private:

	Fortschrittanzeige* progress;

	/// Prüfung ob irgendein Werkauftrag im Projekt verändert wurde
	bool is_modified() const;

	/// Datum des Bearbeitungsbeginns
	MXDate dtstart; 

	void set_startdate();

//Attribute
	bool disable_autofreigabe; 

	/// Liste mit den Root Objekten
	CObArray objects;

	///Liste mit den Root WaxPart Objekten (Inhalt identisch mit objects)
	WaxPartListe m_waxPartListe;


	/// Speichert den Pointer auf das aktuell im Baum ausgewählte Objekt
	CObject * active_object;

	MxFilesystem::FileLoader prjfile;

	void get_updatelist(vector<const WaxPart*>& lst) const;

	/// Versendet eine Email mit der Projektdatei als Attachment
	void on_mail();
	
	/// Prüfung, ob Mapi installiert ist
	void on_update_mail(CCmdUI* pCmdUI);
	
	/// Setzt die Daten der Projektleitkarte für alle Unterobjekte. Wird 
	/// aufgerufen, wenn Änderungen in der Projektleitkarte vorgenommen wurden.
	void on_copy_prjleitkarte();
	
	/// Blendet den Menüpunkt Statusaktualisierung ab, wenn kein gültiger Werkauftrag ausgewählt ist
	void on_update_statusaktualisieren(CCmdUI* pCmdUI);
	
	/// Überschreibung von CDocument::OnFileClose. Leider wird dort nur das 
	/// Modified Flag geprüft, es wird aber nicht gesetzt (über CanCloseFrame).
	void on_close_file();

	/// Durchläuft alle Ansichten/Views und sorgt für ein Speichern der Daten. Wird 
	/// lst übergeben, wird die Liste mit den Objektkten gefüllt, die geändert wurden.
	void update_all_views();

	///Statistik in DB schreiben
	void on_updateStatistik();

	///KC NUmmer aktualisieren
	void on_updateKcNummer();

	void on_update_afos();

	/// Führt eine Freigabe für alle geänderten Werkaufträge durch
	//void auto_freigabe(vector<const WaxPart*>& lst);

	
	/// Zeigt (falls nötig) einen Hinweistext zum Werkauftrag an
	void show_hinweis(const WaxPart* const w) const;

	int cur_steps;

	void connect_signals();

	void load_start(bool is_loading, const string& file, int steps);
	void load_step();
	void load_end();
	static CString m_strCurrentPath;

};
