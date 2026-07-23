#include "stdafx.h"
#include "resource.h"


#include "Leitkarte.h"
#include "Werkauftrag.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif 


//Konstruktor
Leitkarte::Leitkarte() : sdAuftragsnr(0), sdPositionsnr(0), sdMaterialnummer(0), prj_produktlinie(11),
					    kcPositionsnr(0), m_IsChoosenIppeNr(false), m_ippeKnotenNr(""), m_ippeMatNr(""), m_Brandschutztechnik(0)
{
	/**
	Die nl Kennung wird initialisiert durch: Wert von 'IB_NL_KENNUNG' aus Systemparametern
	*/
	init_wunschliefertermin();
	set_prj_nl(WaxSys::para_int(IB_NL_KENNUNG));
	sachbearbeiterNL = WaxSys::para_str(BEARBEITER);
	set_vk3(true);
	sdMaterialnummer = 0;	
}


//Destruktor
Leitkarte::~Leitkarte()
{

}

//Serialisierung der statischen Attribute
void Leitkarte::serializeStatics(BinaryArchive &ar) 
{
	/**
	Datenserialisierung: Projektfelder
	Serialisiert die statischen Datenattribute aus LeitkarteData
	*/
	int version = ar.get_version() > 0 ? ar.get_version() : 1000;

	if (ar.is_storing())
	{
		ar << prj_auftragsbez;
		ar << prj_nl;
		ar << prj_bauPlz;
		ar << prj_bauOrt;
		ar << prj_pspElement;
		ar << prj_produktlinie;
		ar << prj_bauAbschnitt;
		ar << prj_COauftrag;
	}
	else
	{
		CString prj_bauName;
		ar >> prj_auftragsbez;
		ar >> prj_nl;
		if (version < 17)
			ar >> prj_bauName;
		ar >> prj_bauPlz;
		ar >> prj_bauOrt;
		ar >> prj_pspElement;
		ar >> prj_produktlinie;
		if (version > 15)
			ar >> prj_bauAbschnitt;
		if (version > 21)
			ar >> prj_COauftrag;
	}	
}

//Serialisierung
void Leitkarte::serialize(BinaryArchive &ar) 
{
	/**
	Datenserialisierung
	Serialisiert die Datenattribute aus LeitkarteData
	*/

	int version = ar.get_version() > 0 ? ar.get_version() : 1000;

	serializeStatics(ar);

	serialize_bool(vk3_abrechnung, ar);
	serialize_bool_if(version >= 60, m_IsChoosenIppeNr, ar);

	if (ar.is_storing())
	{
		ar << sachbearbeiterNL;
		ar << emailNl;
		ar << (COleDateTime&)wunschliefertermin;
		ar << sdAuftragsnr;
		ar << sdPositionsnr;
		ar << sdMaterialnummer;
		ar << kcAuftragsnr;
		ar << kcPositionsnr;
		ar << m_ippeKnotenNr;
		ar << m_ippeMatNr;
		ar << m_Brandschutztechnik;
		ar << zulassungsnummer;


		preisanfrage.serialize(ar);

	}
	else 
	{
		ar >> sachbearbeiterNL;
		if (version >= 50)
			ar >> emailNl;

		ar >> (COleDateTime&)wunschliefertermin;

		if (version >= 49)
			ar >> sdAuftragsnr;
		else
		{
			unsigned long tmp;
			ar >> tmp;
			sdAuftragsnr = tmp;
		}

		ar >> sdPositionsnr;
		ar >> sdMaterialnummer;
		if (version == 42 || version == 43)
		{
			int temp;
			ar >> temp;
			ar >> temp;
		}
		if (version >= 44)
		{
			ar >> kcAuftragsnr;
			ar >> kcPositionsnr;
		}

		if(version >= 60)
		{
			ar >> m_ippeKnotenNr;
			ar >> m_ippeMatNr;
		}

		if(version >= 62)
		{
			ar >> m_Brandschutztechnik;
		}
		ar >> zulassungsnummer;
		if (version >= 47)
		{
			preisanfrage.serialize(ar);
		}
	}
}

