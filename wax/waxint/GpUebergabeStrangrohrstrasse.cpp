#include "stdafx.h"


#include "GpUebergabeStrangrohrstrasse.h"
#include "DbExcelUeberagbeRobby.h"
#include "..\rohrkonst\Element.h"
#include "resource.h"

#include "..\waxprj\HtmlDruck.h"
#include "..\waxprj\WaxHtmlFile.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

GpUebergabeStrangrohrstrasse::GpUebergabeStrangrohrstrasse(void)
{
	m_dnRohr = 0;
	m_muffenlaenge = 0;
	m_muffenkehlung = -1;
	m_rohrQuali = 0;

	m_isMultiWA = false;		//Wenn mehrere Werkaufträge zusammengeasst werden = true

	m_stVariabel = "variabel";	//Wenn Werte unterschiedlich sind wird der Wert auf variabel gesetzt
}

GpUebergabeStrangrohrstrasse::~GpUebergabeStrangrohrstrasse(void)
{
}


bool GpUebergabeStrangrohrstrasse::setKopfwerte()
{

//----------------------- Makros----------------

#define GET_WERT(_function,_varName,_arrayName)\
get_werkauftraege()->_function(_arrayName);\
if (_arrayName.GetSize() == 1)\
	_varName = _arrayName[0];\
else\
	_varName = m_stVariabel;

#define GET_WERT_INTO_STRING(_function,_varName,_intArrayName)\
get_werkauftraege()->_function(_intArrayName);\
if (_intArrayName.GetSize() == 1)\
	_varName.Format("%i",_intArrayName[0]);\
else\
	_varName = m_stVariabel;


#define GET_ULONG_INTO_STRING(_function,_varName,_intArrayName)\
get_werkauftraege()->_function(_intArrayName);\
if (_intArrayName.GetSize() == 1)\
	_varName.Format("%u",_intArrayName[0]);\
else\
	_varName = m_stVariabel;



#define GET_INT64U_INTO_STRING(_function,_varName,_intArrayName)\
get_werkauftraege()->_function(_intArrayName);\
if (_intArrayName.GetSize() == 1)\
	_varName.Format("%I64d",_intArrayName[0]);\
else\
	_varName = m_stVariabel;

//----------------------- Makros----------------



	CArray<int,int> arInt;

	CArray<ULONG,ULONG> arULong;

	CArray<__int64, __int64> arInt64u;

	CStringArray    arString;

	const int MAX_LAENGE_WANR = 6;


	if (!get_werkauftraege() || get_werkauftraege()->isEmpty())
		return false;


	
	//Kopfwerte aus der Liste mit Werkaufträgen ermitteln

	//WA Nummer (es wird immer die 1. WaNr genommen, da die Maschine auf 6 stellen prüft)
	get_werkauftraege()->getAllWaNr(arInt);
	if (arInt.GetSize() <= 0)
		return false;
	
	m_stWaNr.Format("%d",arInt[0]);

	if (arInt[0] > 1)
	{
		//Schalter setzen, dass es sich um Zusammengefasste Aufträge handelt
		m_isMultiWA = true;

		//Wa Nr so ändern, das an der 1. Stelle eine 9 steht. (zur Zeit sind die WaNr 6stellig und < 200000)
		//damit die zusammengefassten Aufträge von den Einzelaufträgen unterschieden werden können
		if (arInt[0] < 900000)
		{
			CString temp9 = "900000";

			int laenge = m_stWaNr.GetLength();
			if (laenge == MAX_LAENGE_WANR)
				laenge--;
				 
			m_stWaNr = temp9.Left(MAX_LAENGE_WANR - laenge) + m_stWaNr.Right(laenge);
		}
	}
	

	//Rohr Dn ermitteln. Muß für alle Rohre gleich sein
	get_werkauftraege()->getAllRohrDn(arInt);
	if (arInt.GetSize() != 1) 
	{
		return false;
	}
	m_dnRohr = arInt[0];

	//Rohrquali ermitteln. Muß für alle Rohre gleich sein
	get_werkauftraege()->getAllRohrQuali(arInt);
	if (arInt.GetSize() != 1) 
	{
		return false;
	}
	m_rohrQuali = arInt[0];


	//Muffen ID. Muß für alle Rohre gleich sein
	get_werkauftraege()->getAllMuffenID(arInt);
	if (arInt.GetSize() != 1) 
	{
		return false;
	}

	//Bohrungsdurchmesser bzw. Muffe Dn
	if (arInt[0] == BOHR24)
	{
		CArray<double,double> arDouble;
		//Bohrungsdurchmesser. Muß für alle Rohre gleich sein
		get_werkauftraege()->getAllDuchmesser(arDouble);
		if (arDouble.GetSize() != 1) 
		{
			return false;
		}
		m_dnMuffe.Format("%.2f",arDouble[0]);
		m_dnMuffe.Replace(".",",");  //für Excel umwandeln
	}
	else
	{
		//Muffen DN. Muß für alle Rohre gleich sein
		get_werkauftraege()->getAllMuffenDn(arInt);
		if (arInt.GetSize() != 1) 
		{
			return false;
		}
		m_dnMuffe.Format("%d",arInt[0]);
	}
	


	//Muffen Laenge. Muß für alle Rohre gleich sein
	get_werkauftraege()->getAllMuffenLaenge(arInt);
	if (arInt.GetSize() != 1) 
	{
		return false;
	}
	m_muffenlaenge = arInt[0];

	//Muffenkehlung. Muß für alle Rohre gleich sein
	get_werkauftraege()->getAllMuffenKehlung(m_muffenkehlung);


	if (m_muffenkehlung < 0)
	{
		return false;
	}


	//übrigen Kopfwerte aus der Liste mit Werkaufträgen lesen. Wenn mehrere Werte übergeben werden, 
	//wird der Wert auf "variabel" gesetzt

	//Bauabschnitt
	GET_WERT(getAllBauabschnitte,m_stBauabschnitt,arString)
	//Auftragsbezeichnung
	GET_WERT(getAllAuftragsBez,m_stAuftragsBez,arString)
	//Plz
	GET_WERT(getAllPlz,m_stPlz,arString)
	//Ort
	GET_WERT(getAllOrte,m_stOrt,arString)
	//Nl
	GET_WERT_INTO_STRING(getAllNls,m_stNl,arInt)
	//SachbearbeiterNl
	GET_WERT(getAllSachbearbeiterNl,m_stSachbearbeiter,arString)
	//SachbearbeiterOD
	GET_WERT(getAllSachbearbeiterOd,m_stSachbearbeiterOd,arString)
	//Erstelldatum
	GET_WERT(getAllErstelldatum,m_stErstellungDat,arString)
	//Bearbeitungsdatum
	GET_WERT(getAllBearbeitungsdatum,m_stBearbeitungDat,arString)
	//Wunschliefertermin
	GET_WERT(getAllWunschliefertermin,m_stWunschLieferDat,arString)
	//Freigabedatum
	GET_WERT(getAllFreigabedatum,m_stFreigabeDat,arString)
	//Bemerkung
	GET_WERT(getAllBemerkungen,m_stBemerkung,arString)
	//SD Auftrag
	GET_INT64U_INTO_STRING(getAllSdAuftragsNr,m_stSdAuftrag,arInt64u)
	//SD MaterialNr
	GET_WERT_INTO_STRING(getAllSdMaterialNr,m_stSdMaterial,arInt)
	//Sd Position
	GET_WERT_INTO_STRING(getAllSdPositionsNr,m_stSdPos,arInt)
	//Psp
	GET_WERT(getAllPsp,m_stPsp,arString);


	return true;
}



