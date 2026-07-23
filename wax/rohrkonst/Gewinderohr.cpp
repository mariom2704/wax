#include "stdafx.h"


#include "resource.h"
#include "Gewinderohr.h"
#include "Element.h"
#include "RohrlisteAuftrag.h"
#include "ArbeitsplanGewinde.h"
#include "RohrlisteAuftrag.h"

#include <afxcoll.h>


#include <mxutil/strloader.h>
using namespace MxUtil;
using namespace MxErrorHandling;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif 

//Standardkonstruktor
Gewinderohr::Gewinderohr(const EnContainerRohrElemente& eArray, int anfid,int endid)
{
	init();	
	elementArray.copy(eArray);
	set_rohrEnde(endid);
	set_rohrAnfang(anfid);
}

//Standardkonstruktor
Gewinderohr::Gewinderohr()
{
	init();	
}

void Gewinderohr::init()
{
	typ	= WASSER;
	dichtmittel = HANF;
	zmassLinks = 0;
	zmassTyp = 0;
	zmassRefDN = 0;

	set_rohrKFarbe(KEINE);
	set_rohrQuali(RQ2440VZIN);
	set_laenge(0);
	set_dnvl(25);
	set_rohrVerb(GEW);
	set_rohrAnfang(GEW);
	set_rohrEnde(GEW);

	set_einkauf(false);
	get_leitkarte().set_sdMaterialnummer(400031);
	stdbezeichnung = getStammdaten().getText("IDS_GEWINDEROHR").c_str(); 
	get_preis().set_vk3flag(true);
}

//Destruktor
Gewinderohr::~Gewinderohr()
{}


void Gewinderohr::set_kaltbandgeschweisst(int val)
{
	if( val > 0)
	{
		if (get_rohrQuali() == RQEN10255_KALTBAND)
		{
			set_rohrQuali(RQ2440GSCH);
		}
			
		if(get_rohrQuali() == RQEN10255_KALTBAND_VERZ)
		{
			set_rohrQuali(RQ2440VZIN); 
		}
	}
}

void Gewinderohr::auto_set_kaltbandgeschweisst()
{
	/*
	Kaltbandgeschweisst ist true, wenn Rohrqualität eines Rohrstückes 63 oder 62 ist.
	Bei Gewinderohr wird dann die Rohrqualität umgeschaltet (siehe Gewinderohr::setRohrquali()).
	bleibt noch drinnen weil die Büros diese Version noch nicht haben
	*/
	
	if (get_rohrQuali() == RQEN10255_KALTBAND)
	{
		set_rohrQuali(RQ2440GSCH);
	}
		
	if(get_rohrQuali() == RQEN10255_KALTBAND_VERZ)
	{
		set_rohrQuali(RQ2440VZIN); 
	}
}

CString Gewinderohr::copy(const Rohrleitung& other)
{
	/*
	Umwandlung Rohrleitung in Gewinderohr
	*/
	Rohrleitung::copy(other);
	auto_set_kaltbandgeschweisst();
	setRohrquali();

	return CString("");
}


void Gewinderohr::serialize(BinaryArchive& ar)
{
	setLaengeDnvl();

	int version = ar.get_version() > 0 ? ar.get_version() : 1000;

	Rohrleitung::serialize(ar);
	if (ar.is_storing()) 
	{
		ar << elementArray.GetSize() << typ << dichtmittel << zmassLinks << zmassTyp;
		int kaltbandgeschweisst = 0; // kaltband wieder raus
		ar << zmassRefDN << kaltbandgeschweisst;
		
//#ifdef _DEBUG
//		CDumpContext dc;
//		elementArray.Dump(dc);
//#endif
		for (int i=0; i<elementArray.GetSize(); i++) 
		{
			Element *ele = elementArray.get_element_at(i);
			if (ele)
				ele->serialize(ar);
		}
	} 
	else 
	{
		int anzahlElemente = 0;
		ar >> anzahlElemente >> typ >> dichtmittel >> zmassLinks >> zmassTyp;
		if (version > 20)
			ar >> zmassRefDN;
		int kaltbandgeschweisst = 0; // kaltband wieder raus
		if (version >= 43)
		{
			ar >> kaltbandgeschweisst;
			set_kaltbandgeschweisst(kaltbandgeschweisst);
		}
		else
			kaltbandgeschweisst = 0; //alte Aufträge sind streckreduziert

		for (int i=0; i<anzahlElemente; i++) 
		{
			Element *ele = new Element;
			ele->serialize(ar);
			elementArray.Add(ele);
		}
	}
}

//Rohrkonstruktion auf korrekte Werte checken
CString Gewinderohr::checkPlausis()
{
	Werkauftrag::checkPlausis();
	CString err;
	
	err = checkTypDichtmittel();
	if (err.GetLength() > 0)
		return err;

	if ((get_rohrQuali() == RQEN10255_KALTBAND || get_rohrQuali() == RQEN10255_KALTBAND_VERZ) &&
		get_dichtmittel() != ArbeitsplanGewinde::HANF)
	{
		err = getStammdaten().getText("IDS_FALSCHEROHRQUALI").c_str(); 
		return err;
	}

	err = checkRohr();
	if (err.GetLength() > 0)
		return err;

	try
	{
		check_rohrAnfangEndeVerb();
		check_dnvl();
		check_posnr();
		check_elementArray();	
	}
	catch (CString exception)
	{
		err = exception;
	}
	catch (char *exception)
	{
		err = exception;
	}
	return err;
}

