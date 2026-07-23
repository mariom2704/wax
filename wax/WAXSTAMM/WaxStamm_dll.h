#pragma once



#ifdef _WAXSTAMMDLL 
#define WAXSTAMMDLL __declspec( dllexport )
#else
#define WAXSTAMMDLL __declspec( dllimport )
#endif

#include "..\mxparam\mxparam_dll.h"


#include "StammDat.h"
#include "Ral.h"
#include "Niederlassungen.h"
#include "Brandschutztechniken.h"
#include "Beschichtungen.h"
#include "Bauelemente.h"
#include "Spanx PreiseArtikelstamm.h"
#include "BauelementGruppen.h"
#include "FormstueckeUndFittinge.h"
#include "Arbeitsfolgen.h"
#include "AbzugRadius.h"
#include "DNzuAussendurchmesser.h"
#include "AfoZeiten.h"
#include "BauelementeAfos.h"
#include "Preisfaktoren.h"
#include "PulvernZeiten.h"
#include "AussendurchmesserWandstaerke.h"
#include "Arbeitsplaetze.h"
#include "Kostenstellen.h"
#include "DummyMaterialnummern.h"
#include "ZMass.h"
#include "Soko.h"
#include "VK3Preise.h"
#include "RohrTexte.h"
#include "Montagezeiten.h"
#include "Sondermaterial.h"
#include "Krantaetigkeit.h"
#include "ProfileArbeitsfolgen.h"
#include "ProfileSaegezeiten.h"
#include "PutzenZeiten.h"
#include "profile.h"
#include "verzinkung.h"
#include "SapArtikelstamm.h"
#include "RohrstutzenBrennzugabe.h"
#include "Texte.h"
#include "Biegung.h"
#include "zulassung.h"

#include "WaxStamm.h"


enum ArtikelstammClassId
{
	TypSonstiges = 1,
	TypBogen = 10,
	TypAnbohrschelle = 11,
	TypDlwb = 12,
	TypFlachBandstahl = 13,
	TypFlansch = 14,
	TypKupplung = 15,
	TypMuffeStutzen = 16,
	TypProfil_L = 17,
	TypProfil_U = 18,
	TypReduzierung = 19,
	TypRohr = 20,
	TypRueckschlagklappe = 21,
	TypSchieber = 22,
	TypSprinkler = 23,
	TypTStueck = 24,
	TypRedWinkel = 25,
	TypHalter = 26,
    TypKnick = 27,
    
	TypZentralenBauteil = 100,
	TypBohrung = 110,
    TypBaugruppeHalter = 120,
    TypBaugruppe = 130
};


/*
	SprinklerschellenClassId = 11,
	SprinklerClassId		 = 23,
	HalterClassId			 = 26,
	KupplungClassId			 = 15,
};
*/


enum ArtikelstammArtikelgruppenId
{
	//Muffen
	eMuffeHandschweissung = 5,	//Muffen für die Handschweißung
	eMuffeFlachStrangrohr = 6,	//Muffen, die automatisch auf derStrangrohrrohrstraße gescheißt werden 
	eMuffeGekehltStrangrohr = 7,	//gekehlte Muffen, die über die Strangrohrstraße gefertigt werden
								//Das Loch für die Muffe wird dort gebohrt und die Muffe wird an der Anlage geheftet
	eMuffeVerteiler = 39,		//Muffen, die automatisch auf der Verteilerrohrstraße gescheißt werden 
	eSondermuffe = 59,			//Sondermuffe mit variabler Länge

	eRohrnippel = 41,			//Rohrnippel (beim Pumpenverteiler)

	//Stutzen
	eStutzenHandschweissung = 2, //Stutzen für Handschweißung
	eStutzenVerteiler = 40,		//Stutzen, die automatisch auf der Verteilerrohrstraße gescheißt werden

	eGStutzen = 4,				//Stutzen für Feuerwehreinspeisung

	//Bögen
	eBogen3SSchwarz = 28,		//normaler 3S Bogen
	eBogen3SGestrahlt = 36,		//Sandgestrahlter 3S Bogen für Bauteile mit der Beschichtung Verzinkung (Bezeichnung Wax: Bogen Verzinkt)
	
	//RK Bauteile Viktaulik
    eBogenRkRotViktaulik = 103,
    eBogenRkVerzinktViktaulik = 105,
    eTStueckRkRotViktaulik = 108,
    eTStueckRkVerzinktViktaulik = 109,
    eEndkappeRkVerzinktViktaulik = 114,
    eEndkappeRkRotViktaulik = 113,

    //RK Bauteile Modgal
    eBogenRkRotModgal = 805,
    eBogenRkVerzinktModgal = 806,
    eTStueckRkRotModgal = 807,
    eTStueckRkVerzinktModgal = 808,
    eEndkappeRkVerzinktModgal = 810,
    eEndkappeRkRotModgal = 809,

    //RK Bauteile PicoFix
    eBogenRkRotPicoFix = 842,
    eBogenRkVerzinktPicoFix = 843,
    eTStueckRkRotPicoFix =844,
    eTStueckRkVerzinktPicoFix = 845,
    eEndkappeRkVerzinktPicoFix = 847,
    eEndkappeRkRotPicoFix = 846,


