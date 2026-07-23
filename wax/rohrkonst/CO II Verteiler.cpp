#include "stdafx.h"
#include "resource.h"


#include "CO II Verteiler.h"
#include "GpConvertOldVersion.h"
#include "ZinkKosten.h"
#include "Element.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif 

using namespace MxErrorHandling;




//Standardkonstruktor
COIIVerteiler::COIIVerteiler()
{
	set_dnvl(100);
	memset(elementRelAbst,				0, sizeof(elementRelAbst));
	memset(elementAbsAbst,				0, sizeof(elementAbsAbst));
	memset(elementDN,					0, sizeof(elementDN));
	memset(elementTypAbgangEnde,		0, sizeof(elementTypAbgangEnde));
	memset(elementTypGegenFLansch,		0, sizeof(elementTypGegenFLansch));
	memset(elementLaengeGegenFLansche,	0, sizeof(elementLaengeGegenFLansche));
	memset(AbstandBockAbsolut,			0, sizeof(AbstandBockAbsolut));

	elementEndschalterTyp = 0;
	PointElemente_Before  = -1;

	set_rohrQuali(RQ2448NLOS_CO2_DICK);
	set_laenge(650);
		
	rohrAnfang = FLANSCH_COII;
	rohrEnde   = FLANSCH_COII;


	m_AbgangEntleerungVorhanden = 0; 
	m_AbgangEntleerungRechts =0;
	
	m_AbgangSicherheitsventilVorhanden = 0; 
	m_AbgangSicherheitsventilRechts = 0;

	
	elementlEndeLaenge= 0;
	elementrEndeLaenge= 0;
	elementDN[0] = 40; 
	m_iAnzahlAnschluesse = 1;
	elementRelAbst[0] = 100;
	elementAbsAbst[0] = 100;
	AbstandBockAbsolut[0] = 200;
	AbstandBockAbsolut[1] = 0;
	AbstandBockAbsolut[2] = 0;
	AbstandBockAbsolut[3] = 1000;
	m_iTrestdleToRightTubeEnd = 200;
	m_iTrestdleToLeftTubeEndFehlerAbstandZuKleinOrGross  = 0;
	m_iTrestdleToRightTubeEndFehlerAbstanZuKleinOrGross = 0;
	PreElementId = -1;

	memset(rohrLaengen, 0, sizeof(rohrLaengen));

	
	iSearchDnToIndex[15]  = 0;
	iSearchDnToIndex[25]  = 1;
	iSearchDnToIndex[40]  = 2;
	iSearchDnToIndex[50]  = 3;
	iSearchDnToIndex[65]  = 4;
	iSearchDnToIndex[80]  = 5;
	iSearchDnToIndex[100] = 6;
	
	stdbezeichnung = getStammdaten().getText("IDS_CO_II_VERT").c_str(); 


	CString str;
	str = getStammdaten().getText("IDS_STDTEXT_COIIVERTI").c_str(); 
	str += getStammdaten().getText("IDS_STDTEXT_COIIVERTII").c_str(); 
	str += getStammdaten().getText("IDS_STDTEXT_COIIVERTIII").c_str(); 
	
	set_stdbemerkung(str);
}

//Destruktor
COIIVerteiler::~COIIVerteiler()
{}



void COIIVerteiler::ResetRohrElementePointElementBeforeWinkel()
{
	PointElemente_Before = -1;
}


void COIIVerteiler::SetRohrPointElementBeforeWinkel(double Before_PointElemente)
{
	PointElemente_Before = Before_PointElemente;
}


void COIIVerteiler::SetPreElementId(int LocalPreElementId)
{
	PreElementId = LocalPreElementId;
}


int COIIVerteiler::GetPreElementId()
{
	return PreElementId;
}


double COIIVerteiler::GetRohrPointElementBeforeWinkel()
{
	return PointElemente_Before;
}


//Speichern und Laden ders COIIVerteilers.
void COIIVerteiler::serialize(BinaryArchive& ar)
{
	int version = ar.get_version() > 0 ? ar.get_version() : 1000;
	// hier nicht ändern
	Rohrleitung::serialize(ar);
	if (ar.is_storing())
	{

		ar	<< m_AbgangEntleerungVorhanden << m_AbgangEntleerungRechts;
		ar	<< m_AbgangSicherheitsventilVorhanden << m_AbgangSicherheitsventilRechts;
		ar  << AbstandBockAbsolut[0] << AbstandBockAbsolut[1];
		if(version >= 61)
		{
			ar  <<  AbstandBockAbsolut[2];
		}
		
		ar  << AbstandBockAbsolut[3] << m_iTrestdleToRightTubeEnd;
		ar  << m_iAnzahlAnschluesse;
		
		int i;
		for (i = 0; i < m_iAnzahlAnschluesse; i++)
		{
			ar << elementRelAbst[i];
			ar << elementDN[i];
			ar << elementTypAbgangEnde[i];
			ar << elementTypGegenFLansch[i];
			ar << elementLaengeGegenFLansche[i];
		}
		ar << elementEndschalterTyp;
	} 
	else 
	{
		ar	>> m_AbgangEntleerungVorhanden >> m_AbgangEntleerungRechts;
		ar	>> m_AbgangSicherheitsventilVorhanden >> m_AbgangSicherheitsventilRechts;
		ar  >> AbstandBockAbsolut[0] >>	AbstandBockAbsolut[1];
		if(version >= 61)
		{
			ar	>>  AbstandBockAbsolut[2];
		}
		ar >> AbstandBockAbsolut[3] >> m_iTrestdleToRightTubeEnd;
		ar  >> m_iAnzahlAnschluesse;
		int i;
	
		for (i = 0; i < m_iAnzahlAnschluesse; i++)
		{
			ar >> elementRelAbst[i];
			ar >> elementDN[i];
			ar >> elementTypAbgangEnde[i];
			ar >> elementTypGegenFLansch[i];
			ar >> elementLaengeGegenFLansche[i];
		}

		ar >> elementEndschalterTyp;

		// Elementdaten setzen
		setElementdaten(elementDN,    elementRelAbst, elementTypAbgangEnde, elementTypGegenFLansch, elementLaengeGegenFLansche, elementEndschalterTyp,
			 rohrAnfang, rohrEnde,
			m_AbgangEntleerungVorhanden, m_AbgangEntleerungRechts,
			m_AbgangSicherheitsventilVorhanden, m_AbgangSicherheitsventilRechts);
	}
}

//Elementdaten setzen
void COIIVerteiler::setElementdaten(
									int elementDN[], int elementRelAbst[], int elementTypAbgangEnde[], int elementTypGegenFLansch[], int elementLaengeGegenFLansche[],  int elementEndschalterTypUebergabe,  
									int elementlEnde, int elementrEnde, 
									int AbgangEntleerungVorhanden, int AbgangEntleerungRechts,
									int AbgangSicherheitsventilVorhanden, int AbgangSicherheitsventilRechts
								   )
{
	int i;
	for (i = 0; i < MAXCOUNT; i++) 
	{
		if (elementRelAbst[i] == 0) 
			break;
		if (i == 0) 
			elementAbsAbst[i] = elementRelAbst[i];
		else        
			elementAbsAbst[i] = elementAbsAbst[i-1] + elementRelAbst[i];
	}
	
	for (i = 0; i < MAXCOUNT; i++) 
	{
		this->elementDN[i]					= elementDN[i];
		this->elementRelAbst[i]				= elementRelAbst[i];
		this->elementTypAbgangEnde[i]		= elementTypAbgangEnde[i];
		this->elementTypGegenFLansch[i]		= elementTypGegenFLansch[i];
		if((elementTypGegenFLansch[i] == GEGENFLANSCH_GEWINDE_COII || elementTypGegenFLansch[i] == GEGENFLANSCH_GEWINDE_MUFFE_COII) && elementDN[i] < 100 && elementLaengeGegenFLansche[i] >= 0 && elementLaengeGegenFLansche[i] < 300  )
		{
				elementLaengeGegenFLansche[i] = 300;
		}


		if((elementTypGegenFLansch[i] == GEGENFLANSCH_GEWINDE_COII || elementTypGegenFLansch[i] == GEGENFLANSCH_GEWINDE_MUFFE_COII) && elementDN[i] == 100 && elementLaengeGegenFLansche[i] >= 0 && elementLaengeGegenFLansche[i] < 400  )
		{
				elementLaengeGegenFLansche[i] = 400;
		}	
		this->elementLaengeGegenFLansche[i]	= elementLaengeGegenFLansche[i];
	}

	
	 elementEndschalterTyp	= elementEndschalterTypUebergabe;	
	 rohrAnfang 			= elementlEnde;
	 rohrEnde				= elementrEnde;
	
	this->m_AbgangEntleerungVorhanden = AbgangEntleerungVorhanden; 
	this->m_AbgangEntleerungRechts = AbgangEntleerungRechts;

	this->m_AbgangSicherheitsventilVorhanden = AbgangSicherheitsventilVorhanden; 
	this->m_AbgangSicherheitsventilRechts = AbgangSicherheitsventilRechts;
}


//Erzeugung der Rohrkonstruktion
void COIIVerteiler::createModell()
{
	Rohrleitung::createModell();

	// Das Array mit Elementen fuellen
	fill_elementArray();
	
	if (get_laenge() > 0) 
	{
		// Restabstand setzen
		set_restabstand(0);

		// Löschen aller Elemente aus der Liste
		elementList.deleteAllElemente();

		// Laengen für Anfang/Ende/Verb. setzen 
		setAnfaEndeVerbLaenge();

		// Berechnen der Rohrstuecke
		CString err = setAutoRohrlaengen();
		if (err.GetLength())
			return;
		//setRohrlaengen();


		// Elementliste fuer Rohrkonstruktion generieren
		if (!makeRohre(elementList))
			return; // Schwerer Fehler

		// Bei den Rohrstuecken links/rechts den Beschriftungstext setzen 
		rohreBeschriften(elementList);

		// Bei Rohrstuecken setzen ob mit od. ohne Rohrstutzen (fuers Pulvern)
		setRohrMitStut();
		calc_laengeProNr();		
	}
	delete_stueckliste();
	create_stueckliste();

	createVk3Liste();
	
}