CString Gewinderohr::checkTypDichtmittel()
{
	CString err;
	CString frmtstr; frmtstr = getStammdaten().getText("IDS_ERROR27").c_str(); 

	if (typ == SCHAUM && dichtmittel == PERMABOND)
	{
		return "";
	}

	switch (dichtmittel)
	{
		case LOCTITE:
			if (get_beschichtung().isGepulvert())
			{
				CString loc; loc = getStammdaten().getText("IDS_LOCTITE").c_str(); 
				CString han; han = getStammdaten().getText("IDS_HANF").c_str(); 
				err.Format(getStammdaten().getText("IDS_LOCTITE_GEPULVERT").c_str(), loc, han);
			}
			if (typ == GAS)
			{
				CString loc; loc = getStammdaten().getText("IDS_LOCTITE").c_str(); 
				CString was; was = getStammdaten().getText("IDS_WASSERSPRINKLER").c_str(); 
				err.Format(frmtstr, loc, was);
			}
			break;
		case NEOFERMIT:
		if (typ == GAS)
		{
				CString neo; neo = getStammdaten().getText("IDS_NEOFERMIT").c_str(); 
				CString was; was = getStammdaten().getText("IDS_WASSERSPRINKLER").c_str(); 
				err.Format(frmtstr, neo, was);
		}
		break;
		case SYNTHESOL:
			if (typ == WASSER)
			{
				CString syn; syn = getStammdaten().getText("IDS_SYNTHESOL").c_str(); 
				CString gas; gas = getStammdaten().getText("IDS_GAS").c_str(); 
				err.Format(frmtstr, syn, gas);
			}
			break;
		case TEFLON:
			if (typ == WASSER)
			{
				CString tef; tef = getStammdaten().getText("IDS_TEFLON").c_str(); 
				CString gas; gas = getStammdaten().getText("IDS_GAS").c_str(); 
				err.Format(frmtstr, tef, gas);
			}
			break;
		case HANF:
		
			break;

		case BESTMK2241:
		if (typ == GAS)
		{
				CString neo; neo = getStammdaten().getText("IDS_NEOFERMIT").c_str(); 
				CString was; was = getStammdaten().getText("IDS_WASSERSPRINKLER").c_str(); 
				err.Format(frmtstr, neo, was);
		}
		break;

		case OHNE:
			{
				Element *e = NULL;
				for (int i = 0; i < elementArray.GetSize(); i++)
				{
					e = elementArray.get_element_at(i);
					if (e && e->get_id() == GRFITTING)
						break;
				}
				if (!e || e && e->get_id() != GRFITTING)
					break;
			}
		default:
			err = getStammdaten().getText("IDS_ERROR28").c_str(); 
			break;
	}

	return err;
}

CString Gewinderohr::checkRohr()
{
	CString err;
	int rohrcounter = 0;

	for (int i=0; i<elementArray.GetSize(); i++)
	{
		Element* ele = elementArray.get_element_at(i);
		if (!ele)
			continue;
		if (ele->get_abstRel() != 0 && !ele->get_verkettung())
			rohrcounter++;
		
		if (rohrcounter > 6)
		{
			err = getStammdaten().getText("IDS_ERROR29").c_str(); 
			return err;
		}

		// todo: muss noch zmass-links&rechts abziehen
		if (ele->get_abstRel() > 6000)
		{
			CString max; max = getStammdaten().getText("IDS_ERROR1").c_str(); 
			err.Format(max, 6000);
			return err;
		}
	}

	// Rohr auf NUT und GLATT checken
	// Jetzt möglich, nur Hinweistext.  
	if (elementArray.GetSize() > 0)
	{
		Element* ele = elementArray.get_element_at(0);
		if (ele && ele->isEleRohr())
		{
			if ((rohrAnfang == NUT   && rohrEnde == NUT)
			 || (rohrAnfang == NUT   && rohrEnde == GLATT)
			 || (rohrAnfang == GLATT && rohrEnde == NUT)
			 || (rohrAnfang == GLATT && rohrEnde == GLATT))
			{
				err = getStammdaten().getText("IDS_ERROR30").c_str(); 
				getErrorHandler().appendError(MxError((LPCSTR)err, MxError::Information));
				err = "";
			}
		}
	}

	return err;
}

