#include "stdafx.h"
#include <mxodbc\mxodbc.h>



	#include "VK3Preise.h"

VK3Preise::VK3Preise() : StammDat("", "", "counter")
{
}

void VK3Preise::load(const string& db)
{
	// Tabelle initialisieren
	MXTable vk3preise(get_database(db.c_str()), "tmpTable");

	// Tabellenfelder definieren
	DECLARE_COL(Int, counter, vk3preise);
	DECLARE_COL(Int, Fertigungsstaette, vk3preise);
	DECLARE_COL(Int, Id, vk3preise);
	DECLARE_COL(Int, Dn, vk3preise);
	DECLARE_COL(Int, Dnvl, vk3preise);
	DECLARE_COL(Int, RohrLaengeVon, vk3preise);
	DECLARE_COL(Int, RohrLaengeBis, vk3preise);
	DECLARE_COL(Int, ElementAnzahlVon, vk3preise);
	DECLARE_COL(Int, ElementAnzahlBis, vk3preise);
	DECLARE_COL(Int, ElementLaengeVon, vk3preise);
	DECLARE_COL(Int, ElementLaengeBis, vk3preise);
	DECLARE_COL(Int, ElementAnzahlId, vk3preise);
	DECLARE_COL(Double, ElementAnzahlDnVon, vk3preise); 
	DECLARE_COL(Double, ElementAnzahlDnBis, vk3preise); 
	DECLARE_COL(Bool, Anlage, vk3preise);
	DECLARE_COL(Double, Preis, vk3preise);
	DECLARE_COL(Double, PreisVerzinkt, vk3preise);
	DECLARE_COL(Int, PreisWertigkeit, vk3preise);
	DECLARE_COL(String, BezeichnungDeutsch, vk3preise);


	string sql = "SELECT Vk3Pos.counter, Vk3Pos.fertigungsstaette, Vk3Pos.Id, Vk3Pos.Dn, Vk3Pos.Dnvl, Vk3Pos.RohrLaengeVon, Vk3Pos.RohrLaengeBis,\
				  Vk3Pos.ElementAnzahlVon, Vk3Pos.ElementAnzahlBis, Vk3Pos.ElementLaengeVon, Vk3Pos.ElementLaengeBis, \
				  Vk3Pos.ElementAnzahlId, Vk3Pos.ElementAnzahlDnVon, Vk3Pos.ElementAnzahlDnBis, \
				  Vk3Pos.Anlage, Vk3Pos.Preis, Vk3Pos.PreisVerzinkt,  Vk3Pos.PreisWertigkeit,  vk3bezeichnung.BezeichnungDeutsch\
				  FROM Vk3Pos LEFT JOIN vk3bezeichnung ON Vk3Pos.Id = vk3bezeichnung.Id\
				  ORDER BY Vk3Pos.Id, Vk3Pos.Dn, Vk3Pos.Dnvl, ElementAnzahlId DESC";

	vk3preise.set_sql(sql);
	
	// Tabelle öffnen und Daten laden
	vk3preise.load();

	lst.RemoveAll();
	
	while (!vk3preise.isEof())
	{
		data* thedata = new VK3Preise::data;

		thedata->counter			= counter.data;
		thedata->Fertigungsstaette  = Fertigungsstaette.data;
		thedata->elemid				= Id.data;
		thedata->dn					= Dn.data;
		thedata->dnvl				= Dnvl.data;
		thedata->rohrlaengevon		= RohrLaengeVon.data;
		thedata->rohrlaengebis		= RohrLaengeBis.data;
		thedata->elementanzahlvon	= ElementAnzahlVon.data ;
		thedata->elementanzahlbis	= ElementAnzahlBis.data;
		thedata->elementlaengevon	= ElementLaengeVon.data;
		thedata->elementlaengebis	= ElementLaengeBis.data;
		thedata->elementanzahlid	= ElementAnzahlId.data;
		thedata->elementanzahldnVon	= ElementAnzahlDnVon.data;
		thedata->elementanzahldnBis	= ElementAnzahlDnBis.data;
		thedata->Anlage				= Anlage.data;
		thedata->preis_verzinkt		= PreisVerzinkt.data;
		thedata->PreisWertigkeit	= PreisWertigkeit.data;     
		thedata->preis				= Preis.data;
		thedata->bezeichnung		= (LPCSTR)*BezeichnungDeutsch.data;
		
		lst.AddTail(thedata);
		vk3preise.moveNext();
	}
}



