#include "stdafx.h"


#include "Monitor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

Monitor::Monitor()
{

}

Monitor::~Monitor()
{

}


const CWordArray& Monitor::get_aktionen() const
{
	return aktionen;
}

void Monitor::set_currentAction(int value)
{
	currentAction = value;
}

void Monitor::initAction()
{
/*
@mfunc Liste mit Action-IDs (aktionen) und Aktionsnamen initialisieren.
@comm "aktionen" enthaelt die gueltigen Action-IDs. 
*/
	aktionen.RemoveAll();

	MonitorOpen mono(1);

	if (WaxSys::isAV())
	{
		aktionen.Add(mono.SAEGENUTANLAGE);
		aktionen.Add(mono.VERTEILER_1);
		aktionen.Add(mono.STRANGROHR_1);
		//aktionen.Add(mono.STRANGROHR_2);
		//aktionen.Add(mono.STRANGROHR_3);
		//aktionen.Add(mono.STRANGROHR_4);
	}
}

CString Monitor::getName(int id)
{
/*
@mfunc Liefert den Namen (für Menueeintrag) fuer eine Aktion. 
@todo Mehrsprachlichkeit einbauen fuer alle Aktionen, die in den NL benutzt werden
*/
	if (aktionen.GetSize() == 0)
		initAction();
	
	MonitorOpen mono(id);
	return mono.getName(id, true);
}


void Monitor::makeAction(int id)
{
/*
@mfunc Gewuenschte Aktion ausfuehren
@comm ID gibt die auszufuehrende Aktion an. Default ist id = -1, was zur Folge hat, dass der Wert
von currentAction benutzt wird. Ist currentAction nicht gesetzt, wird die erste in aktionen definierte 
Aktion als Default gesetzt und ausgefuehrt.  
*/
	int actionid = id == -1 ? currentAction : id;

	// Keine Auswahl getroffen: In der Sapvorschau wurde eine andere Position ausgewählt. 
	if (currentAction < 0) 
	{ // Automatisch default initialisieren
		initAction();
		XASSERT(aktionen.GetSize());
		if (aktionen.GetSize())
			actionid = currentAction = aktionen.GetAt(1);
	}

	showAnlage(actionid);
}

void Monitor::showAnlage(int anlage)
{
	MonitorOpen mono(anlage);
	mono.DoModal();
}


