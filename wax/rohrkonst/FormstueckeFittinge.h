#pragma once

#include "Werkauftrag.h"

/**
Werkauftragsart Formstücke und Fittinge
*/
class ROHRKONSTDLL FormstueckeFittinge : public Werkauftrag  
{
public:
	virtual ~FormstueckeFittinge();

	FormstueckeFittinge();

	/// Generiert automatisch den Arbeitsplan 
	virtual void createAutoArbeit(bool beschichtung = true);
};


