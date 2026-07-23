#ifndef __WAXUSER_MESSAGES__
#define __WAXUSER_MESSAGES__

#define WAXID_FIRST WM_USER+110
#define WAXID_LAST WM_USER+141
#define WAXIDS_FIRST WAXID_LAST+1

/* 
Achtung: Diese Datei soll NUR editiert werden, wenn IDs modulübergreifend benutzt werden MÜSSEN. 

Alle Commandmessages zwischen WAXID_FIRST und WAXID_LAST werden an die Treeview LeftView weiter
gereicht. Hier wird dann die entsprechende Funktion ausgeführt. 

Das Kontextmenü im Baum wird durch das WaxPart definiert. Dies ist nötig, um z.B. für den Sammelauftrag spezielle
Menüpunkte abblenden/einbauen zu können, die aber von der Exploreransicht gehandelt werden müssen. 

In CTabCtrlView::PreTranslateMessage werden alle Messages an das WaxPart weitergegeben und dann werden alle
Messages zwischen WAXID_FIRST und WAXID_LAST an die Explorer View weitergereicht (ganz mfcmässig über die MessageMap). 
Dieser Mechanismus ist ebenfalls in WaxHtmlView::HandleMsgToLeftView eingebaut. 
*/

//View updaten
#define WM_UPDATEVIEW WM_USER+110

//View updaten
#define WAXID_NEW WM_USER+111

//Position in Zwischenablage einfügen
#define WAXID_COPY WM_USER+112

//Position aus Zwischenablage einfügen
#define WAXID_PASTE WM_USER+113

//Position in Datei einfügen
#define WAXID_COPY_TOFILE  WM_USER+114

//Position aus Datei einfügen
#define WAXID_PASTE_FROMFILE WM_USER+115

//Position löschen
#define WAXID_DELETE WM_USER+116

//Position ausschneiden
#define WAXID_CUT WM_USER+117

//Baumanzeige updaten
#define WAXID_DOTREEUPDATE WM_USER+118

//Leitkartendaten übernehmen
#define WAXID_LEITKARTENDATEN_UEBERNEHMEN WM_USER+119

//Projektleitkarte zuweisen
#define WAXID_PRJLEITKARTE WM_USER+120

//Neue Untergliederung im Hauptzweig einfügen
#define WAXID_NEUEUNTERGLIEDERUNG_ROOT WM_USER+121

//Neue Untergliederung einfügen
#define WAXID_NEUEUNTERGLIEDERUNG WM_USER+122

//Neuen Sammelauftrag einfügen
#define WAXID_NEUER_SAMMELAUFTRAG WM_USER+123

//Neuen Status updaten
#define WAXID_UPDATESTATUS WM_USER+124

//WaxPart umbenennen
#define WAXID_UMBENENNEN WM_USER+125

// Segment Rohr

#define WAXID_NEU_STRANGROHR				WM_USER+127
#define WAXID_NEU_VERTEILER					WM_USER+128

// Segment Gewinderohr
#define WAXID_NEU_GEWINDEROHR				WM_USER+129

// Sondermz und Apax
#define WAXID_NEUE_ENTNAHMELEITUNG			WM_USER+130
#define WAXID_NEUE_PUMPENPROBIERLEITUNG		WM_USER+131
#define WAXID_NEUER_PUMPENVERTEILER			WM_USER+132
#define WAXID_NEU_STATIONSVERTEILER			WM_USER+133
#define WAXID_NEU_FORMSTUECKEUNDFITTINGE	WM_USER+134
#define WAXID_NEU_HANDELSLAENGE				WM_USER+135
#define WAXID_NEU_APAX						WM_USER+136
#define WAXID_NEU_CO2VERTEILER				WM_USER+137
#define WAXID_NEUE_PREISANFRAGE				WM_USER+138
#define WAXID_NEU_MX1230MX200VERTEILER		WM_USER+139
#define WAXID_NEU_SAMMEL_AUFTRAEGE_VIA_LIST	WM_USER+140
// Wax+/Spanx Schnittstelle
#define ID_SPANXANFRAGE_STRANGROHR (WM_USER+141)
#define ID_SPANXANFRAGE_VERTEILER (WM_USER+142)

// Diese Definitionen müssen nicht hier erstellt werden
#define WAXID_UPDATESTATISTIK (WM_USER+143)
#define WAXID_UPDATEKCNUMMER (WM_USER+144)
#define WAXID_UPDATEAFOS (WM_USER+145)




