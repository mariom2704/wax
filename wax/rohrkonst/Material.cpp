#include "stdafx.h"

#include "Werkauftrag.h"
#include "Element.h"

		
		#include "Material.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif 


//Destruktor
Material::~Material()
{

}

//Konstruktor
Material::Material() : id(0), urid(0), posnr("1"), anzahl(1), festPreis(0.0), sapPreis(0.0), gewicht(0), artikelnr(0), 
					   mass1(0), mass2(0), oberfl(0.0), durchmesser(0.0),
					   halbzeug(false), einkauf(false), 
					   constpreis(false), pulverpreis(0.0)
{
}


///Gibt eine neues mit new erzeugtes Objekt zurück
WaxBase *Material::deepCopy()const		
{
	Material *pNew = new Material;

	pNew->copy(*this);

	return pNew;
}



//Kopiert die Attribute eines anderen Materialobjektes. 
//Wird in Element gebraucht.
void Material::copy(const Material& other)
{
	posnr		= other.posnr;	
	anzahl		= other.anzahl;
	artikelnr	= other.artikelnr;
	bezeichnung	= other.bezeichnung;
	gewicht		= other.gewicht;
	idcounterVk3Preis	= other.idcounterVk3Preis;
	id			= other.id;
	mass1		= other.mass1;
	mass2		= other.mass2;
	festPreis	= other.festPreis;
	sapPreis	= other.sapPreis;
	halbzeug	= other.halbzeug;
	einkauf		= other.einkauf;
	kurztext	= other.kurztext;
	constpreis  = other.constpreis;
	durchmesser = other.durchmesser;
	urid		= other.urid;
	pulverpreis = other.pulverpreis;
}

//Speichern und Laden von Material
void Material::serialize(BinaryArchive& ar)
{
	int version = ar.get_version() > 0 ? ar.get_version() : 1000;
	
	serialize_bool(halbzeug, ar);
	serialize_bool(einkauf, ar);
	serialize_bool_if(version > 23, constpreis, ar);

	int idummy = 0;
	if (ar.is_storing())
	{
		
		ar << anzahl << artikelnr << bezeichnung << gewicht << id << mass1 << mass2; 
		ar << festPreis << sapPreis << kurztext << durchmesser << posnr << urid; 
		ar << pulverpreis;
	}
	else
	{
		double tmpmass1, tmpmass2;
		ar >> anzahl >> artikelnr >> bezeichnung >> gewicht >> id;
		if (version < 10)
		{ // Vor Version 10 sind mass1 und mass2 double Werte
			ar >> tmpmass1 >> tmpmass2;
			mass1 = (int)tmpmass1;
			mass2 = (int)tmpmass2;
		}
		else
			ar >> mass1 >> mass2;

		ar >> festPreis;

		if (version < 48)
			sapPreis = 0;
		else
			ar >> sapPreis;

		if (version < 48)
			ar >> idummy;
		
		ar >> kurztext;

		// Kurztext neu setzen
		if (id == FORMSTUECKE || id == FITTINGE || id == GRFITTING)
			setKurztext();

		if (version > 25)
			ar >> durchmesser;
		if (version > 27)
		{
			ar >> posnr;
			ar >> urid;
		}
		if (version > 34)
			ar >> pulverpreis;
	}
}

///Initialisierung der Stammdaten
void Material::initStammdaten()
{
	if (get_artikelnr() == 0)
		return;

	// Initialisierung des Materials mit den Stammdaten aus der Tabelle Profile
	const CObList& pro = getStammdaten().get_profile();
	POSITION pos;
	for (pos = pro.GetHeadPosition(); pos;)
	{
		Profile::data *p = (Profile::data *)pro.GetNext(pos);
		if (p->get_artikelnr() == get_artikelnr()) 
		{
			init(*p);
			return;
		}
	}
	// Initialisierung des Materials mit den Stammdaten aus der Tabelle FormstueckeUndFittinge
	const CObList& fuf = getStammdaten().get_formstueckefittinge();
	for ( pos = fuf.GetHeadPosition(); pos;)
	{
		FormstueckeUndFittinge::data *f = (FormstueckeUndFittinge::data *)fuf.GetNext(pos);
		if (f->get_artikelnr() == get_artikelnr()) 
		{
			init(*f);
			return;
		}
	}
	// Initialisierung des Materials mit den Stammdaten aus der Tabelle Bauelemente
	const CObList& grp = getStammdaten().get_bauelemente();
	for (pos = grp.GetHeadPosition(); pos;)
	{
		Bauelemente::data *g = (Bauelemente::data *)grp.GetNext(pos);
		if (g->get_artikelnr() == get_artikelnr())
		{
			init(*g);
			return;
		}
	}
	// Initialisierung des Materials mit den Stammdaten aus der Tabelle Sondermaterial
	const CObList& son = getStammdaten().get_sondermaterial();
	for (pos = son.GetHeadPosition(); pos;)
	{
		Sondermaterial::data *s = (Sondermaterial::data *)son.GetNext(pos);
		if (s->get_artikelnr() == get_artikelnr())
		{
			init(*s);
			return;
		}
	}
}

