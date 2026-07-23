#include "stdafx.h"


#include "resource.h"
#include "Beschichtung.h"
#include "Werkauftrag.h"
#include "Material.h"
#include "Preis.h"
#include "Element.h"
#include "Rohrleitung.h"
#include "Gewinderohr.h"
#include "Strangrohr.h"

#include "..\rohrkonst\MxString.h"

#include "typeinfo.h"

#include <iomanip>
#include <sstream>




#ifdef _DEBUG
#define new DEBUG_NEW
#endif 

//Standardkonstruktor
Werkauftrag::Werkauftrag() : waid(0), m_iPreiswertigkeit(0), aenderungsdatum(MXDate::get_current_date()), erstellungsdatum(MXDate::get_current_date()), einkauf(true), plausiOk(false), preis(true), m_bUseSammelauftrag(false)
{
	anzahl = 1;
	sichtbereich   = 0;
	zeugnis = 0;
	m_KollilisteExport = 0;
	maxlaenge = 0;
	set_plausiOk(true); 
	einkauf = true;
	mindermenge = false;
	preis.set_werkauftrag(this);
	posNr = "1";
	cadpos = false; // Wird beim Konvertieren von CADPositionen gesetzt
	Set_Fertigungsstaette(Werkauftrag::OLDESLOE);
	m_FertigungMagdeburgMoeglich = false;
}

//Destruktor
Werkauftrag::~Werkauftrag()
{
	vk3elementeListe.deleteAllElemente();
	stueckliste.clear();
}

void Werkauftrag::copy(const Werkauftrag& other)
{
	/*
	Kopieren der Attribute
	Nicht vergessen: Bei Änderungen an den Attributen diese Methode up to date halten!!!
	*/
	waid = other.waid;
	strGUID = other.strGUID;
	status.copy(other.get_status_const());
	anhang.copy(*other.get_anhang());
	leitkarte.copy(other.get_leitkarte());
	beschichtung.copy(other.getBeschichtung());
	material.copy(other.get_material());
	arbeit.copy(other.get_arbeit());
	preis.copy(other.get_preis());
	zulassungsstelle= other.zulassungsstelle;// other.zulassungsstelle;
	
	erstellungsdatum = other.erstellungsdatum;
	aenderungsdatum  = other.aenderungsdatum;

	bezeichnung = other.bezeichnung; 
	posNr	= other.posNr;	
	anzahl	= other.anzahl;			
	bemerkung = other.bemerkung;		

	einkauf = other.einkauf;
	sichtbereich = other.sichtbereich;
	zeugnis = other.zeugnis;
	m_KollilisteExport  = other.m_KollilisteExport;
	m_iFertigungsstaette = other.m_iFertigungsstaette;
	mindermenge = other.mindermenge;
	cadpos = other.cadpos;
	m_FertigungMagdeburgMoeglich = other.m_FertigungMagdeburgMoeglich;

	vk3elementeListe.deleteAllElemente();
	///VK3Elemente kopieren
	const CObList& vk3elelist = other.get_vk3elementeListe();
	POSITION pos;
	for (pos = vk3elelist.GetHeadPosition(); pos != NULL;)	
	{
		const Element* element = dynamic_cast<const Element*>(vk3elelist.GetNext(pos));
		XASSERT(element);
		if (element)
		{
			Element* ele = new Element;
			ele->copy(*element);
			vk3elementeListe.AddTail(ele);
		}
	}
}