/**
Werkaufträge in Excelübergabedatein für die Scheißstrassen schreiben

Zur Zeit erstmal nur für die Strangrohrstrasse. Wenn mehrere Werkaufträge (Sammelaufträge) übergeben werden, werden 
diese zusammengefasst. Es wird eine Dummy WA Nummer vergeben.
Dabei wird überprüft ob die Strangrohr alle die gleiche RohrDn, RohrQuali, Muffenlaenge, MuffenDN haben.
*/
bool GpUebergabeStrangrohrstrasse::writeUebergabeDaten()			
{	
	DbExcelUeberagbeRobby	excel;

	CString		path,
				stOffset = "17",
				stLeer = ".",
				stTemp,
				avNachbearbeitung;



	int			counter = 0;



	//Kopfwerte ermitteln
	if (!setKopfwerte())
		return false;

	
	//Spaltennamen	definieren
	CStringArray spalten;
	spalten.Add("Positionsnummer");
	spalten.Add("AVNachbearbeitung");
	spalten.Add("AnzahlRohreSollwert");
	spalten.Add("AnzahlRohreIstwert");
	spalten.Add("Rohranfang");
	spalten.Add("Rohrende");
	spalten.Add("GesamteStranglänge");
	spalten.Add("AnzahlMuffen");
	spalten.Add("Muffenabstand1");
	spalten.Add("Muffenabstand2");
	spalten.Add("Muffenabstand3");
	spalten.Add("Muffenabstand4");
	spalten.Add("Muffenabstand5");
	spalten.Add("Muffenabstand6");
	spalten.Add("Muffenabstand7");
	spalten.Add("Muffenabstand8");
	spalten.Add("Muffenabstand9");
	spalten.Add("Muffenabstand10");
	spalten.Add("Restmass");
	spalten.Add("BedienerGeändert");
	spalten.Add("Nummer");
	spalten.Add("Kappenfarbe");
	spalten.Add("ZeitenStatistik1");
	spalten.Add("ZeitenStatistik2");
	spalten.Add("ZeitenStatistik3");
	spalten.Add("ZeitenStatistik4");
	spalten.Add("WaxAfoZeit");
	


	if(spalten.GetSize() != DbExcelUeberagbeRobby::eZEILEN_SPALTEN)
	{
		//Bei Spaltenerweiterung diese Variable ändern => eZEILEN_SPALTEN
		DebugBreak();
	}
	//Pfad + Dateinamen
	path = setGesPfadUebergabeDatei(m_stWaNr);

	//Exceldatei initialisieren
	if (!excel.initDatei(path,spalten))
		return false;


	//Kopfwerte in Exceldatei schreiben

	//Zeile1
	stTemp.Format("%d",m_dnRohr);
	excel.appendKopfZeile("WA-Nummer",m_stWaNr,"Offset",stOffset,"Rohr DN",stTemp,"Auftrag geprüft",++counter);

	//Zeile2
	stTemp.Format("%d",get_werkauftraege()->getSize());
	excel.appendKopfZeile("Bau",m_stAuftragsBez,"Positionen",stTemp,"Wandstärke[DIN]",Rohrleitung::getRohrQualikurztext(m_rohrQuali,m_dnRohr),"Auftrag abgearbeitet",++counter);

	//Zeile3
	excel.appendKopfZeile("Plz",m_stPlz,stLeer,stLeer,"Muffen DN",m_dnMuffe,"Auftrag geändert",++counter);

	//Zeile4
	stTemp.Format("%d",m_muffenlaenge);
	excel.appendKopfZeile("Ort",m_stOrt,stLeer,stLeer,"Muffenhoehe",stTemp,stLeer,++counter);

	//Zeile5
	(m_muffenkehlung == 1 ? stTemp = "X" : stTemp = stLeer);		
	excel.appendKopfZeile("IB",m_stNl,stLeer,stLeer,"Muffenkehlung",stTemp,stLeer,++counter);

	//Zeile6
	excel.appendKopfZeile("Sachbearbeiter NL",m_stSachbearbeiter,stLeer,stLeer,stLeer,stLeer,stLeer,++counter);

	//Zeile7
	excel.appendKopfZeile("Sachbearbeiter OD",m_stSachbearbeiterOd,stLeer,stLeer,stLeer,stLeer,stLeer,++counter);

	//Zeile8
	excel.appendKopfZeile("Erstellungsdatum",m_stErstellungDat,stLeer,stLeer,stLeer,stLeer,stLeer,++counter);

	//Zeile9
	excel.appendKopfZeile("Bearbeitungsdatum",m_stBearbeitungDat,stLeer,stLeer,stLeer,stLeer,stLeer,++counter);

	//Zeile10
	excel.appendKopfZeile("Wunschliefertermin",m_stWunschLieferDat,stLeer,stLeer,stLeer,stLeer,stLeer,++counter);

	//Zeile11
	excel.appendKopfZeile("Freigabedatum",m_stFreigabeDat,stLeer,stLeer,stLeer,stLeer,stLeer,++counter);

	//Zeile12
	excel.appendKopfZeile("Bemerkung",m_stBemerkung,stLeer,stLeer,stLeer,stLeer,stLeer,++counter);

	//Zeile13
	excel.appendKopfZeile("SD Auftragsnr",m_stSdAuftrag,stLeer,stLeer,stLeer,stLeer,stLeer,++counter);

	//Zeile14
	excel.appendKopfZeile("SD Materialnr",m_stSdMaterial,stLeer,stLeer,stLeer,stLeer,stLeer,++counter);

	//Zeile15
	excel.appendKopfZeile("SD Positionsnr",m_stSdPos,stLeer,stLeer,stLeer,stLeer,stLeer,++counter);

	//Zeile16
	excel.appendKopfZeile("PSP-Element",m_stPsp,stLeer,stLeer,stLeer,stLeer,stLeer,++counter);



	//einzelne Positionen in Exceldatei einfügen
	int posCounter = 0,
		waNr = 0;

	MxIterator iter(*get_werkauftraege());
	while (iter.next())
	{
		Strangrohr *pStrang = dynamic_cast<Strangrohr*>(get_werkauftraege()->getAt(iter));

		//mds#425
		Werkauftrag *wa=dynamic_cast<Werkauftrag*>(get_werkauftraege()->getAt(iter));

		XASSERT(pStrang);
		if (!pStrang)
			return false;

		//Modell erzeugen, damit alle Listen gesetzt sind
		pStrang->createModell();

		(pStrang->get_avnachbearbeitung() ? avNachbearbeitung = "x" : avNachbearbeitung = stLeer);

		//Wenn Liste aus mehreren Werkaufträgen besteht, laufende Nr. davor schreiben.
		if (m_isMultiWA)
		{
			//Bei jedem neuen Auftrag Counter hochsetzen
			if (waNr != pStrang->get_status().get_wanr())
			{
				posCounter++;
				waNr = pStrang->get_status().get_wanr();
			}
			stTemp.Format("%d_%s",posCounter,pStrang->get_posNr());
		}
		else
			stTemp = pStrang->get_posNr();

		int rohrende = 0;
		if (pStrang->isRedBogEinz())
			rohrende = GLATT;
		else
			rohrende = pStrang->get_rohrEnde();

		//Muffenrestabstand berechnen
		pStrang->set_muffenRestabstand(0);

		pStrang->createAutoArbeit();

	Arbeitsplan& ArPlan = pStrang->get_arbeit();
	double afoStrangzeit =0;
	for (POSITION pos = ArPlan.get_arbeitsfolgen().GetHeadPosition(); pos != NULL;)
	{
		Arbeitsfolge* afo = (Arbeitsfolge*)ArPlan.get_arbeitsfolgen().GetNext(pos);
		if ( afo->get_id() == 51)
		{
			afoStrangzeit = afo->get_zeit()/pStrang->get_anzahl();
		}
	}

		excel.appendPosZeile(stTemp,avNachbearbeitung,pStrang->get_anzahl(),
							 getRohrAnfangEndeElementAsString(pStrang->get_rohrAnfang()),getRohrAnfangEndeElementAsString(rohrende),
							 pStrang->getLaengeZuschnitt(),pStrang->getAnzahlMuffen(),
							 pStrang->getRelLaengeStdMuffe(0),pStrang->getRelLaengeStdMuffe(1),pStrang->getRelLaengeStdMuffe(2),pStrang->getRelLaengeStdMuffe(3),pStrang->getRelLaengeStdMuffe(4),
							 pStrang->getRelLaengeStdMuffe(5),pStrang->getRelLaengeStdMuffe(6),pStrang->getRelLaengeStdMuffe(7),pStrang->getRelLaengeStdMuffe(8),pStrang->getRelLaengeStdMuffe(9),
							 pStrang->getRelLaengeStdMuffe(99),pStrang->getKappenfarbe(pStrang->get_rohrKFarbe()),
							 afoStrangzeit, ++counter);
	}
	
	excel.closeDatei();

	return true;
}