///Initialisierung des Materials mit den Stammdaten aus der Tabelle Profile
void Material::init(const Profile::data &p)
{
	set_artikelnr(p.get_artikelnr());
	set_idCounterVk3Preis(p.get_artikelnr()); // Erst einmal die Vk3 counter ID aus dem Artikelstamm setzen 
	set_halbzeug(p.get_halbzeug());
	set_einkauf(p.get_einkauf());
	set_festPreis(p.get_festPreis());
	set_sapPreis(p.get_sapPreis());
	set_gewicht(p.get_gewicht());
}

///Initialisierung des Materials mit den Stammdaten aus der Tabelle FormstueckeUndFittinge
void Material::init(const FormstueckeUndFittinge::data &f)
{
	set_artikelnr(f.get_artikelnr());
	set_idCounterVk3Preis(f.get_artikelnr()); // Erst einmal die Vk3 counter ID aus dem Artikelstamm setzen 
	set_halbzeug(f.get_halbzeug());
	set_einkauf(f.get_einkauf());
	set_festPreis(f.get_festPreis());
	set_sapPreis(f.get_sapPreis());
	set_pulverpreis(f.get_pulverpreis());
	set_gewicht(f.get_gewicht());
}

///Initialisierung des Materials mit den Stammdaten aus der Tabelle Bauelemente
void Material::init(const Bauelemente::data &g)
{
	set_artikelnr(g.get_artikelnr());
	set_idCounterVk3Preis(g.get_artikelnr()); // Erst einmal die Vk3 counter ID aus dem Artikelstamm setzen 
	set_halbzeug(g.get_halbzeug());
	set_einkauf(g.get_einkauf());
	set_festPreis(g.get_festPreis());
	set_sapPreis(g.get_sapPreis());
	set_gewicht(g.get_gewicht());
}

///Initialisierung des Materials mit den Stammdaten aus der Tabelle Sondermaterial
void Material::init(const Sondermaterial::data &s)
{
	set_artikelnr(s.get_artikelnr());
	set_idCounterVk3Preis(s.get_artikelnr()); // Erst einmal die Vk3 counter ID aus dem Artikelstamm setzen 
	set_halbzeug(s.get_halbzeug());
	set_einkauf(s.get_einkauf());
	set_festPreis(s.get_preis());
	set_sapPreis(s.get_preis());
	set_gewicht(s.get_gewicht());
}

void Material::setKurztext()
{
	FormstueckeUndFittinge::data ff;
	for (POSITION pos = getStammdaten().get_formstueckefittinge().GetHeadPosition(); pos;)
	{
		FormstueckeUndFittinge::data *f = (FormstueckeUndFittinge::data *)getStammdaten().get_formstueckefittinge().GetNext(pos);

		if (f->get_elemid() == get_id() && f->get_artikelnr() == get_artikelnr())
		{
			kurztext = f->get_kurztext();
			break;
		}
	}
}

void Material::set_artikelnr(long value)
{
	const long exattrib = artikelnr & FSTAHL | artikelnr & USTAHL | artikelnr & LSTAHL;
	if (value == NULL) 
	{
		const CObList& grp = getStammdaten().get_bauelemente();
		for (POSITION gpos = grp.GetHeadPosition(); gpos;)
		{
			Bauelemente::data *g = (Bauelemente::data *)grp.GetNext(gpos);
			if (g->get_elemid() == get_id())
			{
				artikelnr = g->get_artikelnr();
			}
		}
	}
	else 
	{
		artikelnr = value;
	}
	artikelnr |= exattrib;
}

//Liest bezeichnung aus, wenn bezeichnung leer ist, wird es anhand der id aus 
//den Stammdaten geladen.
const CString& Material::get_bezeichnung()
{
	if (!bezeichnung.GetLength())
	{
		// Bezeichnung aus Stammdaten über die Elementid laden
		BauelementGruppen bg;
		CString st = bg.getBezeichnung(get_id());
		set_bezeichnung(st);
	}
	return bezeichnung;
}

/// Konst Methode, die Bezeichnung nicht initialisiert
/// todo: Benamsung tauschen mit get_bezeichnung.
/// get_ Methoden sollten immer const sein.
const CString& Material::getBezeichnung() const
{
	return bezeichnung;
}
	
const CString& Material::get_kurztext() 
{
	// Bezeichnung muss zuerst gesetzt werden
	if (!kurztext.GetLength())
		kurztext = get_bezeichnung();
	return kurztext;
}

void Material::set_kurztext(const CString& value)
{
	kurztext = value;
}

double Material::getGewicht(bool anz) const
{
	double gewicht = 0;

	// Bei Rohr ist Laenge in Mass1 gespeichert
	if (get_mass1() > 0)
	{
		CString einheit = getStammdaten().getMaterialeinheit(get_artikelnr());

		if (einheit.MakeUpper() == "MM")
			gewicht = get_gewicht() * (double)get_mass1();
		else 
			gewicht = get_gewicht() * (double)get_mass1() / 1000;

		if (anz)
			gewicht *= get_anzahl(); 
	}
	else 
		gewicht = get_gewicht() * get_anzahl();

	return gewicht;
}