#define WM_NAVIGATE_JSCRIPT  	(WM_APP + 1)
#define WM_NAVIGATE_ANHANG  	(WM_APP + 2)
#define WM_LBUTTONDOWN_POSTED	(WM_APP + 3)
#define WM_GRIDENDEDIT_POSTED   (WM_APP + 4)
#define WM_GRID_CLICKED			(WM_APP + 5)






#define IDS_LEER                        WAXIDS_FIRST+1
#define IDS_FLANSCH						WAXIDS_FIRST+2
#define IDS_BESCHICHTUNG                WAXIDS_FIRST+3
#define IDS_ANFANG				        WAXIDS_FIRST+4
#define IDS_ENDE		                WAXIDS_FIRST+5
#define IDS_LAENGE                      WAXIDS_FIRST+6
#define IDS_DN                          WAXIDS_FIRST+7
#define IDS_ARTNR                       WAXIDS_FIRST+8
#define IDS_PREIS                       WAXIDS_FIRST+9
#define IDS_GEWICHT                     WAXIDS_FIRST+10
#define IDS_BESCHREIBUNG                WAXIDS_FIRST+11
#define IDS_WINKEL                      WAXIDS_FIRST+12
#define IDS_POSNR_ABK                   WAXIDS_FIRST+14
#define IDS_NENNWEITE                   WAXIDS_FIRST+15
#define IDS_VERSAND                     WAXIDS_FIRST+16
#define IDS_SCHWEISSEREI				WAXIDS_FIRST+17
#define IDS_PULVERANLAGE				WAXIDS_FIRST+18
#define IDS_KOPF						WAXIDS_FIRST+19
#define IDS_LFDNR                       WAXIDS_FIRST+20
#define IDS_TEXT						WAXIDS_FIRST+21


// IDs der Rohrqualis nicht aendern!
#define IDS_2440STDVZIN					WAXIDS_FIRST+46
#define IDS_2440STD						WAXIDS_FIRST+47
#define IDS_2458STD						WAXIDS_FIRST+48
#define IDS_ROHRQUALITAETEN             WAXIDS_FIRST+49
#define IDS_RQSONSTIGES					WAXIDS_FIRST+30
#define IDS_RQ2440_2458GSCH             WAXIDS_FIRST+31
#define IDS_RQ2440NLOS                  WAXIDS_FIRST+32
#define IDS_RQ2448NLOS                  WAXIDS_FIRST+34
#define IDS_RQ2440VZIN                  WAXIDS_FIRST+37
#define IDS_RQ2458VZIN                  WAXIDS_FIRST+38
#define IDS_RQ2458GSCH                  WAXIDS_FIRST+51
#define IDS_RQ2440GSCH                  WAXIDS_FIRST+52
#define IDS_RQEN10217			        WAXIDS_FIRST+56
#define IDS_RQEN10217_CO2				WAXIDS_FIRST+60
#define IDS_RQEN10217VERZ				WAXIDS_FIRST+61

#define IDS_UNBEKANNT					WAXIDS_FIRST+59

#define IDS_ROHRST                      WAXIDS_FIRST+61
#define IDS_NUTST                       WAXIDS_FIRST+62
#define IDS_SATST                       WAXIDS_FIRST+63
#define IDS_GWRST                       WAXIDS_FIRST+64
#define IDS_MUFFE                       WAXIDS_FIRST+65
//#define IDS_RFMUFFE                     WAXIDS_FIRST+66
//#define IDS_RKMUFFE                     WAXIDS_FIRST+67
#define IDS_FFLAN                       WAXIDS_FIRST+68
#define IDS_BLFLAN                      WAXIDS_FIRST+69
#define IDS_VFLAN                       WAXIDS_FIRST+70
#define IDS_VFL16                       WAXIDS_FIRST+71
#define IDS_AFLAN                       WAXIDS_FIRST+72
#define IDS_SCHWEISS                    WAXIDS_FIRST+73
#define IDS_SFLAN                       WAXIDS_FIRST+74
#define IDS_SCHEIB                      WAXIDS_FIRST+75
#define IDS_BODEN                       WAXIDS_FIRST+76
#define IDS_SPANS                       WAXIDS_FIRST+77
#define IDS_KBOD40                      WAXIDS_FIRST+78
#define IDS_KBOD50                      WAXIDS_FIRST+79
#define IDS_NUT                         WAXIDS_FIRST+80
#define IDS_GEWIND                      WAXIDS_FIRST+81
#define IDS_SNAHT                       WAXIDS_FIRST+82
#define IDS_GLATT                       WAXIDS_FIRST+83
#define IDS_BOHR24                      WAXIDS_FIRST+84
#define IDS_EINZ                        WAXIDS_FIRST+85
#define IDS_KRED                        WAXIDS_FIRST+86
#define IDS_ERED                        WAXIDS_FIRST+87
#define IDS_BOGEN                       WAXIDS_FIRST+88
#define IDS_BOG2S                       WAXIDS_FIRST+89