    //Sonstige RK Bauteile 
    eTStueckRkRotReduziert = 110,
    eReduzierungKonzentrischRk = 425,
	eFlanschRkRot = 119,
	
	//Kupplungen
	eKupplungRotViktaulik = 116,
    eKupplungVerzinktViktaulik = 117,

    eKupplungRotModgal = 811,
    eKupplungVerzinktModgal = 812,

    eKupplungRotPicoFix = 848,
    eKupplungVerzinktPicoFix = 849,

    eRedKupplungRot = 800,

	//Endböden
	eScheibe = 15,
	eBoden = 16,

	//Reduzierungen
	eReduzierungKonzentrisch = 26,
	eReduzierungExzentrisch = 27,

    //Übergangsstück
    eUebergangsstueck = 130,

	//Einziehung
	eEinziehung = 25,

	//Flansche
	eFlachflansch = 8,
	eBlindflansch = 9,
	eAnsiFlansch = 12,
	eVFlanschPn10 = 57,
	eVFlanschPn16 = 58,

	//Rohre 
	eRohrDinEn10217Geschweisst = 51,			//Standardrohr DIN EN 10217-1 geschweißt (vormals DIN 2458)
	eGewindeRohrDinEn10255Geschweisst = 52,	//DIN EN 10255 Gewinderohr geschweißt (vormals DIN 2440)
	eGewindeRohrDinEn10255Nahtlos = 32,		//DIN EN 10255 Gewinderohr nahtlos
	eGewindeRohrDinEn10255Verzinkt = 37,		//DIN EN 10255 Gewinderohr geschweißt, verzinkt (vormals DIN 2440)
	eRohrDin2448Nahtlos = 34,				//DIN 2448 nahtlos, normalwandig
	eRohrDin2458Geschweisst = 49,			//DIN 2458 standard, geschweisst, dickwandig
	eApsadRohrDinEn10217 = 56,				//DIN EN10217-1, APSAD-Rohre
	eGewinderohrKaltbandDinEn10255 = 62,		//DIN EN 10255 Gewinderohr Kaltbandgeschweißt (vormals DIN 2440)
	eGewinderohrKaltbandDinEn10255Verzinkt = 63, //DIN EN 10255 Gewinderohr Kaltbandgeschweißt verzinkt (vormals DIN 2440)

	//T-Stücke (Fitting)
	eTStueckSchwarz = 153,
	eTStueckVerzinkt = 429,
	eTStueckSchwarzReduziert = 154,
	eTStueckVerzinktReduziert = 430,

	//Winkel (Fitting)
	eWinkelSchwarz = 141,
	eWinkelVerzinkt = 426,
	eWinkelSchwarzReduziert = 143,
	eWinkelVerzinktReduziert = 427,

	//Red-Nippel
	eRednippel = 149,
    eDoppelnippel = 150,

    //sonstige Fittinge
    Stopfen = 158,
    Kappe = 159,

	//Bohrung
	eBohrungStrangrohr = 24,
	eBohrungVerteiler = 35,

	//Wax Fitting
	eWaxFitting = 44,

	//Messblende
	eMessblende = 331,

	//Bauteile Pumpenprobierleitung
	eSchieberPumpenprobierLtg = 433,
	eStutzenPumpenprobierLtg = 434,

	//Sprinklerschellen
	eSprinklerschelleVirotec = 138,
	eSprinklerschelleFig522 = 435,

    //Anbohrschellen
    eAnbohrschelleGewindeRot = 134,
    eAnbohrschelleGewindeVerzinkt = 136,

    //Anbohrschellen Viktaulik
    eAnbohrschelleNutRotViktaulik = 133,
    eAnbohrschelleNutVerzinktViktaulik = 135,

    //Anbohrschellen Modgal
    eAnbohrschelleNutRotModgal = 813,
    eAnbohrschelleNutVerzinktModgal = 814,

    //Anbohrschellen PicoFix
    eAnbohrschelleNutRotPicoFix = 850,
    eAnbohrschelleNutVerzinktPicoFix = 852,

    //Kreuzanbohrschellen Viktaulik
    eKreuzanbohrschelleNutRotViktaulik = 816,
    eKreuzanbohrschelleNutVerzinktViktaulik = 815,

    //Kreuzanbohrschellen Modgal
    eKreuzanbohrschelleNutRotModgal = 818,
    eKreuzanbohrschelleNutVerzinktModgal = 817,

	//Sprinklerschläuche
	eSchlauch = 233,

    //Steiger, Fallrohre
    eFallrohrOhneAussengewinde = 228,
    eFallrohrMitAussengewinde = 234,

    //Armaturen
    eKugelhahn = 292,
    eSpuelanschluss = 298,
    eEntleerung = 299,

	ePrallblech = 229,
    //Biegung
    eBiegung = 64,
};


WAXSTAMMDLL WaxStamm& getStammdaten();