void Gewinderohr::check_elementArray()
{
	string str;
	int sumlaenge = 0;
	bool kopf = false;
	Element* prev = NULL;
	Element* next = NULL;
	try
	{
		for (int i=0; i<elementArray.GetSize(); i++)
		{
			string form = getStammdaten().getText("IDS_FORMAT_ELE_ERROR");
			str = boost::str(boost::format(form) % get_posNr() % (i+1) );

			Element* ele = elementArray.get_element_at(i);
			// vorheriges und naechstes fuer diverse Abfragen
			if (i>0)
				prev = elementArray.get_element_at(i-1);
			if (i<elementArray.GetSize()-1)
				next = elementArray.get_element_at(i+1);

			// Laengen addieren
			sumlaenge += ele->get_abstRel();
		
			// Rohrstueck
			if (ele->get_id() == ROHR_Q1)
			{
				if (prev)
				{
					// nach Rohr ohne Fitting ist Pos. zu Ende 
					if (prev->get_id() == ROHR_Q1)
					{
						string txt = getStammdaten().getText("IDS_ERROR34");
						throw str + txt;
					}

					// vorheriges Element, das auch nicht verkettet ist, suchen 
					int k=i;
					Element* prevele = prev;
					while (prevele && prevele->get_verkettung() && k > 0)
					{
						k--;
						prevele = elementArray.get_element_at(k);
					}

					if (prevele && prevele->get_dn() != ele->get_dn())
					{
						string txt = getStammdaten().getText("IDS_NENNWEITE_LETZTES_ROHR");
						throw str + txt;
					}
					continue;
				}
				continue;
			}

			// Kopfanschluss checken
			if (kopf && !ele->get_verkettung())
			{
				throw str + getStammdaten().getText("IDS_ERROR31");
			}

			if (ele->get_kopfstueck())
			{
				if (i>0)
					kopf = true;
				if (ele->get_gruppenid() != FormstueckeUndFittinge::T_STUECK)
				{
					throw str + getStammdaten().getText("IDS_ERROR32");
				}
			}
			
			// check Z-Mass links
			if (i == 0 && ele->get_nurFitting() && zmassLinks > 0)
			{
				throw str + getStammdaten().getText("IDS_ERROR33");
			}
			
			if (prev)
			{
				// nach Rohr ohne Fitting ist Pos. zu Ende 
				if (prev->get_id() == ROHR_Q1)
				{
					throw str + getStammdaten().getText("IDS_ERROR34");
				}

				// keine Verkettung bzw. nur Rohr
				if (ele->get_verkettung() == 0)
				{
					// vorheriges Element, das auch nicht verkettet ist, suchen 
					int k=i;
					Element* prevele = prev;
					while (prevele->get_verkettung() && k > 0)
					{
						k--;
						prevele = elementArray.get_element_at(k);
					}
	 			
					if (ele->get_id() == ROHR_Q1)
					{
						if (ele->get_anschlussDN() != prevele->get_dn())
						{
							string txt = getStammdaten().getText("IDS_ERROR48");
							string err = boost::str(boost::format(txt) % str % ele->get_anschlussDN() % prevele->get_dn());
							throw err;
						}
					}
					// bei Kopfstueck DN2 checken
					else if (ele->get_gruppenid() == FormstueckeUndFittinge::T_STUECK && ele->get_kopfstueck())
					{
						if (ele->get_dn2() != prevele->get_dn())
						{
							string txt = getStammdaten().getText("IDS_ERROR49");
							string err = boost::str(boost::format(txt) % str % ele->get_dn2() % prevele->get_dn());
							throw err;
						}
					}
					// bei T-Stueck DN1 od. DN3 checken
					else if (ele->get_gruppenid() == FormstueckeUndFittinge::T_STUECK)
					{
						if (ele->get_dn1() != prevele->get_dn())
						{
							if (ele->get_dn3() != prevele->get_dn())
							{
								string txt = getStammdaten().getText("IDS_ERROR50");
								string err = boost::str(boost::format(txt) % str % ele->get_dn1() % ele->get_dn3() % prevele->get_dn());
								throw err;
							}
							ele->set_changeDN(true);
						}
					}
					// bei Kreuz-Stueck DN1 od. DN2 checken
					else if (ele->get_gruppenid() == FormstueckeUndFittinge::KREUZ_STUECK)
					{
						if (ele->get_dn1() != prevele->get_dn())
						{
							if (ele->get_dn2() != prevele->get_dn())
							{
								string txt = getStammdaten().getText("IDS_ERROR51");
								string err = boost::str(boost::format(txt) % str % ele->get_dn1() % ele->get_dn2() % prevele->get_dn());
								throw err;
							}
							ele->set_changeDN(true);
						}
					}
					// sonst DN1 od. DN2 checken
					else 
					{
						if (ele->get_dn1() != prevele->get_dn())
						{
							if (ele->get_dn2() == 0)
							{
								string txt = getStammdaten().getText("IDS_ERROR52");
								string err = boost::str(boost::format(txt) % str % ele->get_dn1() % prevele->get_dn());
								throw err;
							}
							if (ele->get_dn2() != prevele->get_dn())
							{
								string txt = getStammdaten().getText("IDS_ERROR51");
								string err = boost::str(boost::format(txt) % str % ele->get_dn1() % ele->get_dn2() % prevele->get_dn());
								throw err;
							}
							ele->set_changeDN(true);
						}
					}
			
					// bei Verschluss ist Ende
					if (prevele->get_gruppenid() == FormstueckeUndFittinge::STOPFEN || prevele->get_gruppenid() == FormstueckeUndFittinge::KAPPE)
					{
						throw str + getStammdaten().getText("IDS_ERROR35");
					}
						
					// Fittinge mit nur Aussengewinde
					if (ele->hasEleAussengewinde()) 
					{
						// Fittinge mit Aussengewinde koennen nicht ans Rohr ran
						if (ele->get_nurFitting() == 0)
						{
							throw str + getStammdaten().getText("IDS_ERROR36");
						}

						if (prevele->hasEleAussengewinde()) 
						{
							throw str + getStammdaten().getText("IDS_ERROR37");
						}
					}
					// Fittinge mit nur Innengewinde
					else if ( ele->hasEleInnengewinde()) 
					{
						// Wenn Fitting an Fitting dann
						if (ele->get_nurFitting())
						{
							if ( prevele->hasEleInnengewinde()) 
							{
								throw str + getStammdaten().getText("IDS_ERROR38");
							}
						}
					}
				}	
				// Verkettung an vorherigen Fitting
				else
				{
					// Fittinge mit nur Innengewinde
					if ( ele->hasEleInnengewinde()) 
					{
						if ( prev->hasEleInnengewinde()) 
						{
							throw str + getStammdaten().getText("IDS_ERROR38");
						}
					}
						
					// 1.Fitting
					if (prev->get_verkettung() == 0)
					{
						// Wenn DN3=0, dann kein Mittelabgang vorhanden  
						if (prev->get_dn3() == 0)
						{
							throw str + getStammdaten().getText("IDS_ERROR39");
						}
						// Anschluss bisher nur mit DN1 an DN2 moeglich
						if (prev->get_dn2() != ele->get_dn1())
						{
							string txt = getStammdaten().getText("IDS_ERROR53");
							string err = boost::str(boost::format(txt) % str % prev->get_dn2() % ele->get_dn1());
							throw err;
						}
					}
					// weitere Fittinge
					else
					{
						if (prev->get_gruppenid() == FormstueckeUndFittinge::KAPPE || prev->get_gruppenid() == FormstueckeUndFittinge::STOPFEN)
						{
							throw str + getStammdaten().getText("IDS_ERROR35");
						}
						if (prev->get_dn2() != ele->get_dn1() && prev->get_dn1() != ele->get_dn1())
						{
							string txt = getStammdaten().getText("IDS_ERROR54");
							string err = boost::str(boost::format(txt) % str % prev->get_dn1() % prev->get_dn2() % ele->get_dn1());
							throw err;
						}
					}
				}
			}
			else
			{
				// bei Kopfstueck DN2 checken
				if (ele->get_gruppenid() == FormstueckeUndFittinge::T_STUECK && ele->get_kopfstueck())
				{
					if (ele->get_dn2() != get_dnvl())
					{
						string txt = getStammdaten().getText("IDS_ERROR49");
						string err = boost::str(boost::format(txt) % str % ele->get_dn2() % get_dnvl());
						throw err;
					}
				}
				// bei T-Stueck DN1 od. DN3 checken
				else if (ele->get_gruppenid() == FormstueckeUndFittinge::T_STUECK)
				{
					if (ele->get_dn1() != get_dnvl())
					{
						if (ele->get_dn3() != get_dnvl())
						{
							string txt = getStammdaten().getText("IDS_ERROR50");
							string err = boost::str(boost::format(txt) % str % ele->get_dn1() % ele->get_dn3() % get_dnvl());
							throw err;
						}
						ele->set_changeDN(true);
					}
				}
				// bei Kreuz-Stueck DN1 od. DN2 checken
				else if (ele->get_gruppenid() == FormstueckeUndFittinge::KREUZ_STUECK)
				{
					if (ele->get_dn1() != get_dnvl())
					{
						if (ele->get_dn2() != get_dnvl())
						{
							string txt = getStammdaten().getText("IDS_ERROR51");
							string err = boost::str(boost::format(txt) % str % ele->get_dn1() % ele->get_dn2() % get_dnvl());
							throw err;
						}
						ele->set_changeDN(true);
					}
				}
				// sonst DN1 od. DN2 checken
				else 
				{
					if (ele->get_dn1() != get_dnvl())
					{
						if (ele->get_dn2() == 0)
						{
							string txt = getStammdaten().getText("IDS_ERROR52");
							string err = boost::str( boost::format(txt) % str % ele->get_dn1() % get_dnvl());
							throw err;
						}
						if (ele->get_dn2() != get_dnvl())
						{
							string txt = getStammdaten().getText("IDS_ERROR51");
							string err = boost::str(boost::format(txt) % str % ele->get_dn1() % ele->get_dn2() % get_dnvl());
							throw err;
						}
						ele->set_changeDN(true);
					}
				}
			}
		}

		// Gesamtlaenge checken
		if (sumlaenge > 12000)
		{
			string txt = getStammdaten().getText("IDS_ERROR1a");
			string err = boost::str(boost::format(txt) % 12000);
			throw err;
		}
	}
	catch (string& errstr)
	{
		getErrorHandler().appendError(MxErrorHandling::MxError(errstr));
	}
}