/**
Rohranfang bzw. Ende als String für die Excelausgabe erstellen
*/
CString GpUebergabeStrangrohrstrasse::getRohrAnfangEndeElementAsString(int eleId)
{
	CString bezEl;

	CString	stNut		= "N",
		    stGewinde	= "G",
			stGlatt		= "-",
			stScheibe	= "(";

	if (eleId == NUT)
		bezEl = stNut;
	else if (eleId == GEW)
		bezEl = stGewinde;
	else if (eleId == GLATT)
		bezEl = stGlatt;
	else if (eleId == SCH)
		bezEl = stScheibe;

	return bezEl;
}

/**
Rückgabe des Pfad + Datei für die Übergabedatei für Strangrohrstrassee
*/
const CString& GpUebergabeStrangrohrstrasse::setGesPfadUebergabeDatei(const CString &stWaNr)
{
	CString path;

	path = WaxSys::para_str(DIR_STRANG_FREE);

	m_stGesPfadUebergabedatei = makeGesPfad(path,stWaNr,"xls");
	
	return m_stGesPfadUebergabedatei;
}

/**
Gesamtpfad + Dateiname erstellen
*/
CString GpUebergabeStrangrohrstrasse::makeGesPfad(const CString &pfad,const CString& waNr,const CString &ext)
{
	CString gesPfad = pfad;

	// wenn am Ende kein Backslasch, dann einen dazu 
	if (gesPfad.Right(1) != "\\")
		gesPfad += "\\";
	
	//evt. Nullen vorne anhängen
	if (waNr.GetLength() < 6)
	{
		CString stNull = "000000";
		gesPfad += stNull.Left(6 - waNr.GetLength()) + waNr;
	}
	else
		gesPfad += waNr;

	gesPfad += "." + ext;	

	return gesPfad;
}

