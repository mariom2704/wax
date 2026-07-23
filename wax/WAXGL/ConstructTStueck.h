#pragma once

#include "ConstructFitting.h"

class ConstructTStueck : public ConstructFitting  
  
{
private:
	//Setzt die Texte für die DN Angaben: text und text2
	void setDNText();

	//Zweiter DN Text. Wird gesetzt, wenn Element erstes oder letztes ist.
	CString text2;

public:
	ConstructTStueck();

	virtual ~ConstructTStueck();

	virtual void createBeschriftung();

	virtual void createElement();

};
