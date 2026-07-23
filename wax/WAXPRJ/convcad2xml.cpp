#include "stdafx.h"


#include "convcad2xml.h"

#include <mxodbc\mxodbc.h>



using namespace MxXml;

ConvCad2Xml::ConvCad2Xml(DWORD dbhandle)
{ 
	db = dbhandle;
	doc = new MxXml::DomDocument();
	doc->create("Position");
}

ConvCad2Xml::~ConvCad2Xml()
{
	delete doc;
}

/// Laden aller Tabellen und erzeugen von Xml
std::string ConvCad2Xml::loadxml(int waid)
{
	std::string ret;

	try
	{
		DOM::DOMElement* position = load_position(waid, doc->get_doc()->getDocumentElement() );
		DOM::DOMElement* bauteil = load_bauteil(waid, position);
		load_btpos(waid, bauteil);

		DOM::DOMNodeIterator* iterator = doc->get_doc()->createNodeIterator (bauteil, DOM::DOMNodeFilter::SHOW_ELEMENT, NULL, true);
  	
		for (DOM::DOMNode* current = iterator->nextNode(); current != 0; current = iterator->nextNode() )
		{
			if (current->getNodeType() == DOM::DOMNode::ELEMENT_NODE)
			{
				DOM::DOMElement* elem = (DOM::DOMElement*)current;

				std::string artnr = XStr(elem->getAttribute( X("Artikelnummer"))).to_string();
				if (artnr.length())
				{
					const Bauelemente::data* dat = getStammdaten().queryBauelemente.query<Bauelemente::data>( atoi(artnr.c_str()));

					int id = dat ? dat->get_elemid() : 0;
					elem->setAttribute(X("ElementId"), X(str( format("%1%") % id )));
					std::string bz = (LPCSTR)getStammdaten().getBauelementGruppen().getBezeichnung(id);
					elem->setAttribute(X("Bezeichnung"), X(bz.c_str()));
				}

			}
		}
		dump(waid, doc->to_string());
		ret = doc->to_string();
	}
	catch (...)
	{
		// @todo Fehlerbehandlung: Welche Exceptions können hier auftreten?
	}
		
	
	return ret;
}


DOM::DOMElement* ConvCad2Xml::load_position(int waid, DOM::DOMElement* root) const
{
	MXTable Position(db, "Position");
	
	DECLARE_COL(Int, Waid, Position);
	DECLARE_COL(String, Datei, Position);
	DECLARE_COL(Int, KappenFarbe, Position);
	DECLARE_COL(String, FertHinweis, Position);
	DECLARE_COL(Bool, Trockenanlage, Position);
	DECLARE_COL(Bool, MitUeberlaenge, Position);
	DECLARE_COL(Int, Dichtmittel, Position);
	DECLARE_COL(Bool, IsHandelslaenge, Position);
	DECLARE_COL(Bool, IsApaxBauteil, Position);

	std::string sel = str( format("Waid = %1%") % waid );
	std::string sort("Waid");

	Position.load(sel, sort);
	
	tab_to_xml(root, Position, false); // Node 'Position' ist bereits Rootnode im Dokument, also kein Tablenode erzeugen
	return root;
}

DOM::DOMElement* ConvCad2Xml::load_bauteil(int waid, DOM::DOMElement* base) const
{
	/**
	Tabelle existiert (noch) nicht. Der Tag nimmt die BtPos Elemente auf.
	*/
	UNREFERENCED_PARAMETER(waid);
	return doc->create_node(base, "Bauteil");
}

void ConvCad2Xml::load_btpos(int waid, DOM::DOMElement* base) const
{
	/**
	Tabelle existiert noch nicht. Der Tag nimmt die BtPos Elemente auf.
	*/
	MXTable BauteilPos(db, "BauteilPos");

	DECLARE_COL(BigInt, Id, BauteilPos);
	DECLARE_COL(Int, Artikelnummer, BauteilPos);
	DECLARE_COL(Int, WaxWinkel, BauteilPos);
	DECLARE_COL(Bool, IsAnlage, BauteilPos);
	DECLARE_COL(Int, Artikelgruppe, BauteilPos);
	DECLARE_COL(Int, Sondermuffenlaenge, BauteilPos);
	DECLARE_COL(Int, Durchmesser, BauteilPos);
	DECLARE_COL(Double, MinExt_x, BauteilPos);
	DECLARE_COL(Double, MinExt_y, BauteilPos);
	DECLARE_COL(Double, MinExt_z, BauteilPos);
	DECLARE_COL(Double, MaxExt_x, BauteilPos);
	DECLARE_COL(Double, MaxExt_y, BauteilPos);
	DECLARE_COL(Double, MaxExt_z, BauteilPos);


	std::string sort("Waid");
	std::string sel = str( format("Waid = %1%") % waid);
	BauteilPos.load(sel.c_str(), sort);

	while (!BauteilPos.isEof())
	{
		DOM::DOMElement* btpos = tab_to_xml(base, BauteilPos);
		
		load_typrohr(waid, Id.data, btpos);
		load_anschluesse(waid, Id.data, btpos);
		load_anbauteile(waid, Id.data, btpos);

		// Untertags laden
		BauteilPos.moveNext();
	}	
}


