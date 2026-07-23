#include "stdafx.h"
#include "resource.h"


#include "Mx200 1230 Verteiler.h"
#include "GpConvertOldVersion.h"
#include "Element.h"
#include "ZinkKosten.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif 

using namespace MxErrorHandling;


//Standardkonstruktor
CMx2001230Verteiler::CMx2001230Verteiler()
{
	set_rohrQuali(RQ2448NLOS_MX1230200);

	stdbezeichnung = getStammdaten().getText("IDS_MX1230_MX200").c_str(); 

	CString str = getStammdaten().getText("IDS_STDTEXT_CFVERTI").c_str(); 
	str += getStammdaten().getText("IDS_STDTEXT_CFVERTII").c_str(); 
	str += getStammdaten().getText("IDS_STDTEXT_CFVERTIII").c_str(); 

	set_stdbemerkung(str);
}


bool CMx2001230Verteiler::createVk3Liste()
{
	if( get_dnvl() == 65 && !get_beschichtung().isVerzinkt())
	{
		vk3elementeListe.SetRohrType((EnCObListVk3::ERohrType)ROHR_Q19);
	}
	else
	{
		vk3elementeListe.SetRohrType((EnCObListVk3::ERohrType)ROHR_Q18);
	}

	return createOIIVerteilerVk3Liste();
}


void CMx2001230Verteiler::setSicherheitsventil(CObList& templist)
{

	Element* pElem = NULL;
	if(m_AbgangSicherheitsventilVorhanden  > 0)
	{
		//Sicherheitsventil 66 bar
		pElem = new Element(0);
		pElem->initStammdaten(888007);
		templist.AddTail(pElem);
	}
}



///Rohranfang, Rohrende und Rohrverbindung checken
void CMx2001230Verteiler::check_rohrAnfangEndeVerb() const
{
	//todo: getLastRohrDN() statt dnvl bei rohrende
	CString err, str;

	int rohrVerb = get_rohrVerb();
	if (rohrVerb == LEER) 
	{ 
		err = getStammdaten().getText("IDS_ERROR87").c_str();  //Bitte Verbindung auswählen!
		throw err;
	}
	if (rohrAnfang == LEER || rohrEnde == LEER) 
	{ 
		err = getStammdaten().getText("IDS_ERROR88").c_str();  //Bitte Anfang und Ende auswählen!
		throw err;
	}

	int rohrQuali = get_rohrQuali();
	if (rohrQuali == RQ2458GSCH && (rohrAnfang == GEW || rohrEnde == GEW)) 
	{
		err = getStammdaten().getText("IDS_ERROR89").c_str();  //Gewinde ist bei dieser Rohrqualität und Nennweite nicht möglich. Bitte andere Rohrqualität auswählen\noder als Element NUT wählen und über SAP ein Verbindungsstück Nut/Gewinde bestellen!
			throw err;
	} 

	check_AnfangEnde_ScheibeOk();
	
	struct
	{
		int verbArt;
		bool checkrohrverb;
		int min_dnvl;
		int max_dnvl;
	} 
	
	verb_check[] = 
	{
		NUT,		true,	25,		400,
		GEW,		false,	15,		80,
		FFL,		true,	32,		400,
		VFL16,		true,	32,		400,
		VFL10,		true,	200,	400,
		AFL,		true,	40,		400,
		SFL,		true,	40,		400,
		BOD,		false,	40,		400,
		KB40,		false,	50,		200,
		KB50,		false,	65,		200,
		0,			false,	0,		0,
	};

	for (int i = 0; verb_check[i].verbArt; i++)
	{
		if (rohrAnfang == verb_check[i].verbArt || (verb_check[i].checkrohrverb && rohrVerb == verb_check[i].verbArt))
		{
			if (dnvl < verb_check[i].min_dnvl || dnvl > verb_check[i].max_dnvl)
			{
				if (verb_check[i].verbArt == VFL10)
					str = getStammdaten().getText("IDS_ERROR21").c_str();  //Bei PN10 und DN 32-150 ist der Flachflansch Standard.
				else
					str = getStammdaten().getText("IDS_ERROR90").c_str();  //"Bei %s muss DN zwischen %i und %i sein!

				err.Format(str, getStammdaten().getBaugruppenBezeichnung(verb_check[i].verbArt), verb_check[i].min_dnvl, verb_check[i].max_dnvl);
				throw err;
			}
		}
		if (rohrEnde == verb_check[i].verbArt)
		{
			if (getLastRohrDN() < verb_check[i].min_dnvl || getLastRohrDN() > verb_check[i].max_dnvl)
			{
				if (verb_check[i].verbArt == VFL10)
					str = getStammdaten().getText("IDS_ERROR21").c_str();  //Bei PN10 und DN 32-150 ist der Flachflansch Standard.
				else
					str = getStammdaten().getText("IDS_ERROR90").c_str();  //"Bei %s muss DN zwischen %i und %i sein!

				err.Format(str, getStammdaten().getBaugruppenBezeichnung(verb_check[i].verbArt), verb_check[i].min_dnvl, verb_check[i].max_dnvl);
				throw err;
			}
		}
	}
}





