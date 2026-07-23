#pragma once
#include <mxutil/gdiobjectmanager.h>

class WAXPRJDLL IconManager 
: public GdiObjectManager<HICON>
{

public:
	IconManager();

	virtual ~IconManager();
	
	//Dekrementiert den Benutzungszählter, bei 0 wird das Objekt gelöscht. 
	//Übergeben wird die ID des Objektes.
	virtual void decrUsage(UINT id);

	void set_index(UINT icon, UINT index);

	int get_index(UINT icon) const ;

protected:
	//Initialisiert das übergebene Objekt. Muss in abgeleiteten Klassen 
	//überschrieben werden.
	virtual void initObject(HICON& object, UINT id);

private:
	typedef map<UINT,UINT> Iconmap;
	
	Iconmap::iterator it_indexmap;

	Iconmap indexmap;
};

