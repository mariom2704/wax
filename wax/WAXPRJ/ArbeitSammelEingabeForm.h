#ifndef __ARBEITSAMMEL__
#define __ARBEITSAMMEL__

#include "ArbeitEingabeForm.h"

/**
Bearbeitung des Arbeitsplanes eines Sammelauftrages
*/
class WAXPRJDLL ArbeitSammelEingabeForm : public ArbeitEingabeForm
{
	DECLARE_DYNCREATE(ArbeitSammelEingabeForm)
	DECLARE_MESSAGE_MAP()

public:
	/// Konstruktor
	ArbeitSammelEingabeForm();

	/// Destruktor
	virtual ~ArbeitSammelEingabeForm();

	/// Datenaustausch 
	virtual void DoDataExchange(CDataExchange* pDX);

	virtual void updateSpecialAF(Arbeitsfolge* afo);

private:
	
	/// Arbeitsfolgen aller Unterpositionen in den Arbeitsplan des Sammelauftrages eintragen
	void prepareArbeitsfolgen();

	/// Alle Arbeitspläne aller Unteraufträge löschen und den Arbeitsplan in den ersten Unterauftrag kopieren
	void moveArbeitsfolgen();
};

#endif