/**
Dateinamen für die Übersichtsdatei erzeugen
*/
const CString& GpUebergabeStrangrohrstrasse::setGesPfadUebersichtsDatei(const CString &stWaNr) 
{
	CString path,
		    str;
	
	path = WaxSys::GetTempDir();
	if (path.Right(1) != "\\")
		path += "\\";

	//Präfix für dateinamen
	str = getStammdaten().getText("IDS_ZUSAMMENFASSEN_WA").c_str();
	str += stWaNr;

	m_stGesPfadUebersichtsdatei = makeGesPfad(path,str,"html");

	return m_stGesPfadUebersichtsdatei;
}



/**
prüfen ob Werkauftrag über die Strangrohrtsrasse gefertigt werden kann
*/
bool GpUebergabeStrangrohrstrasse::checkWerkauftragOk(Werkauftrag *wa)
{

	if (!wa)
		return false;

	//prüfen ob Werkauftrag ein Strangrohr ist
	Strangrohr *pStrang = dynamic_cast<Strangrohr*>(wa);
	if (!pStrang)
	{
		CString error,
		        stTemp;
		stTemp = getStammdaten().getText("IDS_KEIN_STRANGROHR").c_str();
		error.Format(stTemp,wa->get_status().get_wanr(),wa->get_posNr());
		appendErrorDescription((LPCSTR)error);
		return false;
	}

	return true;
}