void COIIVerteiler::AdditionalStuecklist(CObList& templist)
{
	
	int iArtikelnummern[] = 
	{
		885540,	886221,	886221,	886221, 886221,	886269,
		//erster Bock
		822583, // Bock ist als Element vorhanden => nicht hier einfuegen
		842562, 842562,	842562, 842562,	215084, 770221,	770221,	110017,	109886,	109886,	109886,	109886,
		
		//letzter Bock
		822583,	// Bock ist als Element vorhanden => nicht hier einfuegen
		842562,	842562,	842562,	842562,	215084, 770221, 770221,	110017,	109886,	109886,	109886,	109886
	};
	



	int iWieviele = sizeof(iArtikelnummern)/sizeof(int);
	for(int u =0; u < iWieviele; u++)
	{
		Element* pElem = new Element(0);
		pElem->initStammdaten(iArtikelnummern[u]);
		templist.AddTail(pElem);
	}


	if(AbstandBockAbsolut[1] != 0)
	{
		//Mittlerer Bock
		int iArtikelnummernII[] = 
		{
			822583, // Bock ist als Element vorhanden => nicht hier einfuegen
			842562,	842562,	215084,	770221,	770221,	110017,	109886,	109886,	109886,	109886,	103811,	103811
		};

		iWieviele = sizeof(iArtikelnummernII)/sizeof(int);
		for(int u =0; u < iWieviele; u++)
		{
			Element* pElem = new Element(0);
			pElem->initStammdaten(iArtikelnummernII[u]);
			templist.AddTail(pElem);
		}
	}

	if(AbstandBockAbsolut[2] != 0)
	{
		//Mittlerer Bock
		int iArtikelnummernII[] = 
		{
			822583, // Bock ist als Element vorhanden => nicht hier einfuegen
			842562,	842562,	215084,	770221,	770221,	110017,	109886,	109886,	109886,	109886,	103811,	103811
		};

		iWieviele = sizeof(iArtikelnummernII)/sizeof(int);
		for(int u =0; u < iWieviele; u++)
		{
			Element* pElem = new Element(0);
			pElem->initStammdaten(iArtikelnummernII[u]);
			templist.AddTail(pElem);
		}
	}

	BereichsVentilStuecklist(templist);
}


void COIIVerteiler::BereichsVentilStuecklist(CObList& templist)
{
	bool Handauslösehebel_Klein = false;
	bool Handauslösehebel_Gross = false;

	for (int i = 0; i < MAXCOUNT; i++) 
	{
		
		elementAbsAbst[i] = i ? elementAbsAbst[i-1] + elementRelAbst[i] : elementRelAbst[i];
		
		
		// nicht ausgefüllte Felder werden übergangen
		if (!elementRelAbst[i] || !elementDN[i])
				continue;
		
		if(elementDN[i] > 2) // alles was keine Einspeisung ist 
		{
			if(elementDN[i] < 65)
			{
				//Bereichsventil DN 25, 40,50
				Element* pElem = new Element (5015);
				pElem->initStammdaten(elementDN[i], 0, 5015);
				templist.AddTail(pElem);

				if(!Handauslösehebel_Klein)
				{
					//Handauslösehebel
					pElem = new Element(0);
					pElem->initStammdaten(860346);
					templist.AddTail(pElem);
					Handauslösehebel_Klein = true;
				}

			}
			else
			{
				//Bereichsventil DN 65 80 100
				Element* pElem = new Element(5015);
				pElem->initStammdaten(elementDN[i], 0, 5015);
				templist.AddTail(pElem);
				if(!Handauslösehebel_Gross)
				{
					//Handauslösehebel
					pElem = new Element(0);
					pElem->initStammdaten(823976);
					templist.AddTail(pElem);
					Handauslösehebel_Gross = true;
				}
		
			}
		}

		if(elementTypGegenFLansch[i] !=0 )
		{
			switch(elementTypGegenFLansch[i])
			{
				case GEGENFLANSCH_GEWINDE_COII:
					templist.AddTail(new Element(ROHR, elementLaengeGegenFLansche[i], elementDN[i], get_dnvl(), 
										elementRelAbst[i], elementAbsAbst[i]));
				break;

				case GEGENFLANSCH_GEWINDE_MUFFE_COII:
					templist.AddTail(new Element(ROHR, elementLaengeGegenFLansche[i], elementDN[i], get_dnvl(), 
										elementRelAbst[i], elementAbsAbst[i]));
					
					Element* ele = new Element(MUFFE_COII, 0, 15, 0, 
										elementRelAbst[i], elementAbsAbst[i]);
					ele->initStammdaten(15, get_dnvl(), MUFFE_COII);
					templist.AddTail(ele);
				break;
			}
		}		
	}


	if(elementEndschalterTyp)
	{
		Element* pElem = new Element(elementEndschalterTyp);
		pElem->initStammdaten(0, 0, elementEndschalterTyp);
		pElem->set_anzahl(m_iAnzahlAnschluesse);
		templist.AddTail(pElem);
	}

	EntleerungAndOtherStuecklist(templist);
}
  

void COIIVerteiler::setSicherheitsventil(CObList& templist)
{
	Element* pElem = NULL;
	if(m_AbgangSicherheitsventilVorhanden  > 0)
	{
		//Sicherheitsventil
		pElem = new Element(0);
		pElem->initStammdaten(886282);
		templist.AddTail(pElem);
	}

}


void COIIVerteiler::EntleerungAndOtherStuecklist(CObList& templist)
{
	Element* pElem = NULL;
	
	setSicherheitsventil(templist);
	
	if(m_AbgangEntleerungVorhanden > 0)
	{
		//Entleerung
		pElem = new Element(0);
		pElem->initStammdaten(774822);
		templist.AddTail(pElem);
	}

	
	MxCObList&  materialsList = get_material().get_matlist();

	int iAnzahlStreben = 0;
	for( int u = 0; u < 2; u++)
	{
		if(AbstandBockAbsolut[u] > 0) 
		{
			iAnzahlStreben++;
		}
	}

	int iCounter = 0;
	for (POSITION pos = materialsList.GetHeadPosition();pos;)
	{			
		Material* pM = ((Material*)materialsList.GetNext(pos));
		if(pM->get_artikelnr() == 775280)
		{
			materialsList.RemoveAt(materialsList.FindIndex(iCounter));
			delete pM;
			iCounter--;
		}
		if (pM->get_artikelnr() == 103008)
		{
			materialsList.RemoveAt(materialsList.FindIndex(iCounter));
			delete pM;
			iCounter--;
		}
		iCounter++;
	}
	
	
	for( int u = 0; u < 3; u++)
	{
		if(AbstandBockAbsolut[u] > 0) // Flachstahl Strebe
		{			
			Material* pElem = new Material();
			Profile::data *dat = NULL;
			for (POSITION pos = getStammdaten().get_profile().GetHeadPosition(); pos;)
			{
				dat = (Profile::data *)getStammdaten().get_profile().GetNext(pos);
				if (dat->get_artikelnr() == 775280)
				{
					break;
				}
			}
			pElem->set_anzahl( 2 * get_anzahl() );
			pElem->set_artikelnr(dat->get_artikelnr());
			pElem->set_bezeichnung(dat->get_kurztext());
			pElem->set_profilkennung("F");
			pElem->set_mass1(150);
			pElem->set_id(PROFIL);
			materialsList.AddTail(pElem);

			pElem = new Material(); // Strebe U-Stahl
			dat = NULL;
			for (POSITION pos = getStammdaten().get_profile().GetHeadPosition(); pos;)
			{
				dat = (Profile::data *)getStammdaten().get_profile().GetNext(pos);
				if (dat->get_artikelnr() == 103008)
				{
					break;
				}
			}
			pElem->set_anzahl( 1 * get_anzahl() );
			pElem->set_artikelnr(dat->get_artikelnr());
			pElem->set_bezeichnung(dat->get_kurztext());
			pElem->set_profilkennung("U");
			int j = u+1;
			if( AbstandBockAbsolut[j] == 0) // mittlere Bock links nicht vorhanden
			{
				if( j == 1)
				{
					j = j + 2; //letzten Bock nehmen
				}
				if( j == 2)
				{
					j = j + 1; //letzten Bock nehmen
				}
			}

			pElem->set_mass1( AbstandBockAbsolut[j] -  AbstandBockAbsolut[u] - 12);
			pElem->set_id(PROFIL);
			materialsList.AddTail(pElem);		
		}
	}
	
	
	//Kabelkanal
	pElem = new Element(0);
	pElem->initStammdaten(140187);
	pElem->set_laenge(AbstandBockAbsolut[3] - AbstandBockAbsolut[0] + 100);
	templist.AddTail(pElem);

	
	FlanschBlindflanschAndOtherStuecklist(templist);
}



void COIIVerteiler::PutDichtungAndStrews(CObList& templist, int iDnvl )
{	

	//Flansch + Blindflansch Dichtung
	Element* pElem = new Element(0);
	pElem->initStammdaten(get_dnvl(), 0, 5021);
	templist.AddTail(pElem);
	
	
	for( int u = 0; u < 8; u++)
	{
		//Schrauben
		pElem = new Element(0);
		pElem->initStammdaten(iDnvl, 0, 5024);
		templist.AddTail(pElem);
		//Muttern
		pElem = new Element(0);
		pElem->initStammdaten(iDnvl, 0, 5025);
		templist.AddTail(pElem);

		if(getBeschichtung().isGepulvert())
		{
			pElem = new Element(0);
			pElem->initStammdaten(iDnvl, 0, 5034);
			templist.AddTail(pElem);
		}
	}
}



void COIIVerteiler::FlanschBlindflanschAndOtherStuecklist(CObList& templist)
{

	
	/// Element ID vom Rohranfang
	if( rohrAnfang == BLINDFLANSCH_COII	|| rohrAnfang ==  GEGENFLANSCH_COII_SCHARZ_HAUPTROHR )
	{
		PutDichtungAndStrews(templist, get_dnvl());
	}

	/// Element ID vom Rohranfang
	if( rohrEnde == BLINDFLANSCH_COII || rohrEnde ==  GEGENFLANSCH_COII_SCHARZ_HAUPTROHR )
	{
		PutDichtungAndStrews(templist, get_dnvl());
	}
}

// virtual  =>  wird für GEWINDE_KAPPE_TYP_D in CMx2001230Verteiler behandelt
void COIIVerteiler::setGewindeAnfangEnde(CObList& templist)
{
	Element* pElem = NULL;
	if( rohrEnde == GEWINDE_KAPPE_COII && rohrAnfang == GEWINDE_KAPPE_COII )
	{
		pElem = new Element(0);
		pElem->initStammdaten(get_dnvl(), 0, GEWINDE_KAPPE_COII);
		templist.AddTail(pElem);
	}
	else
	{
		if( rohrEnde == GEWINDE_KAPPE_COII || rohrAnfang == GEWINDE_KAPPE_COII  )
		{
			pElem = new Element(0);
			pElem->initStammdaten(get_dnvl(), 0, GEWINDE_KAPPE_COII);
			templist.AddTail(pElem);
		}
	}
	
}



void COIIVerteiler::setQualiRohrstutzen(CObList& templist)
{
	///Sollen standardmaessig in der selben Quali wie das Rohr ausgefuehrt werden
	for (POSITION pos = templist.GetHeadPosition(); pos;)	
	{
		Element *ele = dynamic_cast<Element*>(templist.GetNext(pos));
		XASSERT(ele);
		if (!ele) 
			return;

		setQualiRohrstutzen( ele );
	}
}