//Kopiert die Daten der Leitkarte, aber nicht die Statusdaten wie z.B. 
//Freigabedatum
void Leitkarte::copy(const Leitkarte& other)
{
	prj_auftragsbez  = other.prj_auftragsbez;
	prj_bauOrt       = other.prj_bauOrt;
	prj_bauPlz       = other.prj_bauPlz;
	prj_nl           = other.prj_nl;
	prj_pspElement   = other.prj_pspElement;
	prj_produktlinie = other.prj_produktlinie;
	prj_bauAbschnitt = other.prj_bauAbschnitt;
	prj_COauftrag	 = other.get_prj_COauftrag();
	
	m_IsChoosenIppeNr = other.m_IsChoosenIppeNr;
	wunschliefertermin = other.wunschliefertermin;

	m_ippeMatNr			 = other.m_ippeMatNr;
	m_ippeKnotenNr		 = other.m_ippeKnotenNr;
	m_Brandschutztechnik = other.m_Brandschutztechnik;
	if (other.sdAuftragsnr)
		sdAuftragsnr = other.sdAuftragsnr;

	if (other.sdPositionsnr)
		sdPositionsnr = other.sdPositionsnr;

	if (other.sdMaterialnummer)
		sdMaterialnummer = other.sdMaterialnummer;

	
	if (other.kcAuftragsnr)
		kcAuftragsnr = other.kcAuftragsnr;

	if (other.kcPositionsnr)
		kcPositionsnr = other.kcPositionsnr;


	preisanfrage.copy(other.preisanfrage);

	
	set_vk3(other.get_vk3());
}

//Kopiert alle Daten der Leitkarte, wird für das Setzen der Leitkarte eines 
//Sammelauftrages benötigt
void Leitkarte::clone(const Leitkarte& other)
{
	copy(other);
	sachbearbeiterNL = other.get_sachbearbeiterNL();
	zulassungsnummer = other.get_zulassungsnummer();
	emailNl = other.get_emailNl();
}

//Setzt den Wunschliefertermin auf heute + Systemparameter "LIEFERFRIST".
void Leitkarte::init_wunschliefertermin()
{
	int frist = WaxSys::para_int(LIEFERFRIST);
	COleDateTimeSpan diff(frist, 0, 0, 0);
	
	wunschliefertermin = wunschliefertermin.get_current_date() ;
	wunschliefertermin += diff;
}

bool Leitkarte::setPrjdata(const Leitkarte& other)
{
	prj_auftragsbez		= other.get_prj_auftragsbez();
	prj_nl				= other.get_prj_nl();
	prj_bauPlz			= other.get_prj_bauPlz();
	prj_bauOrt			= other.get_prj_bauOrt();
	

	if (!prj_COauftrag.GetLength())
		prj_COauftrag   = other.get_prj_COauftrag();

	if (!prj_bauAbschnitt.GetLength())
	{
		prj_bauAbschnitt = other.get_prj_bauAbschnitt(); 
	}
	

	if (!prj_pspElement.GetLength() || !atoi(prj_pspElement.Left(6)))
	{
		prj_pspElement		 = other.get_prj_pspElement();
		prj_produktlinie	 = other.get_prj_produktlinie();
		m_IsChoosenIppeNr	 = other.get_IsChoosenIppeNumbers();
		m_Brandschutztechnik = other.get_Brandschutztechnik();
	}

	return true;
}

CString Leitkarte::check() const
{
	CString err = "";
/*
	Wunschliefertermin herausgenommen
*/
	return err;
}

