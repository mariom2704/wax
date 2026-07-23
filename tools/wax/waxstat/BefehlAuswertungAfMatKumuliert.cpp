#include ".\befehlauswertungafmatkumuliert.h"
#include <QtGui>
#include "BefehlArbeitsfolgenProDn.h"
#include "BefehlMaterialKumuliert.h"
#include "BefehlShowError.h"


BefehlAuswertungAfMatKumuliert::BefehlAuswertungAfMatKumuliert(void)
{
}

BefehlAuswertungAfMatKumuliert::~BefehlAuswertungAfMatKumuliert(void)
{
}

void BefehlAuswertungAfMatKumuliert::setStartDate(const QDate &date)
{
	m_startDate = date;
}

void BefehlAuswertungAfMatKumuliert::setEndDate(const QDate &date)
{
	m_endDate = date;
}

void BefehlAuswertungAfMatKumuliert::setVerzeichnis(const QString &verz)
{
	m_verzeichnis = verz;
}

void BefehlAuswertungAfMatKumuliert::setDatei(const QString &dat)
{
	m_datei = dat;
}


/**
Auswertung Arbeitsfolgen und Material pro Wax Positionen kumulieren.
Das Ergebnis wird in eine Exceldatei geschrieben

Hinweis:
Wenn das Exceldokument existiert, wird es gelöscht. Wenn das Dokument nicht gelöscht 
wird, und sich die Anzahl der Spalten ändert, gibt es sonst ein Fehler beim Ausführen des 
Sql Statements
*/
bool BefehlAuswertungAfMatKumuliert::start()
{
	BefehlArbeitsfolgenProDn befehlAfProDn;

	//Datum prüfen
	if (m_startDate > m_endDate)
	{

		QMessageBox::information(0, "Hinweis",
									"Das Enddatum muß größer als das Startdatum sein!", 
									QMessageBox::Ok,
									QMessageBox::NoButton);

		return false;
	}


	
	QString file = m_verzeichnis;
	
	//Verzeichnisstring prüfen
	if (!file.endsWith("\\") && !file.endsWith("/"))
	{
		file += "\\";
	}

	//Gesamtpfad + datei
	file += m_datei;

	//Prüfen ob das Exceldokument schon vorhanden ist
	QFile qFile;
    qFile.setFileName(file);
		
	//Wenn Exceldokument vorhanden ist dieses nach Rückfrage löschen
	if (qFile.exists())
	{
		if (QMessageBox::question(
                0,"Hinweis",
                "Das Exceldokument existiert schon! Soll es überschrieben werden?",
                "&Ja", "&Nein",
                QString(), 0, 1)){
			return false;
		}

		//Exceldokument löschen
		if (!qFile.remove())
		{

			QMessageBox::critical(0,"Achtung",
									"Das Exceldokument kann nicht überschrieben werden! \nEs ist evt. noch geöffnet.", 
									QMessageBox::Ok,
									QMessageBox::NoButton);

			return false;
		}
	}


	//Werte für die Auswertung übergeben und starten
	befehlAfProDn.setExceldatei(file);

	//Start und Enddatum der Auswertung setzen
	befehlAfProDn.setStartDatum(m_startDate);
	befehlAfProDn.setEndDatum(m_endDate);

	
	//Befehl Arbeitsfolgen pro DN-Wert ausführen
	if (!befehlAfProDn.start())
	{
		BefehlShowError showError;

		showError.appendError(befehlAfProDn.getErrorList());
		showError.start();

		return false;
	}


	BefehlMaterialKumuliert matKumuliert;

	matKumuliert.setExceldatei(file);

	//Start und Enddatum der Auswertung setzen
	matKumuliert.setStartDatum(m_startDate);
	matKumuliert.setEndDatum(m_endDate);

	
	//Befehl Kumuliertes Material der Wax-Positionen
	if (!matKumuliert.start())
	{
		BefehlShowError showError;

		showError.appendError(matKumuliert.getErrorList());
		showError.start();
		return false;
	}


	QStringList argumente;
	argumente.append(file);

	//Exceldatei starten
	QProcess::execute("C:\\Programme\\Microsoft Office\\Office\\EXCEL.EXE",argumente);


	return true;
}