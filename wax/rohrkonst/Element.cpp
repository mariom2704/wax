#include "stdafx.h"
#include "resource.h"

#include "..\WaxDef.h"
#include "resource.h"
#include "Rohrleitung.h"

#include "GpConvertOldVersion.h"


		#include "Element.h"

using namespace MxUtil;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

///Standardkonstruktor
Element::Element()
{
	init_ele();
}

///Standardkonstruktor
Element::Element(int eleid, const CString& elebez, int eleanz, int eledn, int eleanschlussdn, double festpreis, double sappreis)
{
	init_ele();
	set_rohrNr(0);
	set_id(eleid);
	set_bezeichnung(elebez);
	set_anzahl(eleanz);
	set_dn(eledn);
	set_anschlussDN(eleanschlussdn);
	set_festPreis(festpreis);
	set_sapPreis(sappreis);
	set_korrektur_winkel(0);
	m_IgnoreRohrOrVk3Rohr = false;
}



///Kopykonstruktor
Element::Element(const Element& element)
{
	copy(element);
}

///Konstruktor
Element::Element(int id, int laenge, int dn, int anschlussdn, int abstRel, int abstAbs, int winkel)
{
	// Erst ein Standardelement erzeugen, um alle Daten zu initialisieren
	Element ele;
	copy(ele);

	initDaten(abstAbs, abstRel, laenge, id, winkel);
	set_dn(dn);
	set_anschlussDN(anschlussdn);
	set_korrektur_winkel(0);
	m_IgnoreRohrOrVk3Rohr = false;
}

///Destruktor
Element::~Element()
{
	uelementList.deleteAllElemente();
}


WaxBase *Element::deepCopy()const		///Gibt eine neues mit new erzeugtes Objekt zurück
{
	Element *pNew = new Element;

	pNew->copy(*this);

	return pNew;
}


///Kopieren der Elementattribute
void Element::copy(const Element& other)
{
	Material::copy((Material&)other);

	wanr			= other.wanr;
	abstAbs			= other.abstAbs;
	abstRel			= other.abstRel;
	anschlussDN		= other.anschlussDN;
	dn				= other.dn;
	laenge			= other.laenge;
	pos				= other.pos;
	verkettung		= other.verkettung;
	verkettungAbst	= other.verkettungAbst;
	winkel			= other.winkel;
	rohrLaenge		= other.rohrLaenge;
	rohrPosNr		= other.rohrPosNr;
	beschriftungLinks  = other.beschriftungLinks;
	beschriftungRechts = other.beschriftungRechts;
	firstele		= other.firstele;
	lastele			= other.lastele;
	makeAnlage      = other.makeAnlage;
	ElementAnzahlid = other.ElementAnzahlid;
	ElementAnzahldnMuffeOrDBohrung = other.ElementAnzahldnMuffeOrDBohrung;
	//Fitting
	dn1				= other.dn1;
	dn2				= other.dn2;
	dn3				= other.dn3;
	dn4				= other.dn4;
	gruppenid		= other.gruppenid;
	gfzeichen		= other.gfzeichen;
	zmass			= other.zmass;
	nurFitting		= other.nurFitting;
	kopfstueck		= other.kopfstueck;
	changeDN        = other.changeDN;
	zmass2			= other.zmass2;
	zmass3			= other.zmass3;
	korrektur_winkel = other.get_korrektur_winkel();
	laengeZuschnitt = other.laengeZuschnitt;
	m_IgnoreRohrOrVk3Rohr = other.m_IgnoreRohrOrVk3Rohr;

	//Unterelementliste kopieren
	//Element *xEle = const_cast<Element*>(&other);
	copy_uelementlist(other.get_uelementList());
	
}

void Element::copy_uelementlist(const EnCObListModell& ue)
{
	uelementList.copy(ue);
}



///Speichern und Laden der Elementdaten
void Element::serialize(BinaryArchive& ar)
{
	/**
	Beim Speichern in eine Datei müssen aktuelle Werte aus den Stammdaten abgespeichert 
	werden.
	*/
	/** Erstmal disabled, wg. Verzinkungskosten. Serialize darf eigentlich nichts mehr berechnen!
	
	if (ar.is_storing() && ar.m_strFileName.GetLength())
		initStammdaten();
	*/
	Material::serialize(ar);

	int version = ar.get_version() > 0 ? ar.get_version() : 1000;

	serialize_bool_if(version > 11, changeDN, ar);
	serialize_bool_if(version > 12, makeAnlage, ar);

	if (ar.is_storing())	
	{
		ar << abstAbs << abstRel << dn << anschlussDN << laenge << winkel << verkettung;
		//Fitting
		ar << nurFitting << zmass << kopfstueck << zmass2 << zmass3; 
	} 
	else 
	{
		ar >> abstAbs >> abstRel >> dn >> anschlussDN >> laenge >> winkel >> verkettung;	
		//Fitting
		ar >> nurFitting >> zmass >> kopfstueck; 

		if (version > 22)
			ar >> zmass2;
		if (version > 26)
			ar >> zmass3;

		if (get_id() == GRFITTING)
		{
			const CObList& fuf = getStammdaten().get_formstueckefittinge();
			for (POSITION pos = fuf.GetHeadPosition(); pos;)
			{
				FormstueckeUndFittinge::data *f = (FormstueckeUndFittinge::data *)fuf.GetNext(pos);
				if (f->get_elemid() == get_id() && f->get_artikelnr() == get_artikelnr()) 
				{
					gruppenid = f->get_gruppenid();
					gfzeichen = f->get_gfzeichen();
					dn1 = f->get_dn1();
					dn2 = f->get_dn2();
					dn3 = f->get_dn3();
					dn4 = f->get_dn4();
					set_zmass(f->get_zmass1());
					set_zmass2(f->get_zmass2());
					set_zmass3(f->get_zmass3());
					if (changeDN)
						changeDNundZMass();
					///Setzt die (Anschluss-)Nennweite
					setAnschlussNennweite();

					break;
				}
			}
		}

				// Kurztext ergaenzen
		if (get_id() == GRFITTING && get_kopfstueck())
			set_kurztext(getStammdaten().getText("IDS_KOPF").c_str() + kurztext);
		

		if (version <= 37)
		{	
			//alte FlanschID konvertieren
			set_id(GpConvertOldVersion::getNewFlanschId(get_id(),dn));
		}
	}
}

void Element::init(const FormstueckeUndFittinge::data& f)
{
	Material::init(f);

	gruppenid = f.get_gruppenid();
	gfzeichen = f.get_gfzeichen();
	
	if (get_kopfstueck())
		set_kurztext(getStammdaten().getText("IDS_KOPF").c_str() + f.get_kurztext());
	else
		set_kurztext(f.get_kurztext());
	
	set_laenge(f.get_laenge());

	set_id(GRFITTING);
	
	
	dn1 = f.get_dn1();
	dn2 = f.get_dn2();
	dn3 = f.get_dn3();
	dn4 = f.get_dn4();
	set_zmass(f.get_zmass1());
	set_zmass2(f.get_zmass2());
	set_zmass3(f.get_zmass3());

	if (f.get_elemid() != GRFITTING)
	{
		throwError(__LINE__);
	}

	if (changeDN)
		changeDNundZMass();
	///Setzt die (Anschluss-)Nennweite
	setAnschlussNennweite();
}

///Initialisierung des Elements mit den Stammdaten aus der Tabelle Bauelemente
void Element::init(const Bauelemente::data &g)
{
	Material::init(g);
	
	set_bezeichnung("");
	set_kurztext("");
	get_bezeichnung();
	get_kurztext();
	//if (get_mass1() == 0)
	set_mass1(g.get_mass1());
	//if (get_mass2() == 0)
	set_mass2(g.get_mass2());

	if (get_laenge() == 0)
		set_laenge(get_mass1()); 

	set_dn(g.get_dn());
	if (g.get_dnvl())
		set_anschlussDN(g.get_dnvl());

	if (get_durchmesser() == 0)
		set_durchmesser(get_mass2());
}

///Initialisierung des Elements mit den Stammdaten aus der Tabelle Sondermaterial
void Element::init(const Sondermaterial::data &s)
{
	Material::init(s);
	
	set_mass1(s.get_mass1());
	set_mass2(s.get_mass2());
	set_bezeichnung("");
	if (s.get_kurztext().GetLength())
		set_kurztext(s.get_kurztext());
	get_bezeichnung();
	get_kurztext();
	set_kurztext(get_kurztext()+s.get_bez());
	if (get_laenge() == 0)
		set_laenge(get_mass1()); 

	set_dn(s.get_dn());
	if (s.get_dnvl())
		set_anschlussDN(s.get_dnvl());
	set_id(s.get_elemid());
}