//Speichern und Laden des Werkauftrages.
void Werkauftrag::serialize(BinaryArchive& ar)
{
	bool vk3abrechnung = false;
	int version = ar.get_version() > 0 ? ar.get_version() : 1000;
	
	MxString st;
	if (ar.is_storing())
	{
		ar << waid;
		ar << bezeichnung << bemerkung;
		ar << sichtbereich << zeugnis;
		ar << (COleDateTime&)erstellungsdatum;
		ar << (COleDateTime&)aenderungsdatum;
		ar << anzahl;
		ar << posNr;
		ar << m_KollilisteExport;
		ar << m_iFertigungsstaette;

		
		if (version > 63) 
		{
			ar << strGUID;
		}
		
		st = "Speichern: "; 
	}
	else
	{
		ar >> waid;
		ar >> bezeichnung >> bemerkung;
		ar >> sichtbereich >> zeugnis;
		ar >> (COleDateTime&)erstellungsdatum;
		ar >> (COleDateTime&)aenderungsdatum;
		ar >> anzahl;
		ar >> posNr;
		if (version > 13)
			ar >> m_KollilisteExport;

		if (version > 57)
			ar >> m_iFertigungsstaette;
		
		if (version > 63) 
		{
			ar >> strGUID;
		}
		

		st = "Lesen: ";
	}


	serialize_bool_if(version > 58, m_FertigungMagdeburgMoeglich, ar);
	serialize_bool_if(version > 10, plausiOk, ar);
	serialize_bool_if(version > 17, mindermenge, ar);
	serialize_bool_if(version > 19, vk3abrechnung, ar);
	serialize_bool_if(version > 42, cadpos, ar);
	
	material.serialize(ar);
	preis.serialize(ar); 
	leitkarte.serialize(ar);
	status.serialize(ar);
	arbeit.serialize(ar);
	beschichtung.serialize(ar);
	anhang.serialize(ar);


	/*
	if (!WaxSys::isAV() && (isApax() || isRohr()))
	{
		//Werte bei jedem Lesen und Schreiben in LOG Datei schreiben, wegen der Fehlersuche warum die WAID manschmal nicht gespeichert wird
		MXDate cur = MXDate::GetCurrentTime();
		MxString date = cur.Format("%d.%m.%Y %H:%M:%S");
		MxString file = ar.get_filename().c_str();
		st += MxString("WAID= {0} Datum:{1} SD: {2} SDPos: {3} Datei: {4}").arg(waid).arg(date).arg(leitkarte.get_sdAuftragsnr()).arg(leitkarte.get_sdPositionsnr()).arg(file);
		LOG(st.getString());
	}
	*/
}

/**
Anhand der SD Materialnummer prüfen ob es ein Verteiler ist
*/
bool Werkauftrag::isSdMaterialNrVerteiler() const
{
	const int sdmatnr = get_leitkarte().get_sdMaterialnummer();
	if (sdmatnr == Leitkarte::VerteilerGepulvert || 
		sdmatnr == Leitkarte::VerteilerVerzinkt || 
		sdmatnr == Leitkarte::CO2VerteilerGepulvert ||
		sdmatnr == Leitkarte::CO2VerteilerVerzinkt)
		return true;

	return false;
}

bool Werkauftrag::isSdMaterialNrStrangrohr() const
{
	return get_leitkarte().get_sdMaterialnummer() == Leitkarte::StrangrohrGepulvertVerzinkt;
}

bool Werkauftrag::isSdMaterialNrApax() const
{
	return get_leitkarte().get_sdMaterialnummer() == Leitkarte::ApaxVerzinkt || get_leitkarte().get_sdMaterialnummer() == Leitkarte::ApaxGepulvert;
}

bool Werkauftrag::isSdMaterialNrGewinderohr() const
{
	const int sdmatnr = get_leitkarte().get_sdMaterialnummer();
	return sdmatnr == Leitkarte::GewinderohreGepulvert || sdmatnr == Leitkarte::GewinderohreVerzinkt;
}

const CString& Werkauftrag::get_bemerkung() const
{
	return bemerkung;
}

void Werkauftrag::set_bemerkung(const CString& value)
{
	bemerkung = value;
}

//Direkter Zugriff auf die Beschichtung
Beschichtung& Werkauftrag::get_beschichtung()
{
	return beschichtung;
}

void Werkauftrag::set_beschichtung(const Beschichtung& value)
{
	beschichtung = value;
}

//Direkter Zugriff auf den Arbeitsplan
Arbeitsplan& Werkauftrag::get_arbeit() 
{
	return arbeit;
}


//Direkter Zugriff auf die Materialliste
 MaterialListe &  Werkauftrag::get_material() 
{
	return material;
}

const MaterialListe&  Werkauftrag::get_material() const 
{
	return material;
}

//Direkter Zugriff auf den Anhang
Werkauftrag::Anhang* Werkauftrag::get_anhang()
{
	return &anhang;
}

//Standardkonstruktor
Werkauftrag::Anhang::Anhang() : data(NULL), len(0), MAXLEN_ANHANG(2048000), showAnhang(true)
{
}

//Destruktor
Werkauftrag::Anhang::~Anhang()
{
	set_data(NULL, 0);
}

