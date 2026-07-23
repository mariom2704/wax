#pragma once

class ConstructElement;

#include <vector>
#include "Globj.h"

#include "../rohrkonst/cadwerkauftrag.h"
#include "SVGFile.h"

#include "WaxGrafik.h"

/**
Klasse ConstructGraphik
Basisklasse fuer Graphikkonstruktionsklassen 
Generieren von Graphikelementen durch Auswertung der Rohrkonstruktionsdaten 
Rohrleitung in Listen von Graphikelementen
*/
class ConstructGraphik 
{
public:
	/// Beschreibt den Konstruktionstyp des Elements
	enum TYP 
	{
		IMROHR = 0,
		
		ABG = 1,
		
		ABGABG = 2
	};

	/// Konstruktor
	ConstructGraphik( Rohrleitung* rohr, enum WaxGrafik::GRAFIKTYP tp, const string& graficfile = "");

	/// Destruktor
	virtual ~ConstructGraphik();

	/// Einstiegsfunktion zur Graphik
	virtual void createGraphik();
	
	/// Setzen der Grahpikfaktoren je nach Masse der Konstruktion 
	virtual void setGraphikfaktoren();

	/// Die Elementliste wird abgearbeitet
	virtual void runElementList();

	void handle_rohr(Element* ele);
	
	/// Aufruf der Spezialfunktionen zum Setzen des neuen Startpunktes hinter einem Element
	virtual void setNewPoint();

	/// Fasst alle Grafikobjekte in einer Liste zusammen.
	std::vector<GlObj*> getObjectList();
	
	/// set avnachbearbeitung
	const bool get_avnachbearbeitung() const;

	/// set avnachbearbeitung
	void set_avnachbearbeitung(bool value);

	std::string get_filename() const { return filename; }
	
	string get_fileextension() const { return WaxGrafik::Creator::get_fileextension(grafiktyp); }

	void set_grafiktyp(WaxGrafik::GRAFIKTYP typ)
	{
		grafiktyp = typ;
	}
protected:
	/// Die Graphikelemente werden serialisiert
	void serializeGraphicElements(const std::string& filename); 

	/// Initialisiert die Bemassungshöhe und Tiefe
	void initBemassung(Element* element);

	/// Die Konstruktionselemente der Abgaenge werden erzeugt
	void createAbgList(int rohrLaenge, int rohrDn, bool knickeBemassen, bool nurBemassung);
	
	/// Bemassung der Restmasse
	void doRestmassBemassen(bool erstesElement);

	/// X/Y-Masse fuer Gesamtbemassung
	void setNewXYLaenge();

	bool knickeBemassen();

	/// Erstellt ein neues ConstructElement Objekt in Abhängigkeit der Element ID
	ConstructElement* constructElementFromID( Element & ele) const;

	/// Array fuer alle Konstruktionselemente
	std::vector<ConstructElement*> cg_list;

	/// Liste mit Dateinamen 
	std::string filename;

	/// FaktorRohrlaenge, durch diesen Wert wird die Elelaenge geteilt.
	double faktorElelaenge;
	
	/// FaktorRohrlaenge, durch diesen Wert wird die Elebreite geteilt.
	double faktorElebreite;
	
	/// FaktorRohrlaenge, durch diesen Wert wird die Rohrlaenge geteilt.
	double faktorRohrlaenge;
	
	/// FaktorRohrbreite, durch diesen Wert wird die Rohrbreite geteilt.
	double faktorRohrbreite;

	/// Merker-Element auf aktuelles Element
	Element elemerk;

	/// Parameter ob Nennweite in Graphik in DN oder Zoll angezeigt werden soll
	bool dn2zoll;
	
	/// Rohrkonstruktion
	Rohrleitung* rohrleitung;

	/// Merker-Element auf vorheriges Element
	Element prevelemerk;

	/// Merker-Element auf naechstes Element
	Element nextelemerk;

/// X-Startkoordinate f. Zeichnung eines Rohrstueckes u. dessen Elemente
	double X0;
	
	/// Y-Startkoordinate f. Zeichnung eines Rohrstueckes u. dessen Elemente
	double Y0;
	
	/// rohrcounter, zaehlt die Rohrstuecke in der Konstruktion
	int rohrcounter; 
	