//Füllen des Elementarrays bei vordefinierten Rohrleitungen, z.B. 
//Stationsverteiler oder Entnahmeleitungen
void Gewinderohr::fill_elementArray()
{
	Rohrleitung::fill_elementArray();
}

//Rohrkonstruktion erzeugen
void Gewinderohr::createModell()
{
	Rohrleitung::createModell();

	// Löschen aller Elemente aus der Liste
	elementList.deleteAllElemente();

	if (elementArray.GetSize() <= 0 || !setLaengeDnvl())
		return;

	set_restabstand(); 

	//nicht noetig, da moegliche Elemente alle Laenge 0 haben 
	// Laengen für Anfang/Ende/Verb. setzen 
	//setAnfaEndeVerbLaenge();

	// Rohrlaengen definieren
	setRohrlaengen();		

	// Rohrkonstruktion
	makeGewinderohr(elementList, getErrorHandler());

	// Setzt die Rohrqualität des Rohres für die Druckliste
	setRohrquali();

	// Bei den Rohrstuecken links/rechts den Beschriftungstext setzen 
	rohreBeschriften(elementList);

	// Bei Rohrstuecken setzen ob mit od. ohne Rohrstutzen (fuers Pulvern)
	setRohrMitStut();
	calc_laengeProNr();		


	delete_stueckliste();
	create_stueckliste();


	//vk3 Listen erzeugen 
	createVk3Liste();
	createPulverlaengenListe();


	

//#ifdef _DEBUG
//	afxDump << get_elementList();
//	afxDump << get_elementListSap();
//#endif


	
/*
#ifdef _DEBUG
	afxDump << get_stueckliste();

	afxDump << get_stuecklisteSap();
#endif
*/
}