///Initialisiert die Daten des Elementes, je nach Typ mit der richtigen Methode
void Element::initStammdaten(bool verzinkt)
{
	if (get_id() == GRFITTING)
	{
		try
		{
			initStammdaten(get_artikelnr(), get_id());
		}
		catch(Element::Error e)
		{
			TRACE("%s\n", e.get_errorText().c_str() );	
		}
	}
	else if (get_id() == FORMSTUECKE || get_id() == FITTINGE || get_id() == PROFIL)
	{
	}
	else 
	{
		try
		{
			// Die VK3Elemente wie Pulvern, Passlaenge haben IDs ueber 100 
			// und sind nicht in der Tabelle Bauelementegruppen drin
			if(MAX_MAX_ID > get_id() && get_id() > MAX_MIN_ID)
				initStammdatenVK3Ele(get_dn(), get_anschlussDN(), get_id(), verzinkt, 0, 0, 0, 0, 0, 0);
			else 
				initStammdaten(get_dn(), get_anschlussDN(), get_id(), get_laenge());
			
			if(SPRINKLERSCHELLE_VIROTEC == get_id() || FFL16 == get_id() || FFL10 == get_id() || VFL10NEU == get_id()  || VFL16NEU == get_id() )
			{
				initStammdaten(get_dn(), get_anschlussDN(), get_id(), get_laenge());
			}

		}
		catch (Element::Error e)
		{
			TRACE("%s\n", e.get_errorText().c_str() );
		}
	}
}

 
///Alle Elemente moeglich
void Element::initStammdaten(int artnr, int id)
{
	const Bauelemente::data* dat = getStammdaten().queryBauelemente.query<Bauelemente::data>(artnr, id);	
	if (dat)
	{
		set_id(dat->get_elemid());
		init(*dat);
		return;
	}
	const FormstueckeUndFittinge::data* fdat = getStammdaten().queryFormstueckeFittinge.query<FormstueckeUndFittinge::data>(artnr, id);
	if (fdat)
	{
		set_id(id);
		init(*fdat);
		return;
	}

	// Kein Material in Tabelle Bauelemente bzw. FormstückeUndFittinge, letzter Versuch: Sondermaterial
	const Sondermaterial::data* sondermat = getStammdaten().querySondermaterial.query<Sondermaterial::data>(artnr);
	if (sondermat)
	{
		init(*sondermat);
		return;
	}


	// ThrowError Aufruf macht hier keinen Sinn, da ja nur die Artikelnummer und evtl. die Id bekannt sind. 
	CString errstr;
	errstr.Format("Fehler bei Element::initStammdaten(int artnr, int id): Artikelnummer '%i (id=%i)' nicht gefunden!", artnr, id);
	throw Error(errstr);
}

///Initialisierung der Stammdaten
void Element::initStammdaten(int id, int dn, int gruppenid, CString kurztext, CString gfzeichen)
{
	// Achtung: nur fuer Gewinderohrfittinge und hier zunaechst nur fuer Kappe bei createStueckliste()
	set_id(id);
	set_dn(dn);
	set_gruppenid(gruppenid);
	set_kurztext(kurztext);
	set_gfzeichen(gfzeichen);
	
	XASSERT(get_id() == GRFITTING);

	const CObList& fuf = getStammdaten().get_formstueckefittinge();
	for (POSITION pos = fuf.GetHeadPosition(); pos;)
	{
		FormstueckeUndFittinge::data *f = (FormstueckeUndFittinge::data *)fuf.GetNext(pos);
		if (f->get_elemid() == get_id())
		{
			if (f->get_gruppenid() == get_gruppenid() //&& f->get_kurztext() == get_kurztext() 
													  && f->get_gfzeichen() == get_gfzeichen()
													  && f->get_dn1() == get_dn())
			{
				set_artikelnr(f->get_artikelnr());
				Material::init(*f);
				
				gfzeichen = f->get_gfzeichen();
				set_laenge(f->get_laenge());
				
				dn1 = f->get_dn1();
				dn2 = f->get_dn2();
				dn3 = f->get_dn3();
				dn4 = f->get_dn4();
				set_zmass(f->get_zmass1());
				set_zmass2(f->get_zmass2());
				set_zmass3(f->get_zmass3());
				if (changeDN)
					changeDNundZMass();
				///Setzt die (Anschluss-)Nennweite
				setAnschlussNennweite();
				return;
			}
		}
	}
}

///Initialisierung der Stammdaten
void Element::initStammdaten(int dn, int anschlussDN, int id, int laenge)
{
	// Achtung: nicht fuer Gewinderohrfittinge 
	this->dn = dn;
	this->anschlussDN = anschlussDN;
	set_id(id);

	XASSERT(get_id() != GRFITTING);

	// beim STDSTUTZEN muss die Laenge beruecksichtigt werden
	bool checklaenge = get_id() == STDSTUTZEN ? true : false;	
	if (checklaenge)
	{
		DNzuAussendurchmesser dntoad;
		int aussendurchmesser = dntoad.getAussendurchmesserToInt(anschlussDN);
		laenge = laenge - aussendurchmesser/2;
	}
	

	const CObList& grp = getStammdaten().get_bauelemente();
	
	for (POSITION gpos = grp.GetHeadPosition(); gpos;)
	{
		Bauelemente::data *g = (Bauelemente::data *)grp.GetNext(gpos);
		
		if (g->get_elemid() != get_id())
			continue;

		if (g->get_dn() == dn && (g->get_dnvl() == anschlussDN || g->get_dnvl() == 0))
 		{
			if (checklaenge && g->get_mass1() < laenge) 
				continue;
			init(*g);

			//Länge wieder setzen, da Werte aus Db diese mit Null überschreiben
			if (get_id() == SONDERMUFFE)
				set_mass1(laenge);

			return;
		}
	}
	const CObList& som = getStammdaten().get_sondermaterial();
	for (POSITION spos = som.GetHeadPosition(); spos;)
	{
		Sondermaterial::data *s = (Sondermaterial::data *)som.GetNext(spos);
		
		if (s->get_elemid() == get_id() && s->get_dn() == dn && (s->get_dnvl() == anschlussDN || s->get_dnvl() == 0))
 		{
			init(*s);
			return;
		}
	}
	// Wenn das Element nicht gefunden wurde, Debug Info initialisieren
	if (dn || anschlussDN)
		throwError(__LINE__);
}

///Initialisierung der Stammdaten für VK3Elemente
int Element::initStammdatenVK3Ele(int dn, int anschlussDN, int id, bool verzinkt, int Rohlaenge, int AnzahlElemente, int ElementAnzahld, double ElementAnzahldn, int ElementLaenge, int Anlage)
{
	// Achtung: nur fuer VK3Elemente 
	this->dn = dn;
	this->anschlussDN = anschlussDN;
	set_id(id);

	//Preis auf Null setzen, damit Warnung kommt, wenn kein Vk3 Preis gefunden wird. (Da sonst Materialpreis 
	//genommen wird!)
	set_festPreis(0.0);

	const VK3Preise& vk3 = getStammdaten().get_vk3preise();
	const VK3Preise::data *v = vk3.query(Werkauftrag::OLDESLOE, id, dn, anschlussDN, Rohlaenge, AnzahlElemente, ElementAnzahld, ElementAnzahldn, ElementLaenge, Anlage);

	if (v)
	{
		set_idCounterVk3Preis(v->get_elemidcounter());
		double dPreis = 0;
		if(verzinkt)
		{
			dPreis =  v->get_preis_verzinkt();
		}
		if(dPreis == 0)
		{
			dPreis =  v->get_preis();
		}
		set_festPreis(dPreis);
		//Preis bleibt const., sonst wird bei Serialize Element der VK3Elemente z.B. bei
		//Muffe der Preis aus den Stammdaten geholt und überschreibt den VK3Preis.
		set_constpreis(true);
		set_bezeichnung(v->get_bezeichnung().c_str());
		set_kurztext(v->get_bezeichnung().c_str());
	}
	else
	{
		set_idCounterVk3Preis(-1);
	}
	return v==0 ? 0 : v->get_PreisWertigkeit();
}