void COIIVerteiler::setQualiRohrstutzen(Element *ele)
{	
	if (ele->get_id() == ROHR)
	{
		if(ele->get_urid() == GEW)
		{
			ele->set_id(ROHR_Q8); // Einspeisung
		}
		else
		{
			ele->set_id(get_rohrQuali()+ROHR_Q1-1);
		}
		ele->initStammdaten(get_beschichtung().isVerzinkt());
		if(get_beschichtung().isVerzinkt())
		{
			if((get_dnvl() == 65 || get_dnvl() == 100) && ele->get_dn() == 65)
			{	
				ele->initStammdaten(857604);
			}
			if(get_dnvl() == 100 && ele->get_dn() == 80)
			{
				ele->initStammdaten(791178);
			}
		}
	}
	
}




void COIIVerteiler::create_stueckliste()
{
///Hier werden die Elemente aus der Elementlist in die Stueckliste gepackt.

	///Temporaere Stueckliste generieren
	CObList templist;
	create_templist(templist, elementList);

	AdditionalStuecklist(templist);
	///Rohrqualitaet bei Rohrstutzen setzen
	setQualiRohrstutzen(templist);


	///Templiste in Stueckliste zusammenfassen
	
	putTemplistInStueckliste(templist, stueckliste);

	///Temporaere Objekte wieder loeschen
	for (POSITION xpos = templist.GetHeadPosition(); xpos; )
	{
		Element *x = dynamic_cast<Element*>(templist.GetNext(xpos));
		if (x)
			delete x;
	}
	templist.RemoveAll();

	///Ausschuss addieren 
	addRohrausschuss(stueckliste);
}



void COIIVerteiler::create_templist(CObList& templist, CObList& elementList)
{

#define INSERT_ELEMENT(dn, dnvl, id, laenge) \
	Element *ele = new Element; \
	ele->initStammdaten(dn, dnvl, id); \
	ele->initDaten(0, 0, laenge, id, 0); \
	templist.AddTail(ele); 

#define INSERT_ELEMENT2(element, laenge) \
	Element *ele = new Element(*element); \
	ele->set_laenge(laenge); \
	templist.AddTail(ele); 

	int eleSLaenge = 0, flanschlaenge = 0;

	///Elementliste abarbeiten und in Templiste packen.
	///Hier werden die Sonderbehandlungen vorgenommen.
	if (!elementList.IsEmpty())
	{	
		for (POSITION pos = elementList.GetHeadPosition(); pos != NULL;)	
		{
			Element *element = dynamic_cast<Element*>(elementList.GetNext(pos));
			XASSERT(element);
			if (!element)
				break;

			if (element->isEleRohr() && element->get_laenge() <= 0)
				continue;
			
			if (element->get_id() == SNAHT    || element->get_id() == LEER || element->get_id() == GLATT 
			 || element->get_id() == SCHWEISS || element->get_id() == NUT  || element->get_id() == GEW)
				continue;
try {
			// Spuelanschluss sind eigentlich 2 Elemente:
			// Scheibe/Boden + Spans
			if (element->get_id() == KB40) 
			{
				if (element->get_anschlussDN() == 50)
					;
				else if (element->get_anschlussDN() < 200) 
				{
					INSERT_ELEMENT(dnvl, dnvl, SCH, 0)
				}
				else
				{
					INSERT_ELEMENT(dnvl, dnvl, BOD, 0)
				}
				INSERT_ELEMENT(40, dnvl, SPANS, 0)
			} 
			else if (element->get_id() == KB50) 
			{
				if (element->get_anschlussDN() < 200) 
				{
					INSERT_ELEMENT(dnvl, dnvl, SCH, 0)
				}
				else
				{
					INSERT_ELEMENT(dnvl, dnvl, BOD, 0)
				}
				INSERT_ELEMENT(50, dnvl, SPANS, 0)
			}
			else if (element->get_id() == BLINDFLANSCH_COII) 
			{
				INSERT_ELEMENT(dnvl, dnvl, BLINDFLANSCH_COII, element->get_laenge());
				{
					INSERT_ELEMENT(dnvl, dnvl, FLANSCH_COII, element->get_laenge());	
				}				
			}else if (element->get_id() == GEGENFLANSCH_COII_SCHARZ_HAUPTROHR) 
			{
				INSERT_ELEMENT(dnvl, dnvl, FLANSCH_COII, element->get_laenge());
				{
					INSERT_ELEMENT(dnvl, dnvl, FLANSCH_COII, element->get_laenge());	
				}
			}
			else if (element->get_id() == GEWINDE_KAPPE_COII || element->get_id() ==  GEWINDE_KAPPE_TYP_D) 
			{
				setGewindeAnfangEnde(templist);
			}
			else 	
			{
				INSERT_ELEMENT2(element, element->get_laenge());
			}
} 
catch (Element::Error e) 
{	
	getErrorHandler().appendErrorDescription(e.get_errorText(), MxError::Warning, __FILE__, __LINE__);
	
} 
			for (POSITION upos = element->get_uelementList().GetHeadPosition(); upos != NULL;)
			{
				Element *uelement = dynamic_cast<Element*>(element->get_uelementList().GetNext(upos));
				XASSERT(uelement);
				if (!uelement)
					break;

				if (uelement->get_id() == BOHR24 || uelement->get_id() == BOHR50)
					continue;

				for (POSITION uupos = uelement->get_uelementList().GetHeadPosition(); uupos != NULL;)
				{
					Element *uuelement = dynamic_cast<Element*>(uelement->get_uelementList().GetNext(uupos));
					XASSERT(uuelement);
					if (!uuelement)
						break;

					if (uuelement->get_id() == NUT || uuelement->get_id() == GEW  || uuelement->get_id() == GEWINDE_KAPPE_COII || uuelement->get_id() == GEWINDE_KAPPE_TYP_D || uuelement->get_id() == GLATT)
					{
						uelement->set_urid(uuelement->get_id());
						continue;
					}

					INSERT_ELEMENT2(uuelement, uuelement->get_laenge());
					if(uuelement->get_id() == FLANSCH_COII)
					{
						flanschlaenge = uuelement->get_laenge();
					}
				}
				// erst tatsaechliche Rohrstutzenlaenge berechnen
				if (uelement->get_id() == ROHR || (uelement->get_id() == STDSTUTZEN && uelement->get_urid() == ROHR)) 
				{
					eleSLaenge = berechnungRohrstutzen(uelement->get_dn(), uelement->get_anschlussDN(), uelement->get_laenge(), flanschlaenge, uelement->get_makeAnlage());
				}
				INSERT_ELEMENT2(uelement, (uelement->get_id() == ROHR || (uelement->get_id() == STDSTUTZEN && uelement->get_urid() == ROHR))?eleSLaenge:uelement->get_laenge());
			}
		}
	} 
}



// Prueft, ob zwei Gegenflansche mit muffe  hinter einander folgende
// damit der Grafik faktor angepasst werden kann 
bool COIIVerteiler::has_GegenflanschWithMuffe()
{

	if (!elementList.IsEmpty())
	{	
		bool bFoundGegenFlanschWithMuffeAfterOneAnother = 0; 
		for (POSITION pos = elementList.GetHeadPosition(); pos != NULL;)	
		{
			Element *element = dynamic_cast<Element*>(elementList.GetNext(pos));
			XASSERT(element);
			if (!element)
				break;

			if (element->isEleRohr() && element->get_laenge() <= 0)
				continue;
			
			
			for (POSITION upos = element->get_uelementList().GetHeadPosition(); upos != NULL;)
			{
				Element *uelement = dynamic_cast<Element*>(element->get_uelementList().GetNext(upos));
				XASSERT(uelement);
				if (!uelement)
					break;

				if (uelement->get_id() == BOHR24 || uelement->get_id() == BOHR50)
					continue;

				bool bfoundAfter= false;
				for (POSITION uupos = uelement->get_uelementList().GetHeadPosition(); uupos != NULL;)
				{
					Element *uuelement = dynamic_cast<Element*>(uelement->get_uelementList().GetNext(uupos));
					XASSERT(uuelement);
					if (!uuelement)
						break;

					
					if (uuelement->get_id() == GEGENFLANSCH_GEWINDE_MUFFE_COII ||uuelement->get_id() == GEGENFLANSCH_GEWINDE_COII)
					{
						uelement->set_urid(uuelement->get_id());
						if(uelement->get_abstRel() < 521 && bFoundGegenFlanschWithMuffeAfterOneAnother == true)
						{
							return true;
						}
						if (uuelement->get_id() == GEGENFLANSCH_GEWINDE_MUFFE_COII)
						{
							bFoundGegenFlanschWithMuffeAfterOneAnother = true;
							bfoundAfter = true;
						}
						break;
					}

				}
				if(!bfoundAfter)
				{
					bFoundGegenFlanschWithMuffeAfterOneAnother = false;
				}
			}
		}
	} 
	return false;
}




int COIIVerteiler::GetAbstandBockZumLinkenRohrEnde()
{
	return AbstandBockAbsolut[0]; 
};



void COIIVerteiler::SetAbstandBockZumLinkenRohrEnde(int iAbstandBockAbsolut)
{
	 AbstandBockAbsolut[0] = iAbstandBockAbsolut; 
};



void COIIVerteiler::SetAbstandBockZumRechtenRohrEnde(int iAbstandBockRel )
{
	m_iTrestdleToRightTubeEnd = iAbstandBockRel;
};


void COIIVerteiler::SetAbstandErsterMittlererBockZumLinkenRohrEnde(int iAbstandBockRel )
{
	if(iAbstandBockRel)
	{
		AbstandBockAbsolut[1] = AbstandBockAbsolut[0] + iAbstandBockRel;
	}
	else
	{
		AbstandBockAbsolut[1] = 0;
	}
}


void COIIVerteiler::SetAbstandZweiterMittlererBockZumErstenMittlerenBock(int iAbstandBockRel )
{
	if(iAbstandBockRel)
	{
		AbstandBockAbsolut[2] = AbstandBockAbsolut[1] + iAbstandBockRel;
	}
	else
	{
		AbstandBockAbsolut[2] = 0;
	}
}



int COIIVerteiler::GetAbstandErsterMittlererBockZumLinkenRohrEnde()
{
	int iMittlererBockAbstand  = 0;
	if( AbstandBockAbsolut[1] <= 0 || (AbstandBockAbsolut[1] - AbstandBockAbsolut[0]) < 200)
	{
		iMittlererBockAbstand =  0;
	}

	if((AbstandBockAbsolut[1] - AbstandBockAbsolut[0]) >= 200)
	{
		iMittlererBockAbstand = AbstandBockAbsolut[1] - AbstandBockAbsolut[0];
	}
	 return  iMittlererBockAbstand;
}


