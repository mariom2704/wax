#ifndef _WaxPropPage
#define _WaxPropPage

#include "mxparameter.h"

/// Makros, die einen Paramter in die Stringliste bzw. Checkboxliste einfügen und gleichzeitig in die allgemeine Parameterliste für 
/// den jeweiligen Dialog
#define ADD_PARA_STR(id) strpara.insert(make_pair<UINT, int>(IDC_PARA_##id, id)); paralst.insert(make_pair<UINT, int>(IDC_PARA_##id, id));
#define ADD_PARA_CHK(id) checkpara.insert(make_pair<UINT, int>(IDC_PARA_##id, id)); paralst.insert(make_pair<UINT, int>(IDC_PARA_##id, id));
#define ADD_PARA_X(id) paralst.insert(make_pair<UINT, int>(IDC_PARA_##id, id));
#define ADD_PARA_XX(idc, id) paralst.insert(make_pair<UINT, int>(idc, id));


class WaxPropPage : public CPropertyPage  
{
	DECLARE_MESSAGE_MAP()
public:
	WaxPropPage(UINT nIDTemplate, MxParameter *para);
	
	~WaxPropPage();

	/// Standardeinstellungen wiederherstellen
	void set_standard();
	
	/// Wird auf true gesetzt, wenn einmal pro Start des Programmes das richtige Passwort eingegeben wurde
	static bool passwd_ok;

	//muß vor DoModal aufgerufen werden!
	void setPageTitle(const string& str);
	
protected:
		
	/// Dialoginitialisierung
	BOOL OnInitDialog();

	/// Shellfunktion: Nach einer Datei suchen
	bool browse_file(const CWnd *wnd, const int idEdit);
	
	/// Update
	void on_update();

	/// Liste mit Stringparametern und den dazugehörigen Parameterids zur Automatisierung 
	/// von set_default und enable_items
	std::map<UINT, int> strpara;

	/// Parameter, die durch Checkboxen angewählt werden
	std::map<UINT, int> checkpara;

	/// Alle Parameter des Dialog. Diese Liste dient nur dazu, den Bemerkungstext anzuzeigen
	std::map<UINT, int> paralst;

	/// Die Parametersammlung
	MxParameter *parameter;
	
	/// Default-Werte setzen
	virtual void set_default();

	/// Shellfunktion für das Auswählen eines Pfades
	bool browse_path(const CWnd *wnd, const int idEdit);

	virtual BOOL OnApply();

	/// Enablen/Disabeln in Abhängigkeit eines Systemparameters
	bool enable_item(CWnd *wnd, UINT item, UINT par);

	void setWindowsText(int id, const string& txtId);
	void setWindowsTextWithColon(int id, const string& txtId);

	
	
private:
	/// Alle Stringitems in strpara enablen und setzen, alle Parameter in paralst enablen
	void enable_all_items();

	/// Setzt den Defaultstring, falls das Eingabefeld enabled ist
	void set_default_val(UINT idc, int para);

	/// Callbackfunktion zur Auswahl des aktuellen Pfades bei browse_path
	static int CALLBACK browseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData);

	/// Für die Aktivierung der Tooltips überschrieben
	virtual void PreSubclassWindow();

	/// Anzeigen der Tooltips
	INT_PTR OnToolHitTest( CPoint point, TOOLINFO* pTI ) const;
};

#endif