///Initialisierung der Stammdaten bei Sondermaterial
void Element::initStammdatenSoma(int dn, int anschlussDN, int id)
{
	// Achtung: nicht fuer Gewinderohrfittinge 
	this->dn = dn;
	this->anschlussDN = anschlussDN;
	set_id(id);

	XASSERT(get_id() != GRFITTING);
	
	const CObList& soma = getStammdaten().get_sondermaterial();
	for (POSITION pos = soma.GetHeadPosition(); pos;)
	{
		Sondermaterial::data *s = (Sondermaterial::data *)soma.GetNext(pos);
		
		
		if (s->get_elemid() == get_id() && s->get_dn() == dn && (s->get_dnvl() == anschlussDN || s->get_dnvl() == 0))
 		{
			init(*s);
			return;
		}
	}
	// Wenn das Element nicht gefunden wurde, Debug Info initialisieren
	throwError(__LINE__);
}

///Wirft eine Exception mit Debug Info
void Element::throwError(int line)
{
	// Bei initStammdaten()
	// Aus der Elementid die Bauteilbezeichnung holen
	BauelementGruppen bg;
	CString elebez = bg.getBezeichnung(get_id());

	string strModule = __FILE__; 

	string e = str(format(getStammdaten().getText("IDS_ERROR44")) % (LPCSTR)elebez % get_id() % get_artikelnr() % dn % anschlussDN % line );
	e = _T("    ") + e + _T(" Modul/File:") + strModule;  
	throw Error(e.c_str());
}

///Initialisierung der fehlenden Daten
void Element::initDaten(int abstAbs, int abstRel, int laenge, int id, int winkel, double durchmesser)
{
	// Auslesen - Initialisierung der fehlenden Daten
	XASSERT(id >= 0);       // Nach SCHWEISS kommt LEER = 0 
	//XASSERT(MAX_MAX_ID > MAX_MAX_ID || id <= MAX_MIN_ID);
	
	set_id(id);
	this->abstAbs = abstAbs;
	this->abstRel = abstRel;
	if (laenge != 0) 
		this->laenge = laenge;
	else 
		this->laenge = (int)get_mass1();
	this->winkel = winkel;
	this->set_durchmesser(durchmesser);
}

///Prüft Elemente
bool Element::verify()
{
	/*
	Pruefung diverser Attribute auf gueltigen Wert
	*/
	if (get_id() == 0 || dn == 0 || anschlussDN == 0)  
		 return false;
	return true;
}