/**
Plausis prüfen. 
Prüfen ob alle Rohre zusammen in einem Dummyauftrag gefertigt werden können
*/
bool GpUebergabeStrangrohrstrasse::checkPlausis()
{
	CString error;

	CArray<int,int> arInt;



	if (!get_werkauftraege())
	{
		error = getStammdaten().getText("IDS_KEINE_WERKAUSTRAEGE").c_str();
		appendErrorDescription((LPCSTR)error);
		return false;
	}

	if (get_werkauftraege()->isEmpty())
	{
		error = getStammdaten().getText("IDS_KEINE_WERKAUSTRAEGE").c_str();
		appendErrorDescription((LPCSTR)error);
		return false;
	}

	//Rohr Dn ermitteln. Muß für alle Rohre gleich sein
	get_werkauftraege()->getAllRohrDn(arInt);
	if (arInt.GetSize() != 1) 
	{
		error = getStammdaten().getText("IDS_STRANGROHR_UNGLEICH").c_str();
		appendErrorDescription((LPCSTR)error);
		return false;
	}

	//Rohrquali ermitteln. Muß für alle Rohre gleich sein
	get_werkauftraege()->getAllRohrQuali(arInt);
	if (arInt.GetSize() != 1) 
	{
		error = getStammdaten().getText("IDS_STRANGROHR_UNGLEICH").c_str();
		appendErrorDescription((LPCSTR)error);
		return false;
	}

	//Muffen ID. Muß für alle Rohre gleich sein
	get_werkauftraege()->getAllMuffenID(arInt);
	if (arInt.GetSize() != 1) 
	{
		error = getStammdaten().getText("IDS_STRANGROHR_UNGLEICH").c_str();
		appendErrorDescription((LPCSTR)error);
		return false;
	}

	if (arInt[0] == BOHR24)
	{
		CArray<double, double> arDouble;
		//Bohrungsdurchmesser. Muß für alle Rohre gleich sein
		get_werkauftraege()->getAllDuchmesser(arDouble);
		if (arDouble.GetSize() != 1) 
		{
			error = getStammdaten().getText("IDS_STRANGROHR_UNGLEICH").c_str();
			appendErrorDescription((LPCSTR)error);
			return false;
		}
	}
	else
	{
		//Muffen DN. Muß für alle Rohre gleich sein
		get_werkauftraege()->getAllMuffenDn(arInt);
		if (arInt.GetSize() != 1) 
		{
			error = getStammdaten().getText("IDS_STRANGROHR_UNGLEICH").c_str();
			appendErrorDescription((LPCSTR)error);
			return false;
		}
	}

	//Muffen Laenge. Muß für alle Rohre gleich sein
	get_werkauftraege()->getAllMuffenLaenge(arInt);
	if (arInt.GetSize() != 1) 
	{
		error = getStammdaten().getText("IDS_STRANGROHR_UNGLEICH").c_str();
		appendErrorDescription((LPCSTR)error);
		return false;
	}

	//Muffenkehlung. Muß für alle Rohre gleich sein
	int muffenkehlung = 0;
	get_werkauftraege()->getAllMuffenKehlung(muffenkehlung);
	if (muffenkehlung < 0)
	{
		error = getStammdaten().getText("IDS_STRANGROHR_UNGLEICH").c_str();
		appendErrorDescription((LPCSTR)error);
		return false;
	}

	return true;
}

/**
Werkaufträge zu den WA Nummern aus der DB lesen und an interne Liste hängen
*/
bool GpUebergabeStrangrohrstrasse::appendWerkauftraege(const CUIntArray &arNewWa)
{
	CUIntArray arNewWaGeprueft,
			   arWaids;	


	//prüfen ob Werkauftraege schon in der Liste stehen
	for (int i=0; i<arNewWa.GetSize(); i++)
	{
		bool bFound = false;
		long newWa = arNewWa[i];

		MxIterator iter(*get_werkauftraege());
		while (iter.next())
		{
			Werkauftrag *pWa =  dynamic_cast<Werkauftrag*>(get_werkauftraege()->getAt(iter));
			if (!pWa)
				continue;

			//prüfen ob WA Nr schon in der Liste mit Werkaufträgen steht
			if (newWa == pWa->get_status().get_wanr())
			{
				bFound = true;
				break;
			}
		}

		if (!bFound)
			arNewWaGeprueft.Add(newWa);
	}

	//Werkaufträge aus DB lesen
	if (!arNewWaGeprueft.IsEmpty())
	{
		//Zu den WA Nummern die Waids ermitteln. (Positionen der Werkaufträge)
		//readWaidsFromDb(arNewWaGeprueft,arWaids);
		for (int i=0; i < arNewWaGeprueft.GetSize(); i++)
		{
			arWaids.RemoveAll();
			m_dbWerkauftrag.readWaids(arNewWaGeprueft[i], arWaids);

			WerkauftragListe tmpWa;

			for (int i=0; i<arWaids.GetSize(); i++)
			{
				//Zu der Waid den Werkauftrag aus der DB lesen
				Werkauftrag *wa = readWerkauftragFromDb(arWaids[i]);

				//Prüfen ob ok für Strangrohrstraße
				if (!checkWerkauftragOk(wa))
					continue;

				Rohrleitung *rohr = dynamic_cast<Rohrleitung*>(wa);
				if (!rohr)
					continue;

				//Länge des Rohres
				int laenge = rohr->getStandardRohrlaenge();
				int pos = 0;

				//Der Länge nach absteigend sortieren
				MxIterator it(tmpWa);
				for (it.first();it.isValid();it++)
				{
					Werkauftrag *waTmp  = (Werkauftrag*)tmpWa.getAt(it);
					Rohrleitung *rohr2 = dynamic_cast<Rohrleitung*>(waTmp);
					if (!rohr2)
						continue;

					int laenge2 = rohr2->getStandardRohrlaenge();
					if (laenge2 <= laenge)
						break;

					pos++;
				}
				
				tmpWa.insertAt(pos, wa);
			}
			
			//in der Gesamtliste speichern
			appendWerkauftragListe(tmpWa);
		}
	}

	return true;
}


/**
Alte Exceldateien löschen
*/
bool GpUebergabeStrangrohrstrasse::deleteOldUebergabeWerte()
{
	CArray <int,int> arWaNr;

	CString	pfad;


	WerkauftragListe *waListe = get_werkauftraege();

	if (!waListe)
		return true;

	//Alle WA Nummern aus der Liste ermitteln
	waListe->getAllWaNr(arWaNr);


	//Wenn mehrere Werkaufträge zusammengefasst wurden, evt. alte Übergabedateien löschen
	if (arWaNr.GetSize() > 1)
	{
		for (int i=0; i<arWaNr.GetSize(); i++)
		{
			CString stTemp;
			stTemp.Format("%d",arWaNr[i]);

			pfad = setGesPfadUebergabeDatei(stTemp);		
			DeleteFile(pfad);
		}
	}

	return true;
}



