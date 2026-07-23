#pragma once
#include "..\rohrkonst\mxcoblist.h"
#include "..\rohrkonst\WerkauftragListe.h"


class WAXPRJDLL WaxPartListe :
	public MxCObList
{
public:
	WaxPartListe(void);
	virtual ~WaxPartListe(void);

	//gibt die Liste mit Werkaufträgen zurück, die den WaxPart Objekten zugeordnet sind
	WerkauftragListe& getListeWerkauftraege(bool mitUnterListen,bool mitDummyWp);

	void resetListe();	///< Nur die Liste zurücksetzen, die Elemente werden nicht freigegeben

private:
	WerkauftragListe m_listWp;
};