///Pruefung der DN
CString Element::check_dn()
{
	CString err, str;
	if ((dn > anschlussDN) && !((get_id() == ROHR  || get_id() == NUTST) && (dn == 50 && anschlussDN == 40 || dn == 40 && anschlussDN == 32))
						   &&  (get_id() != ERED && get_id() != KRED && get_id() != EINZ)) 
	{
		err = getStammdaten().getText("IDS_ERROR2a").c_str(); //Nennweite der Abgänge grösser Rohrnennweite ist ungülti").c_str();
		return err;
	}

	if (dn !=  10 && dn !=  15 && dn !=  20 && dn !=  25 
				  && dn !=  32 && dn !=  40 && dn !=  50
				  && dn !=  65 && dn !=  80 && dn != 100 
				  && dn != 125 && dn != 150 && dn != 200 
				  && dn != 250 && dn != 300 && dn != 350 
				  && dn != 400 && dn !=   0)  
	{
		err = getStammdaten().getText("IDS_ERROR3a").c_str();  //Nennweite ungültig!
		return err;
	}

	switch (get_id())
	{
		case LEER: 
		{
			err = getStammdaten().getText("IDS_ERROR4a").c_str(); 
			return err;
		}
		case ROHR:
			if (dn < 20 || dn > 300) 
			{
				err = getStammdaten().getText("IDS_ERROR5a").c_str();  //Bei Rohrstutzen muss Nennweite zwischen DN 20 und 300 sein!
				return err;
			}
			break;
		case STDMUFFE:
			{
				int len; double d;
				if (!get_Stammdaten(len, d, get_id(), dn, anschlussDN))
				{
					err = getStammdaten().getText("IDS_ERROR128").c_str(); 
					return err;
				}
			}
			break;
		case MUFFE:
			if (dn < 10 || dn > 65) 
			{
				err = getStammdaten().getText("IDS_ERROR6a").c_str();  //Bei Muffe muss Nennweite zwischen DN 10 und 65 sein!
				return err;
			}
			if ((dn == 10) && (anschlussDN < 25 || anschlussDN > 300)) 
			{
				str; str = getStammdaten().getText("IDS_ERROR8a").c_str();  //Bei DN %i muss AnschlussDN zwischen %i und %i sein!
				err.Format(str, 10, 25, 300);
				return err;
			}
			if ((dn == 15) && (anschlussDN < 25 || anschlussDN > 300)) 
			{
				str = getStammdaten().getText("IDS_ERROR8a").c_str();  //Bei DN %i muss AnschlussDN zwischen %i und %i sein!
				err.Format(str, 10, 25, 300);
				return err;
			}
			if ((dn == 20) && (anschlussDN < 25 || anschlussDN > 300)) 
			{
				str = getStammdaten().getText("IDS_ERROR8a").c_str();  //Bei DN %i muss AnschlussDN zwischen %i und %i sein!
				err.Format(str, 10, 25, 300);
				return err;
			}
			if ((dn == 25) && (anschlussDN < 25 || anschlussDN > 300))
			{
				str = getStammdaten().getText("IDS_ERROR8a").c_str();  
				err.Format(str, 10, 25, 300);
				return err;
			}
			if ((dn == 32) && (anschlussDN < 32 || anschlussDN > 300)) 
			{
				str = getStammdaten().getText("IDS_ERROR8a").c_str(); 
				err.Format(str, 10, 32, 300);
				return err;
			}
			if ((dn == 40) && (anschlussDN < 40 || anschlussDN > 300))
			{
				str = getStammdaten().getText("IDS_ERROR8a").c_str();  
				err.Format(str, 10, 40, 300);
				return err;
			}
			if ((dn == 50) && (anschlussDN < 50 || anschlussDN > 300))
			{
				str = getStammdaten().getText("IDS_ERROR8a").c_str();  
				err.Format(str, 10, 50, 300);
				return err;
			}
			if ((dn == 65) && (anschlussDN < 65 || anschlussDN > 300))
			{
				str = getStammdaten().getText("IDS_ERROR8a").c_str();  
				err.Format(str, 10, 65, 300);
				return err;
			}
			break;
		case FRMUFFE:
			if (WaxSys::isAV()) 
			{
				if (dn < 10 || dn > 25) 
				{
					err = getStammdaten().getText("IDS_ERROR7").c_str();
					return err;
				}
				if ((dn == 10) && (anschlussDN < 32 || anschlussDN > 65)) 
				{
					str = getStammdaten().getText("IDS_ERROR8a").c_str();
					err.Format(str, 10, 32, 65);
					return err;
				}
				if ((dn == 15) && (anschlussDN < 32 || anschlussDN > 65)) 
				{
					str = getStammdaten().getText("IDS_ERROR8a").c_str();
					err.Format(str, 15, 32, 65);
					return err;
				}
				if ((dn == 20) && (anschlussDN < 32 || anschlussDN > 65)) 
				{
					str = getStammdaten().getText("IDS_ERROR8a").c_str();
					err.Format(str, 20, 32, 65);
					return err;
				}
				if ((dn == 25) && (anschlussDN < 50 || anschlussDN > 65)) 
				{
					str = getStammdaten().getText("IDS_ERROR8a").c_str();
					err.Format(str, 25, 50, 65);
					return err;
				}
			}
			break;
		case KRMUFFE:
			if (WaxSys::isAV()) 
			{
				if (dn < 10 || dn > 25) 
				{
					err = getStammdaten().getText("IDS_ERROR9a").c_str();
					return err;
				}
				if (anschlussDN < 32 || anschlussDN > 80) 
				{
					str = getStammdaten().getText("IDS_ERROR8a").c_str();
					err.Format(str, dn, 32, 65);
					return err;
				}


				if (dn < 20 && anschlussDN == 80) 
				{
					err = getStammdaten().getText("IDS_MUFFEN_GEKEHLT1").c_str();
					return err;
				}
				
			}
			break;
		case NUTST:
			if (dn < 32 || dn > 65) 
			{
				err = getStammdaten().getText("IDS_ERROR10a").c_str();
				return err;
			}
			if ((dn == 32) && (anschlussDN < 32 || anschlussDN > 300)) 
			{
				str = getStammdaten().getText("IDS_ERROR8a").c_str();
				err.Format(str, dn, 32, 300);
				return err;
			}
			if ((dn == 40) && (anschlussDN < 32 || anschlussDN > 300)) 
			{
				str = getStammdaten().getText("IDS_ERROR8a").c_str();
				err.Format(str, dn, 32, 300);
				return err;
			}
			if ((dn == 50) && (anschlussDN < 40 || anschlussDN > 300)) 
			{
				str = getStammdaten().getText("IDS_ERROR8a").c_str();
				err.Format(str, dn, 40, 300);
				return err;
			}			
			if ((dn == 65) && (anschlussDN < 65 || anschlussDN > 150)) 
			{
				str = getStammdaten().getText("IDS_ERROR8a").c_str();
				err.Format(str, dn, 65, 150);
				return err;
			}
			break;
		case GWRST:
			if (dn != 65) 
			{
				err = getStammdaten().getText("IDS_ERROR11a").c_str();
				return err;
			}
			break;
		case KRED:
			if (dn == anschlussDN)
			{
				err = getStammdaten().getText("IDS_ERROR12a").c_str();
				return err;
			}
			if (dn < 25 || dn > 400) 
			{
				err = getStammdaten().getText("IDS_ERROR13a").c_str();
				return err;
			}
			str = getStammdaten().getText("IDS_ERROR45").c_str();
			if ((dn == 25) && (anschlussDN != 50)) 
			{
				err.Format(str, 25, 50);
				return err;
			}
			str = getStammdaten().getText("IDS_ERROR8a").c_str();
			if ((dn == 32) && (anschlussDN < 40 || anschlussDN > 80))
			{
				err.Format(str, 32, 40, 80);
				return err;
			}
			if ((dn == 40) && (anschlussDN < 32 || anschlussDN > 100)) 
			{
				err.Format(str, 40, 32, 100);
				return err;
			}
			if ((dn == 50) && (anschlussDN < 25 || anschlussDN > 150)) 
			{
				err.Format(str, 50, 25, 150);
				return err;
			}
			if ((dn == 65) && (anschlussDN < 32 || anschlussDN > 150)) 
			{
				err.Format(str, 65, 32, 150);
				return err;
			}
			if ((dn == 80) && (anschlussDN < 32 || anschlussDN > 200)) 
			{
				err.Format(str, 80, 32, 200);
				return err;
			}
			if ((dn == 100) && (anschlussDN < 40 || anschlussDN > 250)) 
			{
				err.Format(str, 100, 40, 250);
				return err;
			}
			if ((dn == 125) && (anschlussDN < 50 || anschlussDN > 400)) 
			{
				err.Format(str, 125, 50, 400);
				return err;
			}
			if ((dn == 150) && (anschlussDN < 50 || anschlussDN > 400)) 
			{
				err.Format(str, 150, 50, 400);
				return err;
			}
			if ((dn == 200) && (anschlussDN < 80 || anschlussDN > 400)) 
			{
				err.Format(str, 200, 80, 400);
				return err;
			}
			if ((dn == 250) && (anschlussDN < 100 || anschlussDN > 400)) 
			{
				err.Format(str, 250, 100, 400);
				return err;
			}
			if ((dn == 300) && (anschlussDN < 125 || anschlussDN > 400)) 
			{
				err.Format(str, 300, 125, 400);
				return err;
			}
			if ((dn == 350) && (anschlussDN < 150 || anschlussDN > 400)) 
			{
				err.Format(str, 350, 150, 400);
				return err;
			}
			if ((dn == 400) && (anschlussDN < 150 || anschlussDN > 400)) 
			{
				err.Format(str, 400, 150, 400);
				return err;
			}
			break;
		case ERED:
			if (dn == anschlussDN)
			{
				err = getStammdaten().getText("IDS_ERROR12a").c_str();
				return err;
			}
			if (dn < 32 || dn > 400)
			{
				err = getStammdaten().getText("IDS_ERROR13a").c_str();
				return err;
			}
			str = getStammdaten().getText("IDS_ERROR8a").c_str();
			if ((dn == 32) && (anschlussDN < 40 || anschlussDN > 80)) 
			{
				err.Format(str, 32, 40, 80);
				return err;
			}
			if ((dn == 40) && (anschlussDN < 32 || anschlussDN > 100)) 
			{
				err.Format(str, 40, 32, 100);
				return err;
			}
			if ((dn == 50) && (anschlussDN < 32 || anschlussDN > 125)) 
			{
				err.Format(str, 50, 32, 125);
				return err;
			}
			if ((dn == 65) && (anschlussDN < 32 || anschlussDN > 150)) 
			{
				err.Format(str, 65, 32, 150);
				return err;
			}
			if ((dn == 80) && (anschlussDN < 32 || anschlussDN > 200)) 
			{
				err.Format(str, 80, 32, 200);
				return err;
			}
			if ((dn == 100) && (anschlussDN < 40 || anschlussDN > 250)) 
			{
				err.Format(str, 100, 40, 250);
				return err;
			}
			if ((dn == 125) && (anschlussDN < 50 || anschlussDN > 400)) 
			{
				err.Format(str, 125, 50, 300);
				return err;
			}
			if ((dn == 150) && (anschlussDN < 65 || anschlussDN > 400)) 
			{
				err.Format(str, 150, 65, 300);
				return err;
			}
			if ((dn == 200) && (anschlussDN < 80 || anschlussDN > 400)) 
			{
				err.Format(str, 200, 80, 300);
				return err;
			}
			if ((dn == 250) && (anschlussDN < 100 || anschlussDN > 400)) 
			{
				err.Format(str, 250, 100, 300);
				return err;
			}
			if ((dn == 300) && (anschlussDN < 125 || anschlussDN > 400)) 
			{
				err.Format(str, 300, 125, 250);
				return err;
			}
			if ((dn == 350) && (anschlussDN < 150 || anschlussDN > 400)) 
			{
				err.Format(str, 350, 150, 400);
				return err;
			}
			if ((dn == 400) && (anschlussDN < 150 || anschlussDN > 400)) 
			{
				err.Format(str, 400, 150, 350);
				return err;
			}
			break;
		case EINZ:
			if (dn < 25 || dn > 50) 
			{
				err = getStammdaten().getText("IDS_ERROR14").c_str();
				return err;
			}
			str = getStammdaten().getText("IDS_ERROR45").c_str();
			if ((dn == 25) && (anschlussDN != 32)) 
			{
				err.Format(str, 25, 32);
				return err;
			}
			str = getStammdaten().getText("IDS_ERROR46").c_str();
			if ((dn == 32) && (anschlussDN != 40) && (anschlussDN != 25)) 
			{
				err.Format(str, 32, 40, 25);
				return err;
			}
			if ((dn == 40) && (anschlussDN != 50) && (anschlussDN != 32)) 
			{
				err.Format(str, 40, 50, 32);
				return err;
			}
			str = getStammdaten().getText("IDS_ERROR45").c_str();
			if ((dn == 50) && (anschlussDN != 40)) 
			{
				err.Format(str, 50, 40);
				return err;
			}
			break;
		case BOGEN:
		case BOGVERZ:
		case BOG2S:
			if (dn < 25 || dn > 400) 
			{
				err = getStammdaten().getText("IDS_ERROR15").c_str();
				return err;
			}
			if (dn != anschlussDN) 
			{
				err = getStammdaten().getText("IDS_ERROR16").c_str();
				return err;
			}
			break;
		case BOHR24:
		case BOHR50:
			if (dn != 0) 
			{
				err = getStammdaten().getText("IDS_ERROR17").c_str();
				return err;
			}
			break;
		case NUT:
			if (dn < 25 || dn > 300) 
			{
				err = getStammdaten().getText("IDS_ERROR18").c_str();
				return err;
			}
			break;
		case GEW:
			if (dn < 15 || dn > 65) 
			{
				err = getStammdaten().getText("IDS_ERROR19").c_str();
				return err;
			}
			break;
		case FFL:
		case VFL16:
			if (dn < 32 || dn > 300) 
			{
				err = getStammdaten().getText("IDS_ERROR20").c_str();
				return err;
			}
			break;
		case VFL10:
			if (dn < 200) 
			{
				err = getStammdaten().getText("IDS_ERROR21").c_str();
				return err;
			}
			break;
		case AFL:
		case SFL:
			if (dn < 40 || dn > 300) 
			{
				err = getStammdaten().getText("IDS_ERROR22").c_str();
				return err;
			}
			break;
		case SCH:
			if (dn < 32 || dn > 150) 
			{
				str = getStammdaten().getText("IDS_ERROR23").c_str();
				err.Format(str, 32, 150); 
				return err;
			}
			break;
		case BOD:
			if (dn < 40 || dn > 300) 
			{
				err = getStammdaten().getText("IDS_ERROR24").c_str();
				return err;
			}
			if ((dn != 10)	&& (dn !=  15) && (dn !=  20)
					&& (dn !=  25) && (dn !=  32)
					&& (dn !=  40) && (dn !=  50)
					&& (dn !=  65) && (dn !=  80)
					&& (dn != 100) && (dn != 125)
					&& (dn != 150) && (dn != 200)
					&& (dn != 250) && (dn != 300)) 
			{
				err = getStammdaten().getText("IDS_ERROR24").c_str();
				return err;
			}
			break;
	}
	return "";
}

