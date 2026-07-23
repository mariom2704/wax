#pragma once
//#include "afx.h"

#include "..\rohrkonst\MxString.h"
#include "..\rohrkonst\StringPair.h"


class MXTable;

class WAXPRJDLL DbBase :
	public WaxBase
{
public:
	DbBase(DWORD dbhandle);
	virtual ~DbBase();



	/// Speichern oder Laden
	enum MODE 
	{
		/// Speichern der Daten
		save,

		/// Laden der Daten
		load
	};

	bool deleteDataset(MxString filter, MXTable& tab);

	MxString getFilter(list<StringPair> keyWerte);

	bool rollbackTransaction();
	bool beginTransaction();
	bool commitTransaction();

protected:

	void openDb();

	/// Handle auf die geöffnete Datenbank
	DWORD dbhandle;

	/// Hilfstabelle für den automatischen Aufbau einer Serververbindung
	MXTable* opentab;

};