int COIIVerteiler::GetAbstandZweiterMittlererBockZumErstenMittlerenBock()
{
	int iMittlererBockAbstand  = 0;
	if( AbstandBockAbsolut[2] <= 0 /* || (AbstandBockAbsolut[1] - AbstandBockAbsolut[0]) < 200 */)
	{
		iMittlererBockAbstand =  0;
	}
	else
	{
		iMittlererBockAbstand = AbstandBockAbsolut[2] - AbstandBockAbsolut[1];
	}
	return  iMittlererBockAbstand;
}

int COIIVerteiler::GetAbstandBockZumRechtenRohrEnde()
{
	return m_iTrestdleToRightTubeEnd;
};



int COIIVerteiler::FirstAbgangLinks()
{
	for (int i = 0; i < MAXCOUNT; i++) 
	{
		elementAbsAbst[i] = i ? elementAbsAbst[i-1] + elementRelAbst[i] : elementRelAbst[i];
		//rel_abst[i] = elementRelAbst[i];
		
		// nicht ausgefüllte Felder werden übergangen
		if (!elementRelAbst[i] || !elementDN[i])
			continue;

		return elementDN[i];
	}
	return -1;
}



int COIIVerteiler::LastAbgangRechts()
{
	int j =0;
	for (int i = 0; i < MAXCOUNT; i++) 
	{
		elementAbsAbst[i] = i ? elementAbsAbst[i-1] + elementRelAbst[i] : elementRelAbst[i];
		//rel_abst[i] = elementRelAbst[i];
		
		// nicht ausgefüllte Felder werden übergangen
		if (!elementRelAbst[i] || !elementDN[i])
			continue;
		j=i;
		
	}
	return j;
}



void COIIVerteiler::SicherventilLinks(int& TubeBeginToSecureNozzleRelLeft)
{	
	 SicherventilBothSides(rohrAnfang, TubeBeginToSecureNozzleRelLeft);

	 if ( m_AbgangSicherheitsventilVorhanden == 1 && m_AbgangSicherheitsventilRechts == 0 )
	{
		Element* ele3 = new Element(MUFFE_COII, extraAbgObenLaenge, extraAbgObenDN, get_dnvl(), TubeBeginToSecureNozzleRelLeft, TubeBeginToSecureNozzleRelLeft, 0);
		ele3->initStammdaten(15, get_dnvl(), MUFFE_COII);
		elementArray.Add(ele3);
	}
}



void COIIVerteiler::SicherventilBothSides(int SicherventilBothSides, int& TubeBeginToSecureNozzleRelLeft)
{
	if(SicherventilBothSides == FLANSCH_COII || SicherventilBothSides == BLINDFLANSCH_COII || SicherventilBothSides == GEGENFLANSCH_COII_SCHARZ_HAUPTROHR)
	{
		TubeBeginToSecureNozzleRelLeft = 150;
		if(get_dnvl() > 80)
		{
			 TubeBeginToSecureNozzleRelLeft = 200;
		}
	}
	if(SicherventilBothSides == BODEN_COII)
	{
		TubeBeginToSecureNozzleRelLeft = 100;
		if(get_dnvl() == 100)
		{
			TubeBeginToSecureNozzleRelLeft = 150;
		}
	}
	if(SicherventilBothSides == GEW || SicherventilBothSides == GEWINDE_KAPPE_COII || SicherventilBothSides == GEWINDE_KAPPE_TYP_D)
	{
		TubeBeginToSecureNozzleRelLeft = 100;	
	}
}



void COIIVerteiler::SicherventilRechts(int iLastAbsolutelementAbsAbst, int TreastleSpaceRohende)
{
	
	int TubeBeginToSecureNozzleRelRight = 200;
	SicherventilBothSides(rohrEnde, TubeBeginToSecureNozzleRelRight);

	//Sicherheitsventil at the end
	if ( m_AbgangSicherheitsventilVorhanden  == 1 && m_AbgangSicherheitsventilRechts == 1 )
	{
		int iLaenge = get_laenge();
		int AbsSicherheitsventilAbst = iLaenge - TubeBeginToSecureNozzleRelRight;
		int rel = AbsSicherheitsventilAbst - iLastAbsolutelementAbsAbst;
	
		Element* ele3 = new Element(MUFFE_COII, extraAbgObenLaenge, extraAbgObenDN, get_dnvl(), rel, AbsSicherheitsventilAbst, 0);
		ele3->initStammdaten(15, get_dnvl(), MUFFE_COII);

		if(TubeBeginToSecureNozzleRelRight > TreastleSpaceRohende)
		{	// Do not occur until now only, if dependencies will change
			elementArray.InsertAt(elementArray.GetSize()-1, ele3);
		}
		else
		{
			elementArray.Add(ele3);
		}
	}
}


// Entfernung vom Rohanfang zum ersten Bock
void COIIVerteiler::SetRohrAnfangToTrestle()
{
	int TrestleSpaceToTubeLeftEnd = 200;
	if(rohrAnfang == FLANSCH_COII || rohrAnfang == BLINDFLANSCH_COII || rohrAnfang == GEGENFLANSCH_COII_SCHARZ_HAUPTROHR )
	{
		TrestleSpaceToTubeLeftEnd = GetTrestleSpaceToFlanschLeft(get_dnvl());
	}
	if(rohrAnfang == BODEN_COII)
	{
		TrestleSpaceToTubeLeftEnd = GetTrestleSpaceToBodenLeft(get_dnvl());
	}
	if(rohrAnfang == GEW || rohrAnfang == GEWINDE_KAPPE_COII || rohrAnfang == GEWINDE_KAPPE_TYP_D)
	{
		TrestleSpaceToTubeLeftEnd = GetTrestleSpaceToGewindeLeft(get_dnvl());
	}
	

	if( eTrestdleToRightOrLeftTubeEnd < AbstandBockAbsolut[0] )
	{
		m_iTrestdleToLeftTubeEndFehlerAbstandZuKleinOrGross = AbstandBockAbsolut[0];
		AbstandBockAbsolut[0] = eTrestdleToRightOrLeftTubeEnd;
	}
	if( AbstandBockAbsolut[0] < TrestleSpaceToTubeLeftEnd)
	{
 		m_iTrestdleToLeftTubeEndFehlerAbstandZuKleinOrGross = TrestleSpaceToTubeLeftEnd;
		AbstandBockAbsolut[0] = TrestleSpaceToTubeLeftEnd;
	}
}



void COIIVerteiler::FillInEntleerungOrMiddleBockFirstPosition(int i)
{
	if ( m_AbgangEntleerungVorhanden == 1 && m_AbgangEntleerungRechts == 0 && i == 0 )
	{
		if( AbstandBockAbsolut[0] + 200 <= elementAbsAbst[i])
		{
			int rel = 200;
			int AbsSicherheitsventilAbst =  AbstandBockAbsolut[0] + 200;
			Element* ele3 = new Element(MUFFE_COII, extraAbgObenLaenge, extraAbgObenDN, get_dnvl(), rel, AbsSicherheitsventilAbst, 180);
			ele3->initStammdaten(15, get_dnvl(), MUFFE_COII);
			elementArray.Add(ele3);		
		}
	}

	//MittlererBock hinter Entleerung
	if( AbstandBockAbsolut[0] + 200 <=  AbstandBockAbsolut[1] && AbstandBockAbsolut[1] <=  elementAbsAbst[i] && i == 0)
	{
		Element* eleWinkelMitte = new Element(WINKEL_RECHTS_COII, 0, 25, get_dnvl(),  AbstandBockAbsolut[1], AbstandBockAbsolut[1], 180);
		eleWinkelMitte->initStammdaten( 25,  get_dnvl(), WINKEL_RECHTS_COII);
		elementArray.Add(eleWinkelMitte);
	}


	if ( m_AbgangEntleerungVorhanden == 1 && m_AbgangEntleerungRechts == 1 )
	{
		if( AbstandBockAbsolut[3] - 150 <=  elementAbsAbst[i])
		{
			int rel = elementAbsAbst[i] - (AbstandBockAbsolut[3] - 150);
			int AbsSicherheitsventilAbst = AbstandBockAbsolut[3] - 150;
	
			Element* ele3 = new Element(MUFFE_COII, extraAbgObenLaenge, extraAbgObenDN, get_dnvl(), rel, AbsSicherheitsventilAbst, 180);
			ele3->initStammdaten(15, get_dnvl(), MUFFE_COII);
			elementArray.Add(ele3);	
		}
	}
}


void COIIVerteiler::FillInEntleerungSecondPosition(int i)
{
	if ( m_AbgangEntleerungVorhanden  == 1 && m_AbgangEntleerungRechts == 0 && i==0 )
	{
		
		//Entleerung nach elementAbsAbst[i] einfügen
		if( AbstandBockAbsolut[0] + 200 > elementAbsAbst[i] )
		{
			int rel = 200;
			Element* ele3 = new Element(MUFFE_COII, extraAbgObenLaenge, extraAbgObenDN, get_dnvl(), rel, AbstandBockAbsolut[0] + rel, 180);
			ele3->initStammdaten(15, get_dnvl(), MUFFE_COII);
			elementArray.Add(ele3);
		}
	}
}


void COIIVerteiler::FillInEntleerungLastPosition(int iMerkDNLast)
{
	if ( m_AbgangEntleerungVorhanden == 1 && m_AbgangEntleerungRechts == 1 )
	{
		if( AbstandBockAbsolut[3] - 150 >  elementAbsAbst[iMerkDNLast])
		{
			int rel =  (AbstandBockAbsolut[3] - 150) - elementAbsAbst[iMerkDNLast];
			int AbsSicherheitsventilAbst = AbstandBockAbsolut[3] - 150;
		
			Element* ele3 = new Element(MUFFE_COII, extraAbgObenLaenge, extraAbgObenDN, get_dnvl(), rel, AbsSicherheitsventilAbst, 180);
			ele3->initStammdaten(15, get_dnvl(), MUFFE_COII);
			elementArray.Add(ele3);	
		}
	}
}



void COIIVerteiler::SetRestUndLaenge(int& TreastleSpaceRohende)
{
	
	if(rohrEnde == FLANSCH_COII || rohrEnde == BLINDFLANSCH_COII ||  rohrEnde == GEGENFLANSCH_COII_SCHARZ_HAUPTROHR)
	{
		TreastleSpaceRohende = GetTrestleSpaceToFlanschRight(get_dnvl());
	}
	if(rohrEnde == BODEN_COII)
	{
		TreastleSpaceRohende = GetTrestleSpaceToBodenRight(get_dnvl());
	}
	if(rohrEnde == GEW || rohrEnde == GEWINDE_KAPPE_COII || rohrEnde == GEWINDE_KAPPE_TYP_D)
	{
		TreastleSpaceRohende = GetTrestleSpaceToGewindeRight(get_dnvl());
	}

	if( eTrestdleToRightOrLeftTubeEnd >= m_iTrestdleToRightTubeEnd && m_iTrestdleToRightTubeEnd >= TreastleSpaceRohende)
	{
		 TreastleSpaceRohende = m_iTrestdleToRightTubeEnd;	 
	}
	else
	{

		if(m_iTrestdleToRightTubeEnd < TreastleSpaceRohende)
		{
			m_iTrestdleToRightTubeEndFehlerAbstanZuKleinOrGross = m_iTrestdleToRightTubeEnd = TreastleSpaceRohende;
		}
		else
		{
			m_iTrestdleToRightTubeEndFehlerAbstanZuKleinOrGross = m_iTrestdleToRightTubeEnd = eTrestdleToRightOrLeftTubeEnd;
		}
	}

	set_laenge( AbstandBockAbsolut[3] + TreastleSpaceRohende);
}