///Prueft, ob Rohr fuer Anlage gedreht werden muss 
int Element::checkDrehen(int laenge, int anfang, int ende)
{
	// Hier wird der Abstand des 1. Elements, das auf der Anlage gefertigt werden soll,
	// zum Anfang geprüft. Ist dieser zu klein, wird geprueft, ob das Rohr gedreht werden kann!   
	// return: 0 = nein, 1 = ja, 2 = Fehler
	bool first = true;
	for (POSITION upos = get_uelementList().GetHeadPosition(); upos != NULL;)	
	{
		Element* uele = dynamic_cast<Element*>(get_uelementList().GetNext(upos));
		XASSERT(uele);
		if (!uele)
			break;

		if (!uele->get_makeAnlage())
			continue;

		if (first)
		{
			first = false;
			if (uele->get_abstAbs() >= laenge + anfang)
				return 0;
			if (uele->get_abstAbs() > laenge + get_laenge() - ende)
				return 2;
		}
		else
		{
			if (uele->get_abstAbs() > laenge + get_laenge() - anfang)
				return 2;
		}
	}
	return 1;
}

///Hier wird ermittelt welche Afos ein Element hat. Ist firstafo gleich true, 
///wird nur die erste ermittelte Afo zurückgeliefert.
void Element::getAfos(int iProduktTyp, CArray<int,int> &afoNummern)
{
	afoNummern.RemoveAll();
	BauelementeAfos::data *ba = NULL;
	
	int laenge = 0;

	//todo: Schnittlaenge des Rohrst. nehmen 
	if (get_id() == ROHR)
		laenge = get_laenge();
	else 
		laenge = get_rohrLaenge();

	for (POSITION bapos = getStammdaten().get_bauelementeafos().GetHeadPosition(); bapos;)
	{
		ba = (BauelementeAfos::data *)getStammdaten().get_bauelementeafos().GetNext(bapos);

		//TRACE("ElemID: %d, Dn Von: %d, Dn Bis: %d, Code: %d\n", ba->get_elemid(),ba->get_dnvl_von(),ba->get_dnvl_bis(),ba->get_code1());

		if( ba->get_produktTyp() == Rohrleitung::WASSER && iProduktTyp == Rohrleitung::WASSER)
		{
			if (get_id() == BOHR24)
			{
				if ((ba->get_elemid() == get_id()) 
				 && (ba->get_isAnlage() == get_makeAnlage())) 
				{
					afoNummern.Add(ba->get_code1()); 
					afoNummern.Add(ba->get_code2());            
					afoNummern.Add(ba->get_code3());            
					afoNummern.Add(ba->get_code4());            
					break;
				}
			}
			else if (get_id() == SONDERMUFFE)
			{
				//für Sondermuffe ist noch der Schalter Anlage in der Tabelle gesetzt. 
				//Wenn dieser für alle Bauteile gepflegt ist, kann die extra Behandlung der Sondermuffe gelöscht werden!

				if ((ba->get_elemid()     == get_id()) 
		 		 && (ba->get_dn()         == get_dn()			|| ba->get_dn() == 0) 
				 && (ba->get_laenge_von() <= laenge				|| ba->get_laenge_von() == 0)
				 && (ba->get_laenge_bis() >= laenge				|| ba->get_laenge_bis() == 0)
				 && (ba->get_dnvl_von()   <= get_anschlussDN()  || ba->get_dnvl_von() == 0)
				 && (ba->get_dnvl_bis()   >= get_anschlussDN()	|| ba->get_dnvl_bis() == 0)
				 && ba->get_isAnlage() == get_makeAnlage())
				{
					afoNummern.Add(ba->get_code1()); 
					afoNummern.Add(ba->get_code2());            
					afoNummern.Add(ba->get_code3());            
					afoNummern.Add(ba->get_code4());            
					break;
				}
			}
			else
			{	
				if ((ba->get_elemid()     == get_id()) 
		 		 && (ba->get_dn()         == get_dn()			|| ba->get_dn() == 0) 
				 && (ba->get_laenge_von() <= laenge				|| ba->get_laenge_von() == 0)
				 && (ba->get_laenge_bis() >= laenge				|| ba->get_laenge_bis() == 0)
				 && (ba->get_dnvl_von()   <= get_anschlussDN()  || ba->get_dnvl_von() == 0)
				 && (ba->get_dnvl_bis()   >= get_anschlussDN()	|| ba->get_dnvl_bis() == 0))
				{
					afoNummern.Add(ba->get_code1()); 
					afoNummern.Add(ba->get_code2());            
					afoNummern.Add(ba->get_code3());            
					afoNummern.Add(ba->get_code4());            
					break;
				}
			}
		}

		if( ba->get_produktTyp() == Rohrleitung::GAS && iProduktTyp == Rohrleitung::GAS )
		{
				if ((ba->get_elemid()     == get_id()) 
		 		 && (ba->get_dn()         == get_dn()			|| ba->get_dn() == 0) 
				 && (ba->get_laenge_von() <= laenge				|| ba->get_laenge_von() == 0)
				 && (ba->get_laenge_bis() >= laenge				|| ba->get_laenge_bis() == 0)
				 && (ba->get_dnvl_von()   <= get_anschlussDN()  || ba->get_dnvl_von() == 0)
				 && (ba->get_dnvl_bis()   >= get_anschlussDN()	|| ba->get_dnvl_bis() == 0))
				{
					afoNummern.Add(ba->get_code1()); 
					afoNummern.Add(ba->get_code2());            
					afoNummern.Add(ba->get_code3());            
					afoNummern.Add(ba->get_code4());            
					break;
				}
		}
	}
	if (afoNummern.GetSize() == 0)
	{
		CString logentry;
		logentry.Format("Keine Afos gefunden für Element %i\n", get_id());
		LOG((LPCSTR)logentry);
	}
}

bool Element::isEleRohr() const
{
	return get_id() == ROHR_Q0 
		|| get_id() == ROHR_Q1
		|| get_id() == ROHR_Q3 
		|| get_id() == ROHR_Q5 
		|| get_id() == ROHR_Q6 
		|| get_id() == ROHR_Q7
		|| get_id() == ROHR_Q8
		|| get_id() == ROHR_Q9
		|| get_id() == ROHR_Q10
		|| get_id() == ROHR_Q11
		|| get_id() == ROHR_Q12
		|| get_id() == ROHR_Q13
		|| get_id() == ROHR_Q14
		|| get_id() == ROHR_Q15
		|| get_id() == ROHR_Q16
		|| get_id() == ROHR_Q17
		|| get_id() == ROHR_Q18
		|| get_id() == ROHR_Q19
		|| get_id() == ROHR_Q20;
}

bool Element::isEleAbgang() const
{
	return get_id() == ROHR    
		|| get_id() == MUFFE  
		|| get_id() == GWRST   
		|| get_id() == NUTST  
		|| get_id() == KRMUFFE
		|| get_id() == FRMUFFE
		|| get_id() == NEW_ROBBI_MUFFE
		|| get_id() == SONDERMUFFE
		|| get_id() == BOHR24 
		|| get_id() == BOHR50
		|| get_id() == STDMUFFE
		|| get_id() == STDSTUTZEN
		|| get_id() == MUFFE_COII;
}

