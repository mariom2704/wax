#pragma once


class WAXUIDLL ElementeEingabe : public ElementeEingabeForm  
{
	DECLARE_DYNCREATE(ElementeEingabe)
	DECLARE_MESSAGE_MAP()

public:
	/// Konstruktor
	ElementeEingabe();

	/// Destruktor
	virtual ~ElementeEingabe();

	/// Plausibilitätscheck, wird aufgerufen, bevor UpdateData(TRUE) aufgerufen wird.
	virtual bool check_plausis();

	/// Dialog initialisieren
	virtual BOOL OnInitDialog();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	void on_grafik_drehen(NMHDR* pNMHDR, LRESULT* pResult);

/// Enablen/Disablen von Eingabefeldern je nach ElementID
	virtual void enableDisableFields();


private:
    void elem_setFields();

	/// Nächsten Drehwinkel ermitteln und setzen
	int iterate_drehwinkel(int delta) ;

	/// Rohr 'umdrehen'
	void on_elementeingabe_invertieren();

	/// Anlagenflag wurde angeklickt
	void on_anlage();
	void OnSize(UINT nType, int cx, int cy);
};