//Das Array mit Elementen fuellen
void COIIVerteiler::fill_elementArray()
{
	elementArray.deleteAll();
		
	int iTubeBeginToSecureNozzleRelLeft = 200;
	SicherventilLinks( iTubeBeginToSecureNozzleRelLeft );										
	
	Element* eleWinkelLinks = new Element(WINKEL_LINKS_COII, 0, 0, get_dnvl(),  AbstandBockAbsolut[0], AbstandBockAbsolut[0], 180);
	eleWinkelLinks->initStammdaten( 25,  get_dnvl(), WINKEL_LINKS_COII);
	
	//Trestle is infront of the ScureNozzle
	if( AbstandBockAbsolut[0] < iTubeBeginToSecureNozzleRelLeft)
	{
		elementArray.InsertAt(0, eleWinkelLinks);
	}
	else
	{
		elementArray.Add(eleWinkelLinks);
	}
	
	// Flansche auf Rohrstutzen und Gegenflansch
	Element FlanschAndGegenflansche[MAXCOUNT];
	
	int iLastAbsolutelementAbsAbst = 0; 
	int iLastDN  = -1;
	
	for (int i = 0; i < MAXCOUNT; i++) 
	{
		elementAbsAbst[i] =	 ( i ? elementAbsAbst[i-1] + elementRelAbst[i] : elementRelAbst[i] );
		if( i == 0)
		{
			elementAbsAbst[i] = AbstandBockAbsolut[0] + elementRelAbst[0];
		}
		

		if (!elementRelAbst[i] || !elementDN[i])
			continue;
		
		SetRohrAnfangToTrestle();
		
		if( i == 0 )
		{
			m_iTrestdleToLeftNozzle = -1;
			if(elementRelAbst[0] < GetTrestleSpaceLeftAreaNozzle(elementDN[0]))
			{
				CString err = getStammdaten().getText("IDS_ERROR149").c_str(); 
				m_strFirstBVOrBockSpaceWrong.Format(err, elementRelAbst[0], GetTrestleSpaceLeftAreaNozzle(elementDN[0]));
				m_iTrestdleToLeftNozzle = GetTrestleSpaceLeftAreaNozzle(elementDN[0]);
			}
		}	

		FillInEntleerungOrMiddleBockFirstPosition(i);
		
		try
		{

			if(elementDN[i] == 2)
			{
				// 2"Zoll Einspeisung
				Element* pEle = new Element(ROHR, 400, 50 /*DN*/, get_dnvl(), 
										elementRelAbst[i], elementAbsAbst[i]);
				pEle->set_mass2(-1);
				elementArray.Add(pEle);
				elementArray.Add(new Element(GEW, 0, 50 /*DN*/, get_dnvl(), 
										elementRelAbst[i], elementAbsAbst[i]));
			}
			else
			{
				if( elementTypAbgangEnde[i] == FLANSCH_COII)
				{// Rohrstutzen + Flansch
				
					FlanschAndGegenflansche[i].initStammdaten(elementDN[i], elementDN[i], FLANSCH_COII);
					FlanschAndGegenflansche[i].initDaten(elementAbsAbst[i], 0, 0, FLANSCH_COII, 0);
					// Rohrstutzen
					int iFlanschStutzenLaenge = 0;
					if(elementDN[i] <= 65 )
					{
						iFlanschStutzenLaenge = 260;
					}
					else if(elementDN[i] <= 80 )
					{
						iFlanschStutzenLaenge = 256;
					}
					else
					{ // elementDN = 100;
						iFlanschStutzenLaenge = 237;
					}

					elementArray.Add(new Element(ROHR, iFlanschStutzenLaenge, elementDN[i], get_dnvl(), 
										elementRelAbst[i], elementAbsAbst[i]));
					// Flansch
					elementArray.Add(new Element(FlanschAndGegenflansche[i]));
					

					if(elementTypGegenFLansch[i] != 0)
					{
						// Gegenflansche wurde zubvor initzialisiert unjetzt auf eine neue ID =>
						// GEGENFLANSCH_GEWINDE_COII	oder GEGENFLANSCH_GEWINDE_MUFFE_COII
						// Die artukelnummer bleibt die vom Flansch wird also in der Stueckliste richtig gemapped ( artikelNum == Artikelnum)
						FlanschAndGegenflansche[i].set_id(elementTypGegenFLansch[i]);
						FlanschAndGegenflansche[i].set_laenge(elementLaengeGegenFLansche[i]);
						elementArray.Add(new Element(FlanschAndGegenflansche[i]));
					}
				}
				else
				{
					Element* pElement = new Element(ROHRNIPPEL, 30, elementDN[i], get_dnvl(), 
										elementRelAbst[i], elementAbsAbst[i]);

					pElement->initStammdaten(elementDN[i], get_dnvl(), ROHRNIPPEL_COII);
					elementArray.Add(pElement);
				}

				FillInEntleerungSecondPosition(i);

				int elementAbsAbstPlusEins = 0;
				if( i < MAXCOUNT - 1)
				{
					elementAbsAbstPlusEins = elementAbsAbst[i] + elementRelAbst[i+1];
				}
				if( AbstandBockAbsolut[1] >= AbstandBockAbsolut[0] && AbstandBockAbsolut[3] >= AbstandBockAbsolut[1]
				&& elementAbsAbst[i] < AbstandBockAbsolut[1] && AbstandBockAbsolut[1]  <= elementAbsAbstPlusEins)
				{
					
					Element* eleWinkelMitte = new Element(WINKEL_RECHTS_COII, 0, 25, get_dnvl(),  AbstandBockAbsolut[1], AbstandBockAbsolut[1], 180);
					eleWinkelMitte->initStammdaten( 25,  get_dnvl(), WINKEL_RECHTS_COII);
					elementArray.Add(eleWinkelMitte);				
				}

				if( AbstandBockAbsolut[2] >= AbstandBockAbsolut[1] && AbstandBockAbsolut[3] >= AbstandBockAbsolut[2]
				&& elementAbsAbst[i] < AbstandBockAbsolut[2] && AbstandBockAbsolut[2]  <= elementAbsAbstPlusEins)
				{
					
					Element* eleWinkelMitte = new Element(WINKEL_RECHTS_COII, 0, 25, get_dnvl(),  AbstandBockAbsolut[2], AbstandBockAbsolut[2], 180);
					eleWinkelMitte->initStammdaten( 25,  get_dnvl(), WINKEL_RECHTS_COII);
					elementArray.Add(eleWinkelMitte);				
				}
			}
		}
		catch (Element::Error e) 
		{
			getErrorHandler().appendErrorDescription(e.get_errorText(), MxError::Information);	
		}
		
		iLastAbsolutelementAbsAbst = elementAbsAbst[i];
		iLastDN = i;
	}
	



	FillInEntleerungLastPosition(iLastDN);

	if( AbstandBockAbsolut[2] == 0)
	{
		if( elementAbsAbst[iLastDN] <  AbstandBockAbsolut[1] && AbstandBockAbsolut[1] < AbstandBockAbsolut[3])
		{
			Element* eleWinkelMitte = new Element(WINKEL_RECHTS_COII, 0, 25, get_dnvl(),  AbstandBockAbsolut[1], AbstandBockAbsolut[1], 180);
			eleWinkelMitte->initStammdaten( 25,  get_dnvl(), WINKEL_RECHTS_COII);
			elementArray.Add(eleWinkelMitte);
		}
	}
	else
	{  //zweite mittlere Bock
		if( elementAbsAbst[iLastDN] <  AbstandBockAbsolut[2] && AbstandBockAbsolut[2] < AbstandBockAbsolut[3])
		{
			Element* eleWinkelMitte = new Element(WINKEL_RECHTS_COII, 0, 25, get_dnvl(),  AbstandBockAbsolut[2], AbstandBockAbsolut[2], 180);
			eleWinkelMitte->initStammdaten( 25,  get_dnvl(), WINKEL_RECHTS_COII);
			elementArray.Add(eleWinkelMitte);
		}
	}
	
	AbstandBockAbsolut[3] = iLastAbsolutelementAbsAbst + GetTrestleSpaceRightAreaNozzle(elementDN[iLastDN]);
	Element* eleWinkel = new Element(WINKEL_RECHTS_COII, 0, 25, get_dnvl(), GetTrestleSpaceRightAreaNozzle(elementDN[iLastDN]), AbstandBockAbsolut[3], 180);
	eleWinkel->initStammdaten( 25,  get_dnvl(), WINKEL_RECHTS_COII);
	elementArray.Add(eleWinkel);

	int TreastleSpaceRohende = 200;
	SetRestUndLaenge(TreastleSpaceRohende);

	SicherventilRechts( iLastAbsolutelementAbsAbst, TreastleSpaceRohende);

	elementArray.calcRelAbst();

	DetectAndMarkEqualDistanceBockAbgang();
}


// Diese Funktion bestimmt, ob ein Winkel direkt unter einem Element liegt dann wurde dieses nicht bemaßt
void COIIVerteiler::DetectAndMarkEqualDistanceBockAbgang()
{
	Element* pElementWinkel[4] = {NULL, NULL, NULL};

	int iLauf =0;
	for (int i = 0; i < elementArray.GetSize(); i++) 
	{		
		Element* ele = elementArray.get_element_at(i);

		if(ele->get_id() == WINKEL_LINKS_COII || ele->get_id() == WINKEL_RECHTS_COII )
		{
			pElementWinkel[iLauf] = ele;
			iLauf++;
		}
	}

	for (int i = 0; i <  elementArray.GetSize(); i++) 
	{
		
		Element* ele = elementArray.get_element_at(i);
		if(ele->get_id() != WINKEL_LINKS_COII && ele->get_id() != WINKEL_RECHTS_COII )
		{
			for( int j = 0; j < iLauf; j++)
			{
				if(pElementWinkel[j]->get_abstAbs() == ele->get_abstAbs())
				{
					pElementWinkel[j]->set_dn1(-1); // Merker fuer die Bemaßung siehe ConstructWinkel Bemassung
				}
			}
		}
	}
}