bool Gewinderohr::setLaengeDnvl()
{
	Element* ele = elementArray.get_element_at(0);
	if (!ele)
		return false;
	
	set_dnvl(ele->get_anschlussDN());

	int len = 0;
	for (int i=0; i<elementArray.GetSize(); i++)
	{
		Element* ele = elementArray.get_element_at(i);
		if (!ele)
			return false; 
		
		if (ele->get_abstRel() > 0)
			len += ele->get_abstRel();

		else if (ele->get_nurFitting() == 1 && !ele->get_verkettung())
			len +=  ele->get_zmass();
	}

	set_laenge(len);
	return len > 0 && get_dnvl() > 0;
}

int Gewinderohr::getRohrquali(int dn) const
{
	int rohrquali = 0; 
	if (get_typ() == GAS)
	{
		rohrquali = RQ2458VZIN;
	}
	else
	{
		
		{
			if (getBeschichtung().isVerzinkt() || dn < 20)
				rohrquali = RQ2440VZIN;
			else 
				rohrquali = RQ2440GSCH;
		}
	}
	return rohrquali+ROHR_Q1-1;
}

void Gewinderohr::setRohrquali()
{
	/**
	Setzt die Rohrqualität für den Werkauftrag. Diese Rohrqualität ist nur für die 
	Druckliste wichtig. 
	*/
	int rohrquali = 0;

	if (get_typ() == GAS)
	{
		rohrquali = RQ2458VZIN;
	}
	else
	{
		rohrquali = get_beschichtung().isVerzinkt() ? RQ2440VZIN : RQ2440GSCH;
	}

	set_rohrQuali(rohrquali);

	if (hasRohr2Quali())
		set_rohrQuali(RQ2440VZIN);
}

bool Gewinderohr::makeGewinderohr(EnCObListModell& elementList, MxErrorHandling::MxErrorHandler& error) const
{
	/*
	Rohrkonstruktion fuer 1 Rohr 
	*/
	int i=0, zmasslinksmerk = 0, relAbst = 0;
	const Element* ele = NULL;
	Element* prevele = NULL;
	Element* prevprevele = NULL;
	bool firstEle = true;
	bool letztesRohrOhneFitting = false;

	// Elemente initialisiert?
	try 
	{ 
		// Elemente
		while (i < elementArray.GetSize()) 
		{
			ele = elementArray.get_element_at(i);
			Element *elemente = NULL;
			
			if (firstEle) 
			{
				// Rohranfang
				if (ele->get_abstRel() > 0)
				{
					Element *elementA = new Element;
					elementList.AddTail(elementA);
					elementA->initStammdaten(ele->get_anschlussDN(), ele->get_anschlussDN(), rohrAnfang);
					elementA->initDaten(zmassLinks, zmassLinks, rohrAnfaLaenge, rohrAnfang, 0);
					elementA->set_firstele(true);
					firstEle = false;
				}
			}
			
			if (i > 0) 
			{
				// Fitting mit Verkettung nach oben
				if (ele->get_verkettung()) 
				{
					elemente = new Element;
					prevele->get_uelementList().AddTail(elemente);
					elemente->copy(*ele);
					elemente->initStammdaten(ele->get_artikelnr(), ele->get_id());
					if (i == elementArray.GetSize() - 1)
						prevele->set_lastele(true);
				}
			}
			if (!ele->get_verkettung())
			{
				// nur Fitting rechts
				if (ele->get_abstRel() == 0) 
				{
					// Fitting
					elemente = new Element;
					elementList.AddTail(elemente);
					elemente->copy(*ele);
					elemente->initStammdaten(ele->get_artikelnr(), ele->get_id());
					elemente->set_abstAbs((int)elemente->getAbstand()); 

					// rel. Abstand fuer Fittinge mit Rohrlaenge 0 
					if (ele->hasEleInnengewinde())
						relAbst += ele->getZmass(Element::RECHTS);
					else if (ele->get_gruppenid() == FormstueckeUndFittinge::DOPPELNIPPEL)
						relAbst += ele->get_laenge();

					if (elemente->hasEleInnengewinde() && i>0)
					{
						elemente->set_abstRel(relAbst);
						elemente->set_abstAbs(relAbst);
					}
										
					if (firstEle)
					{
						elemente->set_firstele(true);
						firstEle = false;
					}
					if (i == elementArray.GetSize() - 1)
						elemente->set_lastele(true);

					//mehrere Fittinge hintereinander die Z-Masse addieren
					if (i > 0 && ele->get_gruppenid() == FormstueckeUndFittinge::RED_NIPPEL) 
						zmasslinksmerk += prevele->getZmass(Element::LINKS);
				}
				// Rohr mit Fitting rechts
				else
				{
					if (ele->get_id() == GRFITTING)
					{
						// Rohr mit beidseitigem Gewinde
						Element *elementR = new Element;
						elementList.AddTail(elementR);
						elementR->initStammdaten(ele->get_anschlussDN(), ele->get_anschlussDN(), getRohrquali(ele->get_anschlussDN()));
						
						int zmassAbzugLinks = zmassLinks;
						int zmassAbzugRechts = 0;

						// rel. Abstand fuer Fittinge mit Rohrlaenge 0 
						relAbst	= ele->getZmass(Element::LINKS);

						//Z-Mass Abzug
						if (i>0)
						{
							zmassAbzugLinks = prevele->getZmass(Element::LINKS);
							zmassAbzugLinks += zmasslinksmerk;
							zmasslinksmerk = 0;
						}
						elementR->set_zmass(zmassAbzugLinks);
						
						zmassAbzugRechts = ele->getZmass(Element::RECHTS);
						
						// Ueber die Anlage?
						elementR->set_makeAnlage(ele->get_makeAnlage());
						
						elementR->initDaten(0, 0, ele->get_abstRel()-zmassAbzugLinks-zmassAbzugRechts, getRohrquali(ele->get_anschlussDN()), 0);

						// Fitting rechts
						elemente = new Element;
						elementList.AddTail(elemente);
						elemente->copy(*ele);
						elemente->initStammdaten(ele->get_artikelnr(), ele->get_id());
					
						if (i == elementArray.GetSize() - 1)
							elemente->set_lastele(true);
					}
					else
					{
						letztesRohrOhneFitting = true;
						break;
					}
				}
			}
			i++;
			if (!ele->get_verkettung())
			{
				prevele = elemente;
				prevprevele = prevele;
			}
		}

		if (letztesRohrOhneFitting)
		{
			Element *elementR = new Element;
			elementList.AddTail(elementR);

			int zmassAbzugLinks = zmassLinks;

			if (i>0)
			{
				if (prevele->get_gruppenid() == FormstueckeUndFittinge::F_MUFFE || prevele->get_gruppenid() == FormstueckeUndFittinge::RED_MUFFE)
					zmassAbzugLinks = prevele->get_zmass()/2;
				else
					zmassAbzugLinks = prevele->get_zmass();
			}
			elementR->set_zmass(zmassAbzugLinks);
			
			// Ueber die Anlage?
			elementR->set_makeAnlage(ele->get_makeAnlage());
			
			// Letztes Rohrstueck
			elementR->initStammdaten(ele->get_dn(), ele->get_dn(), getRohrquali(ele->get_dn()));
			elementR->initDaten(0, 0, ele->get_abstRel()-zmassAbzugLinks, getRohrquali(ele->get_dn()), 0);

			// Glatt, Nut od. Gewinde am Ende
			Element *elementE = new Element;
			elementList.AddTail(elementE);
			elementE->initStammdaten(ele->get_dn(), ele->get_dn(), rohrEnde);
			elementE->initDaten(ele->get_abstAbs(), ele->get_abstRel(), 
								rohrEndeLaenge, rohrEnde, 0);
			elementE->set_lastele(true);
		}
	}
	catch (Element::Error e) 
	{
		elementList.deleteAllElemente();
		error.appendError(MxError(e.get_errorText(), MxError::Information));
	}

	return getErrorHandler().getErrorCount() ? false : true;
}

