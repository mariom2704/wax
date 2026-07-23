#pragma once

#include <mxcontrols/floatedit.h>

//Bearbeitung von Materialdaten
class EditMaterial : public CPropertyPage
{
public:
	EditMaterial(Material *mat   );

	virtual ~EditMaterial();

	virtual BOOL OnInitDialog();

	virtual void OnOK();

	virtual void DoDataExchange(CDataExchange* pDX);

	void setWindowsText(int id, const string& txtId);
	void setWindowsTextWithColon(int id, const string& txtId);

protected:

	//Indicates the user clicked a button
	 void onClickedStandard();
	//Indicates the display is updated after text changes
	 void onChangeAnzahl();
	//Indicates the display is updated after text changes
	 void onChangeLaenge();
	
	DECLARE_MESSAGE_MAP()
private:
	BOOL halbzeug;

	BOOL einkauf;

	//Schaut in den Stammdaten nach der Artikelnummer und prüft, ob das Material 
	//eine Längenangabe hat
	bool kann_laenge();

	FloatEditCtrl ed_preis;

	FloatEditCtrl ed_pulverpreis;

	FloatEditCtrl ed_gewicht;

	FloatEditCtrl ed_gesamtpreis;

	FloatEditCtrl ed_gesamtpulverpreis;

	FloatEditCtrl ed_gesamtgewicht;

	//Objektdaten
	Material* material;

	int anzahl;

	int laenge;

	int artnr;

	int id;

	CString bezeichnung;

	double preis;

	double pulverpreis;

	double gewicht;

	double gesamtpreis;

	double gesamtpulverpreis;

	double gesamtgewicht;

	CString posnr;

};