const VK3Preise::data* VK3Preise::query( int iFertigungsstätte, int id, int dn, int anschlussDN, int laenge, int AnzahlElemente,  int ElementAnzahld, double ElementAnzahldn, int ElementLaenge, BOOL Anlage) const
{
		

	for (POSITION pos = lst.GetHeadPosition(); pos;)
	{
		
		const VK3Preise::data* v = (const VK3Preise::data *)lst.GetNext(pos);
		
		
		if (
			(v->Get_FertigungsstaetteVk() == iFertigungsstätte)
			&& (v->get_elemid()	== id) 
			&& (v->get_dn()     == dn		    || v->get_dn() == 0)
			&& (v->get_dnvl()   == anschlussDN  || anschlussDN == 0) 
			
			&& ( v->get_rohr_laenge_von() < laenge && laenge <= v->get_rohr_laenge_bis()  || 
					v->get_rohr_laenge_von() < laenge && v->get_rohr_laenge_bis() == 0 || 
					v->get_rohr_laenge_von() == 0 && v->get_rohr_laenge_bis() == 0 )

			&& ( v->get_element_anzahl_von() < AnzahlElemente && AnzahlElemente <= v->get_element_anzahl_bis() ||
					v->get_element_anzahl_von() < AnzahlElemente &&  v->get_element_anzahl_bis() == 0  ||
					v->get_element_anzahl_von() == 0 &&  v->get_element_anzahl_bis() == 0 )

			&& ( v->get_element_anzahl_id() == ElementAnzahld || v->get_element_anzahl_id() == 0 )

			&& ( v->get_element_anzahl_dnVon() < ElementAnzahldn && ElementAnzahldn <= v->get_element_anzahl_dnBis() ||
				v->get_element_anzahl_dnVon() == 0 && v->get_element_anzahl_dnBis() == 0 )

			&& ( v->get_element_laenge_von() < ElementLaenge && ElementLaenge <= v->get_element_laenge_bis() ||
					v->get_element_laenge_von() < ElementLaenge && v->get_element_laenge_bis() == 0  ||	
					v->get_element_laenge_von() == 0 && v->get_element_laenge_bis() == 0)  
			&&  ( v->get_Is_Anlage() == Anlage)
		   ) 
		{
			return v;
		}
	}
	

	return NULL;
}



const int VK3Preise::data::get_elemid() const
{
	return elemid;
}


const int VK3Preise::data::get_dn() const
{
	return dn;
}

const int VK3Preise::data::get_dnvl() const
{
	return dnvl;
}


const int VK3Preise::data::get_rohr_laenge_von() const
{
	return rohrlaengevon;
}


const int VK3Preise::data::get_rohr_laenge_bis() const
{
	return rohrlaengebis;
}


const int VK3Preise::data::get_element_anzahl_von() const
{
	return elementanzahlvon;
}
		


const int VK3Preise::data::get_element_anzahl_bis() const
{
	return elementanzahlbis;
}


const int VK3Preise::data::get_element_laenge_von() const
{
	return elementlaengevon;
}


const int VK3Preise::data::get_element_laenge_bis() const
{
	return elementlaengebis;
}


const double VK3Preise::data::get_preis() const
{
	return preis;
}


const double VK3Preise::data::get_preis_verzinkt() const
{
	return preis_verzinkt;
}


const string& VK3Preise::data::get_bezeichnung() const
{
	return bezeichnung;
}


