#include ".\befehlshowerror.h"
#include <QtGui>
#include "CcFehlermeldungen.h"

BefehlShowError::BefehlShowError(void)
{
	m_fehlerDokument = "Fehlermeldungen.mdb";
}

BefehlShowError::~BefehlShowError(void)
{
}

/**
Aufgetretene Fehler in einer Dialogbox anzeigen (zur Zeit noch eine einfache MessageBox)
*/
bool BefehlShowError::start()
{
	QString fehler;


	fehler = getFehlerString();

	QMessageBox::critical(0,"Es ist ein Fehler aufgetreten:",
							fehler, 
							QMessageBox::Ok,
							QMessageBox::NoButton);


	return true;
}

/**
Aus der Fehlerliste einen "groﬂen" String erzeugen

Die Bezeichnungen der FehlerIDs werden aus dem Exceldokument Fehlermeldungen.xls gelesen
*/
const QString BefehlShowError::getFehlerString()const
{
	QString error,
			errorBez,
			errorId,
			errorDbNotFound,
			newError,
			stTemp;

	CcFehlermeldungen fehlerMeldungen;
			
	EnDatenbankParameter dbParameterExcel;

	QList<EnError> errorList;

	bool foundDb = true;

	
	//Liste mit Fehlermeldungen holen
	errorList = getErrorList();

	if (errorList.size() == 0)
	{
		return "Ein unbekannter Fehler ist aufgetreten";
	}


	
	//Wenn FehlerID nicht gelesen werden konnten pr¸fen ob Datei f¸r Fehlermeldungen verf¸gabr ist
	if (!QFile::exists(m_fehlerDokument))
	{
		errorDbNotFound = QString("\nACHTUNG: Die Datei %1 f¸r die Beschreibung der Fehlermeldungen wurde nicht gefunden.\n").arg(errorId).arg(m_fehlerDokument);
		foundDb = false;
	}

	//Datenbankparameter f¸r Exceldatei setzen
	dbParameterExcel.setDatabaseName(m_fehlerDokument);
	dbParameterExcel.setDbtyp(EnDatenbankParameter::ACCSESS);	

	fehlerMeldungen.setDatenbankParameter(dbParameterExcel);


	
	//Errorliste r¸ckw‰rts abarbeiten
	for (int i = errorList.size()-1; i >= 0; i--) 
	{
		errorId = errorList.at(i).getErrorId();

		if (errorId.isEmpty())
		{
			//Pr¸fen ob anstatt der Id ein Fehlertext hinterlegt wurde
			errorBez = errorList.at(i).getErrorDescription();

			if (!errorBez.isEmpty())
				error += stringTeilen(QString("%1\n").arg(errorBez));
		}
		else
		{
			if (foundDb)
			{
				//zu der ErrorID den Text ermitteln
				EnError fehler = errorList.at(i);
				if (fehlerMeldungen.readFromDb(fehler))
				{
					error += stringTeilen(QString("%1\n").arg(fehler.getErrorDescription()));
				}
				else
				{
					error += stringTeilen((QString("Zu der FehlerID %1 konnte der Text nicht aus der %2 gelesen werden\n").arg(errorId).arg(m_fehlerDokument)));
				}
			}
		}

		if (i == errorList.size()-1 && errorList.size() > 1)
		{
			error += "\nUrsache:\n";
		}
    }

	//Wenn die Datei f¸r die Fehlermeldungen nicht gefunden wurde, eine Meldung anh‰ngen
	if (!foundDb)
		error += errorDbNotFound;


	return error;
}

/**
Funktion zum Teilen des Anzeigestrings. 
Die Dialogbox beachtet nicht die Bildschirmauflˆsung und schneidet sonst den String ab.
*/
const QString BefehlShowError::stringTeilen(const QString &str)const
{
	QString newStr,
		    stTemp;

	int maxStringLaenge = 120,  //damit auch bei 1042x768 alles angezeigt wird
		lastSpace;

	if (str.size() > maxStringLaenge)
	{
		stTemp = str.left(maxStringLaenge);

		//letztes Leerzeichen vor maxStringLaenge suchen, damit nicht mitten im Wort geteilt wird
		lastSpace = stTemp.lastIndexOf(" ");

		newStr = str.left(lastSpace) + "\n";

		//stringTeilen rekursiv aufrufen, falls der Reststring noch zu lang ist
		newStr += stringTeilen(str.mid(lastSpace));

		return newStr;
	}
	else
		return str;
}
