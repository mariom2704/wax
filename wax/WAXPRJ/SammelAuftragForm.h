#pragma once

class Leitkarte;

/**
Komponentenleitkarte
*/
class WAXPRJDLL SammelAuftragForm : public WaxPartForm  
{
	DECLARE_MESSAGE_MAP()
	afx_msg void OnCheckBoxClickedIPPEMatNr();
	afx_msg void OnBnClickedButtonIppeNr();
	void OnKillfocusPsp();
	DECLARE_DYNCREATE(SammelAuftragForm)

public:
	//Plausibilitätscheck, wird aufgerufen, bevor UpdateData(TRUE) aufgerufen wird.
	virtual bool check_plausis();

	void initNullVals(UINT id, int nullcount);

protected:
	SammelAuftragForm();   // Standardkonstruktor
	
	virtual INT_PTR OnToolHitTest( CPoint point, TOOLINFO* pTI ) const;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	virtual BOOL OnInitDialog();

	void OnKillfocusSdauftragsnr();
	//Indicates the edit control is losing the input focus
	 void OnKillfocusSdpositionsnummer();
	// Setzt die WA Nummer neu, indem aus den SD Nummern eine neue generiert wird
	 void onWaNrNeu();
	// Setzt die WA Nummer auf Null
	 void onWaNrLoeschen();
	//Zeigt ein Kontextmenü mit den Menüpunkten Löschen, Neu und manuell
	 void onEditwanr();
	//Versucht, die Wanr nach Userangabe zu setzen
	 void onManuellWanr();

	 void onCalcPreis();

	 void onCalcProzent();

private:
	///Testet auf Waid
	bool testWaid();
	

	///Setzt das Aenderungsdatum
	void show_aenderungsdatum() const;

	//Setzt die WaNr für den Werkauftrag und alle Unteraufträge (beim 
	//Sammelauftrag)
	bool setWaNr(int newnr = 0);

	//Initialisiert die Listbox mit den DummyMaterialnummern
	void initSDMaterialnummer();

		//Initialisiert die Listbox mit den DummyMaterialnummern
	void initZulassungsDropDown(Werkauftrag* const werk);

	//Prüft, ob das Projekt eine Projektleitkarte besitzt. Falls ja werden alle 
	//Felder der Projektleitkarte disabled.
	void disablePrjleitkartenItem();

	//Erzeugt aus allen waids aller Unterobjekte einen String und zeigt ihn in an.
	void displayDBids();

	FlexMaskedEdit editPSP;

	CString getSDMaterialKlasse(Werkauftrag * const werk) const;
	
	void checkFreigabe();

	/// Eine bestimmte WA Nummer setzen
	void on_setwanr(UINT id);
public:
	
};