bool Werkauftrag::Anhang::is_graphic() const
{
	if (!filename.GetLength() || !get_len())
		return false;
	
	string fn = (LPCSTR)filename;
	vector<string> fnpart;
	split(fnpart, fn, is_any_of("."));
	if (!fnpart.size())
		return false;

	string endung = to_lower_copy(fnpart.back());
	const string endungen[] = { "svg", "tiff", "gif", "jpg", "bmp", "xml", "dwf", "pdf", "" };
	
	for (int i = 0; endungen[i].length(); i++)
		if (endungen[i] == endung)
			return true;

	return false;
}

void Werkauftrag::Anhang::copy(const Anhang& other)
{
	bezeichnung = other.bezeichnung;
	data = other.data;
	filename = other.filename;
	len = other.len;

	data = new char[len];

	memcpy((void*)data, other.data, len);
}


//Speichern der Daten
void Werkauftrag::Anhang::serialize(BinaryArchive& ar)
{
	if (ar.is_storing())	
	{
		ar << len;
		ar << filename;
		ar << bezeichnung;
		if (get_len()) 
		{
			char *bin = (char*)data;

			for (unsigned int i = 0; i < len; i++)
			ar << bin[i];
		}
	} 
	else 
	{
		ar >> len;
		ar >> filename;
		ar >> bezeichnung;
		if (len) 
		{
			char *dat = new char[len];

			for (unsigned int i = 0; i < len; i++)
			{
				ar >> dat[i];
			}
			data = (void*)dat;

		} else
			data = NULL;
	}
}

//Liest die Daten einer Datei in den Anhang ein
void Werkauftrag::Anhang::setfile(const CString &fn)
{
	TRY
	{
		CFile f(fn, CFile::modeRead);
		if (f.GetLength() > MAXLEN_ANHANG)
			AfxThrowNotSupportedException();
		if (data)
		{
			delete[] data;
			data = NULL;
			len = 0;
			filename.Empty();
		}

		data = new char[(unsigned int)f.GetLength()];
		XASSERT(data);
		if (data)
		{
			CString strTemp = f.GetFileName();
			int iWhere = strTemp.Find(".");
			if(iWhere > -1)
			{
				CString strLeft = strTemp.Left(iWhere + 1);
				CString strRight = strTemp.Mid(iWhere + 1, 5 );
				strRight.MakeLower();
				strTemp = strLeft + strRight;
			}
			
			filename = strTemp;
			len = (unsigned int)f.GetLength();
			TRACE("Dateianhang: %s\n", filename);
			len = f.Read((void*)data, len);
		}
	}
	CATCH(CFileException, e)
	{
		e->ReportError();
	}
	END_CATCH
}

const CString& Werkauftrag::Anhang::get_filename() const
{
	return filename;
}

void Werkauftrag::Anhang::set_filename(const CString& value)
{
	CString strTemp;
	strTemp = value;
	strTemp.Replace( '&', '_');
	filename = strTemp;
}

const void* Werkauftrag::Anhang::get_data() const
{
	return data;
}

void Werkauftrag::Anhang::set_data(void* value, int argname)
{
	if (data)
		delete[] data;
	data = value;
	len = argname;
}

const UINT& Werkauftrag::Anhang::get_len() const
{
	return len;
}

const CString& Werkauftrag::Anhang::get_bezeichnung() const
{
	return bezeichnung;
}

void Werkauftrag::Anhang::set_bezeichnung(const CString &bez)
{
	bezeichnung = bez;
}


void  Werkauftrag::Anhang::set_showAnhang(bool val)
{
	showAnhang = val;
}
		
const bool  Werkauftrag::Anhang::get_showAnhang()const
{
	return showAnhang;
}





//Direkter Zugriff auf die Leitkarte des Werkauftrages
Leitkarte& Werkauftrag::get_leitkarte()
{
	return leitkarte;
}

//const Zugriff auf die Leitkarte des Werkauftrages
const Leitkarte& Werkauftrag::get_leitkarte() const
{
	return leitkarte;
}

//set leitkarte
void Werkauftrag::set_leitkarte(const Leitkarte& value)
{
	leitkarte.copy(value);
}