void Material::set_profilkennung(const CString& db_kennung)
{
	/**
		Mapping der Kennung in der DB zu den binären Flags
	*/
	if (db_kennung == "F")
		set_profilkennung(FSTAHL);
	else if (db_kennung == "L")
		set_profilkennung(LSTAHL);
	else if (db_kennung == "U")
		set_profilkennung(USTAHL);
	else
		XASSERT(db_kennung == "U" || db_kennung == "F" || db_kennung == "L");
}
	
void Material::set_profilkennung(enum EX_ATTRIB exattrib)
{
	/**
		Setzen der erweiterten Attribute für Profile, Flags werden in Artikelnummer gespeichert
		Erlaubt sind USTAHL, FSTAHL, LSTAHL oder Null zum Löschen
	*/
	const long profilflags = USTAHL | FSTAHL | LSTAHL;
	
	XASSERT(profilflags & exattrib || !exattrib);
	if (!(profilflags & exattrib) && exattrib)
		return;
	artikelnr &= ~profilflags;
	artikelnr |= exattrib;
}
	
int Material::get_profilkennung() const
{
	/**
		Auslesen der Profilkennung in den erweiterten Attributen. Null, wenn es kein Profil ist
	*/
	const long profilflags = USTAHL | FSTAHL | LSTAHL;
	return artikelnr & profilflags;
}

///Um alle Winkel abspeichern zu können, braucht man leider mehr als 8 Bit, 
///deshalb hier 9 Bit
///Winkel rechts ist in in den unteren 9 Bit gespeichert
///Winkel links ist in den oberen 9 Bit gespeichert
///Dazwischen ist die Ansicht (Bit 10 bis 14) und die Seitenansicht (Bit 15 
//u/nd 16) abgespeichert
int Material::setMass2(int winkelLinks, int winkelRechts, int ansicht, int seitenansicht) 
{
	winkelLinks <<= (32-9);
	ansicht <<= 10;
	seitenansicht <<= 10+5;	
	return winkelLinks | ansicht | seitenansicht | winkelRechts;
}

///Entschlüsselt mass2 und liefert die Winkel und Ansichten
void Material::getMass2(int mass2, int& winkelLinks, int& winkelRechts, int& ansicht, int &seitenansicht) 
{
	winkelLinks = mass2 >> (32-9);
	winkelRechts = mass2 & 0x1ff;
	ansicht = (mass2 >> 10) & 0x0f;
	seitenansicht = (mass2 >> 15) & 0x03;
}

///Liefert den Profiltyp
CString Material::getProfiltyp()
{
	if (get_profilkennung() & USTAHL)
		return "U";
	if (get_profilkennung() & FSTAHL)
		return "F";
	if (get_profilkennung() & LSTAHL)
		return "L";
	
	return "";
}

///Liefert das Profilende
int Material::getProfilende()
{
	// Aus mass2 werden die relevanten Profildaten ermittelt
	int winkelLinks, winkelRechts, ansicht, seitenansicht;
	getMass2(get_mass2(), winkelLinks, winkelRechts, ansicht, seitenansicht);
	
	//1: beide Seoten gerade
	if ((winkelLinks == 0 || winkelLinks == 90) && (winkelRechts == 0 || winkelRechts == 90))
		return 1;
	
	//2: eine Seite schraege
	if ((winkelLinks != 0 && winkelLinks != 90) && (winkelRechts == 0 || winkelRechts == 90))
		return 2;

	if ((winkelLinks == 0 || winkelLinks == 90) && (winkelRechts != 0 && winkelRechts != 90))
		return 2;

	//3: beide Seiten schraege
	if ((winkelLinks != 0 && winkelLinks != 90) && (winkelRechts != 0 && winkelRechts != 90))
		return 3;

	return 0;
}

///Hier wird ermittelt welche Afo ein Profil hat. 
void Material::getAfos(CUIntArray &afoNr)
{
	if (get_id() != PROFIL)
		return;
	
	ProfileArbeitsfolgen::data *pr = NULL;
	
	//Unterschiedliche Afo fuer Saegen je nach Profiltyp
	for (POSITION pos = getStammdaten().get_profilearbeitsfolgen().GetHeadPosition(); pos;)
	{
		pr = (ProfileArbeitsfolgen::data *)getStammdaten().get_profilearbeitsfolgen().GetNext(pos);

		if (pr->get_typ() == getProfiltyp()) 
		{
			afoNr.Add(pr->get_code()); 
			break;
		}
	}
	afoNr.Add(71);
	afoNr.Add(72);
}


#ifdef _DEBUG
/*
Ausgabe der Elementwerte. Kann zur Prüfung der Elemente etc. verwendet werden.
*/
void Material::Dump(CDumpContext &dc)const
{
	//Werte von Material ausgeben
	dc << "\n Typ: " << get_id() << "  Bez: " << getBezeichnung() << "\n Kurztext: " << kurztext ;
	dc << "\n ArtNr: " << get_artikelnr() << " Durchmesser:  " << get_durchmesser();

}
#endif