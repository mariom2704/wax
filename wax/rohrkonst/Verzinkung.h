#ifndef __VERZINKUNG
#define __VERZINKUNG

#if 0

#include "..\waxstamm\verzinkung.h"

class Rohrleitung;
class Element;

///Klasse fuers Verzinken
class DLLINTERFACE Verzinkung
{
	friend class Druckliste;
public:
    Verzinkung();

	///Konstruktor
	Verzinkung(Rohrleitung* rohrleitung);
	
	///Destruktor
	virtual ~Verzinkung();

	/// Addiert eine Rohrleitung zu den Verzinkungspreisen
	void add_rohr(Rohrleitung* rohrltg);

	///Ermittelt die Gesamtzeit fuer die Elemente
	double getElementZeit();

	///Ermittelt die Putzzeit fuer den Innenbereich
	double getPutzzeitInnen(); 

	///Ermittelt die Putzzeit fuer den Aussenbereich
	double getPutzzeitAussen(); 

	///Liefert die Zeit fuers Putzen aus dem Arbeitsplan
	double getZeit(); 

	///Ermittelt die Gesamtzeit fuers Putzen 
	double getPutzzeit(); 

	///Liefert den Gesamtpreis
	double getPreis(); 

	///Liefert Preis nach Gewicht
	double getPreisGewicht();

	///Liefert Preis für Zinkzuschlag
	double getPreisZinkzuschlag();

	///Liefert Preis fuers Putzen
	double getPreisPutzen();

	///Liefert Preis für Energiezuschlag
	double getPreisEnergiezuschlag();

	///Liefert Preis für Transportversicherung
	double getPreisTransportversicherung();

	double get_zeitPutzen() const;

	double get_preis() const;

private:

	double get_preisPutzen() const;

	double get_preisVerzinkung() const;

	double get_preisZinkzuschlag() const;

	double get_preisEnergiezuschlag() const;

	double get_preisTransportversicherung() const;

	double get_zeitElemente() const;

	double get_zeitStdElemente() const;

	double get_zeitGewElemente() const;

	double get_zeitInnen() const;

	double get_zeitAussen() const;

	void set_rohr(Rohrleitung* value);
private:

		// Template zum sortieren der Map
	template<class Type>
	struct eleless : public binary_function <Type, Type, bool> 
	{
		bool operator()	(
			const Type& _Left, 
			const Type& _Right ) const
		{
				
			if (_Left->get_id() == _Right->get_id())
				return _Left->get_dn() < _Right->get_dn();
			return _Left->get_id() < _Right->get_id();
		} 
	};	

	///Elementliste als map, Liste mit initialisierten Objekten
	//typedef multimap<Element*, double > ElementMap;
	typedef multimap<Element*, double, eleless <Element*> > ElementMap;

	///Addiert die Verzinkungen
	void merge(const Verzinkung& other);

	/// Holt die Stammdaten aus der Verzinkerei Tabelle und setzt stmdat
	bool init_stmdat(int nr);

	///Loescht die Objekte und die Pointer aus der Map
	void clear_map(ElementMap& map);

	///Addiert Listen mit zu putzenden Elementen
	void mergeMaps(const Verzinkung& other);

	///Addiert Listen mit zu putzenden Elementen
	void mergeMap(ElementMap& map, ElementMap m, int liste);

	///Ermittelt die Elemente zum Putzen
	void insertListe();

	///Fuegt die Elemente in die Map ein
	void insertEle(const Element& ele, double zeit);

	///Fuegt die Elemente in die Map ein
	void insert(ElementMap& map, const Element& ele, double zeit, int liste);

	///Liste mit zu putzenden Elementen unterschiedlich nach id und dn
	ElementMap emap;

	///Liste mit zu putzenden Standardelementen unterschiedlich nach anschlussdn
	ElementMap smap;
	
	///Liste mit zu putzenden Gewinden unterschiedlich nach dn
	ElementMap gmap;

	///Gesamtzeit Putzen
	double zeitPutzen;

	///Putzzeit Elemente
	double zeitElemente;

	///Putzzeit Standardelemente
	double zeitStdElemente;

	///Putzzeit Aussengewinde
	double zeitGewElemente;

	///Putzzeit Innenbereich 
	double zeitInnen;

	///Putzzeit Aussenbereich
	double zeitAussen;

	///Preis Verzinkung
	double preisVerzinkung;

	///Preis Zinkzuschlag
	double preisZinkzuschlag;

	///Preis Putzen
	double preisPutzen;

	///Preis Energiezuschlag
	double preisEnergiezuschlag;

	///Preis Transport
	double preisTransportversicherung;

	///Gesamtpreis
	double preis;

	///Pointer auf Rohrleitung
	Rohrleitung* rohr;

	/// Referenz auf die Stammdaten der Verzinkerei
	Verzinkerei::data *stmdat;

};

#endif

#endif 