//Setzt die Freigabedaten zurück, damit die Kopie eines Objektes freigegeben 
//werden kann
void Werkauftrag::reset()
{
	/*
	Setzt die Freigabedaten zurück
	Wird ein Werkauftrag kopiert, kann er nur freigegeben werden, wenn die WA Nummer auf 0 steht.
	Ausserdem wird hier das Freigabedatum und die Datenbankid zurückgesetzt.
	*/
	set_waid(0);
	get_status().reset();
	get_leitkarte().get_preisanfrage().reset();
	get_leitkarte().set_sdPositionsnr(0);
	get_leitkarte().set_kcPositionsnr(0);
    get_leitkarte().set_sdAuftragsnr(0);
	get_leitkarte().set_IsChoosenIppeNumbers(false);
	
	if (!WaxSys::isAV())
	{		
		get_leitkarte().set_kcAuftragsnr("");
	}

}

//Direkter Zugriff auf das Preisobjekt
Preis& Werkauftrag::get_preis()
{
	return preis;
}

/**
Liefert true zurück, wenn die Materialliste und die Arbeitsliste leer ist. 
Wird in Rohrleitung überschrieben, damit auch die Elementliste überprüft 
werden kann.
*/
bool Werkauftrag::isEmpty() const
{
	return arbeit.isEmpty() && material.isEmpty();
}

///Hier wird automatisch der Arbeitsplan generiert
void Werkauftrag::createAutoArbeit(bool beschichtung)
{
	get_arbeit().clean();

	createStdAfos(beschichtung);

	createProfilAfos();

	get_arbeit().sortAfos();
}

///Erzeugt die Arbeitsfolgen fuer Profile
void Werkauftrag::createProfilAfos()
{
	CObList proflist;
	if (get_material().get_profilliste(proflist, Element::USTAHL | Element::FSTAHL | Element::LSTAHL))

	for (POSITION pos = proflist.GetHeadPosition(); pos;)
	{
		Material* mat = dynamic_cast<Material*>(proflist.GetNext(pos));
		XASSERT(mat);
		if (!mat)
			break;

		get_arbeit().addAfos(mat);
	}
}

///Erzeugt die Standardarbeitsfolgen
void Werkauftrag::createStdAfos(bool beschichtung)
{
	Arbeitsplan& plan = get_arbeit();
	
	// Diese Afo (Uebergabe an Produktion) immer zuerst
	plan.addAfo_Produktion();

	// Erzeugen der Afos fuer die Beschichtung 
	if(get_GUID().IsEmpty() == TRUE)
	{
		if (beschichtung)
			createBeschAfos(plan);
	}

	// Erzeugen der Afos fuers Anliefern
	createAnliefAfos(plan);

	// Zeugnis
	if (get_zeugnis())
		plan.addAfo(new Arbeitsfolge(37));
	plan.updateSpecialAF(NULL, this->get_beschichtung(), this->getGesGewicht(), this->getMaxLaenge(), this->get_status().get_wanr());
}

///Erzeugen der Afos fuers Anliefern
void Werkauftrag::createAnliefAfos(Arbeitsplan& plan)
{
	plan.addAfo_Anlieferung(get_beschichtung().isVerzinkt());
}

///Erzeugen der Afos fuer die Beschichtung
void Werkauftrag::createBeschAfos(Arbeitsplan& plan)
{
	plan.addAfo_Beschichtung(get_anzahl(), get_beschichtung(), getPulverzeit(), getGrundierungszeit(), getPutzzeit());
}



CString Werkauftrag::getPosBezeichnung(bool sdnr) const
{
	CString bez, str;
	// Jetzt in AV immer Text automatisch, damit die auch bei 
	// geaendertem Text in NL die Posnr sehen
	if (WaxSys::isAV())
	{
		if (sdnr)
		{
			str = getStammdaten().getText("IDS_POSNR_ABK").c_str();
			bez.Format("%06u %s: %s %s", get_status_const().get_wanr(), str, get_posNr(), stdbezeichnung); 
			return bez;
		}
		else 
		{
			str = getStammdaten().getText("IDS_POSNR_ABK").c_str();
			bez.Format("%s: %s", str, get_posNr()); 
			return bez;
		}
	}
	else 
	{
		if (sdnr)
		{
			str = getStammdaten().getText("IDS_POSNR_ABK").c_str();
			bez.Format("%I64d  %u %s: %s %s", get_leitkarte().get_sdAuftragsnr(), get_leitkarte().get_sdPositionsnr(), str, get_posNr(), stdbezeichnung);
			return bez;
		}
		else
		{
			str = getStammdaten().getText("IDS_POSNR_ABK").c_str();
			bez.Format("%s: %s", str, get_posNr()); 
			return bez;
		}
	}
}

