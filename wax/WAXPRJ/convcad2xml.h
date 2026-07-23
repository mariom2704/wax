#pragma once

#include <xml/dom/mxxml_dom.h>

/**
Einlesen der CAD spezifischen Datenbanktabellen und erzeugen des XML Modells
*/
class ConvCad2Xml
{
public:
	ConvCad2Xml(DWORD dbhandle);
	virtual ~ConvCad2Xml();

	/// Laden aller Tabellen und erzeugen von Xml
	std::string loadxml(int waid);

private:
	/// Handle für den Zugriff auf die Serverdatenbank
	DWORD db;
	
	/// Hauptnode laden
	DOM::DOMElement* load_position(int waid, DOM::DOMElement* root) const;

	/// Erstellen der Node 'Bauteil'
	DOM::DOMElement* load_bauteil(int waid, DOM::DOMElement* base) const;

	/// Daten aus Tabelle BtPos 
	void load_btpos(int waid, DOM::DOMElement* base) const;
	
	/// Daten aus Tabelle TypRohr dazuladen	
	void load_typrohr(int waid, __int64 id, DOM::DOMElement* btpos) const;

	void load_anschluesse(int waid, __int64 id, DOM::DOMElement* btpos) const;

	void load_anbauteile(int waid, __int64 id, DOM::DOMElement* btpos) const;

	/// XML Dokument
	MxXml::DomDocument* doc;
	
	DOM::DOMElement* tab_to_xml(DOM::DOMElement* base, const MXTable& t, bool insert_tablenode = true)const ;

	void dump(int waid, const std::string& xml) const;
};


