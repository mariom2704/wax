#pragma once
#include "befehlbase.h"

/**
Auswertung Arbeitsfolgen und Material pro Wax Positionen kumulieren
*/
class BefehlAuswertungAfMatKumuliert :
	public BefehlBase
{
public:
	BefehlAuswertungAfMatKumuliert(void);
	virtual ~BefehlAuswertungAfMatKumuliert(void);

	virtual bool start();

	void setStartDate(const QDate &date);
	void setEndDate(const QDate &date);

	void setVerzeichnis(const QString &verz);
	void setDatei(const QString &dat);

private:
	QDate m_startDate,
		  m_endDate;

	QString m_verzeichnis,
		    m_datei;
};