//Liefert die Bezeichnung des Werkauftrages. Wenn bezeichnung nicht gesetzt 
//wurde, wird die stdbezeichnung erzeugt.
CString Werkauftrag::get_bezeichnung() const
{
	if (bezeichnung.GetLength()) 
		return bezeichnung;
	// Wenn keine Standardbezeichnung vorgegeben ist (Projektleitkarte und Sammelauftrag besitzen
	// einen Dummywerkauftrag), wird kein String zurückgeliefert
	if (stdbezeichnung.GetLength())
		return stdbezeichnung;
	return "";
}

void Werkauftrag::set_bezeichnung(const CString& value)
{
	bezeichnung = value;
}

const int Werkauftrag::get_sichtbereich() const
{
	return sichtbereich;
}

void Werkauftrag::set_sichtbereich(int value)
{
	sichtbereich = value;
}

const int Werkauftrag::get_zeugnis() const
{
	return zeugnis;
}

const int Werkauftrag::get_zulassungsstelle() const
{
	return zulassungsstelle;
}

const string Werkauftrag::get_barcodeStringForGuidLineStart(int rohrQuali, double ad, bool isStart) const
{
	bool onlyFM=true;
	string rohrStaerke="S";

	if(rohrQuali>=31 && zulassungsstelle==1)
	{
		rohrStaerke="T";
	}

	std::stringstream stream;
	stream << std::fixed << std::setprecision(1) << ad;
	std::string s = stream.str();


	MxString aud = MxString("{0}").arg(ad);
	string bc="";

	bc.append("MXOD ").append(rohrStaerke);

	if(isStart)
	{
		if(zulassungsstelle==1 && !onlyFM)
		{
			bc.append(" ").append(s);
			bc.append(" VdS");
			return bc;
		}
		if(zulassungsstelle==2)
		{
			bc.append(" ").append(s);
			bc.append(" <FM>");
			return bc;
		}
	}
	else
	{
		if(zulassungsstelle==1 && !onlyFM)
		{
			return bc;
		}
		if(zulassungsstelle==2)
		{
			bc.append(" <FM>");
			return bc;
		}
	}

	return "";
}

const string Werkauftrag::get_barcodeStringForGuidLineEnd(int rohrQuali, double ad, int wanr) const
{
	bool onlyFM=true;
	string bc= get_barcodeStringForGuidLineStart( rohrQuali,  ad, false);
	MxString kcstr = MxString("M{0}").arg(wanr);

	if(zulassungsstelle==2)
	{
		bc.append(" ").append(kcstr);
		return bc;
	}
	if(onlyFM)
		return "";

	return bc;
}

void Werkauftrag::set_zeugnis(int value)
{
	zeugnis = value;
}

void Werkauftrag::set_zulassungsstelle(int value)
{
	m_KollilisteExport=value;
	zulassungsstelle = value;
}

Status &  Werkauftrag::get_status()
{
	return status;
}

const int Werkauftrag::get_waid() const
{
	return waid;
}

void Werkauftrag::set_waid(int value)
{
	waid = value;
}

void Werkauftrag::set_GUID(CString value)
{
	strGUID = value;
}

CString Werkauftrag::get_GUID()
{
	return strGUID;
}




const MXDate& Werkauftrag::get_erstellungsdatum() const
{
	return erstellungsdatum;
}


const MXDate& Werkauftrag::get_aenderungsdatum() const
{
	return aenderungsdatum;
}


///Errechnet die Pulverzeit für den Werkauftrag
double Werkauftrag::getPulverzeit() 
{
	return 0.0;
}

///Errechnet die Putzzeit für den Werkauftrag
double Werkauftrag::getPutzzeit() 
{
	return 0.0;
}

///Errechnet die Grundierzeit für den Werkauftrag 
double Werkauftrag::getGrundierungszeit()
{
	return 0.0;
}

//Liefert eine Anzahl zurück: Bei Rohrleitung die Anzahl 
//der Rohrstücke (???), bei Sondermz einfach nur die 
//Bestellanzahl, default ist 1
const int Werkauftrag::get_anzahl() const
{
	return anzahl;
}

