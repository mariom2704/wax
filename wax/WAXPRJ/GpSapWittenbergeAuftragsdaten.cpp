#include "stdafx.h"



#include "GpSapWittenbergeAuftragsdaten.h"

#include <mxcontrols/msgbox.h>
using namespace MxGui;


const int POS_AUFTRAGSNR  =  0;
const int POS_PROJEKTNR   =  12;
const int POS_PSP_ELEMENT =  20;
const int POS_SD_AUFTRNR  =  40;
const int POS_SD_POSINR   =  50;
const int POS_LIEFDAT     =  56;
const int POS_SD_MATNR    =  64;
const int POS_SACHBEARB   =  82;
const int POS_BEZEICHNUNG =  94;
const int POS_DEBITORENNR =  114;
const int POS_AUFTRAGSBEZ =  149;
const int POS_KEY         =  152;
const int POS_PLZ         =  162;
const int POS_ORT         =  172;
const int POS_NL          =  207;
const int POS_BAU         =  219;
const int POS_AUSSTELLER  =  231;
const int POS_DATUM       =  243;
const int POS_KC_AUFTRNR  =  251;
const int POS_KC_POSINR   =  261;
const int POS_GUID		  =	 267;
const int POS_ENDE		  =  300;



GpSapWittenbergeAuftragsdaten::GpSapWittenbergeAuftragsdaten(void)
{}



GpSapWittenbergeAuftragsdaten::~GpSapWittenbergeAuftragsdaten(void)
{}


bool GpSapWittenbergeAuftragsdaten::readDatei()
{
	CStdioFile file;
	CFileException e;
	CString fn = "", str;

	CString path(WaxSys::para_str(DIR_SAPTOWAXWITTENBERGE));
	fn = path;

	if (!fn.GetLength())
	{
		MsgBox::show_error("Update der KC Nummern nicht möglich:\nParameter DIR_SAPTOWAX ist nicht gesetzt!");
		return false;
	}

	if (file.Open(fn, CStdioFile::modeRead | CStdioFile::shareDenyNone, &e))
	{	

		while (	file.ReadString(str) ) 
		{
			zeilen.Add(str);
		}
		file.Close();
		
	}
	else 
	{
		string msg = boost::str(boost::format("Datei %s wurde nicht gefunden!") % (LPCSTR)fn );
		MsgBox::show_error(msg);
		return false;
	}

	return true;
}



bool GpSapWittenbergeAuftragsdaten::getKcNummer( CString GUID, int& sdPos, CString &kcNummer, int &kcPos, CString &psp, int &nl, int& iProduktline_Brandschutztechnik, COleDateTime& Wunschliefertermin )
{
	CString zeile;
	if(!GUID.IsEmpty())
	{
		if (!findEintragDependGUIDNummer(GUID, zeile))
			return false;
	}
	
	sdPos = 0;
	CString sdPosLocal("");
	getText(zeile, POS_LIEFDAT - POS_SD_POSINR, POS_SD_POSINR, sdPosLocal, true);
	sdPos = atoi(sdPosLocal);

	kcNummer.Empty();
	getText(zeile, POS_KC_POSINR - POS_KC_AUFTRNR, POS_KC_AUFTRNR, kcNummer, true);

	CString stPos;
	getText(zeile, POS_GUID - POS_KC_POSINR, POS_KC_POSINR, stPos, true);
	kcPos = atoi(stPos);

	psp.Empty();
	getText(zeile, POS_SD_AUFTRNR - POS_PSP_ELEMENT, POS_PSP_ELEMENT, psp, false);

	CString tmp = psp.Mid(13,2);
	if (!tmp.IsEmpty())
	{
		iProduktline_Brandschutztechnik = atoi(tmp);
	}

	tmp = psp.Mid(9,3);
	if (!tmp.IsEmpty())
	{
		nl = atoi(tmp);
	}

	tmp = psp.Mid(0,1);
	tmp += psp.Mid(2,6);
	CString tmp2 = psp.Mid(16);
	tmp2.Replace(' ','0');
	psp = tmp + tmp2;

	CString strWunschliefertermin;
	getText(zeile, POS_SD_MATNR - POS_LIEFDAT, POS_LIEFDAT, strWunschliefertermin, false);
	int dtDay = atoi(strWunschliefertermin.Mid(6,2));
	int dtWeek = atoi(strWunschliefertermin.Mid(4,2));
	int dtYear = atoi(strWunschliefertermin.Mid(0,4));
	Wunschliefertermin.SetDate(dtYear, dtWeek, dtDay);
	
	return true;
}




bool GpSapWittenbergeAuftragsdaten::findEintragDependGUIDNummer(CString GUID, CString &zeile)
{
	if (zeilen.GetSize() == 0)
		return false;

	CString GUIDLocal;
		    
	for (int i = 0; i < zeilen.GetSize(); i++)
	{	
		getText(zeilen.GetAt(i), POS_ENDE - POS_GUID, POS_GUID, GUIDLocal, true);
			
		if(GUIDLocal.Compare(GUID) == 0)
		{
			zeile = zeilen.GetAt(i);
			return true;
		}
	}
	
	return false;
}




bool GpSapWittenbergeAuftragsdaten::findEintragReadAllGUID(CStringArray& ResultZeilen)
{
	if (zeilen.GetSize() == 0)
		return false;

	CString stGUID;
		   
	for (int i = 0; i < zeilen.GetSize(); i++)
	{	
		//getText(zeilen.GetAt(i), POS_SD_POSINR - POS_SD_AUFTRNR, POS_SD_AUFTRNR, stAuftrag, true);
		getText(zeilen.GetAt(i), POS_ENDE - POS_GUID, POS_GUID, stGUID, true);
		//getText(zeilen.GetAt(i), POS_LIEFDAT - POS_SD_POSINR, POS_SD_POSINR, stPos, true);
	
		//if ((__int64)_atoi64(stAuftrag) == sdNummer && atoi(stPos) == sdPos)
		{
			ResultZeilen.Add(stGUID);
			//return true;
		}
	}
	
	return true;
}



bool GpSapWittenbergeAuftragsdaten::getText(const CString &text, int offset, int pos, CString &result, bool trim)
{
	result = text.Mid(pos, offset);

	if (trim)
	{
		result.TrimLeft();
		result.TrimRight();
	}

	return true;
}