//create stueckliste
void Gewinderohr::create_stueckliste()
{
	if (elementList.GetCount() > 0)
	{
		Element *element = dynamic_cast<Element*>(elementList.GetHead());
		XASSERT(element);
		if (!element)
			return;

		if (element->get_id() == GEW)
		{
			Element *e = new Element;
			FormstueckeUndFittinge::data f;
			CString str; str = getStammdaten().getText("IDS_KAPPE_GELB").c_str(); 
			CString txt = f.setKurztext(GRFITTING, str, FormstueckeUndFittinge::KAPPE, element->get_dn()); 
			e->initStammdaten(GRFITTING, element->get_dn(), FormstueckeUndFittinge::KAPPE, txt);
			stueckliste.add(e);
		}
		element = dynamic_cast<Element*>(elementList.GetTail());
		XASSERT(element);
		if (!element)
			return;

		if (element->get_id() == GEW)
		{
			Element *e = new Element;
			FormstueckeUndFittinge::data f;
			CString str; str = getStammdaten().getText("IDS_KAPPE_GELB").c_str(); 
			CString txt = f.setKurztext(GRFITTING, str, FormstueckeUndFittinge::KAPPE, element->get_dn()); 
			e->initStammdaten(GRFITTING, element->get_dn(), FormstueckeUndFittinge::KAPPE, txt);
			stueckliste.add(e);
		}
	}
	Rohrleitung::create_stueckliste();
}

//Hier wird automatisch der Arbeitsplan generiert
void Gewinderohr::createAutoArbeit(bool beschichtung)
{
	UNREFERENCED_PARAMETER(beschichtung);


	//Ermittlung Afo nach neuen Schema. (erst nach Freigabe von Herrn Scheel freischalten!)

	Rohrleitung::createAutoArbeit();

	
	//wenn nicht verzinkt, anliefern Pulveranlage 
	//if (!get_beschichtung().isVerzinkt()) 
	//	get_arbeit().addAfo_Anlieferung(get_beschichtung().isVerzinkt(), true);


	ArbeitsplanGewinde GewindeArbeitsplan;

	GewindeArbeitsplan.set_Werte(this, ArbeitsplanGewinde::GEWINDEROHR,get_beschichtung(),get_elementList());
	GewindeArbeitsplan.set_Gewinderohrwerte((ArbeitsplanGewinde::DICHTMITTEL)get_dichtmittel());

	//Arbeitsplan erzeugen
	GewindeArbeitsplan.createArbeitsplan(get_arbeit());

	GewindeArbeitsplan.FilterFertigugAfos(get_anzahl());

	if(Get_Fertigungsstaette() ==  Werkauftrag::OLDESLOE)
	{
		if (get_anzahl() > 1)
		{
			GewindeArbeitsplan.setGesamtAnzahl(get_anzahl());
		}
	}


	if(!GewindeArbeitsplan.Get_UseArbeitsplanForVk3())
	{
		if(!get_UseSammelauftrag())
		{
			
			RohrlisteAuftrag RohrlisteAuftrag (this);
			GewindeArbeitsplan.createExtraAutoArbeitSaegen(&RohrlisteAuftrag);

		}
	}




}


//get typ
int Gewinderohr::get_typ() const
{
	return typ;
}

