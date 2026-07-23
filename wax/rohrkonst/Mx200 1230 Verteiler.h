#pragma once

#include "Rohrleitung.h"


class ROHRKONSTDLL CMx2001230Verteiler : public COIIVerteiler  
{
	public:
		CMx2001230Verteiler();
		///  Rohrqualitaet bei Rohrstutzen setzen
		virtual void setQualiRohrstutzen(Element *ele);
		virtual void setSicherheitsventil(CObList& templist);
		virtual void setGewindeAnfangEnde(CObList& templist);
		virtual void check_rohrAnfangEndeVerb() const;
		virtual Arbeitsfolge* CreateDruckprobe();
		virtual void AfoChangeVirtualAssembledElements(Element* pElement);
		void SchweissUHeftFaktorenhinzufuegen(Arbeitsfolge *afo);
		virtual ~CMx2001230Verteiler();
		virtual bool createVk3Liste();
};


