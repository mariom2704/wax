#pragma once
#include "ccbaselisten.h"
#include "EnMaterial.h"

/**
Contral Klasse der Anwendungsschicht für das Material. 
Liest und schreibt die kumulierten Materiallisten der Wax-Positionen
*/
class CcMaterial :
	public CcBaseListen
{
public:
	CcMaterial(void);
	virtual ~CcMaterial(void);

	

	bool readFromDb(QList<EnMaterial> &listMaterial);
	bool writeAuswertung(const QList<EnMaterial> &listMaterial);
};