void CMx2001230Verteiler::setGewindeAnfangEnde(CObList& templist)
{
	Element* pElem = NULL;


	if( rohrAnfang == GEWINDE_KAPPE_TYP_D)
	{
		pElem = new Element(0);
		pElem->initStammdaten(get_dnvl(), 0, GEWINDE_KAPPE_TYP_D);
		templist.AddTail(pElem);
	}
	
	if( rohrEnde == GEWINDE_KAPPE_TYP_D )
	{
		pElem = new Element(0);
		pElem->initStammdaten(get_dnvl(), 0, GEWINDE_KAPPE_TYP_D);
		templist.AddTail(pElem);
	}
}


void CMx2001230Verteiler::setQualiRohrstutzen(Element* ele)
{
	XASSERT(ele);
	if (!ele) 
		return;

	if (ele->get_id() == ROHR)
	{
		if(ele->get_urid() == GEW)
		{
			ele->set_id(ROHR_Q8);  //Einspeisung
		}
		else
		{
			ele->set_id(get_rohrQuali()+ROHR_Q1-1);
		}
		ele->initStammdaten(get_beschichtung().isVerzinkt());

		if(!get_beschichtung().isVerzinkt() &&get_dnvl() == 65 && ele->get_dn() == 65)
		{
			ele->initStammdaten(885245);	
		}
	}
}


Arbeitsfolge* CMx2001230Verteiler::CreateDruckprobe()
{
	///Druckprobe
	Arbeitsfolge* pDruckprobe = new Arbeitsfolge(1090);
	pDruckprobe->set_anz(get_anzahl());
	pDruckprobe->set_nr(101);
	pDruckprobe->set_ruestzeit(0.25);
	pDruckprobe->set_txt(pDruckprobe->get_txt()+ CString(_T("/Prüfen nach PAA262")));
	get_arbeit().addAfo(pDruckprobe);
	return pDruckprobe;
}


void CMx2001230Verteiler::SchweissUHeftFaktorenhinzufuegen(Arbeitsfolge *afo)
{
	double iFactor = 1;
	if (afo->get_id() == 1060 ) // Vorrichten
	{
		iFactor = 3;
	}
	
	if (afo->get_id() == 1070 )// schweissen
	{
		iFactor = 4;
	}
	afo->set_zeit(afo->get_zeit() * iFactor);
}



void CMx2001230Verteiler::AfoChangeVirtualAssembledElements(Element *element)
{
		COIIVerteiler::AfoChangeVirtualAssembledElements( element );

	if(element->get_id() == GEWINDE_KAPPE_TYP_D)
	{
		element->set_id(GEW);
	}	
}




//Destruktor
CMx2001230Verteiler::~CMx2001230Verteiler()
{
	
}