void ConvCad2Xml::load_typrohr(int waid, __int64 id, DOM::DOMElement* btpos) const
{
	MXTable TypRohr(db, "TypRohr");
	DECLARE_COL(Int, DnId, TypRohr);
	DECLARE_COL(BigInt, Id, TypRohr);
	DECLARE_COL(Double, P1_X, TypRohr);
	DECLARE_COL(Double, P1_Y, TypRohr);
	DECLARE_COL(Double, P1_Z, TypRohr);
	DECLARE_COL(Double, P2_X, TypRohr);
	DECLARE_COL(Double, P2_Y, TypRohr);
	DECLARE_COL(Double, P2_Z, TypRohr);

	std::string sel = str( format("Waid = %1% AND Id = %2%") % waid % id);
	std::string sort = "Id";

	TypRohr.load(sel, sort);
	if (!TypRohr.isEof())
		tab_to_xml(btpos, TypRohr); // Gibt nur max. einen Eintrag
}



	

void ConvCad2Xml::load_anschluesse(int waid, __int64 id, DOM::DOMElement* btpos) const
{
	MXTable Anschluesse(db, "Anschluss");

	DECLARE_COL(BigInt, AnschlussId, Anschluesse);
	DECLARE_COL(Int, VbArt, Anschluesse);
	DECLARE_COL(Double, P_X, Anschluesse);
	DECLARE_COL(Double, P_Y, Anschluesse);
	DECLARE_COL(Double, P_Z, Anschluesse);
	DECLARE_COL(Double, R_X, Anschluesse);
	DECLARE_COL(Double, R_Y, Anschluesse);
	DECLARE_COL(Double, R_Z, Anschluesse);
	DECLARE_COL(Int, DnId, Anschluesse);
	DECLARE_COL(Int, Rohrverlaengerung, Anschluesse);

	DOM::DOMElement* base = doc->create_node(btpos, "Anschluesse");

	std::string sel = str( format("Waid = %1% AND Id = %2%") % waid % id);
	sel += " AND anbauteil = false";
	std::string sort = "Id";
	Anschluesse.load(sel, sort);
	
	while (!Anschluesse.isEof())
	{
		tab_to_xml(base, Anschluesse);
		Anschluesse.moveNext();
	}
}


void ConvCad2Xml::load_anbauteile(int waid, __int64 id, DOM::DOMElement* btpos) const
{
	MXTable Anbauteile(db, "Anschluss");

	DECLARE_COL(BigInt, AnschlussId, Anbauteile);
	DECLARE_COL(Int, VbArt, Anbauteile);
	DECLARE_COL(Double, P_X, Anbauteile);
	DECLARE_COL(Double, P_Y, Anbauteile);
	DECLARE_COL(Double, P_Z, Anbauteile);
	DECLARE_COL(Double, R_X, Anbauteile);
	DECLARE_COL(Double, R_Y, Anbauteile);
	DECLARE_COL(Double, R_Z, Anbauteile);
	DECLARE_COL(Int, DnId, Anbauteile);

	std::string sel = str( format("Waid = %1% AND Id = %2%") % waid % id);
	std::string selext = " AND anbauteil = true";
	std::string sort = "Id";
	Anbauteile.load(sel+selext, sort);
	
	if (!Anbauteile.isEof())
	{
		DOM::DOMElement* base = doc->create_node(btpos, "Anbauteile");

		while (!Anbauteile.isEof())
		{
			tab_to_xml(base, Anbauteile);
			Anbauteile.moveNext();
		}
	}
}


void ConvCad2Xml::dump(int waid, const std::string& xml) const
{
	/*
#ifdef _DEBUG

	std::string fn = str(format("%1%_xml.xml") % waid );
	
	stream<file_sink> out(fn);
    out << xml;
    out.close();	
#endif
	*/
}


DOM::DOMElement* ConvCad2Xml::tab_to_xml(DOM::DOMElement* base, const MXTable& t, bool insert_tablenode) const 
{
	/**
	Einlesen einer Tabellespalte und umwandeln nach XML
	Ist insert_tablenode true, wird für die Tabelle eine Node mit dem Tabellenamen generiert, sonst
	werden die Attribute direkt unter base platziert.
	*/
	vector<const Col*> cols = t.get_cols();

	if (insert_tablenode)
		base = doc->create_node(base, (LPCSTR)t.get_tableName());

	for (vector<const Col*>::iterator it = cols.begin(); it != cols.end(); it++)
	{
		base->setAttribute(X((*it)->get_name().c_str()), X((*it)->val_to_str().c_str()));
		//doc->create_node(base, (*it)->get_name(), (*it)->val_to_str());
	}
	return base;
}


