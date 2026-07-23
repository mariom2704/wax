#include "stdafx.h"


#include "Arbeitsfolge.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Arbeitsfolge nach ID erstellen bzw. Standardkonstruktor
Arbeitsfolge::Arbeitsfolge(int id) : nr(0), arbpl(""), kostst(0), zeit(0.0), ppstd(0.0), anz(1.0), ruestzeit(0.0), preis(0.0), sap(true), dn(0), typ(0), zuschlag(0)
{
	isTextgeaendert = false;
	init(id);
}

//Copy´Konstruktor
Arbeitsfolge::Arbeitsfolge(const Arbeitsfolge& orig)
{
	copy(orig);
}

//Destruktor
Arbeitsfolge::~Arbeitsfolge()
{

}

//Copyfunktion
void Arbeitsfolge::copy(const Arbeitsfolge& other)
{
	/*
	Kopieren der Attribute
	Nicht vergessen: Bei Änderungen an den Attributen diese Methode up to date halten!!!
	*/
	anz			= other.anz;
	id			= other.id;
	kostst		= other.kostst;
	arbpl		= other.arbpl;
	nr			= other.nr;
	ppstd		= other.ppstd;
	preis		= other.preis;
	txt			= other.txt; 
	originalTxt	= other.originalTxt; 
	zeit		= other.zeit;
	ruestzeit	= other.ruestzeit;
	sap         = other.sap;
	dn          = other.dn;
	zuschlag    = other.zuschlag;
	isTextgeaendert = other.isTextgeaendert;
	typ         = other.typ;

	xdat = other.xdat;
}

//Addiert zwei Arbeitsfolgen
bool Arbeitsfolge::merge(const Arbeitsfolge& other)
{
	XASSERT(id == other.get_id());
	if (id != other.get_id() || kostst != other.get_kostst() || ppstd != other.get_ppstd() || sap != other.get_sap())
		return false;
	anz += other.get_anz();
	zeit += other.get_zeit();

	//Rüstzeit nur einmal zählen
	if (ruestzeit <= 0)
		ruestzeit += other.get_ruestzeit();

	if (!xdat.isEmpty()
		|| !other.xdat.isEmpty())
	{
		if (!xdat.isEmpty())
		{
			if (xdat.dn != other.xdat.dn || xdat.dnvl != other.xdat.dnvl ||	xdat.eleid != other.xdat.eleid)
				return false;
		}
		xdat.anzahl += other.xdat.anzahl;
		xdat.laenge += other.xdat.laenge;
		xdat.zeit += other.xdat.zeit;
		xdat.dn = other.xdat.dn;
		xdat.dnvl = other.xdat.dnvl;
		xdat.eleid = other.xdat.eleid;
	}

	if (get_typ() == 0)
		calc_preis();
	else
		preis += other.get_preis();

	return true;
}

//Speichern und Laden der Daten
//Achtung: Wird von Rose überschrieben
void Arbeitsfolge::serialize(BinaryArchive& ar)
{
	int version = ar.get_version() > 0 ? ar.get_version() : 1000;
	
	int idummy = 0;
	if (ar.is_storing())
	{
		ar << anz << id << kostst;
		ar << arbpl;
		ar << nr << ppstd << txt << zeit;
		ar << preis << ruestzeit;
		ar << isTextgeaendert;
		ar << typ;
	}
	else
	{
		ar >> anz >> id >> kostst;
		
		if (version >= 42)
			ar >> arbpl;
		else
		{
			int ap;
			ar >> ap;
			arbpl.Format("%i",ap);
		}
		
		ar >> nr >> ppstd >> txt >> zeit;
		ar >> preis;
		
		if (version < 48)
			ar >> idummy >> idummy >> idummy;
		
		if (version > 3)
			ar >> ruestzeit;

		if (version >= 52)
			ar >> isTextgeaendert;
		else
			isTextgeaendert = false;

		if (version >= 54)
			ar >> typ;
	}
	
	if (version < 46)
	{
		bool tmp_akkord = 0;
		serialize_bool(tmp_akkord, ar);
	}

	serialize_bool_if(version > 23, sap, ar);

	
	// Daten der Apaxfelder
	if (ar.is_storing())
	{
		const int count = 1; // Sollten mehrere xdat Objekte serialisiert werden müssen
		// kann hier erweitert werden.
		ar << count;
		xdat.serialize(ar);
	}
	else
	{
		int count;
		ar >> count;
		if (count)
			xdat.serialize(ar);
	}	
}

