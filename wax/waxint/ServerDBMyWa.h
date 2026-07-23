#pragma once

class WaxPart;

/**
Spezielle Ableitung der ServerDBOpen Klasse zum Laden aller Werkaufträge eines 
Sachbearbeiters.
*/
class WAXINT ServerDBMyWa : public ServerDBOpen 
{
	DECLARE_MESSAGE_MAP()
private:
	MXDate von;

	MXDate bis;

	CString sachbearbeiter;

public:	
	ServerDBMyWa();

	virtual ~ServerDBMyWa();

	//Dialogelemente initialisieren
	virtual BOOL OnInitDialog();

	virtual BOOL DestroyWindow();

	//Import starten
	virtual void OnOK();

	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	virtual const CString createSQLSelect() const;

	virtual const CString createSQLSort(CString tablename) const;

	// Initialisierung von neu geladenen Objekten
	virtual void initObject(WaxPart* pt);
};