bool Element::isEleEnde() const
{
	return get_id() == SCH 
		|| get_id() == BOD
		|| get_id() == KB40
		|| get_id() == KB50
		|| get_id() == BODEN_COII
		|| get_id() == GEWINDE_KAPPE_COII	
		|| get_id() == GEWINDE_KAPPE_TYP_D
		|| isEleAbgEnde();
}

bool Element::isEleAbgEnde() const
{
	return get_id() == NUT   
		|| get_id() == GEW 
		|| get_id() == GLATT
		|| isEleFlansch();
}

bool Element::isEleFlansch() const
{
	return get_id() == FFL 
		|| get_id() == FFL16
		|| get_id() == FFL10
		|| get_id() == VFL10
		|| get_id() == VFL16
		|| get_id() == VFL10NEU
		|| get_id() == VFL16NEU
		|| get_id() == AFL 
		|| get_id() == SFL
		|| get_id() == FLANSCH_COII
		|| get_id() == BLINDFLANSCH_COII
		|| get_id() == GEGENFLANSCH_COII_SCHARZ_HAUPTROHR
		;
}

bool Element::isEleRed() const
{
	return get_id() == ERED  
		|| get_id() == KRED;
}

bool Element::isEleRedBog() const
{
	return isEleRed()
		|| isEleBogen();
}

bool Element::isEleRedEinz() const
{
	return isEleRed()
		|| get_id() == EINZ;
}

bool Element::isKnick() const
{
	return get_id() == KNICK;
}

bool Element::isEleBogen() const
{
	return get_id() == BOGEN 
		|| get_id() == BOG2S 
		|| get_id() == BOGVERZ;
}

bool Element::isEleRedBogEinz() const
{
	return isEleRedEinz()
		|| isEleBogen();
}


bool Element::isFitting() const
{
	return get_id() == GRFITTING;
}

bool  Element::isTStueck()const
{
	return get_gruppenid() == TSTUECK;
}

bool  Element::isKreuzstueck()const
{
	return get_gruppenid() == KREUZSTUECK;
}

const bool Element::isEleVK3Rohr() const
{
	return ( get_id() == ROHR_Q0 
		|| get_id() == ROHR_Q0+100 
		|| get_id() == ROHR_Q0+200 
		|| get_id() == ROHR_Q0+300 
		|| get_id() == ROHR_Q0+400 
		|| get_id() == ROHR_Q0+600 

		|| get_id() == ROHR_Q1 
		|| get_id() == ROHR_Q1+100 
		|| get_id() == ROHR_Q1+200 
		|| get_id() == ROHR_Q1+300 
		|| get_id() == ROHR_Q1+400 
		|| get_id() == ROHR_Q1+600 

		|| get_id() == ROHR_Q3 
		|| get_id() == ROHR_Q3+100 
		|| get_id() == ROHR_Q3+200 
		|| get_id() == ROHR_Q3+300 
		|| get_id() == ROHR_Q3+400 
		|| get_id() == ROHR_Q3+600 

		|| get_id() == ROHR_Q5 
		|| get_id() == ROHR_Q5+100 
		|| get_id() == ROHR_Q5+200 
		|| get_id() == ROHR_Q5+300 
		|| get_id() == ROHR_Q5+400 
		|| get_id() == ROHR_Q5+600 

		|| get_id() == ROHR_Q6 
		|| get_id() == ROHR_Q6+100 
		|| get_id() == ROHR_Q6+200 
		|| get_id() == ROHR_Q6+300 
		|| get_id() == ROHR_Q6+400 
		|| get_id() == ROHR_Q6+600 

		|| get_id() == ROHR_Q7
		|| get_id() == ROHR_Q7+100
		|| get_id() == ROHR_Q7+200
		|| get_id() == ROHR_Q7+300
		|| get_id() == ROHR_Q7+400
		|| get_id() == ROHR_Q7+600

		|| get_id() == ROHR_Q8
		|| get_id() == ROHR_Q8+100
		|| get_id() == ROHR_Q8+200
		|| get_id() == ROHR_Q8+300
		|| get_id() == ROHR_Q8+400
		|| get_id() == ROHR_Q8+600

		|| get_id() == ROHR_Q9
		|| get_id() == ROHR_Q9+100
		|| get_id() == ROHR_Q9+200
		|| get_id() == ROHR_Q9+300
		|| get_id() == ROHR_Q9+400
		|| get_id() == ROHR_Q9+600

		|| get_id() == ROHR_Q10
		|| get_id() == ROHR_Q10+100
		|| get_id() == ROHR_Q10+200
		|| get_id() == ROHR_Q10+300
		|| get_id() == ROHR_Q10+400
		|| get_id() == ROHR_Q10+600

		|| get_id() == ROHR_Q11
		|| get_id() == ROHR_Q11+100
		|| get_id() == ROHR_Q11+200
		|| get_id() == ROHR_Q11+300
		|| get_id() == ROHR_Q11+400
		|| get_id() == ROHR_Q11+600

		|| get_id() == ROHR_Q12
		|| get_id() == ROHR_Q12+100
		|| get_id() == ROHR_Q12+200
		|| get_id() == ROHR_Q12+300
		|| get_id() == ROHR_Q12+400
		|| get_id() == ROHR_Q12+600

		|| get_id() == ROHR_Q13
		|| get_id() == ROHR_Q13+100
		|| get_id() == ROHR_Q13+200
		|| get_id() == ROHR_Q13+300
		|| get_id() == ROHR_Q13+400
		|| get_id() == ROHR_Q13+600

		|| get_id() == ROHR_Q20
		|| get_id() == ROHR_Q20+100
		|| get_id() == ROHR_Q20+200
		|| get_id() == ROHR_Q20+300
		|| get_id() == ROHR_Q20+400
		|| get_id() == ROHR_Q20+600

		|| get_id() == ROHR_Q14

		|| get_id() == ROHR_Q15)
		
		&&

		!Get_DoIgnoreRohrOrVk3Rohr();
}



bool Element::isElePulvern()
{
	if (get_id() == 700 || get_id() == 701 || get_id() == 702)
		return true;

	return false;
}

bool Element::isEleRohrQ1() const
{
	return get_id() == ROHR_Q1 
		|| get_id() == ROHR_Q1+100 
		|| get_id() == ROHR_Q1+200 
		|| get_id() == ROHR_Q1+300 
		|| get_id() == ROHR_Q1+400 
		|| get_id() == ROHR_Q1+600; 
}

bool Element::isEleRohrQ11() const
{
	return get_id() == ROHR_Q11 
		|| get_id() == ROHR_Q11+100 
		|| get_id() == ROHR_Q11+200 
		|| get_id() == ROHR_Q11+300 
		|| get_id() == ROHR_Q11+400 
		|| get_id() == ROHR_Q11+600; 
}


///Gewicht berechnen
double Element::getGewicht() const
{
	if (get_id() == ROHR || isEleRohr())
	{
		CString einheit = getStammdaten().getMaterialeinheit(get_artikelnr());

		if (einheit.MakeUpper() == "MM")
			return get_gewicht() * (double)get_laenge();
		else
			return get_gewicht() * (double)get_laenge()/1000;
	}
	return get_gewicht() * (double)get_anzahl();
}

//********************** Get/Set - Methoden *****************************

///get uelementList
EnCObListModell& Element::get_uelementList()
{
	return uelementList;
}

const EnCObListModell& Element::get_uelementList() const 
{
	return uelementList;
}

///get abstAbs
const int Element::get_abstAbs() const
{
	return abstAbs;
}

///get abstRel
const int Element::get_abstRel() const
{
	return abstRel;
}

///get dn
const int Element::get_dn() const
{
	return dn;
}

///get dnvl
const int Element::get_anschlussDN() const
{
	return anschlussDN;
}

///get laenge
int Element::get_laenge() const
{
	return laenge;
}

///get winkel
int Element::get_winkel(bool ohne_korrektur) const
{
	if (ohne_korrektur)
		return winkel;
	int winkelx = winkel + (360 - korrektur_winkel);
	return winkelx % 360;
}

			
///get pos
const int Element::get_pos() const
{
	return pos;
}

///get verkettung
const int Element::get_verkettung() const
{
	return verkettung;
}

///get verkettungAbst
const int Element::get_verkettungAbst() const
{
	return verkettungAbst;
}

///set abstAbs
void Element::set_abstAbs(int value)
{
	abstAbs = value;
}

///set abstRel
void Element::set_abstRel(int value)
{
	abstRel = value;
}



///set laenge
void Element::set_laenge(int value)
{
	laenge = value;
}