//Laenge von Anfang/Ende/Verbindung setzen
void COIIVerteiler::setAnfaEndeVerbLaenge()
{
	Rohrleitung::setAnfaEndeVerbLaenge();

	//Todo: Variablen umbenennen 
	elementlEndeLaenge = rohrAnfaLaenge;
	elementrEndeLaenge = rohrEndeLaenge;
}



const int COIIVerteiler::get_AnzahlAnschluesse()
{
	return m_iAnzahlAnschluesse;
};


const void COIIVerteiler::set_AnzahlAnschluesse(int iAnzahlAnschluesse)
{
	m_iAnzahlAnschluesse = iAnzahlAnschluesse;
};

//get avelementArray
const int COIIVerteiler::get_elementDN(int index) const
{
	return elementDN[index];
}


void COIIVerteiler::set_elementDN(int index, int value)
{
	elementDN[index] = value;
}


const int COIIVerteiler::get_elementTypeAbgang(int index) const
{
	return elementTypAbgangEnde[index];
}

const int COIIVerteiler::get_elementGegenflansch(int index) const
{
	return elementTypGegenFLansch[index];
}


const int COIIVerteiler::get_ElementLaengeGegenflansch(int index) const
{
	return elementLaengeGegenFLansche[index];
}


const int COIIVerteiler::get_EndschalterTyp() const
{
	return elementEndschalterTyp;
}


const int COIIVerteiler::set_EndschalterTyp(int value)
{
	return elementEndschalterTyp = value;
}



const void COIIVerteiler::set_elementTypeAbgang(int index, int value)
{
	elementTypAbgangEnde[index] = value;
}



const int COIIVerteiler::get_elementRelAbst(int index) const
{
	return elementRelAbst[index];
}

void COIIVerteiler::set_elementRelAbst(int index, int value)
{
	elementRelAbst[index] = value;
}


//Hier wird automatisch der Arbeitsplan generiert
void COIIVerteiler::createAutoArbeit(bool beschichtung)
{

	UNREFERENCED_PARAMETER(beschichtung);



	///Standardarbeitsfolgen fuer Werkauftrag generieren
	get_arbeit().SetRohrType(this);

	createModell();

	///Wenn ap 32000, dann wird neu berechnet
	anzahlNennweiten.RemoveAll();

	get_arbeit().clean();
	
	createProfilAfos();
		
	// Diese Afo (Uebergabe an Produktion) immer zuerst
	get_arbeit().addAfo_Produktion();

	// Erzeugen der Afos fuer die Beschichtung 
	if (beschichtung)
	{
		createBeschAfos(get_arbeit());
	}

	///Umstempeln
	Arbeitsfolge* pUmstempeln = new Arbeitsfolge(1136);
	pUmstempeln->set_nr(5);
	pUmstempeln->set_anz(get_anzahl());
	get_arbeit().addAfo(pUmstempeln);

	///Werkstoffliste
	Arbeitsfolge* pWerkstoffliste = new Arbeitsfolge(1162);
	pWerkstoffliste->set_anz(get_anzahl());
	pWerkstoffliste->set_ruestzeit(0.25);
	pWerkstoffliste->set_nr(104);
	get_arbeit().addAfo( pWerkstoffliste);
	
	
	///Komplett montieren
	Arbeitsfolge *pKomplettMontieren = new Arbeitsfolge(1441);
	pKomplettMontieren->set_anz(0);
	pKomplettMontieren->set_nr(121);
	
	get_arbeit().addAfo(pKomplettMontieren);

	///Zeugnis erstellen
	Arbeitsfolge *pZeugnisErstellen = new Arbeitsfolge(37);
	pZeugnisErstellen->set_anz(get_anzahl());
	pZeugnisErstellen->set_nr(126);
	get_arbeit().addAfo(pZeugnisErstellen);

	///Funktionskontrolle
	Arbeitsfolge *pFunktionskontrolle = new Arbeitsfolge(450);
	pFunktionskontrolle->set_nr(122);
	pFunktionskontrolle->set_ruestzeit(0.25);
	pFunktionskontrolle->set_anz(get_anzahl());
	pFunktionskontrolle->set_txt(pFunktionskontrolle->get_txt()+ CString(_T("/Prüfen nach PAA262")));
	pFunktionskontrolle->set_arbpl("34030");
	get_arbeit().addAfo(pFunktionskontrolle);


	///Dichtigkeitsprüfung
	Arbeitsfolge *pDichtigkeitsprüfung = new Arbeitsfolge(1317);
	pDichtigkeitsprüfung->set_anz(get_anzahl());
	pDichtigkeitsprüfung->set_nr(123);
	pDichtigkeitsprüfung->set_ruestzeit(0.25);
	pDichtigkeitsprüfung->set_txt(pDichtigkeitsprüfung->get_txt()+ CString(_T("/Prüfen nach PAA262")));
	get_arbeit().addAfo(pDichtigkeitsprüfung);


	///Anliefern
	Arbeitsfolge *pAnliefern = new Arbeitsfolge(795);
	pAnliefern->set_anz(get_anzahl());
	get_arbeit().addAfo(pAnliefern);
	pAnliefern->set_arbpl("36010");
	

	Arbeitsfolge *pDruckprobe = CreateDruckprobe();
	pDruckprobe->set_zeit(2);

	for (int i = 0; i < MAXCOUNT; i++) 
	{
		if(elementDN[i] != 0)
		{
			if(elementDN[i] < 100)
			{
				pKomplettMontieren->set_zeit( pKomplettMontieren->get_zeit() + 1.5 * get_anzahl());	
				pKomplettMontieren->set_anz(pKomplettMontieren->get_anz() + get_anzahl());
			}
			else
			{
				pKomplettMontieren->set_zeit( pKomplettMontieren->get_zeit() + 2 * get_anzahl());
				pKomplettMontieren->set_anz(pKomplettMontieren->get_anz() + get_anzahl());
			}
		}
		if(elementTypGegenFLansch[i] != 0)
		{
			pDruckprobe->set_zeit( pDruckprobe->get_zeit() + 0.5);
		}
	}


	AddElementeAfos();

	AnfasenFuerGewindeundDruckpruefung();

	get_arbeit().sortAfos(get_arbeit().get_arbeitsfolgen());

	for (POSITION pos = get_arbeit().get_arbeitsfolgen().GetHeadPosition(); pos;)
	{
		Arbeitsfolge *afo  = dynamic_cast<Arbeitsfolge*>(get_arbeit().get_arbeitsfolgen().GetNext(pos));
		XASSERT(afo);
		if(!afo)
		{
			break;
		}

		
		if (afo->get_id() == 1060 || afo->get_id() == 1070 )
		{
			afo->set_ruestzeit(0.1047);
		}

	
		if (afo->get_id() == 72)
		{
			afo->set_ruestzeit(0.0);
		}
		
		if (afo->get_id() == 70)
		{
			afo->set_ruestzeit(0.25);
		}
	}

}



Arbeitsfolge* COIIVerteiler::CreateDruckprobe()
{
	///Druckprobe
	Arbeitsfolge* pDruckprobe = new Arbeitsfolge(1100);
	pDruckprobe->set_anz(get_anzahl());
	pDruckprobe->set_nr(101);
	pDruckprobe->set_ruestzeit(0.25);
	pDruckprobe->set_txt(pDruckprobe->get_txt()+ CString(_T("/Prüfen nach PAA262")));
	get_arbeit().addAfo(pDruckprobe);
	return pDruckprobe;
}



void COIIVerteiler::AnfasenFuerGewindeundDruckpruefung()
{

	//int bRuestUndSchweissZeitFound = 0; 
	bool bRustStrebeHeftenUndSchweissen = false;
	 int count = 0;
	for (POSITION pos = get_arbeit().get_arbeitsfolgen().GetHeadPosition(); pos; )
	{
		
		Arbeitsfolge* afo = dynamic_cast<Arbeitsfolge*>(get_arbeit().get_arbeitsfolgen().GetNext(pos));
		if (!afo)
			continue;

		if (afo->get_id() == 76)// Profil saegen entfaellt
		{
			delete afo;
			get_arbeit().get_arbeitsfolgen().RemoveAt(get_arbeit().get_arbeitsfolgen().FindIndex(count));
		}

		if (afo->get_id() == 32)
		{
			afo->set_txt(afo->get_txt()+ CString(_T("/EN10204-3.1")));
		}

		if (afo->get_id() == 1050) //Rohre saegen UProfile(Strebe)
		{
			afo->set_ruestzeit(0.25);
			for( int u = 0; u < 2; u++)
			{
				if(AbstandBockAbsolut[u] > 0)
				{
					afo->set_zeit( afo->get_zeit() + 0.05);
					afo->set_anz(afo->get_anz()+1); 
				}
			}
		}

		if (afo->get_id() == 70)
		{
			afo->set_nr(20);
		}

		if (afo->get_id() == 71)
		{
			afo->set_nr(22);
		}

		if (afo->get_id() == 72)
		{
			afo->set_nr(24);
			for( int u = 0; u < 2; u++)
			{
				if(AbstandBockAbsolut[u] > 0)
				{
					afo->set_zeit( afo->get_zeit() + 0.01 * 4);
			
					Arbeitsfolge *pStrebeHeften = new Arbeitsfolge(1770);
					pStrebeHeften->set_zeit(2 * 0.1667 * get_anzahl());
					pStrebeHeften->set_nr(27);
					pStrebeHeften->set_anz(pStrebeHeften->get_anz()* 2 * get_anzahl());
					if(!bRustStrebeHeftenUndSchweissen)
					{
						pStrebeHeften->set_ruestzeit(0.75);
						bRustStrebeHeftenUndSchweissen = true;
					}
					get_arbeit().addAfo(pStrebeHeften);
				}
			}
		}
		count++;
	}
}



void COIIVerteiler::SchweissUHeftFaktorenhinzufuegen(Arbeitsfolge *afo)
{

	double iFactor = 1; 
	double dZeit = afo->get_zeit();
	if(	afo->get_id() == 1060 ) // Vorrichten
	{
		int iDN  = afo->get_dn();
		switch(iDN)
		{
			default:
			iFactor = 3;
			break;

			case 100:
			iFactor = 6;
			break;
		}
	} 
		
	if(	afo->get_id() == 1070 )//schweissen
	{
		int iDN  = afo->get_dn();
		switch(iDN)
		{
		default:
			iFactor = 4;
			break;

			case 100:
			iFactor = 6;
			break;
		}
	}
	afo->set_zeit(dZeit * iFactor);	
}


// virtual wird für GEWINDE_KAPPE_TYP_D in CMx2001230Verteiler behandelt 
void COIIVerteiler::AfoChangeVirtualAssembledElements(Element *element)
{
	if(element->get_id() == GEWINDE_KAPPE_COII)
	{
		element->set_id(GEW);
	}

	if(element->get_id() == BLINDFLANSCH_COII)
	{
		element->set_id(FLANSCH_COII);
	}
}


