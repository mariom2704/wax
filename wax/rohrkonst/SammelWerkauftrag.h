#pragma once

#include "Werkauftrag.h"

class ROHRKONSTDLL SammelWerkauftrag : 
public Werkauftrag 
  
{
public:
	SammelWerkauftrag();

	virtual ~SammelWerkauftrag();

	//Hier wird automatisch der Arbeitsplan generiert
	virtual void createAutoArbeit(bool beschichtung = true);

	virtual bool isSammelauftrag() const { return true; }
};