//get preis
const double Arbeitsfolge::get_preis() const
{
	return preis;
}

//Preisberechnung
void Arbeitsfolge::calc_preis()
{
	if (get_typ() == 0)
		preis = (ppstd * zeit) + (ppstd * ruestzeit);
	//TRACE("%s(%i): AF%i: ppstd=%f, zeit=%f, ruestzeit=%f\n", __FILE__, __LINE__, get_id(), ppstd, zeit, ruestzeit);
}

//set zeit
void Arbeitsfolge::set_zeit(double value)
{
	zeit = value;
	calc_preis();
}

//get zeit
const double Arbeitsfolge::get_zeit() const
{
	return zeit;
}

//get id
const int Arbeitsfolge::get_id() const
{
	return id;
}

//get arbpl
const CString Arbeitsfolge::get_arbpl() const
{
	return arbpl;
}

//get nr
const int Arbeitsfolge::get_nr() const
{
	return nr;
}

//get txt
const CString& Arbeitsfolge::get_txt() const
{
	return txt;
}

const CString& Arbeitsfolge::get_originalTxt() const
{
	return originalTxt;
}

//set id
void Arbeitsfolge::set_id( int  value)
{
	init(value);
}

//set arbpl
void Arbeitsfolge::set_arbpl(CString value)
{
	arbpl = value;
}

//set nr
void Arbeitsfolge::set_nr(int value)
{
	nr = value;
}

//set txt
void Arbeitsfolge::set_txt(const CString& value)
{
	txt = value;
}

void Arbeitsfolge::set_originalTxt(const CString& value)
{
	originalTxt = value;
}

//get anz
const double Arbeitsfolge::get_anz() const
{
	return anz;
}

//set anz
void Arbeitsfolge::set_anz(double value)
{
	anz = value;
}

//get kostst
const int Arbeitsfolge::get_kostst() const
{
	return kostst;
}

//set kostst
void Arbeitsfolge::set_kostst(int value)
{
#ifdef _DEBUG
	// ungueltig oder Fremdfertigung mit Kostst 0
	if (!value)
		TRACE("Ungültige Arbeitsfolge %i\n", this->get_id());
#endif
	kostst = value;
}

//get ppstd
const double Arbeitsfolge::get_ppstd() const
{
	return ppstd;
}

//set ppstd
void Arbeitsfolge::set_ppstd(double value)
{
	ppstd = value;
}

const double Arbeitsfolge::get_zuschlag() const
{
	return 0;
}

	
void Arbeitsfolge::set_zuschlag(double val)
{
	zuschlag = val;
}



//set preis
void Arbeitsfolge::set_preis(double value)
{
	preis = value;
}



// Ermittelt anhand der ID die Werte für die 
//Kostenstelle/Arbeitsplatz/Stundensatz
void Arbeitsfolge::init(int id)
{
	this->id = id;
	if (!id)
		return;
	for (POSITION apos = getStammdaten().get_arbeitsfolgen().GetHeadPosition(); apos;)
	{
		Arbeitsfolgen::data *f = (Arbeitsfolgen::data *)getStammdaten().get_arbeitsfolgen().GetNext(apos);

		if (f->get_code() == get_id())
		{
			set_txt(f->get_txt());
			set_originalTxt(f->get_txt());
			set_arbpl(f->get_arbeitsplatz());
			set_nr(f->get_posnr());
			break;
		}
	}
	set_kostst(getKostst(get_arbpl()));
	set_ppstd(getTarif(get_kostst()));
	// Bei Afos Ruesten und Klinken keine extra Ruestzeit berechnen
	if (get_id() != 8 && get_id() != 9)
	{
		Arbeitsplaetze arb;
		set_ruestzeit(arb.getRuestzeit(get_arbpl()));
		set_typ(arb.getTyp(get_arbpl()));
	}
}

