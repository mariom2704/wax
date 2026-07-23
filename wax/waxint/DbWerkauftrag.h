#pragma once
#include "..\waxprj\dbbase.h"
#include "..\waxprj\Server.h"


class WAXINT DbWerkauftrag : public DbBase
{
public:
	DbWerkauftrag(void);
	virtual ~DbWerkauftrag(void);

	bool readAllWaids(CUIntArray &arWaid);			// Liste mit Waid aus DB lesen

	bool readWaids(int waNr,CUIntArray &arWaid);    // Waids zu der WA Nummer aus der Db Lesen




	Werkauftrag* readWerkauftrag(int werkauftragId);		// zu der Waid das WaxPart Objekt aus der DB erzeugen

protected:
	Server m_serverdb;

	bool readWaids(const CString &select,CUIntArray &arWaid);    // Waids aus der Db Lesen



};
