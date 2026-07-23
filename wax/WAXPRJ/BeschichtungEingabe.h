#ifndef __BESCHICHTUNGEINGABE__
#define __BESCHICHTUNGEINGABE__

class Beschichtung;

#define WM_AUTOSOKO (WM_USER+999)

/**
Diese Klasse kapselt die Eingabe der Beschichtung
Zur Benutzung muss in der Eingabemaske ein Dummycontrol (z.B. Static) eingebaut 
werden, dessen Koordinaten benutzt werden.
*/
class BeschichtungEingabe : public CWnd  
{
	DECLARE_MESSAGE_MAP()
public:
	BeschichtungEingabe();

	//Datenaustausch der Dialogelemente mit dem Beschichtungsobjekt
	void doDataExchange(CDataExchange* pDX, Beschichtung *beschichtung);

	//Erzeugen des Controls
	bool create(CWnd* parent = NULL, UINT dlgid = 0);


	//Beschichtung auf Plausibilität prüfen
	bool check_plausis();

private:
	void initMenu(CMenu *menu);

	CStatic txt;

	//Knopf zur Auswahl einer Ral Farbe
	CButton ralbut;

	CEdit raleingabe;

	CWnd* parent;

	/// Verzinkerei wird nur bei Beschichtung verzinkt und AV Verison angezeigt
	CComboBox zinkerei;

	UINT dlgid;

	CMenu menu;

	CComboBox arten;

	void on_select_verzinkerei();

protected:

	//Indicates a new combo box list item is selected
	void onSelchange();

	//Indicates the user clicked a button
	void onClicked();

	void onSetRal(UINT pCmdUI);

	//Called after CWnd gains the input focus.
	void OnSetFocus(CWnd* pOldWnd);

	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
};

#endif