//Prüft die Angaben in der Leitkarte auf Vollständigkeit
CString Leitkarte::checkVars(bool checksap, bool checkSd) const
{
	CString fehlendedaten, err, str;
	if (!get_prj_auftragsbez().GetLength())
	{
		str = getStammdaten().getText("IDS_FEHLENDE_DATEN1a").c_str();
		fehlendedaten += str;
	}
	if (!get_prj_bauOrt().GetLength())
	{
		str = getStammdaten().getText("IDS_FEHLENDE_DATEN2a").c_str();
		fehlendedaten += str;
	}
	if (!get_prj_bauPlz().GetLength())
	{
		err = getStammdaten().getText("IDS_FEHLENDE_DATEN3").c_str();
	}

	if (!get_prj_nl())
	{
		str = getStammdaten().getText("IDS_FEHLENDE_DATEN4").c_str();
		fehlendedaten += str;
	}
	if (!get_sachbearbeiterNL().GetLength())
	{
		str = getStammdaten().getText("IDS_FEHLENDE_DATEN5").c_str();
		fehlendedaten += str;
	}
	if (!get_prj_bauAbschnitt().GetLength())
	{
		str = getStammdaten().getText("IDS_FEHLENDE_DATEN6").c_str();
		err += str;
	}

	if (err.GetLength())
		((CFrameWnd*)AfxGetMainWnd())->SetMessageText(err);

	if (checksap)
	{
		CString last3 = get_prj_pspElement().Right(3);	
		if ((!get_prj_pspElement().GetLength() || atoi(get_prj_pspElement().Left(6)) <= 1) && !prj_COauftrag.GetLength())
		{
			str = getStammdaten().getText("IDS_FEHLENDE_DATEN7").c_str();
			fehlendedaten += str;
		}

		if (checkSd)
		{

			if(m_IsChoosenIppeNr)
			{
						
				int iLength = m_ippeMatNr.GetLength();
				if(m_ippeMatNr.IsEmpty())
				{
					fehlendedaten += getStammdaten().getText("IDS_IPPEMATERIAL_NR").c_str();
				}
				else
				{
					if( iLength == 10 )
					{
						CString strTemp = _T("1");
						if(strTemp.Compare( m_ippeMatNr.Left(1)) != 0)
						{
							fehlendedaten += getStammdaten().getText("IDS_IPPE_NR_TEN_DIGITS").c_str();
						}
					}
					else
					{
						fehlendedaten += getStammdaten().getText("IDS_IPPE_NR_TEN_DIGITS").c_str();
						
					}
				}
			}
			else
			{
				
				CString strSDAuftragNr;
				strSDAuftragNr.Format( "%I64d", get_sdAuftragsnr());
				
				if(!get_sdAuftragsnr())
				{
					str = getStammdaten().getText("IDS_FEHLENDE_DATEN9").c_str();
					fehlendedaten += str;
				}
				else
				{
					int iLength = strSDAuftragNr.GetLength();
					
					if( iLength == 10 )
					{
						CString strTemp = _T("1");	
						if(strTemp.Compare( strSDAuftragNr.Left(1)) == 0)
						{
							fehlendedaten += getStammdaten().getText("IDS_KEINEGUELTIGE_SDNR_IPPE").c_str();
						}
					}
				}

				if(!get_sdPositionsnr())
				{
					str = getStammdaten().getText("IDS_FEHLENDE_DATEN11").c_str();
					fehlendedaten += str;
				}
			}
		}

		if (!get_sdMaterialnummer())
		{
			str = getStammdaten().getText("IDS_FEHLENDE_DATEN10").c_str();
			fehlendedaten += str;
		}
	}
	return fehlendedaten;
}

//Liefert einen formatierten String mit dem PSP Element zur Ausgabe 
//in z.B. Drucklisten
CString Leitkarte::getPSPElement()
{
	if (get_prj_pspElement().GetLength() == 0)
		return get_prj_COauftrag();
	
	CString psp;

	CString strNL;
	strNL.Format("%i", get_prj_nl());
	
	if(get_prj_nl() < 10)
	{
		strNL = CString("00") + strNL;
	}
	
	if(9 < get_prj_nl() && get_prj_nl() < 100)
	{
		strNL = CString("0") + strNL;
	}

	if(9 < get_prj_nl() && get_prj_nl() < 100)
	{
		strNL = CString("0") + strNL;
	}
	psp.Format("%s-%s-%s-%i-%s", get_prj_pspElement().Left(1), get_prj_pspElement().Mid(1, 6), strNL, get_prj_produktlinie(), get_prj_pspElement().Right(3));

	return psp;
}


int Leitkarte::get_sdMaterialnummer() const
{
	return sdMaterialnummer;
}

void Leitkarte::set_sdMaterialnummer(int value)
{
	sdMaterialnummer = value;
}