void COIIVerteiler::AddElementeAfos()
{
	Element *ele = new Element;

	Beschichtung TempBeschichtung  = get_beschichtung();
	if( TempBeschichtung.get_typ() != Beschichtung::VERZINKT)
	{
		Beschichtung TempLoocalBeschichtung(Beschichtung::VERZINKT, -1, "verzinkt");
		set_beschichtung(TempLoocalBeschichtung);

	}

	for (POSITION pos = elementList.GetHeadPosition(); pos != NULL;)	
	{

		Element *element =  dynamic_cast<Element*>(elementList.GetNext(pos));
		AfoChangeVirtualAssembledElements(element);
		XASSERT(element);
		if (!element)
			break;
			

		get_arbeit().addAfos(get_anzahl(), element, get_beschichtung(),  0, "0", Rohrleitung::GAS);

		CObList& uelementList = element->get_uelementList();
		for (POSITION upos = uelementList.GetHeadPosition(); upos != NULL;)	
		{
			Element *uelement = dynamic_cast<Element*>(uelementList.GetNext(upos));
			XASSERT(uelement);
			if (!uelement)
				break;

			get_arbeit().addAfos(get_anzahl(), uelement, get_beschichtung(), 0, "0", Rohrleitung::GAS);

			CObList& uuelementList = uelement->get_uelementList();
			for (POSITION uupos = uuelementList.GetHeadPosition(); uupos != NULL;)	
			{
				Element *uuelement = dynamic_cast<Element*>(uuelementList.GetNext(uupos));
				XASSERT(uuelement);
				if (!uuelement)
					break;

				if(GEGENFLANSCH_GEWINDE_COII == uuelement->get_id() || GEGENFLANSCH_GEWINDE_MUFFE_COII == uuelement->get_id())
				{
					Element elementu(FLANSCH_COII, 0, uuelement->get_dn(), uuelement->get_dn());
					get_arbeit().addAfos(get_anzahl(), &elementu, get_beschichtung(),  0, "0", Rohrleitung::GAS);	

					Element elementuuu(ROHR, 0, uuelement->get_dn(), uuelement->get_dn());
					//get_arbeit().addAfos(1, &elementuuu, get_beschichtung(),  0, "0", Rohrleitung::GAS);	

					Element elementuuuu(GEW, 0, uuelement->get_dn(), uuelement->get_dn());
					get_arbeit().addAfos(get_anzahl(), &elementuuuu, get_beschichtung(),  0, "0", Rohrleitung::GAS);
				
					if(GEGENFLANSCH_GEWINDE_MUFFE_COII == uuelement->get_id())
					{
						Element* ele = new Element(MUFFE_COII, 15, 15);
						ele->initStammdaten(15, get_dnvl(), MUFFE_COII);
						get_arbeit().addAfos(get_anzahl(), ele, get_beschichtung(),  0, "0", Rohrleitung::GAS);
					}

					continue;
				}

				get_arbeit().addAfos(get_anzahl(), uuelement, get_beschichtung(), 0, "0", Rohrleitung::GAS);	
			}
		}
	}
	delete ele;
	
	if( TempBeschichtung.get_typ() != get_beschichtung().get_typ())
	{
		set_beschichtung(TempBeschichtung);

	}
}

//Rohrkonstruktion auf korrekte Werte checken
CString COIIVerteiler::checkPlausis()
{
	Werkauftrag::checkPlausis();
	CString err, str, txt;

	err = checkAnbauElemente();
	if (err.GetLength() > 0)
		return err;

	err.Empty();
	err = Rohrleitung::checkPlausis();
	if(!err.IsEmpty())
	{
		return err;
	}


 	if( m_iTrestdleToLeftNozzle > -1 )
	{		
		CString strFirstBVSpaceWrong;
		strFirstBVSpaceWrong += m_strFirstBVOrBockSpaceWrong;
		m_iTrestdleToLeftNozzle = -1;
		m_iTrestdleToLeftTubeEndFehlerAbstandZuKleinOrGross = 0;
		return strFirstBVSpaceWrong;
	}


	if(m_iTrestdleToRightTubeEndFehlerAbstanZuKleinOrGross)
	{	
		CString errMerge;
		if(m_iTrestdleToRightTubeEndFehlerAbstanZuKleinOrGross < eTrestdleToRightOrLeftTubeEnd)
		{
			err = getStammdaten().getText("IDS_ERROR145").c_str(); 
			errMerge.Format(err, m_iTrestdleToRightTubeEndFehlerAbstanZuKleinOrGross);
		}
		else
		{	
			err = getStammdaten().getText("IDS_ERROR146").c_str(); 
			errMerge.Format(err, m_iTrestdleToRightTubeEndFehlerAbstanZuKleinOrGross);
		}
		m_iTrestdleToRightTubeEndFehlerAbstanZuKleinOrGross = 0;
		return errMerge;
	}


	if(m_iTrestdleToLeftTubeEndFehlerAbstandZuKleinOrGross > eTrestdleToRightOrLeftTubeEnd)
	{	
		m_iTrestdleToLeftTubeEndFehlerAbstandZuKleinOrGross = 0;
		err = getStammdaten().getText("IDS_ERROR147").c_str(); 
		return err;
	}
	if(m_iTrestdleToLeftTubeEndFehlerAbstandZuKleinOrGross)
	{
		int iTrestdleToLeftTubeEndFehlerAbstandZuKleinOrGross = m_iTrestdleToLeftTubeEndFehlerAbstandZuKleinOrGross;
		m_iTrestdleToLeftTubeEndFehlerAbstandZuKleinOrGross = 0;
		err = getStammdaten().getText("IDS_ERROR150").c_str(); 
		CString strErrMerge;
		strErrMerge.Format(err, iTrestdleToLeftTubeEndFehlerAbstandZuKleinOrGross);
		return strErrMerge;
	}

	
	if ( m_AbgangSicherheitsventilVorhanden == 1 && m_AbgangSicherheitsventilRechts == 0 )
	{

		int TubeBeginToSecureNozzleRelLeft = 0;
		SicherventilBothSides(rohrAnfang, TubeBeginToSecureNozzleRelLeft);
		if(elementAbsAbst[0] - TubeBeginToSecureNozzleRelLeft < 200)
		{
			CString strErrMerge = getStammdaten().getText("IDS_ERRORR_ABSTAND_SV_ABGANG").c_str(); 
			return strErrMerge;
		}
	}


	if(getBeschichtung().isGepulvert() && getBeschichtung().get_typ() != Beschichtung::ROT_GEPULV)
	{
		CString strErrMerge = getStammdaten().getText("IDS_ERROR_GEPULVERT_FARBE_NICHT_ROT").c_str(); 
		AfxMessageBox(strErrMerge, MB_OK | MB_ICONINFORMATION);
		return _T("");
	}

	return  "";
}



CString COIIVerteiler::checkAnbauElemente()
{
	CString err, str;

	int i=0;
	
	err = TestDNvlTODNAbgang(0, MAXCOUNT );
	if(!err.IsEmpty())
	{
		return err;
	}

	while (elementDN[i] != 0 && i < MAXCOUNT) 
	{  

		if (elementDN[i] > get_dnvl()) 
		{
			err = getStammdaten().getText("IDS_ERROR2a").c_str(); 
			return err;
		}


		if((elementTypGegenFLansch[i] == GEGENFLANSCH_GEWINDE_COII || elementTypGegenFLansch[i] == GEGENFLANSCH_GEWINDE_MUFFE_COII) && elementDN[i] < 100 && elementLaengeGegenFLansche[i] >= 0 && elementLaengeGegenFLansche[i] < 300  )
		{
				CString strErrorLocal = getStammdaten().getText("IDS_ERROR152").c_str(); 
				err.Format( strErrorLocal, 300);
					return err;
		}


		if((elementTypGegenFLansch[i] == GEGENFLANSCH_GEWINDE_COII || elementTypGegenFLansch[i] == GEGENFLANSCH_GEWINDE_MUFFE_COII) && elementDN[i] == 100 && elementLaengeGegenFLansche[i] >= 0 && elementLaengeGegenFLansche[i] < 400  )
		{
				CString strErrorLocal = getStammdaten().getText("IDS_ERROR152").c_str(); 
				err.Format( strErrorLocal, 400);
					return err;
		}	
		i++;
	}
	return err;
}



EnContainerRohrElemente& COIIVerteiler::getElementArray()
{
	return get_avelementArray();
}

///Liefert den Tabellennamen fuer die Datenbankspeicherung der Elemente
CString COIIVerteiler::getTabname()
{
	return "avelement";
}


int COIIVerteiler::iDNAbgToDNvl[7][7] =	{
											/*		 DN 2"   25   40   50   65   80   100
											/*DN 2"*/	200, 200, 200, 200, 250, 300, 300,
											/*DN 25*/	250, 250, 250, 250, 300, 300, 300,
											/*DN 40*/	250, 250, 250, 250, 350, 350, 350,
											/*DN 50*/	250, 250, 250, 250, 350, 350, 350,
											/*DN 65*/	300, 300, 300, 300, 350, 350, 350,
											/*DN 80*/	300, 300, 300, 300, 350, 350, 350,
											/*DN 100*/	400, 400, 400, 400, 450, 450, 450,
										};



CString COIIVerteiler::TestDNvlTODNAbgang(int iVon, int iBis)
{		
	int i=0;
	CString strFehler = _T("");
	
	bool bFehlerinitialized = false;
	for( i = iVon; i < iBis; i++) 
	{
		if( elementDN[i] != 0 )
		{
			int iDNAbgangLinks = elementDN[i];
			int iIndexAbstandLinks = iSearchDnToIndex[iDNAbgangLinks];
			int k = 0;
			int iIndexAbstandRechts = 0;
			for(k = i + 1; k < MAXCOUNT; k++) 
			{
				if(elementDN[k] != 0)
				{
					int iDNAbgangRechts = elementDN[k];
					iIndexAbstandRechts = iSearchDnToIndex[iDNAbgangRechts];
					break;
				}
			}
			if(k == MAXCOUNT)
			{
				return strFehler;
			}

			int AbstElementMin =  iDNAbgToDNvl[iIndexAbstandLinks][iIndexAbstandRechts];
			
			if(AbstElementMin > elementRelAbst[i+1] || 1500 < elementRelAbst[i+1] )
			{
				if(!bFehlerinitialized)
				{
					CString strFehlerPure = getStammdaten().getText("IDS_ERROR151").c_str(); 
					strFehler.Format( strFehlerPure, i+1, i+2, AbstElementMin);
					bFehlerinitialized = true;
				}
			}
		}
	}
	
	return  strFehler;
}