	/// Laenge des Elements am Rohranfang
	int lEndeLaenge;
	
	/// Laenge des Elements am Rohrende
	int rEndeLaenge;
	
	/// Drehfaktor zeigt an in welche Richtung gezeichnet werden muss, aendert sich mit jedem Bogen
	int drehfaktor;
	
	/// RestmassBemassen, gibt an ob das Restmass bemasst wird
	bool restmassBemassen;
	
	/// Merker-Element fuer Restmass
	Element abgelemerk;

	/// Laenge, gibt die bisher gezeichnete Summe der Rohrlaengen an
	int laenge;
	
	/// Max. Rohrlaenge in Drehrichtung 0
	int maxlaenge_0;
	
	/// Max. Rohrlaenge in Drehrichtung 90
	int maxlaenge_90;
	
	/// Max. Rohrlaenge in Drehrichtung 180
	int maxlaenge_180;
	
	/// Max. Rohrlaenge in Drehrichtung 270
	int maxlaenge_270;

	/// Max. Abganglaenge mit Winkel 0
	int maxlaengeAbg_0;

	/// Max. Abganglaenge mit Winkel 90
	int maxlaengeAbg_90;
	
	/// Max. Abganglaenge mit Winkel 180
	int maxlaengeAbg_180;
	
	/// Max. Abganglaenge mit Winkel 270
	int maxlaengeAbg_270;

	/// Gesamtmass der Konstruktion in X-Richtung
	int xLaenge;
	
	
	/// Max. Rohrlaenge
	int maxlaengeAbg;
	
	/// Laenge des vorherigen Bogens: Laengenabzug  fuer die Position und Bemassung der nachfolgenden Elemente
	int bogenlaenge;
	
	/// Gesamtmass der Konstruktion in Y-Richtung
	int yLaenge;

	/// Laenge in neg. X-Richtung
	int negxLaenge;
	
	/// Laenge in neg.Y-Richtung
	int negyLaenge;

	/// Anzahl der bisher gezeichneten Rohre inkl. Red/Bog
	int nRohre;
	
	/// Merker-Element
	Element merkerRohrele;
	
	/// Merker-Element
	Element merkerAbgele;

	int abstKnick;
	
private:

	
	/// Liste mit temporären Dateinamen initialisieren
	void init_filename(const string& fn);
	
	/// AV_Nachbearbeitungs-Elemente vorhanden, diese muessen extra bemasst werden
	bool avnachbearbeitung;

	/// Max. Abganglaenge nach oben
	int maxlaengeAbgOben;
	
	/// Prüft die Bemassungsobjekte auf ausreichende Abstände.
	void checkBemassung();
	
	
	/// Erstellt ein neues ConstructElement Objekt in Abhängigkeit der Gruppen ID bei GRFITTING
	ConstructElement* constructFittingFormID() const;


	/// Bemassung der der Elemente, wenn Rohrleitung mit Standard- und Nacharbeitselementen 
	void aveleBemassen();

	/// Bemassung der gesamten Konstruktion in X-Richtung
	void gesamtmasseSetzen();
	
	/// Fasst alle Bemassungsobjekte in einer Liste zusammen. Doppelte Bemassungen werden ignoriert.
	std::vector<Bemassung*> getBemassungList();

	/// Setzt neuen Startpunkt hinter Schweissnaht
	void setPointBehindSchweiss();

	/// Setzt neuen Startpunkt hinter Bogen
	void setPointBehindBogen();

	/// Setzt neuen Startpunkt hinter K-Red.
	void setPointBehindKred();

	/// Setzt neuen Startpunkt hinter E-Red.
	void setPointBehindEred();

	/// Die Konstruktionselemente der Rohrstuecke werden erzeugt
	virtual void createRohrList(int rohrLaenge, int rohrDn,	CString rohrPos, int rohrAnzahl, int einz, const bool erstesElement);

	int serialize_fertigungshinweis(GlFile* dwfFile, int x, int y);

	/// Zu erzeugendes Grafikformat
	WaxGrafik::GRAFIKTYP grafiktyp; 

	void scan_rohrteilung();

	/// Wenn das Rohrlängenfeld gefüllt ist, werden mehrere Rohre gezeichnet. Das Rohrlängenfeld soll entfallen.
	int rohrteilung;
};