//Setzt die Anzahl
void Werkauftrag::set_anzahl(int value)
{
	anzahl = value;
}

const int Werkauftrag::get_maxlaenge() const
{
	return maxlaenge;
}

void Werkauftrag::set_maxlaenge(int value)
{
	maxlaenge = value;
}

const bool Werkauftrag::get_einkauf() const
{
	return einkauf;
}

void Werkauftrag::set_einkauf(bool value)
{
	einkauf = value;
}

//set posNr
void Werkauftrag::set_posNr(const CString& value)
{
	posNr = value;
}

//get posNr
const CString& Werkauftrag::get_posNr() const
{
	return posNr;
}

const CString& Werkauftrag::get_stdbezeichnung() const
{
	return stdbezeichnung;
}

/**
Versucht aus der übergebenen Positionsnummer eine neue zu erzeugen. Also 
z.B. soll aus 1.02 1.03 oder aus A.a A.b werden.
*/
void Werkauftrag::initPosnr(const CString& pattern)
{
	if (!pattern.GetLength())
	{
		// Erste Position im Sammelauftrag oder vorige Position hat keine Posnr
		posNr = "1";
		return ;  
	}
	CString prepattern;
	CString tmppattern = pattern;
	// Positionsnummer ohne Trennzeichen verarbeiten
	if (pattern.FindOneOf(".-") == -1)
	{
		tmppattern = pattern;
	}
	else
	{ 
		// ReverseFindOneOf fehlt natürlich bei CString...
		for (int i = pattern.GetLength()-1; i >= 0; i--)
		{
			char pchar = pattern.GetAt(i);
			if (pchar == '.' || pchar == '-')
			{// Aufteilen in Vorstring und Reststring. 
				tmppattern = pattern.Right(pattern.GetLength()-i);
				prepattern = pattern.Left(i);
			}
		}
	}

	// Einfachster Fall: Positionsnummer ist eine Ganzzahl. Dann ist die neue Positionsnummer
	// posnr+1
	int nr = atoi(tmppattern);

	if (nr)
	{
		posNr.Format("%i", nr+1);
		return;
	}
	else
	{ // Wahrscheinlich A oder aa, a1 oder so
		posNr = tmppattern;
		unsigned char letzteszeichen = posNr.GetAt(posNr.GetLength()-1);

		bool ende = false;
		if (posNr.GetLength() > 1)
		{
			unsigned char vorletzteszeichen;
			vorletzteszeichen = posNr.GetAt(posNr.GetLength()-2);
		
			if (letzteszeichen == 'Z')
			{
				++vorletzteszeichen;
				letzteszeichen = 'A'; 
				ende = true;
				posNr.SetAt(posNr.GetLength()-2, vorletzteszeichen);
			}
			if (letzteszeichen == 'z')
			{
				++vorletzteszeichen;
				letzteszeichen = 'a';
				ende = true;
				posNr.SetAt(posNr.GetLength()-2, vorletzteszeichen);
			}
			if (letzteszeichen == '9')
			{
				if (vorletzteszeichen >= 'A' && vorletzteszeichen <= 'Z')
					posNr.Insert(posNr.GetLength()-1, '1');
				if (vorletzteszeichen >= 'a' && vorletzteszeichen <= 'z')
					posNr.Insert(posNr.GetLength()-1, '1');
				if (vorletzteszeichen >= '0' && vorletzteszeichen <= '9')
				{
					++vorletzteszeichen;
					posNr.SetAt(posNr.GetLength()-2, vorletzteszeichen);
				}
				letzteszeichen = '0';
				ende = true;
			}
		}

		while (!ende)
		{
			++letzteszeichen;
			if (letzteszeichen >= 'A' && letzteszeichen <= 'Z')
				break;
			if (letzteszeichen >= 'a' && letzteszeichen <= 'z')
				break;
			if (letzteszeichen >= '0' && letzteszeichen <= '9')
				break;
		}

		posNr.SetAt(posNr.GetLength()-1, letzteszeichen);
	}
	posNr = prepattern+posNr;
}

void Werkauftrag::set_erstellungsdatum(const MXDate& value)
{
	erstellungsdatum = value;
}

