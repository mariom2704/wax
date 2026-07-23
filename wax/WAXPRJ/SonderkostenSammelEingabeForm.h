#ifndef __SOKOSAMMEL__
#define __SOKOSAMMEL__

#include "SonderkostenEingabeForm.h"

/**
	Anzeige der Sonderkosten eines Sammelauftrages
	Eingabe ist z.Zt. disabled, da sonst die Preise für die einzelnen Unteraufträge nicht richtig ange-
	zeigt würden.
*/
class WAXPRJDLL SonderkostenSammelEingabeForm : public SonderkostenEingabeForm
{
	DECLARE_DYNCREATE(SonderkostenSammelEingabeForm)
	DECLARE_MESSAGE_MAP()

public:
	/// Konstruktor
	SonderkostenSammelEingabeForm();

	/// Destruktor
	virtual ~SonderkostenSammelEingabeForm();

	/// Datenaustausch 
	virtual void DoDataExchange(CDataExchange* pDX);

	virtual BOOL OnInitDialog();

private:
	
	/// Sokos aller Unterpositionen in die Sokoliste des Sammelauftrages eintragen
	void prepare_sokos();
};

#endif