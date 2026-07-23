#pragma once

#include "ServerDBOpen.h"
#include <mxcontrols/medit.h>

class CObList;

/// Laden unter Verwendung eines PSP Elements. Es wird aus der Datenbank eine 
/// Projektdatei generiert.
class WAXINT ServerDbPa 
: public ServerDBOpen
{
private:
	Leitkarte lt;

	

public:
	/// Speichert das Projekt
	virtual bool serialize_project(CObList* project);

	/// Dialogelemente initialisieren
	virtual BOOL OnInitDialog();

	ServerDbPa();

	virtual ~ServerDbPa();

	virtual BOOL DestroyWindow();

	virtual void DoDataExchange(CDataExchange* pDX);


protected:
	virtual const CString createSQLSelect() const;

	virtual const CString createSQLSort(CString tablename) const;

	int panr;
	
	DECLARE_MESSAGE_MAP()
};