#define IDS_BOHR50                      WAXIDS_FIRST+95

#define IDS_ROHRNUT						WAXIDS_FIRST+105
#define IDS_DURCHMESSER                 WAXIDS_FIRST+110

#define IDS_GAS							WAXIDS_FIRST+135 

#define IDS_LOCTITE						WAXIDS_FIRST+141
#define IDS_HANF						WAXIDS_FIRST+142
#define IDS_TEFLON						WAXIDS_FIRST+143
#define IDS_SYNTHESOL					WAXIDS_FIRST+144
#define IDS_KEINE						WAXIDS_FIRST+145 

#define IDS_KAPPENFARBEN                WAXIDS_FIRST+150
#define IDS_KEINE_KAPPENFARBE			WAXIDS_FIRST+151

#define IDS_ROT							WAXIDS_FIRST+152
#define IDS_BLAU                        WAXIDS_FIRST+153
#define IDS_GELB                        WAXIDS_FIRST+154
#define IDS_GRUEN                       WAXIDS_FIRST+155
#define IDS_WEISS                       WAXIDS_FIRST+156
#define IDS_SCHWARZ                     WAXIDS_FIRST+157
#define IDS_GRAU                        WAXIDS_FIRST+158

#define IDC_ELEMENTE_ABSTABS            WAXIDS_FIRST+201
#define IDC_ELEMENTE_ID                 WAXIDS_FIRST+202
#define IDC_ELEMENTE_DURCHMESSER        WAXIDS_FIRST+203
#define IDC_ELEMENTE_ABSTREL            WAXIDS_FIRST+204
#define IDC_ELEMENTE_ANSDN              WAXIDS_FIRST+205
#define IDC_ELEMENTE_LAENGE             WAXIDS_FIRST+206
#define IDC_ELEMENTE_ELEDN              WAXIDS_FIRST+207
#define IDC_ELEMENTE_WINKEL             WAXIDS_FIRST+208

#define IDS_ELEM_CHANGE                 WAXIDS_FIRST+221
#define IDS_ELEM_COPY                   WAXIDS_FIRST+222
#define IDS_ELEM_CUT                    WAXIDS_FIRST+223
#define IDS_ELEM_DELETE                 WAXIDS_FIRST+224
#define IDS_ELEM_NEW                    WAXIDS_FIRST+225
#define IDS_ELEM_PASTE                  WAXIDS_FIRST+226

#define IDC_LAENGE_DURCHMESSER          WAXIDS_FIRST+231

#define IDS_WASSER                      WAXIDS_FIRST+218
#define IDS_ANZAHL                      WAXIDS_FIRST+219
#define IDS_PULVERPREIS                 WAXIDS_FIRST+220

#define WAX_BACKSLASH_HTML						_T("wax\\HTML")


#define WAX_VIEWER_FOR_HTML_DWF_AND_SVG_xHTML	_T("WaxViewerForHTMLDWFandSVG.xhtml")
#define WAX_VIEWER_FOR_HTML_DWF_AND_SVG_HTML	_T("WaxViewerForHTMLDWFandSVG.html")

#ifdef __WAX_FUER_DRITTE__
#define WAX_VIEWER_FOR_HTML_DWF_AND_SVG_xHTML	_T("WaxViewerForHTMLDWFandSVG.xhtml")
#endif

#define WAX_BLANK_JPG                           _T("Blank.jpg")

#define WAX_LOADER_FOR_IMAGES_DWFSVG_CSS		_T("LoaderForImagesDwfSvg.css")
#define WAX_LOADER_FOR_IMAGES_DWFSVG_JS			_T("LoaderForImagesDwfSvg.js")

#endif