//Prüft ob die eingegebene SD Nummer für diesen Werkauftrag erlaubt ist.
bool Werkauftrag::checkSDMatNr( ) const
{
	const type_info &typ = typeid(*this);

	CString name = typ.name();
	if (name.Find("class ") == 0)
		name = name.Right(name.GetLength() - 6);

	for (POSITION pos = getStammdaten().get_dummymaterialnummern().GetHeadPosition(); pos; )
	{
		DummyMaterialnummern::data* dat= (DummyMaterialnummern::data*)getStammdaten().get_dummymaterialnummern().GetNext(pos);
		
		if (leitkarte.get_sdMaterialnummer() == dat->get_nummer())
		{
			return dat->get_klasse().Find(name) == -1 ? false : true;
		}
	}
	return false;
}

const bool Werkauftrag::get_plausiOk() const
{
	return plausiOk;
}

void Werkauftrag::set_plausiOk(bool value)
{
	plausiOk = value;
}

//Werkauftrag auf korrekte Werte checken
CString Werkauftrag::checkPlausis()
{
	getErrorHandler().clearErrorList();
	return "";
}

const int Werkauftrag::get_KollilisteExport() const
{
	return m_KollilisteExport;
}

void Werkauftrag::set_KollilisteExport(int value)
{
	m_KollilisteExport = value;
}


//Berechnungs des Aufschlags fuer Rohrverschnitt
void Werkauftrag::addRohrausschuss(CObList& list)
{
	//Virtuell, muss ueberschrieben werden
	UNREFERENCED_PARAMETER(list);
}

const bool Werkauftrag::get_mindermenge() const
{
	return mindermenge;
}

void Werkauftrag::set_mindermenge(bool value)
{
	mindermenge = value;
}

//Arbeitsplan auf Mindermenge checken
bool Werkauftrag::checkMindermenge()
{
	// Mindermenge checken
	for (POSITION posi = get_arbeit().get_arbeitsfolgen().GetHeadPosition(); posi;)
	{
		Arbeitsfolge *afo = dynamic_cast<Arbeitsfolge *>(get_arbeit().get_arbeitsfolgen().GetNext(posi));
		XASSERT(afo);
		if (!afo)
			break;
		
		if ((afo->get_id() == 16) && afo->get_zeit() < 0.3001)
			return true;
	}	
	return false;
}

const Beschichtung& Werkauftrag::getBeschichtung() const
{
	return beschichtung;
}

const CString& Werkauftrag::get_stdbemerkung() const
{
	return stdbemerkung;
}

void Werkauftrag::set_stdbemerkung(const CString& value)
{
	stdbemerkung = value;
}

bool Werkauftrag::avNacharbeitsPruefung() const
{
	return false;
}


//get stueckliste
EnStueckliste& Werkauftrag::get_stueckliste() 
{
	return stueckliste;
}

const EnStueckliste& Werkauftrag::get_stueckliste() const
{
	return stueckliste;
}

/**
Erzeugen des Modells. Muß in den abgeleiteten Klassen überschrieben werden
Außerdem werden von createModell alle Listen gefüllt, da diese erst nach Erstellung des Modells erstellt werden können
*/
void Werkauftrag::createModell()
{
	return;
}

bool Werkauftrag::geaendert_nach_freigabe() const
{
	/**
	Liefert true, wenn nach der letzten Freigabe noch Änderungen 
	gemacht wurden.
	*/
	if (!get_waid())
		return false; // Nicht freigegeben

	// Bei der AV Version wird das Bearbeitungsdatum geändert, bei NL Version das Änderungsdatum
	const MXDate& cmp = get_aenderungsdatum();

	bool ret = cmp.is_newer(WaxSys::isAV() ? get_status_const().get_bearbeitungsdatum() : get_status_const().get_freigabedatum());
	return ret;
}


void Werkauftrag::set_aenderungsdatum(const MXDate& value)
{
	aenderungsdatum = value;
}

///Erzeugt die VK3-Elemente aus der Elementliste
bool Werkauftrag::createVk3Liste()
{
	vk3elementeListe.deleteAllElemente();
	return true;
}


const EnCObListVk3& Werkauftrag::get_vk3elementeListe() const
{
	return vk3elementeListe;
}


EnCObListVk3& Werkauftrag::get_vk3elementeListe()
{
	return vk3elementeListe;
}




int Werkauftrag::getMaxLaenge()const
{
	return get_maxlaenge();
}