/**
Werkaufträge in eine Html Datei ausgeben. 
*/
bool GpUebergabeStrangrohrstrasse::writeUebersicht()
{
	CString ueberschrift,
		    nlBez,
			auftragsBez,
			oberfl,
			str,
			strRohr,
			strDn,
			strMuffe,
			strMuffenDn;

	CStringArray arOberfl;

	int    anzahlMuffen = 0,
		   anzahlPositionen = 0
		  ;

	HtmlDruck html;



	//Kopfwerte setzen
	if (!setKopfwerte())
		return false;


	//Dateinamen + Pfad und Datei automatisch anlegen
	CString pfad = setGesPfadUebersichtsDatei(m_stWaNr);

	//evt. vorhandene Datei löschen
	::DeleteFile(pfad);

	//Html datei öffnen
	html.get_file().openFile(pfad,ueberschrift);

	//Bezeichnung der Niederlassung lesen 
	if (!m_stNl.IsEmpty())
		nlBez = getStammdaten().getNLBezeichnung(atoi(m_stNl));

	//Auftragsbez evt. zurücksetzen, damit dann der Bauabschnitt in insertKopf nicht gesetzt wird
	if (m_stAuftragsBez == m_stVariabel)
		auftragsBez.Empty();

	ueberschrift = getStammdaten().getText("IDS_ZUSAMMENFASSEN_WA").c_str();
	//Kopfwerte einfügen

	Werkauftrag *wa = dynamic_cast<Werkauftrag*>(get_werkauftraege()->getFirst());

	DNzuAussendurchmesser dntoad;
	double aussendurchmesser = dntoad.getAussendurchmesser(m_dnRohr);

	
	string bcText=wa->get_barcodeStringForGuidLineStart(m_rohrQuali,aussendurchmesser, true);

	CString barcode="";
	if(bcText!="")
	{
		barcode=html.get_file().createBarcode(250, 60, bcText, "left").c_str();
	}
	
	html.insertKopf(ueberschrift,"",
						m_stWaNr,m_stSachbearbeiterOd,m_stSachbearbeiter,
						m_stBearbeitungDat,m_stFreigabeDat,m_stWunschLieferDat,
						nlBez,auftragsBez,m_stBauabschnitt,m_stPlz,m_stOrt,
						m_stSdAuftrag,m_stSdPos,m_stSdMaterial,m_stPsp,
						false,m_stVariabel, barcode);


	//Oberfläche ermitteln
	get_werkauftraege()->getAllOberflaechen(arOberfl);
	if (arOberfl.GetSize() == 1)
		oberfl = arOberfl[0];
	else
		oberfl = m_stVariabel;

	//Oberfläche einfügen
	str = getStammdaten().getText("IDS_OBERFLAECHE").c_str();
	html.insertZeile2Spalten(str,125,false,oberfl,400,false);

	//Rohrquali einfügen
	html.insertQuali(m_rohrQuali,false,m_dnRohr);

	//Rohr DN und Muffen Dn einfügen
	strRohr = getStammdaten().getText("IDS_DNROHR").c_str();
	strDn.Format("DN %d",m_dnRohr);
	strMuffe = getStammdaten().getText("IDS_DNMUFFE").c_str();
	strMuffenDn = "DN " + m_dnMuffe;
	html.insertZeile4Spalten(strRohr,125,false,strDn,125,false,
							 strMuffe,125,false,strMuffenDn,125,false);


	// Tabele mit Werkauftragspositionen erstellen
	WaxHtmlFile &w = html.get_file();
	w.startTable();
	
// Table 3 

	const int rows[] = { 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20};

	w.startTable(0,0);
	w.startRow();
	str = "waNr";//getStammdaten().getText("IDS_POSNR_ABK").c_str();
	CString txt = "<b>" + str + "</b>";
	w.appendCol(txt,		rows[0]);
	
	str = "Pos"; //getStammdaten().getText("IDS_KAPPF").c_str();
	txt = "<b>" + str + "</b>";
	w.appendCol(txt,		rows[1]);
	
	str = getStammdaten().getText("IDS_ANZ_STK").c_str();
	txt = "<b>" + str + "</b>";
	w.appendCol(txt,	rows[2]);

	

	str = getStammdaten().getText("IDS_ANFANG").c_str();
	txt = "<b>" + str + "</b>";
	w.appendCol(txt,		rows[4]);
	
	str = getStammdaten().getText("IDS_ENDE").c_str();
	txt = "<b>" + str + "</b>";
	w.appendCol(txt,			rows[5]);
	
	str = getStammdaten().getText("IDS_LAENGE_MM").c_str();
	txt = "<b>" + str + "</b>";
	w.appendCol(txt,	rows[6]);
	
	str = getStammdaten().getText("IDS_ABST").c_str();
	txt = "<b>" + str + "1/6 [mm]</b>";
	w.appendCol(txt,	rows[7]);
	
	str = getStammdaten().getText("IDS_ABST").c_str();
	txt = "<b>" + str + "2/7 [mm]</b>";
	w.appendCol(txt,	rows[8]);
	
	str = getStammdaten().getText("IDS_ABST").c_str();
	txt = "<b>" + str + "3/8 [mm]</b>";
	w.appendCol(txt,	rows[9]);
	
	str = getStammdaten().getText("IDS_ABST").c_str();
	txt = "<b>" + str + "4/9 [mm]</b>";
	w.appendCol(txt,	rows[10]);
	
	str = getStammdaten().getText("IDS_ABST").c_str();
	txt = "<b>" + str + "5/10 [mm]</b>";
	w.appendCol(txt,	rows[11]);
	/*
	str = getStammdaten().getText("IDS_REST_MM").c_str();
	txt = "<b>" + str + "</b>";
	w.appendCol(txt,		rows[12]);
	*/


	str = "AV";//getStammdaten().getText("IDS_ANZ_STK").c_str();
	txt = "<b>" + str + "</b>";
	w.appendCol(txt,	rows[12]);

		str = "Anzahl Muffen";//getStammdaten().getText("IDS_ANZ_STK").c_str();
	txt = "<b>" + str + "</b>";
	w.appendCol(txt,	rows[12]);
	
	
	
	w.endRow();
	

	

	MxIterator iter(*get_werkauftraege());
	//int anzahlPositionen, anzahlMuffen;
	int wanr = 0;
	while (iter.next())
	{
		Strangrohr *pStrang = dynamic_cast<Strangrohr*>(get_werkauftraege()->getAt(iter));
		Werkauftrag *wa = dynamic_cast<Werkauftrag*>(get_werkauftraege()->getAt(iter));
		XASSERT(pStrang);
		if (!pStrang)
			return false;
		create_strangrohrliste(w, pStrang, anzahlPositionen, anzahlMuffen, wanr, wa);
		
	}

	createSum( w, anzahlPositionen, anzahlMuffen);
	html.get_file().closeFile();
	return true;
}



