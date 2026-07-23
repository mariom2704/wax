#include "stdafx.h"
#include "resource.h"
#include "GpWapPreis.h"

#include "FormstueckeFittinge.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif 

FormstueckeFittinge::FormstueckeFittinge()
{
	get_leitkarte().set_sdMaterialnummer(400035);
	stdbezeichnung = getStammdaten().getText("IDS_FORMFITT").c_str(); 
	get_leitkarte().set_vk3(false);
	get_preis().set_vk3flag(false);
}

FormstueckeFittinge::~FormstueckeFittinge()
{
}

//Hier wird automatisch der Arbeitsplan generiert
void FormstueckeFittinge::createAutoArbeit(bool beschichtung)
{
	UNREFERENCED_PARAMETER(beschichtung);
	// Kupplung montieren
	// Jetzt immer bei Firma Körner, dann auch kein Pulvern. 
	// Todo:Stringvergleich nicht gut, besser über Gruppenid, 
	// dann muss Gruppenid Attribut von Material sein
	bool firma = false;

	// Nur hier wenn nicht rot gepulvert (ähnlich Ral 3000) und nicht Kupplung
	if (get_beschichtung().get_typ() == Beschichtung::ROT_GEPULV)
	{
		firma = true;
		for (POSITION pos = get_material().get_matlist().GetHeadPosition(); pos;)
		{
			Material* mat = dynamic_cast<Material*>(get_material().get_matlist().GetNext(pos));
			XASSERT(mat);
			if (!mat)
				break;
			CString str; str = getStammdaten().getText("IDS_KUPPLUNG").c_str(); 
			if (mat->get_kurztext().Find(str, 0) >= 0)
			{
				firma = false;
				break;
			}
		}
	}

	if (firma)	
	{
		Werkauftrag::createAutoArbeit();
		return;
	}

	// Keine Afo fuer Beschichtung
	Werkauftrag::createAutoArbeit(false);

	// Erzeugen der Afos fuer Firma 
	Arbeitsfolge* afo = new Arbeitsfolge(67);
	double gesGldPreis = 0;
	double gesFestPreis = 0;
	double gesPulverpreis = 0;
	double zuschlag = 0;
	double prozent = 0;
	GpWapPreis wapPreis;
	wapPreis.calcMaterialkosten(GpWapPreis::Vollkosten, get_material(), gesFestPreis, gesGldPreis, gesPulverpreis, prozent, zuschlag);
	afo->set_preis(gesPulverpreis);
	get_arbeit().addAfo(afo);

	// Afos sortieren
	get_arbeit().sortAfos(get_arbeit().get_arbeitsfolgen());
}
