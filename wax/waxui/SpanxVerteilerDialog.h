#pragma once

#include "SpanxRohrDialog.h"


class WAXUIDLL SpanxVerteilerDialog : public SpanxRohrDialog
{
	DECLARE_MESSAGE_MAP()
public:
	SpanxVerteilerDialog(CWnd *parent, const string& data);

	virtual ~SpanxVerteilerDialog();

	/// Eingabedaten als XML
	virtual string xml_data();
	
protected:

	void check_muffe();
	virtual string format_langtext();

	/// Daten aus Eingabefeldern lesen
	virtual void DoDataExchange(CDataExchange* pDX);

	/// Dialog initialisieren
	virtual void init_ui();

	/// Rohrtyp-Enum für Zugriff auf Vortexte
	virtual int get_rohrtyp() const;

	/// Rohrtyp-Enum für Zugriff auf Montagezeiten
	virtual int get_rohrtyp_montage() const;

	/// XML Tag für die Datensatzkennung
	virtual string get_xmltag();

	virtual void initRohrAnfangEnde();

	virtual void initElemente();

	virtual void initSchellenVorauswahl();

	virtual void initNennweite();

	virtual double get_montagezeit() const;

	/// Setzen des Einzelpreises bei Änderung der Auswahl in der Combobox

private:

	Verteiler verteiler;
	 CBrush EditBkBrush;

public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	void update_Rohrlaenge_with_Keystroke_Test();
};