void GpUebergabeStrangrohrstrasse::createSum(WaxHtmlFile& w, int anzahlPositionen, int anzahlMuffen )
{
	create_Leerzeile(w);
	const int rows3[] = { 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20};
	//Zeile mit Summen der Positionen und Muffen
	CString str = getStammdaten().getText("IDS_SUMME").c_str();
	w.appendCol("<b>" + str + "</b>", rows3[0]);
	w.appendCol(".",rows3[1]);
	str.Format("%d",anzahlPositionen);
	w.appendCol("<b>" + str + "</b>",rows3[2]);
	str.Format("%d",anzahlMuffen);
	w.appendCol("<b>" + str + "</b>", rows3[3]);
	w.appendCol(".",rows3[4]);
	w.appendCol(".",rows3[5]);
	w.appendCol(".",rows3[6]);
	w.appendCol(".",rows3[7]);
	w.appendCol(".",rows3[8]);
	w.appendCol(".",rows3[9]);
	w.appendCol(".",rows3[10]);
	w.appendCol(".",rows3[11]);
	w.appendCol(".",rows3[12]);
	w.endTable();
}




void GpUebergabeStrangrohrstrasse::create_Leerzeile(WaxHtmlFile& w)
{
	const int rows3[] = { 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20};
	
			w.startRow();
			w.appendCol(".",rows3[0]);
			w.appendCol(".",rows3[1]);
			w.appendCol(".",rows3[2]);
			w.appendCol(".",rows3[3]);
			w.appendCol(".",rows3[4]);
			w.appendCol(".",rows3[5]);
			w.appendCol(".",rows3[6]);
			w.appendCol(".",rows3[7]);
			w.appendCol(".",rows3[8]);
			w.appendCol(".",rows3[9]);
			w.appendCol(".",rows3[10]);
			w.appendCol(".",rows3[11]);
			w.appendCol(".",rows3[12]);
			w.endRow();
}



