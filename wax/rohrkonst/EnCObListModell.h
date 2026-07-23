#pragma once

#include "mxcoblist.h"
#include "MxErrorHandler.h"

/**
In dieser Liste wird das Modell (Rohre + Elemente) dargestellt.
*/
class ROHRKONSTDLL EnCObListModell :
	public MxCObList
{
public:
	EnCObListModell(void);
	virtual ~EnCObListModell(void);


	int getAnzahlElemente(int elemntId);		// Elemente im Bauteil zählen
	int getAnzahlRohre();						// Rohre im Bauteil zählen
	int getAnzahlAbgaenge();					// Abgänge im Bauteil zählen
	int getAnzahlAnlagenteile();				// Anzahl der Bauteile zählen, die über die Anlage gefertigt werden

	int getMinDn(bool bNurRohre);				// Dn des keinsten Elementes im Hauptstrang
	int getMaxDn(bool bNurRohre);				// Dn des größten Elementes im Hauptstrang

	bool hasAnlagenTeile();						//prüfen ob in der Liste Teile für die Anlage vorkommen

	bool copy(const EnCObListModell &other/*,bool deleteThisElemente,bool deepCopyOtherElemente*/);	// kopiert die übergebene Liste. 

	bool newElement(int dn, int andn, int id, int abs, int rel, int laenge, int winkel, int rlaenge, bool initStamm = true);

	#ifdef _DEBUG
	virtual void Dump(CDumpContext& dc) const;	// Ausgabe der Attribute
	#endif

	const MxErrorHandling::MxErrorHandler& getErrorHandler() const 
	{
		return errorhandler;
	}

private:
	int countAbgaenge(bool hasAnlage);

	MxErrorHandling::MxErrorHandler errorhandler;
};

