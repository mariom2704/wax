#pragma once
#include "gpanlagesaegenut.h"

/**
Aus der Datenbank die Werte für die Verteilerrohrstrasse lesen 
und die Excel Schnittstellendatei erzeugen
*/
class DialogGpAnlageVerteiler :
	public GpAnlageSaegeNut
{
public:
	DialogGpAnlageVerteiler(MxCObList *auftraege);
	virtual ~DialogGpAnlageVerteiler();

	//Aufträge für Verteiler in Xls Datei schreiben
	bool createVerteilerXls(bool bDir_verteiler_free = false);

protected:
	//Exceldatei erzeugen
	bool createExcelDatei();

	bool writeLeitkarte();			//Tab Leitkarte in Exceldatei schreiben
	bool writeVerteilerdaten();		//Tab Verteilerdaten
	bool writeRohrdaten();			//Tab Rohrdaten

private:
	bool createTab(ExcelODBC &excel, const MxString &datei, const MxString &table, char* columns[], int type [], int anzahlSpalten);
	bool writeRow(ExcelODBC &excel, const MxString &datei, const MxString &table, char* values[]);

	bool writeDummyZeile(int anzahl, ExcelODBC &excel, const MxString &datei, const MxString &table, char* values[], int anzValue);
	
	ExcelODBC excel;
	MxString datei;
	MxString fieldDummy; // = ".";
};
