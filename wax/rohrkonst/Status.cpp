#include "stdafx.h"
#include "resource.h"


		
			#include "Status.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif 

Status::Status() : freigabecounter(0), statusflag(0), wanr(0)
{
	reset();
	if (WaxSys::isAV())
		sachbearbeiterSegment = WaxSys::para_str(BEARBEITER);
}

Status::~Status()
{
	
}

void Status::serialize(BinaryArchive& ar)
{
	int version = ar.get_version() > 0 ? ar.get_version() : 1000;

	if (ar.is_storing())
	{
		ar << wanr;
		ar << sachbearbeiterSegment;
		ar << (COleDateTime&)mbtermin;
		ar << (COleDateTime&)bearbeitungsdatum;
		ar << (COleDateTime&)freigabedatum;
		ar << freigabecounter;
		ar << statusflag;
		ar << statusstr;
		ar << status;
	}
	else
	{
		ar >> wanr;
		ar >> sachbearbeiterSegment;
		ar >> (COleDateTime&)mbtermin;
		ar >> (COleDateTime&)bearbeitungsdatum;
		ar >> (COleDateTime&)freigabedatum;
		ar >> freigabecounter;
		ar >> statusflag;
		ar >> statusstr;

		if (version >= 47)
			ar >> status;
	}
}

//Setzt die Daten zurück. Muss bei Kopieraktionen aufgerufen werden.
void Status::reset()
{
	set_sachbearbeiterSegment("");

	//mbtermin (Materialbereitstellungstermin) nicht mit MXDate::null initialisieren, da sonst das control das
	//aktuelle datum anzeigt.
	//Beim datum 30.12.1899 (=> m_dt=0) gibt es Probleme mit dem Format Befehl
	
	mbtermin.SetStatus(MXDate::null);
	freigabedatum.SetStatus(MXDate::null);
	bearbeitungsdatum.SetStatus(MXDate::null);

	set_freigabecounter(0);
	set_storniert(false);
	set_wanr(0);

	set_status(Status::NichtGesetzt);
}


void Status::copy(const Status& other)
{
	/**
	Kopieren des Status, ausser den Statusflags. Da bei der Statusaktualisierung
	im Sammelauftrag alle Unteraufträge den Status des ersten Teilauftrages bekommen,
	würde hier das Storniertflag mit kopiert
	*/
	wanr = other.get_wanr();
	sachbearbeiterSegment = other.get_sachbearbeiterSegment();
	mbtermin = other.get_mbtermin();
	bearbeitungsdatum = other.get_bearbeitungsdatum();
	freigabedatum = other.get_freigabedatum();
	freigabecounter = other.get_freigabecounter();
	status = other.status;
}




//Fügt dem statusstr eine Meldung hinzu. Die Meldung wird an die bisherigen 
//Meldungen angehängt.
void Status::addstatusstr(const CString & stat)
{
	statusstr += (CString)"\r\n" + stat;
}


//Setzt das Statusflag CRCERROR und fügt eine Statusmeldung hinzu.
void Status::setCRCError(const CString& stat, const CString &inf)
{
	set_statusflag(CRCERROR);
	CString str, txt;
	txt = getStammdaten().getText("IDS_ERROR109").c_str();
	str.Format(txt, MXDate::GetCurrentTime().Format(), stat);
	if (inf.GetLength())
		str += (CString)" : " + inf;
	addstatusstr(str);
}


bool Status::isFreigabedatumOld()
{
	int t1 = (int)MXDate::GetCurrentTime().m_dt;
	int t2 = (int)freigabedatum.m_dt;
	return t1 > t2;
}

void Status::set_mbtermin(const MXDate& value)
{
	if (value.GetDay() != mbtermin.GetDay() || value.GetYear() != mbtermin.GetYear() || value.GetMonth() != mbtermin.GetMonth())
		mbtermin.copy(value);
}



void Status::set_status(Status::STATUS stat)
{
	status = stat;
}

Status::STATUS Status::get_status()const
{
	return status;
}