///set winkel
void Element::set_winkel(int value)
{
	winkel = value;
}

///set pos
void Element::set_pos(int value)
{
	pos = value;
}

///set verkettung
void Element::set_verkettung(int value)
{
	verkettung = value;
}

///set verkettungAbst
void Element::set_verkettungAbst(int value)
{
	verkettungAbst = value;
}

///get rohrMitStut
const bool Element::get_rohrMitStut() const
{
	return rohrMitStut;
}

///set rohrMitStut
void Element::set_rohrMitStut(bool value)
{
	rohrMitStut = value;
}

///get rohrNr
const int Element::get_rohrNr() const
{
	return rohrNr;
}


///get rohrLaenge
const int Element::get_rohrLaenge() const
{
	return rohrLaenge;
}

///set rohrLaenge
void Element::set_rohrLaenge(int value)
{
	rohrLaenge = value;
}

///get beschriftungLinks
const CString& Element::get_beschriftungLinks() const
{
	return beschriftungLinks;
}

///set beschriftungLinks
void Element::set_beschriftungLinks(const CString& value)
{
	beschriftungLinks = value;
}

///get beschriftungRechts
const CString& Element::get_beschriftungRechts() const
{
	return beschriftungRechts;
}

///set beschriftungRechts
void Element::set_beschriftungRechts(const CString& value)
{
	beschriftungRechts = value;
}

const bool Element::get_firstele() const
{
	return firstele;
}

void Element::set_firstele(bool value)
{
	firstele = value;
}

const bool Element::get_lastele() const
{
	return lastele;
}

void Element::set_lastele(bool value)
{
	lastele = value;
}

const bool Element::get_makeAnlage() const
{
	return makeAnlage;
}

void Element::set_makeAnlage(bool value)
{
	makeAnlage = value;
}

const CString& Element::get_rohrPosNr() const
{
	return rohrPosNr;
}

void Element::set_rohrPosNr(const CString& value)
{
	rohrPosNr = value;
}

//*************** Funktionen fuer Fitting ****************
///Liefert True, wenn Fitting Aussengewinde hat
bool Element::hasEleAussengewinde() const
{
	return gruppenid == FormstueckeUndFittinge::DOPPELNIPPEL || gruppenid == FormstueckeUndFittinge::STOPFEN;
}

///Liefert True, wenn Fitting Innengewinde hat
bool Element::hasEleInnengewinde() const
{
	return gruppenid == FormstueckeUndFittinge::F_MUFFE  
		|| gruppenid == FormstueckeUndFittinge::RED_MUFFE
		|| gruppenid == FormstueckeUndFittinge::T_STUECK 
		|| gruppenid == FormstueckeUndFittinge::KREUZ_STUECK
		|| gruppenid == FormstueckeUndFittinge::WINKEL
		|| gruppenid == FormstueckeUndFittinge::WINKEL_A1_45
		|| gruppenid == FormstueckeUndFittinge::WINKEL_30
		|| gruppenid == FormstueckeUndFittinge::KAPPE
		|| gruppenid == FormstueckeUndFittinge::VERSCHRAUBUNG && gfzeichen  == "340"
		|| gruppenid == FormstueckeUndFittinge::WINKELVERSCHRAUBUNG && gfzeichen == "96"; 
}

///Liefert True, wenn Fitting Innen- und Aussengewinde hat
bool Element::hasEleInnenAussengewinde() const
{
	return gruppenid == FormstueckeUndFittinge::RED_NIPPEL 
		|| gruppenid == FormstueckeUndFittinge::WINKELVERSCHRAUBUNG && gfzeichen == "98"
		|| gruppenid == FormstueckeUndFittinge::WINKEL_A4  
		|| gruppenid == FormstueckeUndFittinge::VERSCHRAUBUNG && gfzeichen == "341";
}

///Liefert den Kurztext
const CString& Element::get_kurztext() const
{
	if (kurztext.GetLength())
		return kurztext;
	
	if (get_id() == 0)
		kurztext = getStammdaten().getText("IDS_GLATT").c_str();
	else
	{
		int id = get_id();
		if (id == ROHR)
			id = ROHR_Q1;
		// Bezeichnung aus Stammdaten über die Elementid laden
		BauelementGruppen bg;
		kurztext = bg.getBezeichnung(id);
		
		if (isEleRohrQ1())
		{
			if (get_dn() < 65)
				kurztext = getStammdaten().getText("IDS_2440STD").c_str();
			else
				kurztext = getStammdaten().getText("IDS_2458STD").c_str();
		}

		if (isEleRohrQ11())
		{
			if (get_dn() < 65)
				kurztext = getStammdaten().getText("IDS_2440STD").c_str();
			else
				kurztext = getStammdaten().getText("APSAD").c_str();
		}

		if (get_id() != GRFITTING)
		{
			bool dn2zoll = (WaxSys::para_int(GF_DN2ZOLL) == 0) ? false : true;
			CString nennw;
			if (dn2zoll)
				nennw.Format(" %s\"", WaxStamm::dn2zoll(dn));
			else 
				nennw.Format(" DN %d", dn);
			kurztext += nennw;
		}
	}
	return kurztext;
}

///Liefert die Nennweite fuer den naechsten Anschluss
int Element::getReferenceDN(bool oben)
{
	if (oben)
	{
		// bei Kopfstueck DN3 
		if (get_gruppenid() == FormstueckeUndFittinge::T_STUECK && get_kopfstueck())
		{
			return get_dn3();
		}
		// bei T-Stueck DN2 
		else if (get_gruppenid() == FormstueckeUndFittinge::T_STUECK)
		{
			return get_dn2();
		}
		// bei Kreuz-Stueck DN2 
		else if (get_gruppenid() == FormstueckeUndFittinge::KREUZ_STUECK)
		{
			return get_dn2();
		}
		// sonst DN2 
		else 
		{
			return get_dn2();
		}
	}
	else
	{
		// bei Kopfstueck DN3 
		if (get_gruppenid() == FormstueckeUndFittinge::T_STUECK && get_kopfstueck())
		{
			return get_dn3();
		}
		// bei T-Stueck DN3 
		else if (get_gruppenid() == FormstueckeUndFittinge::T_STUECK)
		{
			return get_dn3();
		}
		// bei Kreuz-Stueck DN3 
		else if (get_gruppenid() == FormstueckeUndFittinge::KREUZ_STUECK)
		{
			return get_dn3();
		}
		// sonst DN2 checken
		else 
		{
			return (get_dn2())?get_dn2():get_dn1();
		}
	}
}

///Liefert das ZMass fuer den Abzug je nach Rohrseite
int Element::getZmass(enum RICHTUNG seite) const
{
	// In den Stammdaten sind die Z-Masse laut Fittingkatalog
	// Bei Muffe wird nur das halbe Z-Mass gerechnet.
	// Bei Kappe laut Sommer Z-Mass 0
	switch (get_gruppenid())
	{
	case FormstueckeUndFittinge::F_MUFFE:
	case FormstueckeUndFittinge::RED_MUFFE:
		return get_zmass()/2;
	case FormstueckeUndFittinge::T_STUECK:
		return (seite == LINKS) ? get_zmass3() : get_zmass();
	case FormstueckeUndFittinge::KAPPE:
		return 0;
	default:
		return get_zmass();
	}
}

///Wenn in check_elementArray() ermittelt wurde, dass Nennweite getauscht werden muss,
///erfolgt hier die Umsetzung der Nennweite und vom ZMass
void Element::changeDNundZMass()
{
	// DN1 mit DN3 tauschen
	if (get_gruppenid() == FormstueckeUndFittinge::T_STUECK)
	{
		int dn = get_dn1();
		set_dn1(get_dn3());
		set_dn3(dn);
		
		int zm = get_zmass();
		set_zmass(get_zmass3());
		set_zmass3(zm);
	}
	// DN1 mit DN2 & DN3 mit DN4 tauschen 
	else if (get_gruppenid() == FormstueckeUndFittinge::KREUZ_STUECK)
	{
		int dn = get_dn1();
		set_dn1(get_dn2());
		set_dn2(dn);
		dn = get_dn3();
		set_dn3(get_dn4());
		set_dn4(dn);

		int zm = get_zmass();
		set_zmass(get_zmass2());
		set_zmass2(zm);
	}
	// DN1 mit DN2 tauschen
	else 
	{
		int dn = get_dn1();
		set_dn1(get_dn2());
		set_dn2(dn);
	
		int zm = get_zmass();
		set_zmass(get_zmass2());
		set_zmass2(zm);
	}
}

