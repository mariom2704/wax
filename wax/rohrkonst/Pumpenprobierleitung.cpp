#include "stdafx.h"
#include "resource.h"

#include "Pumpenprobierleitung.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//Destruktor
Pumpenprobierleitung::~Pumpenprobierleitung()
{

}

//Konstruktor
Pumpenprobierleitung::Pumpenprobierleitung() : dn1(80), dn2(80)
{
	anschlussA = NUT;
	anschlussB = NUT;
	laenge1	= 900;
	laenge2 = 400;
	laenge3 = 140;
	stdbezeichnung = getStammdaten().getText("IDS_PUMPENPROBIER").c_str(); 
}

//Speichern und Laden der Rohrleitung. Achtung:
void Pumpenprobierleitung::serialize(BinaryArchive& ar)
{
	Werkauftrag::serialize(ar);
	if (ar.is_storing())
	{
		ar << dn2;
		ar << dn1;
		ar << anschlussA;
		ar << anschlussB;
		ar << laenge1;
		ar << laenge2;
		ar << laenge3;
	}
	else
		{
		ar >> dn2;
		ar >> dn1;
		ar >> anschlussA;
		ar >> anschlussB;
		ar >> laenge1;
		ar >> laenge2;
		ar >> laenge3;
	}
}

//Rohrkonstruktion erzeugen
void Pumpenprobierleitung::createModell()
{
	// Löschen aller Elemente aus der Liste
	set_dnvl(dn1);

	fill_elementArray();
	set_laenge(laenge1 + laenge2 + laenge3);

	set_rohrAnfang(anschlussA );
	set_rohrEnde(anschlussB);

	CString laengenfeld;
	laengenfeld.Format("%i %i %i", laenge1, laenge2, laenge3);
	set_rohrDefLaengen(laengenfeld);
	setrohrDefLaengen();
	setmaxLaenge();
	set_rohrVerb(FFL);
	
	setBemerkung();

	Verteiler::createModell();
}

//Füllen des Elementarrays bei vordefinierten Rohrleitungen, z.B. 
//Stationsverteiler oder Entnahmeleitungen
void Pumpenprobierleitung::fill_elementArray()
{
	elementArray.deleteAll();
	const int abstMuffe1 = 55;
	bool reduziert = dn1 != dn2;	

	int abstred = laenge1 + atoi(get_minval(L5));
	int abstandMuffe1 = laenge1 + laenge2 - abstMuffe1;
	int abstandMuffe2 = laenge1 + laenge2 + atoi(get_minval(L4));

	try
	{
		if (reduziert)
		{
			Element reduzierung;
			reduzierung.initStammdaten(dn2, dn1, KRED);
			reduzierung.set_laenge((int)reduzierung.get_mass1());
			reduzierung.set_abstAbs(laenge1 + atoi(get_minval(L5)));
			reduzierung.set_abstRel(laenge1 + atoi(get_minval(L5)));
			elementArray.Add(new Element(reduzierung));
		}

		Element muffe;
		muffe.initStammdaten(15, reduziert ? dn2 : dn1, MUFFE);
		muffe.set_laenge((int)muffe.get_mass1());
		muffe.set_winkel(0);

		muffe.set_abstAbs(abstandMuffe1);
		muffe.set_abstRel(reduziert ? abstandMuffe1-abstred : abstandMuffe1);
		elementArray.Add(new Element(muffe));

		muffe.set_abstRel(abstandMuffe2-abstandMuffe1);
		muffe.set_abstAbs(abstandMuffe2);
		elementArray.Add(new Element(muffe));
	}
	catch (Element::Error e)
	{
	}
}

//Hier wird automatisch der Arbeitsplan generiert
void Pumpenprobierleitung::createAutoArbeit(bool beschichtung)
{
	UNREFERENCED_PARAMETER(beschichtung);
	Verteiler::createAutoArbeit();
}

//Rohrkonstruktion auf korrekte Werte checken
CString Pumpenprobierleitung::checkPlausis()
{
	Werkauftrag::checkPlausis();
	CString err, str;

	int lx[] = { laenge1, laenge2, laenge3 };
	CString minlx[] = { get_minval(L1, dn1, dn2), get_minval(L2, dn1, dn2), get_minval(L3, dn1, dn2) };

	int i = -1;
	CString minl1 = get_minval(L1, dn1, dn2);
	if (!atoi(minl1))
	{
		err = getStammdaten().getText("IDS_ERROR65").c_str(); 
		return err;
	}
	else
	{
		// Minimallängen abprüfen
		for (i = 0; i < 3; i++)
		{
			if (lx[i] < atoi(minlx[i]))
			{
				str = getStammdaten().getText("IDS_ERROR66").c_str(); 
				err.Format(str, minlx[i]);
				return err;
			}
		}
	}

	return "";
}