//set typ
void Gewinderohr::set_typ(int value)
{
	typ = value;
}

//get dichtmittel
const int Gewinderohr::get_dichtmittel() const
{
	return dichtmittel;
}

//set dichtmittel
void Gewinderohr::set_dichtmittel(int value)
{
	dichtmittel = value;
}

//get zmassLinks
const int Gewinderohr::get_zmassLinks() const
{
	return zmassLinks;
}

//set zmassLinks
void Gewinderohr::set_zmassLinks(int value)
{
	zmassLinks = value;
}

const int Gewinderohr::get_zmassTyp() const
{
	return zmassTyp;
}

void Gewinderohr::set_zmassTyp(int value)
{
	zmassTyp = value;
}

bool Gewinderohr::isAnlagOK(int dn, int lange)const
{
	if (lange >= 200 && lange <= 6000 && dn > 20 && dn < 65) 
		return true;
	
	return false;
}

const int Gewinderohr::get_zmassRefDN() const
{
	return zmassRefDN;
}

void Gewinderohr::set_zmassRefDN(int value)
{
	zmassRefDN = value;
}

void Gewinderohr::addRohrausschuss(CObList& list)
{
	//gleicher Verschnitt wie bei Verteilerleitungen
	Rohrleitung::addRohrausschuss(list);
}

void Gewinderohr::check_rohrQuali(int dnvl) const
{
	CString err, str;
	Element ele;
	ele.set_id(ROHR_Q1 + get_rohrQuali() - 1); 

	switch (get_rohrQuali())
	{
		case RQ2458GSCH:
			if (getBeschichtung().isGrundiert() && dnvl < 65)
			{
				str = getStammdaten().getText("IDS_ERROR56").c_str(); 
				throw (CString)(ele.get_bezeichnung() + str);
			}
			else if (get_laenge() > 9000 && dnvl < 50) 
			{
				str = getStammdaten().getText("IDS_ERROR57").c_str(); 
				throw (CString)(ele.get_bezeichnung() + str);
			}
			else if (dnvl < 32) 
			{
				str = getStammdaten().getText("IDS_ERROR58").c_str(); 
				throw (CString)(ele.get_bezeichnung() + str);
			}
			break;
		
	
		case RQ2440_2458GSCH:
			if (dnvl < 25 || dnvl > 300) 
			{
				str = getStammdaten().getText("IDS_ERROR61").c_str(); 
				throw ele.get_bezeichnung() + err;
			}
			break;

		case RQ2448NLOS:
			if (dnvl < 32) 
			{
				str = getStammdaten().getText("IDS_ERROR62").c_str(); 
				err.Format(str, 32);
				throw ele.get_bezeichnung() + err;
			}
			break;
		case RQ2440NLOS:
			if (dnvl > 150) 
			{
				str = getStammdaten().getText("IDS_ERROR63").c_str(); 
				err.Format(str, 150);
				throw ele.get_bezeichnung() + err;
			}
			break;

		case RQ2440VZIN:
			if (dnvl > 50) 
			{
				str = getStammdaten().getText("IDS_ERROR63").c_str(); 
				err.Format(str, 50);
				throw ele.get_bezeichnung() + err;
			}
			break;

		case RQ2458VZIN:
			if (dnvl > 50) 
			{
				str = getStammdaten().getText("IDS_ERROR63").c_str(); 
				err.Format(str, 50);
				throw ele.get_bezeichnung() + err;
			}
			break;
				
		case RQ2440GSCH:
			if (dnvl < 25 || dnvl > 50) 
			{
				str = getStammdaten().getText("IDS_ERROR64").c_str(); 
				throw ele.get_bezeichnung() + str;
			}
			break;
		case RQEN10217VERZ:
			if (dnvl < 32 || dnvl > 200) 
			{
				str = getStammdaten().getText("IDS_ERROR64").c_str(); 
				throw ele.get_bezeichnung() + str;
			}
			break;
		case RQEN10255_KALTBAND:
		case RQEN10255_KALTBAND_VERZ:
			if (dnvl < 32 || dnvl > 50) 
			{
				str = getStammdaten().getText("IDS_ERROR64").c_str(); 
				throw ele.get_bezeichnung() + str;
			}
			
			break;

		case LEER:
			err = getStammdaten().getText("IDS_ERROR40").c_str(); 
			throw err;
			break;
		
		default:
			CString str; str = getStammdaten().getText("IDS_ERROR41").c_str(); 
			XASSERT(err == str); // Ist false, Meldung wird also angezeigt
			throw str;
			break;
	} 
}

bool Gewinderohr::isBogen() const
{
	/*
	Ist ein Bogen/Winkel in der Konstruktion?
	*/
	for (POSITION pos = elementList.GetHeadPosition(); pos != NULL;)	
	{
		const Element *element = dynamic_cast<const Element*>(elementList.GetNext(pos));
		XASSERT(element);
		if (!element)
			break;

		CString str; str = getStammdaten().getText("IDS_WINKEL").c_str(); 
		if (element->get_kurztext().Find(str, 0) > -1)
			return true;
	} 
	return false;
}

bool Gewinderohr::hasRohr2Quali() const 
{
	// RQ2440VZIN &	RQ2440_2458GSCH?
	if (get_rohrQuali() == RQ2458VZIN)
		return false;

	bool verz = false;
	bool gepu = false;

	for (POSITION pos = elementList.GetHeadPosition(); pos != NULL;)	
	{
		const Element *ele = dynamic_cast<const Element*>(elementList.GetNext(pos));
		XASSERT(ele);
		if (!ele)
			break;

		if (ele->get_id() == ROHR_Q6)
			verz = true;
		else if (ele->get_id() == ROHR_Q1)
			gepu = true;
		if (verz && gepu)
			return true;	
	}
	return false;
}