//die Bezeichnung des Arbeitsplatzes über die Stammdaten holen
CString Arbeitsfolge::getArbplBez(CString arbpl)
{
	Arbeitsplaetze arb;
	return arb.getBezeichnung(arbpl);
}

//die Kostenstelle über die Stammdaten holen
long Arbeitsfolge::getKostst(CString arbpl)
{
	Arbeitsplaetze arb;
	//Auswaertig fertigen ist Kostenstelle 0
	//XASSERT(arb.getKostenstelle(arbpl));
	return arb.getKostenstelle(arbpl);
}

//den Tarif über die Stammdaten holen
double Arbeitsfolge::getTarif(long kostst)
{
	Kostenstellen k;
	return k.getTarif(kostst);
}

Arbeitsfolge::apaxdat::apaxdat() : anzahl(0), dn(0), laenge(0), zeit(0), dnvl(0)
{
	eleid = 0;
}

void Arbeitsfolge::apaxdat::serialize(BinaryArchive& ar)
{
	/*
	Damit hier ohne Probleme erweitert werden kann, wird ein Dummy Int Wert mit serialisiert. 
	*/
	if (ar.is_storing())
		ar << anzahl << dn << laenge << zeit << eleid << dnvl << dnvl;
	else
		ar >> anzahl >> dn >> laenge >> zeit >> eleid >> dnvl >> dnvl;
}

//Liefert true zurück, wenn wenigstens ein Wert gesetzt wurde
bool Arbeitsfolge::apaxdat::isEmpty() const
{
	return !eleid && !dnvl && !anzahl && !dn && !laenge && zeit < 0.001;
}

Arbeitsfolge::apaxdat& Arbeitsfolge::get_xdat() 
{
	return xdat;
}

const double Arbeitsfolge::get_ruestzeit() const
{
	return ruestzeit;
}

void Arbeitsfolge::set_ruestzeit(double value)
{
	ruestzeit = value;
	calc_preis();
}

const bool Arbeitsfolge::get_sap() const
{
	return sap;
}

void Arbeitsfolge::set_sap(bool value)
{
	sap = value;
}

void Arbeitsfolge::appendText(const CString& id)
{
	CString str;
	str = getStammdaten().getText(id.GetString()).c_str();
	set_txt(get_txt() + str);
}




//DN Wert auf dem die Afo basiert
void Arbeitsfolge::set_dn(int value)
{
	dn = value;
}

	//DN Wert auf dem die Afo Basiert
int Arbeitsfolge::get_dn()
{
	return dn;
}



#ifdef _DEBUG
//Ausgabe der Elemente in der Liste
void Arbeitsfolge::Dump(CDumpContext &dc) const
{
	//Werte von elementList ausgeben
	dc << "\nAfo Id: " << get_id() << "  Bez: " << get_txt();
	dc << "\nSap: " << get_sap() << "  DN: " << dn << " Zeit: " << zeit;
	dc << "\nAnzahl: " << get_anz() << "\nArbeitsplatz  " << arbpl;
}

#endif


const bool Arbeitsfolge::get_isTextGeandert() const
{
	return isTextgeaendert;
}

	
void Arbeitsfolge::set_isTextGeandert(bool value)
{
	isTextgeaendert = value;
}

const int  Arbeitsfolge::get_typ() const
{
	return typ;
}

void  Arbeitsfolge::set_typ(int value)
{
	typ = value;
}
