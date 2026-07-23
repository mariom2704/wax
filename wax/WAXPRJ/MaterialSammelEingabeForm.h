#ifndef __MaterialSAMMEL__
#define __MaterialSAMMEL__

#include "MaterialEingabeForm.h"

/**
Bearbeitung des Materials eines Sammelauftrages
*/
class WAXPRJDLL MaterialSammelEingabeForm : public MaterialEingabeForm
{
	DECLARE_DYNCREATE(MaterialSammelEingabeForm)
	DECLARE_MESSAGE_MAP()

public:
	/// Konstruktor
	MaterialSammelEingabeForm();

	/// Destruktor
	virtual ~MaterialSammelEingabeForm();

	/// Datenaustausch 
	virtual void DoDataExchange(CDataExchange* pDX);
private:
	
	/// Material aller Unterpositionen in das Material des Sammelauftrages eintragen
	void prepare_material();

	/// Material aller Unteraufträge löschen und das Material des ersten Unterauftrag kopieren
	void move_material();
};

#endif