///Erzeugen der Afos fuer die Beschichtung
void Gewinderohr::createBeschAfos(Arbeitsplan& plan)
{
	if (!get_beschichtung().isVerzinkt())
		plan.addAfo_Beschichtung(get_anzahl(), get_beschichtung(), getPulverzeit(), getGrundierungszeit(), getPutzzeit());
}

/**
Erzeugen der VK3-Elemente. Diese setzen sich aus den Festpreisen zu den Arbeitsfolgen und un dem Material zusammen
*/
bool Gewinderohr::createVk3Liste()
{
	//Bis ca. 31.08.06 wird Vk2 noch nach dem alten Schema ermittelt
	//if (m_isOldAfoVk2)
	//	return true;



	Arbeitsplan   aPlan;

	ArbeitsplanGewinde gpPlan;

	double festPreis;

	//afos nicht kumulieren, damit eine Unterscheidung nach Dn vorgenommen werden kann
	aPlan.set_kumulieren(false);

	//Arbeitsgänge für die Position neu generieren. (Afos liegen immer nur hinter der 1. Position im Sammelauftrag, darum hier neu berechnen)
	gpPlan.set_Werte(this, ArbeitsplanGewinde::GEWINDEROHR,get_beschichtung(),get_elementList(), true);
	gpPlan.set_Gewinderohrwerte((ArbeitsplanGewinde::DICHTMITTEL)get_dichtmittel());

	gpPlan.createArbeitsplan(aPlan);
	MxCObList &afoListe = aPlan.get_arbeitsfolgen();

	// alte Elementliste loeschen
	vk3elementeListe.deleteAllElemente();

	const VK3Preise& vk3 = getStammdaten().get_vk3preise();

	//Afos den Vk3 Elementen zuordnen
	MxCObListIterator iter(afoListe);
	while (iter.next())
	{
		Arbeitsfolge *afo  = dynamic_cast<Arbeitsfolge*>(afoListe.getAt(iter));
		XASSERT(afo);
		if(!afo)
			continue;
		festPreis = 0.0;
		

		const VK3Preise::data *v = vk3.query( Werkauftrag::OLDESLOE, afo->get_id(), 0, afo->get_dn(), 0, 0, 0, 0, 0, 0);
		
		if(v != NULL)
		{
			festPreis = v->get_preis();
		}

		
		//neues Element erzeugen
		Element* ele = new Element;
			
		ele->set_id(afo->get_id());

		
		//Anzahl = Anzahl der Afo *  Anzahl der Position
		ele->set_anzahl((int)afo->get_anz() * get_anzahl());
		ele->set_dn(afo->get_dn());
		ele->set_anschlussDN(afo->get_dn());

		ele->set_bezeichnung(afo->get_txt());
		ele->set_kurztext(afo->get_txt());
		ele->set_festPreis(0.0);

		if (festPreis > 0)
		{
			ele->set_idCounterVk3Preis(v->get_elemidcounter());
			ele->set_festPreis(festPreis);
		}
		ele->set_constpreis(true);
		vk3elementeListe.append(ele);
	}

	
	// gleiche Elemente zusammenfassen Achtung: Material nicht vor dem Kumulieren in die Liste schreiben, da nach Typ kumuliert wird
	// und alle Bauteile als Typ Fitting in der Liste stehen
	vk3elementeListe.kumulieren();
	
	//Material in VK3 Liste schreiben (ist schon kumuliert)
	EnStueckliste &bauteile =  get_stueckliste();

	EnCObListVk3 tempListe;

	iter.setListe(bauteile);
	while (iter.next())
	{
		const Element *ele = dynamic_cast<const Element*>(bauteile.getAt(iter));
		XASSERT(ele);
		if (!ele)
			continue;

		Element* newElement = (Element*)ele->deepCopy();

		tempListe.append(newElement);
	}

	//Auf Halbe Meter runden
	//tempListe.rundenHalbeMeter(); //Ab Version3.2 kein Runden mehr

	vk3elementeListe.appendListe(tempListe);

//#ifdef _DEBUG
//	afxDump << vk3elementeListe;
//#endif
	
	return true;
}


void Gewinderohr::createPulverlaengenListe()
{
	const int VK3_PULVERN_ID = 702;

	// erstmal alte loeschen
	pulverlaengenListe.deleteAllElemente();

	if (!get_beschichtung().isGepulvert())
		return;

	
	//Vk3 für Pulvern berechnen 
	int laenge = get_laenge();
	
	
	//neues Element erzeugen
	Element* elePulvern = new Element;
	elePulvern->set_id(VK3_PULVERN_ID);
	elePulvern->set_laenge(laenge * get_anzahl());

	int dn = get_elementList().getMinDn(true);
	elePulvern->set_dn(dn);
	elePulvern->set_anschlussDN(dn);

	elePulvern->initStammdatenVK3Ele(dn, dn, elePulvern->get_id(), false, 0, 0, 0, 0, 0, 0); 	

	pulverlaengenListe.append(elePulvern);

	// zuletzt noch Preise setzen
	pulverlaengenListe.setVk3Preise(false, 0, 0, 0);
}


string Gewinderohr::get_beschriftung(int dn, bool zoll, int rohrcount) const
{
	zoll = 0; // wg. Warnung unreferenziert
	rohrcount = 0;
	dn = 0;
	return "";
}