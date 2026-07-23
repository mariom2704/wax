#include "stdafx.h"


#include "druckliste.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace WaxDruck;

Druckliste::Druckliste(HtmlDruck& file, WaxPart* pt) : waxpart(pt), html(file)
{
	dn2zoll = (WaxSys::para_int(GF_DN2ZOLL) == 0) ? false : true;
	init_auftragsliste();
	maxlaenge = scan_maxlaenge();
	if (pt)
		twoquali = pt->check2rohrquali();
}


Druckliste::Druckliste(HtmlDruck& file, CObList* ptWerkaufList)	: html(file)
{
	dn2zoll = (WaxSys::para_int(GF_DN2ZOLL) == 0) ? false : true;
	
	maxlaenge = scan_maxlaenge();
	

	auftraege.clear();
	
	
	for (POSITION pos = ptWerkaufList->GetHeadPosition(); pos != NULL;)
	{
		Werkauftrag* werkauftrag = dynamic_cast<Werkauftrag*>(ptWerkaufList->GetNext(pos));
		
		if ( dynamic_cast<SammelWerkauftrag*>(werkauftrag))
		{
			werkauftrag->createModell();
			auftraege.push_back(werkauftrag);
			break;
		}
	}

	for (POSITION pos = ptWerkaufList->GetHeadPosition(); pos != NULL;)
	{
		Werkauftrag* werkauftrag = dynamic_cast<Werkauftrag*>(ptWerkaufList->GetNext(pos));

		if(dynamic_cast<SammelWerkauftrag*>(werkauftrag) != NULL)
		{
			continue;
		}

				
		if (!werkauftrag->isEmpty() && dynamic_cast<Werkauftrag*>(werkauftrag))
		{
			werkauftrag->createModell();
			auftraege.push_back(werkauftrag);
		}
	}

	waxpart = NULL;
}





Druckliste::~Druckliste()
{
}

void Druckliste::init_auftragsliste()
{
	auftraege.clear();
	if (!waxpart)
		return;

	//Createmodell aufrufen, damit Stückliste, etc. erzeugt werden
	waxpart->get_werkauftrag()->createModell();
	auftraege.push_back(waxpart->get_werkauftrag());

	for (POSITION pos = waxpart->get_subobjects().GetHeadPosition(); pos != NULL; )
	{
		WaxPart* next = dynamic_cast<WaxPart*>(waxpart->get_subobjects().GetNext(pos));
		if (next && next->get_werkauftrag())
		{
			if (next->get_deleted())
				continue;

			next->get_werkauftrag()->createModell();

			auftraege.push_back(next->get_werkauftrag());
		}
	}
}

int Druckliste::scan_maxlaenge() const
{
	int maxlen = 0;
	for (Auftragsliste::const_iterator it = auftraege.begin(); it != auftraege.end(); it++)
	{
		if ((*it)->get_maxlaenge() > maxlen)
			maxlen = (*it)->get_maxlaenge();
	}
	return maxlen;
}

//Sortiert Liste mit Rohrelementen+Anfang&Ende auf- od. absteigend nach Laenge
void Druckliste::sort_liste(CObList &liste, bool aufsteigend)
{
	POSITION pos, nextpos, posa, nextposa, pose, nextpose; 
   
	// Erst Anfang&Ende auf gleiche Laenge setzen wie Rohr  
	for (POSITION p = liste.GetHeadPosition(); p != NULL;)	
	{
		Element *element, *elementAnfang, *elementEnde;

		element = dynamic_cast<Element*>(liste.GetNext(p));
		elementAnfang = dynamic_cast<Element*>(liste.GetNext(p));
		elementEnde = dynamic_cast<Element*>(liste.GetNext(p));

		elementAnfang->set_laenge(element->get_laenge());
		elementEnde->set_laenge(element->get_laenge());
	}	
	// Dann Rohre nach Laenge sortieren
	for (int i=0; i < liste.GetCount()-5; i++)	
	{
		pos = liste.FindIndex(i);
		Element *first = dynamic_cast<Element *>(liste.GetAt(pos));
		posa = liste.FindIndex(i+1);
		Element *firsta = dynamic_cast<Element *>(liste.GetAt(posa));
		pose = liste.FindIndex(i+2);
		Element *firste = dynamic_cast<Element *>(liste.GetAt(pose));

		nextpos = liste.FindIndex(i+3);
		Element *second = dynamic_cast<Element *>(liste.GetAt(nextpos));
		nextposa = liste.FindIndex(i+4);
		Element *seconda = dynamic_cast<Element *>(liste.GetAt(nextposa));
		nextpose = liste.FindIndex(i+5);
		Element *seconde = dynamic_cast<Element *>(liste.GetAt(nextpose));

		i++;i++;

		XASSERT(first && second);
		if (!first || !second)
			return;

		if (aufsteigend && (first->get_dn() > second->get_dn() || (first->get_dn() == second->get_dn() && first->get_laenge() > second->get_laenge()))
		|| !aufsteigend && (first->get_dn() < second->get_dn() || (first->get_dn() == second->get_dn() && first->get_laenge() < second->get_laenge())))
		{	
			// Elemente werden vertauscht
			liste.SetAt(nextpos, first);
			liste.SetAt(pos, second);
			liste.SetAt(nextposa, firsta);
			liste.SetAt(posa, seconda);
			liste.SetAt(nextpose, firste);
			liste.SetAt(pose, seconde);
			i = -1; // Wieder von vorn beginnen
		}
	}
}

/**
stutzenAnlage2440 muß leider noch mitgeschleppt werden
@todo: andere Lösung, zu spezifsch auf die Zuschnittslisten ausgelegt.
*/
void Druckliste::insert_rohr_quali(const Rohrleitung* rohr, int stutzenAnlage2440)
{
	if (!rohr)
		return;
	
	int anlagentyp = rohr->get_typ();
	if (anlagentyp == Rohrleitung::LEERTYP)
	{
		if (AfxMessageBox("Soll der Anlagentyp auf Wasser gesetzt werden?", MB_YESNO | MB_ICONQUESTION) == IDYES)
			anlagentyp = Rohrleitung::WASSER;
	}
	html.insertQuali(rohr->get_rohrQuali(), twoquali, rohr->get_dnvl(), stutzenAnlage2440, anlagentyp );	
}



//Laden einer Resource
bool Druckliste::loadResource(CString logo, HRSRC rclog, HMODULE module)
{
	char bmpheader[] = { 0x42, 0x4d, 0x36, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x00, 0x00, 0x00 };
	if (rclog)
	{
	
		DWORD size = ::SizeofResource(module, rclog);
		HGLOBAL hlog = ::LoadResource(module, rclog);
		char* bmp = (char*)::LockResource(hlog);
		if (bmp)
		{
			::DeleteFile(logo);
			TRY
			{
				int *len = (int*)&bmpheader[2];
				*len = (int)size + sizeof(bmpheader);
				
				CFile fbmp(logo, CFile::modeReadWrite | CFile::modeCreate);
				if (logo.Find(".bmp") != -1)
				{ // Bei Bitmaps aus den Resourcen muss noch die Kennung für Bitmap Dateien geschrieben werden
					fbmp.Write(bmpheader, sizeof(bmpheader));

				}
				fbmp.Write(bmp, size);
				fbmp.Close();
				return true;
			}
			CATCH(CFileException, e)
			{
				e->ReportError();
				return false;
			}
			END_CATCH
		}
		else
		{
			TRACE("Logoresource nicht gefunden\n");
			return false;
		}
	}
	else
	{
		TRACE("Kein Zugriff auf Resource\n");
		return false;
	}
}