//Prüft die DN1/DN2 Kombination und liefert den zugehörigen Minimalwert für 
//L4/L5/d oder Lges. valid ist ein Wert aus MINWERTE.
//
//Wird für dn1, dn2 Null übergeben, werden die Objektdaten benutzt.
CString Pumpenprobierleitung::get_minval(int valid, int dn1, int dn2) const
{
	CString retstr("--");
	
	XASSERT(valid+2 < 9 && valid >= 0);
	if (valid < 0 || valid+2 >= 9)
		return retstr;

	if (!dn1)
		dn1 = get_dn1();
	if (!dn2)
		dn2 = get_dn2();

	//Entspricht der Liste in Sondermz Pumpenprobierleitung
	const struct 
	{
		int dna;
		int dnb;
		int l1;
		int l2;
		int l3;
		int l4;
		int l5;
		int dvorkomma;
		int dnachkomma;
		int messblSchieber; 
	} komb[] = {
		80,  80,  825, 400, 140, 85,   0,	245, 5, 5, 
		100, 80, 1025, 690, 140, 85, 490,	245, 6, 5, 
		100,100, 1025, 500, 130, 75,   0,	255, 6, 5, 
		150,100, 1525, 980, 130, 75, 740,	255, 9, 0, 
		150,150, 1525, 750, 110, 55,   0,	275, 9, 0, 
		200,150, 2030,1245, 110, 55, 995,	275, 15,0, 
		0,0,0,0,0,0,0,0,0,
	};

	for (int i = 0; komb[i].dna; i++)
	{
		
		if (dn1 == komb[i].dna && dn2 == komb[i].dnb)
		{
			const int *ret = &komb[i].dna;
			int minval = *(ret + valid+2);
			if (minval)
			{
				if (valid != D)
					retstr.Format("%i", minval);
				// Bei D muss der Doublewert ausgegeben werden
				if (valid == D)
				{
					int minvalnachkomma = *(ret + valid+3);
					retstr.Format("%i,%i", minval, minvalnachkomma);
				}
				else if (valid == LGES)
				{
					int tmp = komb[i].l1 + komb[i].l2 + komb[i].l3 + komb[i].messblSchieber;
					retstr.Format("%i", tmp);
				}
				else
					retstr.Format("%i", minval);
			}
		}
	}
	return retstr;
}

//Setzt das Bemerkungsfeld auf den Standardtext. Erweitert wird der Text um 
//den Hinweis auf die Bohrungsgrösse für die Muffen.
void Pumpenprobierleitung::setBemerkung()
{
	CString str; 
	str = getStammdaten().getText("IDS_STDTEXT_PUMPENPROBIER1").c_str(); 
	str += getStammdaten().getText("IDS_STDTEXT_PUMPENPROBIER2").c_str(); 
	const CString bemerk = str;
	CString dnAnmerkung;
	str = getStammdaten().getText("IDS_BOHR_MUFFE").c_str(); 
	const CString BOHRUNG = str;
	dnAnmerkung.Format("%s %s mm\r\n", BOHRUNG, get_minval(D));

	if (get_stdbemerkung().Find(BOHRUNG, 0) >= 0)
		set_stdbemerkung(dnAnmerkung + bemerk);
	else
		set_stdbemerkung(bemerk);
}

const int Pumpenprobierleitung::get_anschlussA() const
{
	return anschlussA;
}

void Pumpenprobierleitung::set_anschlussA(int value)
{
	anschlussA = value;
}

const int Pumpenprobierleitung::get_anschlussB() const
{
	return anschlussB;
}

void Pumpenprobierleitung::set_anschlussB(int value)
{
	anschlussB = value;
}

const int Pumpenprobierleitung::get_laenge2() const
{
	return laenge2;
}

void Pumpenprobierleitung::set_laenge2(int value)
{
	laenge2 = value;
}

const int Pumpenprobierleitung::get_laenge3() const
{
	return laenge3;
}

void Pumpenprobierleitung::set_laenge3(int value)
{
	laenge3 = value;
}

const int Pumpenprobierleitung::get_laenge1() const
{
	return laenge1;
}

void Pumpenprobierleitung::set_laenge1(int value)
{
	laenge1 = value;
}

const int Pumpenprobierleitung::get_dn1() const
{
	return dn1;
}

void Pumpenprobierleitung::set_dn1(int value)
{
	dn1 = value;
}

const int Pumpenprobierleitung::get_dn2() const
{
	return dn2;
}

void Pumpenprobierleitung::set_dn2(int value)
{
	dn2 = value;
}

void Pumpenprobierleitung::check_absAbst_rohrlaenge(const Element * ele) 
{
	/**
	absichtlich leer, damit der Abstand der Bohrung nicht angemeckert wird
	*/
	UNREFERENCED_PARAMETER(ele);
}

///Pruefen ob Element bei Rohrtrennstelle sitzt und korrigieren
CString Pumpenprobierleitung::checkRohrtrennstelle()
{
	// hier nicht, Elemente zu dicht zusammen ok, Sonderkonstruktion  
	return "";
}