int COIIVerteiler::GetTrestleSpaceRightAreaNozzle(int iDN)
{
	int TrestleSpaceRight = 100;

	switch(iDN)
	{
		case 2:
			TrestleSpaceRight = 100;
		break;

		case 25:
			TrestleSpaceRight = 100;
		break;

		case 40:
			TrestleSpaceRight = 100;
		break;

		case 50:
			TrestleSpaceRight = 100;
		break;

		case 65:
			TrestleSpaceRight = 250;
		break;

		case 80:
			TrestleSpaceRight = 250;
		break;

		case 100:
			TrestleSpaceRight = 400;
		break;

	}
	return TrestleSpaceRight;
}



int COIIVerteiler::GetTrestleSpaceToFlanschLeft(int iDN)
{
	iDN =0;
	return 200;
}

int COIIVerteiler::GetTrestleSpaceToFlanschRight(int iDN)
{
	int TrestleSpaceRight = 150;

	switch(iDN)
	{
		
		case 65:
			TrestleSpaceRight = 150;
		break;

		case 80:
			TrestleSpaceRight = 150;
		break;

		case 100:
			TrestleSpaceRight = 200;
	}

	return TrestleSpaceRight;
}



int COIIVerteiler::GetTrestleSpaceToGewindeLeft(int iDN)
{
	int TrestleSpaceRight = 150;

	switch(iDN)
	{
		
		case 50:
			TrestleSpaceRight = 200;
		break;

		case 65:
			TrestleSpaceRight = 150;
		break;

		case 80:
			TrestleSpaceRight = 150;
	}

	return TrestleSpaceRight;
}



int COIIVerteiler::GetTrestleSpaceToGewindeRight(int iDN)
{
	int TrestleSpaceRight = 100;

	switch(iDN)
	{
		
		case 50:
			TrestleSpaceRight = 100;
		break;

		case 65:
			TrestleSpaceRight = 100;
		break;

		case 80:
			TrestleSpaceRight = 100;
	}

	return TrestleSpaceRight;
}



int COIIVerteiler::GetTrestleSpaceToBodenLeft(int iDN)
{
	iDN = 0;
	return 150;
}



int COIIVerteiler::GetTrestleSpaceToBodenRight(int iDN)
{
	int TrestleSpaceRight = 100;

	switch(iDN)
	{
		case 80:
			TrestleSpaceRight = 100;
		break;

		case 100:
			TrestleSpaceRight = 150;
	}

	return TrestleSpaceRight;
}



int COIIVerteiler::GetTrestleSpaceLeftAreaNozzle(int iDN, int iLeftAbstand)
{
	int iTrestleSpaceLeft = 100;

	switch(iDN)
	{
		case 2:
			iTrestleSpaceLeft = 100;
		break;

		case 25:
			iTrestleSpaceLeft = 100;
		break;

		case 40:
			iTrestleSpaceLeft = 100;
		break;

		case 50:
			iTrestleSpaceLeft = 100;
		break;

		case 65:
			iTrestleSpaceLeft = 100;
		break;

		case 80:
			iTrestleSpaceLeft = 100;
		break;

		case 100:
			iTrestleSpaceLeft = 150;
		break;

	}

	if( iLeftAbstand > iTrestleSpaceLeft )
	{
		iTrestleSpaceLeft = iLeftAbstand;
	}
	return iTrestleSpaceLeft;
}



void COIIVerteiler::check_elementArray()
{
	string err, strx, txt;
	

	//Modell aus den Eingabedaten erzeugen (elementList wird neu gesetzt)
	createModell();

/*
	err = check_abstandRohranfangEnde(A_MIN_ABSTAND_ANFANG,A_MIN_ABSTAND_ENDE,0,0,0,0,false, false, true);

	if (!err.empty())
	{
		if (is_av)
			getErrorHandler().appendErrorDescription(err, MxError::Information);
		else
			throw (CString)err.c_str();
	}

	err = check_abstandZwischenStutzen(A_MIN_ZWISCHEN_STUTZEN,0,true);

	if (!err.empty())
	{
		if (is_av)
			getErrorHandler().appendErrorDescription(err, MxError::Information);
			
		else
			throw (CString)err.c_str();
	}
*/

	// bei Strang etc. Elementarray mit Standardelementen und AVElementen fuellen
	fill_elementArray();


	// Elementarray durchgehen	
	for (int i=0; i < elementArray.GetSize(); i++)
	{
		txt = getStammdaten().getText("IDS_FORMAT_ELE_ERROR");
		strx = str(format(txt) % get_posNr() % (i+1));
		
		Element *ele = elementArray.get_element_at(i);
		// vorheriges & naechstes fuer diverse Abfragen 
		Element *prev = i > 0 ? elementArray.get_element_at(i-1) : NULL;
		Element *next = i < (elementArray.GetSize()-1) ? elementArray.get_element_at(i+1) : NULL;

		// Aussendurchmesser holen fuer diverse Abfragen zum Abstand
		DNzuAussendurchmesser dntoad;
		int anschlussAussendurchmesser = dntoad.getAussendurchmesserToInt(ele->get_anschlussDN());
	
		if (ele->get_abstRel() == 0 && !ele->isEleRed())
		{
			
			if (!prev || 
				ele->get_winkel() == prev->get_winkel() && !ele->isEleAbgEnde() 
				&& ele->get_id() != GEGENFLANSCH_COII_ABGANG
				&& ele->get_id() != GEGENFLANSCH_GEWINDE_COII
				&& ele->get_id() != GEGENFLANSCH_GEWINDE_MUFFE_COII
				&& ele->get_id() != GEGENFLANSCH_COII_SCHWARZ_ABGANG
				      || ele->isEleBogen())
			{
				err = getStammdaten().getText("IDS_FEHLER_GLEICHER_WINKEL_SELBE_POS"); //Relativer Abstand 0 nicht zulässig!
				throw (CString)(strx + err).c_str();
			}
		}
		// DN checken
		err = ele->check_dn();
		if (err.length() > 0)
			throw (CString)(strx + err).c_str();
		
// Verkettung bei erstem Element überprüfen
		if (!i && ele->get_verkettung() == 1)
		{
			err = getStammdaten().getText("IDS_ERROR95"); //Erstes Element kann nicht verkettet sein!
			throw (CString)(strx + err).c_str();
		}

// Quali mit neuer DN checken
		if (ele->isEleRedEinz())
		{
			check_rohrQuali(ele->get_dn());
		}

// Absoluten Abstand vom Anfang und Ende überprüfen
		check_absAbst_rohrlaenge(ele);
		
// Laenge vom Rohrstutzen checken
		if (ele->get_id() == ROHR) 
		{
			
			// min. Laenge Abgang
			const int zuschlag = 50; int laenge = ele->get_laenge();
			if (next && next->isEleAbgEnde())
				laenge -= next->get_laenge();
			if (laenge < (anschlussAussendurchmesser/2 + zuschlag))
			{
				string txt = getStammdaten().getText("IDS_ERROR96");  //Länge vom Rohrstutzen muss min. 1/2*Rohrdurchmesser+Länge Flansch+50mm=%dmm sein!
				if (next)
					err = str(format(txt) % (anschlussAussendurchmesser/2 + zuschlag + next->get_laenge()));
				else
					err = str(format(txt) % (anschlussAussendurchmesser/2 + zuschlag));
				throw (CString)(strx + err).c_str();
			}
		}

// Winkel checken
		if (ele->get_winkel() < 0 && ele->get_winkel() > 359)  
		{
			err = getStammdaten().getText("IDS_ERROR97"); //Winkel der Abgänge kleiner 0° und grösser 359° ungültig!
			throw (CString)(strx + err).c_str();
		}


		// Abgang fuer Abschlusselement checken
		if (ele->isEleAbgEnde()) 
		{
			if (i == 0) 
			{
				err = getStammdaten().getText("IDS_ERROR98"); //Abschlusselement nicht direkt in Rohrleitung einfügbar!
				throw (CString)(strx + err).c_str();
			} 
			else if (prev)
			{
				if (prev->isEleAbgEnde()) 
				{
					err = getStammdaten().getText("IDS_ERROR99"); //Vorheriges Element war schon ein Abschlusselement!
					throw (CString)(strx + err).c_str();
				} 
				else if (prev->get_id() != ROHR && prev->get_id() != STDSTUTZEN) 
				{
					err = getStammdaten().getText("IDS_ERROR100"); //Vorheriges Element muss bei Abschlusselement ein Rohrstutzen sein!
					throw (CString)(strx + err).c_str();
				}
				else if (ele->get_winkel() != prev->get_winkel())
				{
					err = getStammdaten().getText("IDS_ERROR101");  //Rohrstutzen muss gleichen Winkel haben wie Abschlusselement!
					throw (CString)(strx + err).c_str();
				}
				else if (ele->get_abstRel() != 0)
				{
					err = getStammdaten().getText("IDS_ERROR102"); //Der relative Abstand muss 0 sein!
					throw (CString)(strx + err).c_str();
				}
			}
		}
	}
}


///Erzeugt die VK3-Elemente aus der Elementliste
bool COIIVerteiler::createVk3Liste()
{
	vk3elementeListe.SetRohrType((EnCObListVk3::ERohrType)ROHR_Q16);

	return createOIIVerteilerVk3Liste();
}

bool COIIVerteiler::createOIIVerteilerVk3Liste()
{
	// erstmal alte loeschen
	vk3elementeListe.deleteAllElemente();


	// Liste mit Elementen durchgehen
	for (POSITION pos = elementList.GetHeadPosition(); pos;)
	{
		Element *e = dynamic_cast<Element *>(elementList.GetNext(pos));
		XASSERT(e);
		if (!e)
			break;

	

		if (!e->isEleRohr())
			continue;

		//TRACE("Element ID: %d\n",e->get_id());

		Element* ele = new Element;
		ele->copy(*e);



		vk3elementeListe.AddTail(ele);

		CObList& uelementList = ele->get_uelementList();
		for (POSITION upos = uelementList.GetHeadPosition(); upos != NULL;)
		{
			Element *uelement = dynamic_cast<Element*>(uelementList.GetNext(upos));
			XASSERT(uelement);
			if (!uelement)
				break;

			// Element in Liste
			if( uelement->get_id() == ROHRNIPPEL_COII || uelement->get_id() == ROHR )
			{
				if(uelement->get_urid() != GEW)
				{
					Element *e = new Element;
					e->copy(*uelement);
					vk3elementeListe.AddTail(e);
				}
			}
		}
	}

	// gleiche Elemente zusammenfassen
	vk3elementeListe.kumulieren();
	//Menge hochsetzen
	vk3elementeListe.setMenge(get_anzahl());


	// zuletzt noch Preise setzen
	vk3elementeListe.setVk3Preise(get_beschichtung().isVerzinkt(), 0, 0, 0);


#ifdef _DEBUG
//afxDump << get_vk3elementeListe();
#endif
	return true;
}