bool GpUebergabeStrangrohrstrasse::create_strangrohrliste(WaxHtmlFile& w,  Strangrohr*  strang, int& anzahlPositionen, int& anzahlMuffen,  int& wanr, Werkauftrag* wa )
{
	/*
	Liste fuer Strangrohre.
	*/

	CString txt;
	const int rows3[] = { 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20};
	const int rows4[] = {280};
	if (wanr != strang->get_status().get_wanr())
	{
		if (wanr != 0)
		{
			create_Leerzeile(w);
		}
		wanr = strang->get_status().get_wanr();
	}

	HtmlDruck html;
	w.startRow();
	txt.Format("<b>%d</b>", strang->get_status().get_wanr());
	w.appendCol(txt, rows3[0]);
	

	//txt.Format("<b>%d</b>", strang->get_posNr());
	w.appendCol("<b>" + strang->get_posNr() + "</b>", rows3[1]);
	CString temp;
	temp.Format( "%d", strang->get_anzahl());
	w.appendCol(temp, rows3[2]);

	
	//Gesamtanzhal Positionen
	anzahlPositionen += strang->get_anzahl();

	//Gesamtanzahl Muffen
	anzahlMuffen += strang->getAnzahlMuffen() * strang->get_anzahl();


	// Anfangelement
	w.appendCol(CString("<b>") + strang->get_abschluss_kurzbezeichnung(true).c_str() + "<b>", rows3[4]);
	// Endelement
	if(((Strangrohr*)strang)->isRedBogEinz())
	{
		CString strGlatt(_T("Glatt"));
		w.appendCol("<b>" + strGlatt + "<b>", rows3[4]);
	}
	else
	{
		w.appendCol(CString("<b>") + strang->get_abschluss_kurzbezeichnung(false).c_str()+ "<b>", rows3[5]);
	}

	int bogenmass = 0; 
	for (int j=0; j<strang->get_avelementArray().GetSize(); j++)
	{
		// Hinter Bogen duerfen keine Standardelemente mehr sein
		Element *ele = strang->get_avelementArray().get_element_at(j);
		if (ele->isEleBogen())
		{
			bogenmass = ele->get_abstAbs()-ele->get_laenge();
			break;
		}	
	}
	//w.appendCol((bogenmass > 0) ? bogenmass : strang->getLaengeZuschnitt(), rows3[5]);
	temp.Format( "<b>%d</b>", strang->getLaengeZuschnitt());
	w.appendCol(temp, rows3[6]);

	/*
	w.appendCol(strang->get_muffenRelAbst(0), rows3[6]);
	w.appendCol(strang->get_muffenRelAbst(1), rows3[7]);
	w.appendCol(strang->get_muffenRelAbst(2), rows3[8]);
	w.appendCol(strang->get_muffenRelAbst(3), rows3[9]);
	w.appendCol(strang->get_muffenRelAbst(4), rows3[10]);
	*/

	temp.Format( "<b>%d</b>",strang->getRelLaengeStdMuffe(0));
	w.appendCol(temp, rows3[7]);

	temp.Format( "<b>%d</b>", strang->getRelLaengeStdMuffe(1));
	w.appendCol(temp, rows3[8]);

	temp.Format( "<b>%d</b>", strang->getRelLaengeStdMuffe(2));
	w.appendCol(temp, rows3[9]);

	temp.Format( "<b>%d</b>", strang->getRelLaengeStdMuffe(3));
	w.appendCol(temp, rows3[10]);

	temp.Format( "<b>%d</b>", strang->getRelLaengeStdMuffe(4));
	w.appendCol(temp, rows3[11]);
	
	
	
//	w.appendCol(" ", rows3[12]);
	if (strang->get_avnachbearbeitung())	
	{
		txt = "X";
	}
	else
	{
		txt = "-";
	}
	
	w.appendCol("<b>" + txt + "</b>", rows3[13], "CENTER");

	CString strAmoutOfSockets;
	strAmoutOfSockets.Format("%d", strang->getAnzahlMuffen() * strang->get_anzahl());
	w.appendCol("<b>" + strAmoutOfSockets + "</b>", rows3[13]);

	
	if (strang->get_muffenRelAbst(5) != 0)
	{
		
		w.endRow();
		w.startRow();
		txt = "-";
		w.appendCol(txt, rows3[0]);
		w.appendCol(txt, rows3[1]);
		w.appendCol(txt, rows3[2]);
		w.appendCol(txt, rows3[3]);
		w.appendCol(txt, rows3[4]);
		w.appendCol(txt, rows3[5]);
	//	w.appendCol(txt, rows3[6]);

		temp.Format( "<b>%d</b>", strang->getRelLaengeStdMuffe(5));
		w.appendCol(temp,	rows3[7]);

		temp.Format( "<b>%d</b>", strang->getRelLaengeStdMuffe(6));
		w.appendCol(temp, rows3[8]);

		temp.Format( "<b>%d</b>", strang->getRelLaengeStdMuffe(7));
		w.appendCol(temp,	rows3[9]);

		temp.Format( "<b>%d</b>", strang->getRelLaengeStdMuffe(8));
		w.appendCol(temp,	rows3[10]);

		temp.Format( "<b>%d</b>", strang->getRelLaengeStdMuffe(9));
		w.appendCol(temp,	rows3[11]);
	//	w.appendCol(strang->getRelLaengeStdMuffe(99),rows3[12]);	//Restabstand

		w.endRow();
	}



	int zulNr=wa->get_zulassungsstelle();
	wa->set_zulassungsstelle(zulNr);


	//mds#417
	string barcodeText;
	long wnr=strang->get_status().get_wanr();
	int rohrQuali=strang->get_rohrQuali();

	DNzuAussendurchmesser dntoad;
	double aussendurchmesser = dntoad.getAussendurchmesser(strang->get_dnvl());

	barcodeText=wa->get_barcodeStringForGuidLineEnd(rohrQuali,aussendurchmesser,wnr);
	
	if(barcodeText!="")
	{
		CString barcode;
		barcode=html.get_file().createBarcode(250, 60, barcodeText, "left").c_str();
		w.startRow();

		w.appendCol(barcode, rows4[0]);
		w.endRow();
	}

	
	
	/*
	else
		
	{
		//w.appendCol(strang->get_muffenRestabstand(), rows3[11]);
		w.appendCol(strang->getRelLaengeStdMuffe(99), rows3[11]); //Restabstand

		if (strang->get_avnachbearbeitung())	
			txt = "X";
		else					
			txt = "-";
		w.appendCol(txt, rows3[12], "CENTER");
		w.endRow();
	}
	*/
	return true;
}








const CString &GpUebergabeStrangrohrstrasse::get_gesPfadUebersichtsDatei()const
{
	return m_stGesPfadUebersichtsdatei;
}