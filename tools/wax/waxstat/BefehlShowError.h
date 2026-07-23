#pragma once
#include "befehlbase.h"

/**
Zeigt in einer Dialogbox die Fehlermeldungen an.
Vor dem Auftruf von start() muß über appendError(..) die Fehlerliste übergeben werden
*/
class BefehlShowError :
	public BefehlBase
{
public:
	BefehlShowError(void);
	virtual ~BefehlShowError(void);

	const QString getFehlerString()const;

	virtual bool start();

protected:
	/**
	Da lange String nicht komplett in der Dialogbox angezeigt werden können
	wird der String hierüber automatisch geteilt
	*/
	const QString stringTeilen(const QString &str)const;

private:
	QString m_fehlerDokument;  ///Dokument in dem zu den FehlerIds die Bezeichnungen stehen
};