///Setzt die (Anschluss-)Nennweite
void Element::setAnschlussNennweite()
{
	if (get_gruppenid() == FormstueckeUndFittinge::T_STUECK && get_kopfstueck())
	{
		set_anschlussDN(get_dn2());
		set_dn(get_dn2());
	}
	else if (get_gruppenid() == FormstueckeUndFittinge::T_STUECK && !get_kopfstueck())
	{
		set_anschlussDN(get_dn1());
		set_dn(get_dn3());
	}
	else if (get_gruppenid() == FormstueckeUndFittinge::KREUZ_STUECK)
	{
		set_anschlussDN(get_dn1());
		set_dn(get_dn3());
	}
	else if (get_dn2() != 0)
	{
		set_anschlussDN(get_dn1());
		set_dn(get_dn2());
	}
	else
	{
		set_anschlussDN(get_dn1());
		set_dn(get_dn1());
	}
}

///Liefert den Abstand zum vorherigen Fitting, wenn Fitting rechts ohne Rohrstueck
double Element::getAbstand()
{
	switch (gruppenid)
	{
		case FormstueckeUndFittinge::T_STUECK:
		case FormstueckeUndFittinge::KREUZ_STUECK:
		case FormstueckeUndFittinge::KAPPE:
		case FormstueckeUndFittinge::WINKEL:
		case FormstueckeUndFittinge::WINKEL_A1_45:
		case FormstueckeUndFittinge::WINKEL_30:
		case FormstueckeUndFittinge::WINKEL_A4:
		case FormstueckeUndFittinge::WINKELVERSCHRAUBUNG:
			return laenge+zmass;
		case FormstueckeUndFittinge::STOPFEN:
		case FormstueckeUndFittinge::RED_NIPPEL:
		case FormstueckeUndFittinge::F_MUFFE:
		case FormstueckeUndFittinge::RED_MUFFE:
		case FormstueckeUndFittinge::VERSCHRAUBUNG:
			return zmass;
		case FormstueckeUndFittinge::DOPPELNIPPEL:
			return 2*laenge;
		default:
			XASSERT(false);
	}
	return 0;
}

//********************** Get/Set - Methoden *****************************

const bool Element::get_changeDN() const
{
	return changeDN;
}

void Element::set_changeDN(bool value)
{
	if (value == true)
	{
		changeDNundZMass();
		///Setzt die (Anschluss-)Nennweite
		setAnschlussNennweite();
	}
	if (value == true && changeDN == true)
		changeDN = false;
	else
		changeDN = value;
}

const int Element::get_dn1() const
{
	return dn1;
}

void Element::set_dn1(int value)
{
	dn1 = value;
}

const int Element::get_dn2() const
{
	return dn2;
}

void Element::set_dn2(int value)
{
	dn2 = value;
}

const int Element::get_dn3() const
{
	return dn3;
}

void Element::set_dn3(int value)
{
	dn3 = value;
}

const int Element::get_dn4() const
{
	return dn4;
}

void Element::set_dn4(int value)
{
	dn4 = value;
}

const CString &  Element::get_gfzeichen() const
{
	return gfzeichen;
}

void Element::set_gfzeichen(const CString & value)
{
	gfzeichen = value;
}

const int Element::get_gruppenid() const
{
	return gruppenid;
}

void Element::set_gruppenid(int value)
{
	gruppenid = value;
}

const int Element::get_zmass() const
{
	return zmass;
}

void Element::set_zmass(int value)
{
	zmass = value;
}

bool Element::get_nurFitting() const
{
	return nurFitting;
}

void Element::set_nurFitting(bool value)
{
	nurFitting = value;
}

const int Element::get_kopfstueck() const
{
	return kopfstueck;
}

///Nur fuer Element GRFitting und Gruppe T-Stueck erforderlich, Anschluss an DN2
void Element::set_kopfstueck(int value)
{
	kopfstueck = value;
}

const int Element::get_zmass2() const
{
	return zmass2;
}

const int Element::get_zmass3() const
{
	return zmass3;
}

void Element::set_zmass2(int value)
{
	zmass2 = value;
}

void Element::set_zmass3(int value)
{
	zmass3 = value;
}

///Laenge/Durchmesser des Elements aus den Stammdaten holen
int Element::get_Stammdaten(int &laenge, double &durchmesser, int id, int dn, int anschlussDN)
{
	const CObList& grp = getStammdaten().get_bauelemente();
	for (POSITION gpos = grp.GetHeadPosition(); gpos;)
	{
		Bauelemente::data *g = (Bauelemente::data *)grp.GetNext(gpos);
		if ( (g->get_elemid() == id) 
		  && (g->get_dn() == dn) 
		  && (g->get_dnvl() == anschlussDN || g->get_dnvl() == 0) ) 
		{
			laenge = g->get_mass1();
			durchmesser = g->get_mass2();
			return (1);
		}
	}
	return (0);
}

///Prueft Rohr auf Elemente,die ueber Anlage laufen
bool Element::hasRohrStdelemente()
{
	for (POSITION upos = uelementList.GetHeadPosition(); upos;)	
	{
		Element *uele = dynamic_cast<Element*>(uelementList.GetNext(upos));
		XASSERT(uele);
		if (!uele)
			break;

		if (uele->get_makeAnlage())
			return true;
	}
	return false;
}

void Element::init_ele()
{
	abstAbs			= 0;
	abstRel			= 0;
	dn				= 0;
	anschlussDN		= 0;
	laenge			= 0;
	winkel			= 0; 
	pos				= 0;
	verkettung		= 0;
	verkettungAbst	= 0;
	rohrMitStut     = false;
	rohrLaenge      = 0;
	firstele		= false;
	lastele		    = false;
	makeAnlage      = false;
	ElementAnzahlid = 0;
	ElementAnzahldnMuffeOrDBohrung = 0;
	m_IgnoreRohrOrVk3Rohr = false;
	
	// Fitting
	set_dn1(0);
	set_dn2(0);
	set_dn3(0);
	set_dn4(0);

	gruppenid       = FormstueckeUndFittinge::UNKNOWN;
	zmass			= 0;
	zmass2			= 0;
	zmass3			= 0;
	nurFitting		= 0;
	kopfstueck      = 0;
	changeDN		= false;
	set_rohrNr(0);
	korrektur_winkel = 0;
	laengeZuschnitt = 0;
}


void Element::set_rohrNr(int value)
{
	rohrNr = value;
}


/// set dn
void Element::set_dn(int value)
{
	dn = value;
	//TRACE("Element %s -> DN=%i\n", this->getBezeichnung(), this->get_dn());
}

/// set dnvl
void Element::set_anschlussDN(int value)
{
	anschlussDN = value;
}




#ifdef _DEBUG
/*
Ausgabe der Elementwerte. Kann zur Prüfung der Elemente etc. verwendet werden.
*/
void Element::Dump(CDumpContext &dc)const
{
	Material::Dump(dc);

	//Werte von Element ausgeben
	
	if (get_gruppenid() > 0)
	{
		dc << "\n GruppenId: " << get_gruppenid();
	}
	dc << "\n DN: " << get_dn() << " Anschluss DN: " << get_anschlussDN();
	dc << "\n AbstRel: " << get_abstRel() << " AbstAbs: " << get_abstAbs() <<  " Mass1: " << get_mass1();
	dc << "\n Laenge: " << get_laenge() << " Rohrlänge: " << get_rohrLaenge() << " Winkel: " << get_winkel();
	dc << "\n Anlage: " << get_makeAnlage() << " UrsprungsID: " << get_urid() << " Anzahl: " << get_anzahl();

	if (get_uelementList().GetSize() > 0)
	{
		dc << "\n Unterelemente:";

		int i=0;
		for (POSITION upos = get_uelementList().GetTailPosition(); upos != NULL;)	
		{
			const Element* uele = dynamic_cast<const Element*>(get_uelementList().GetPrev(upos));
			XASSERT(uele);
			if (uele)
			{
				dc << "\n" << "  " << i << ". Unterelement:";
				dc << uele;
			}
			i++;
		}
	}
}
#endif

int Element::getLaengeZuschnitt() const
{
	if (laengeZuschnitt == 0)
		return get_laenge();
	else
		return laengeZuschnitt;
}


void Element::set_laengeZuschnitt(int val)
{
	laengeZuschnitt = val;
}