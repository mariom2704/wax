#pragma once

#include "waxcadmodell.h"

#ifdef _DEBUG
#undef new
#endif 

#include <boost/boost.h>
#include <xml/dom/mxxml_dom.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif 


namespace CadModell
{
	class CadXmlParser : public DOM::HandlerBase
	{
	private:
		WaxCadModell& list;
		bool parse_anschluesse; 

		int att2int(const DOM::AttributeList& att,  const string& value) const
		{
			if (!att.getValue( X(value)))
				return 0;

			std::string bv = XStr(att.getValue( X(value.c_str()))).to_string();
			//double baseval = XStr(att.getValue( X(value.c_str()))).to_double();
			boost::algorithm::replace_all(bv, ",", ".");
			double baseval = atof(bv.c_str());
			if (baseval < 1 && baseval > -1)
			{ // Werte kleiner 1 können sonst nicht in int abgebildet werden
				if ((int)(baseval*1000))
					throw (DOM::SAXException((value + (std::string)": Unable to convert double Value to int").c_str())) ;
			}
			return (int)baseval;
		}

		__int64 att2int64(const DOM::AttributeList& att,  const string& value) const
		{
			if (!att.getValue( X(value)))
				return 0;

			std::string bv = XStr(att.getValue( X(value.c_str()))).to_string();
			//double baseval = XStr(att.getValue( X(value.c_str()))).to_double();
			boost::algorithm::replace_all(bv, ",", ".");
			double baseval = atof(bv.c_str());
			if (baseval < 1 && baseval > -1)
			{ // Werte kleiner 1 können sonst nicht in int abgebildet werden
				if ((__int64)(baseval*1000))
					throw (DOM::SAXException((value + (std::string)": Unable to convert double Value to int").c_str())) ;
			}
			return (__int64)baseval;
		}

		double att2dbl(const DOM::AttributeList& att,  const string& value) const
		{
			if (!att.getValue( X(value)))
				return 0;

			std::string bv = XStr(att.getValue( X(value.c_str()))).to_string();
			//double baseval = XStr(att.getValue( X(value.c_str()))).to_double();
			boost::algorithm::replace_all(bv, ",", ".");
			double baseval = atof(bv.c_str());
			return baseval;
		}

		int dbl2int(const DOM::AttributeList& att,  const string& value) const
		{
			/*
			Umwandlung Double nach Int. Die letzten 3 Nachkommastellen werden mit
			übernommen, indem der Wert mal 1000 genommen wird
			*/
			if (!att.getValue( X(value)))
				return 0;

			std::string bv = XStr(att.getValue( X(value.c_str()))).to_string();
			//double baseval = XStr(att.getValue( X(value.c_str()))).to_double();
			boost::algorithm::replace_all(bv, ",", ".");
			double baseval = atof(bv.c_str());
			
			TRACE("Rundung: %f", baseval);
			if (baseval < 0)
				baseval -= 0.5;
			else
				baseval += 0.5;
			int ret = (int)baseval;
			TRACE(" -> %i\n", ret);
			
			return ret;
		}

		bool att2bool(const DOM::AttributeList& att,  const string& value) const
		{
			if (att.getValue( X(value))) 
				return XStr(att.getValue( X(value))).to_bool();
			return false;
		}

	public:
		CadXmlParser(WaxCadModell& elelist) : list(elelist)
		{
			parse_anschluesse = true;
		}

		void startElement(const XMLCh* const name, DOM::AttributeList& att)
		{
			XStr str(name);
			if (str.to_string() == "Artikel")
				handle_artikel(att);
			if (str.to_string() == "Position")
				handle_position(att);
			else if (str.to_string() == "TypRohr")
				handle_typrohr(att);
			else if (str.to_string() == "Bauteil")
				handle_bauteil(att);
			else if (str.to_string() == "BauteilPos")
				handle_bauteilpos(att);
			else if (str.to_string() == "Muffe")
				handle_muffe(att);
			else if (str.to_string() == "BtPos")
				handle_bauteilpos(att);
			else if (str.to_string() == "Bohrung")
				handle_bohrung(att);
			else if (str.to_string() == "Knick")
				handle_knick(att);
			else if (str.to_string() == "Anschluesse")
				handle_anschluesse(att);
			else if (str.to_string() == "Anbauteile")
				handle_anbauteile(att);
			else if (str.to_string() == "Anschluss")
				handle_anschluss(att);
			else if (str.to_string() == "Anschlusspunkt")
				handle_anschlusspunkt1(att);
			else if (str.to_string() == "Punkt")
				handle_anschlusspunkt2(att);
			else if (str.to_string() == "Richtung")
				handle_anschlussrichtung(att);
			else if (str.to_string() == "RohrDn")
				handle_anschluss_rohrdn(att);
			else if (str.to_string() == "Point")
				handle_rohrpoint(att);
			else
				TRACE("Unbehandelter Tag %s\n", str.to_string().substr(0,255).c_str());
		}

		void dump_att(string name, DOM::AttributeList& /* att */)
		{
/*
#ifdef _DEBUG
			TRACE("%s\n", name.c_str());
			for (unsigned int i = 0; i < att.getLength(); i++)
				TRACE("-> %s = %s\n", XStr(att.getName(i)).to_string().substr(0,255).c_str(), XStr(att.getValue(i)).to_string().substr(0,255).c_str());
#endif
*/
		}

		void handle_position(DOM::AttributeList& att)
		{
			int kfarbe = att2int(att, "KappenFarbe");
			list.kappenfarbe = kfarbe;
			list.ferthinweis = XStr(att.getValue("FertHinweis")).to_string();
			list.posnr = XStr(att.getValue("PosNr")).to_string();
			list.dichtmittel = att2int(att, "Dichtmittel");
			list.isHandelslaenge = att2bool(att, "IsHandelslaenge");
			list.m_MitUeberlaenge = att2bool(att, "MitUeberlaenge");
			list.m_IsApaxBauteil = att2bool(att, "IsApaxBauteil");

			// Werte werden hier aus XML Datei gelesen, bei Zugriff auf die Datenbank werden die Werte aus der
			// Werkauftragstabelle übernommen
			list.anzahl = att2int(att, "Anzahl");
			int sd = att2int(att, "SdMatNr");
			if (sd) // kann aus Tabelle Werkauftrag kommen, dann steht hier nix drin
				list.set_sdmaterialnr(sd);
			dump_att("Position", att);
		}

		void handle_bauteil(DOM::AttributeList& att)
		{
			dump_att("Bauteil", att);
		}

		void handle_artikel(DOM::AttributeList& att)
		{
			dump_att("Artikel", att);
			CadElement& e = list.back();
		
			int anr = att2int(att, "Artikelnummer");
			int artgr = att2int(att, "Artikelgruppe");

			if (artgr > 0)
				e.initStammdaten(anr, artgr);
			else
				e.initStammdaten(anr);
			
			if (att.getValue( X("IsAnlage")))
			{
				bool anl = att2bool(att, "IsAnlage");
				e.set_makeAnlage(anl);
			}
		}

		void handle_bohrung(DOM::AttributeList& att)
		{
			dump_att("Bohrung", att);
			CadElement& e = list.back();
		
			double bohr = att2dbl(att, "Durchmesser");
			
			int val = (int)(bohr + 0.5);

			e.set_durchmesser(val);
			e.set_mass2(val);
		}

		void handle_knick(DOM::AttributeList& att)
		{
			dump_att("Knick", att);
			CadElement& e = list.back();
		
			double winkel = att2dbl(att, "Biegung");
			
			int val = (int)(winkel + 0.5);

			e.set_durchmesser(val);
			e.set_mass2(val);
		}

		void handle_muffe(DOM::AttributeList& att)
		{
			dump_att("muffe", att);
			CadElement& e = list.back();
		
			int len = att2int(att, "Sondermuffenlaenge");
			if (len)
			{
				e.set_laenge(len);
				e.set_mass1(len);
			}
		}

		void handle_bauteilpos(DOM::AttributeList& att)
		{
			dump_att("BauteilPos", att);
			

			int artnr = att2int(att, "Artikelnummer");
			__int64 cid = att2int64(att, "Id");
			bool anl = att2bool(att, "IsAnlage");
			int artgrp = att2int(att, "Artikelgruppe");

			double maxx = att2dbl(att, "MaxExt_x");
			double maxy = att2dbl(att, "MaxExt_y");
			double maxz = att2dbl(att, "MaxExt_z");

			double minx = att2dbl(att, "MinExt_x");
			double miny = att2dbl(att, "MinExt_y");
			double minz = att2dbl(att, "MinExt_z");
			int winkel = dbl2int(att, "WaxWinkel");
		
			CadElement ce(artnr, artgrp, cid, anl, Point(minx, miny, minz), Point(maxx, maxy, maxz));
			if (ce.get_artikelnr() != (long)artnr)
				TRACE("Artikelnummer ungleich: Wax=%i != CAD=%i\n", ce.get_artikelnr());
			ce.set_winkel(winkel);
			
			if (artgrp == SONDERMUFFE)
			{
				int sondermuffenlaenge = att2int(att, "Sondermuffenlaenge");
				ce.set_laenge(sondermuffenlaenge);
				ce.set_mass1(sondermuffenlaenge);
			}
			else if (artgrp == BOHR24 || artgrp == BOHR50 || artgrp == KNICK)
			{
				double durchmesser = att2dbl(att, "Durchmesser");
				
				int val = (int)(durchmesser + 0.5);

				ce.set_durchmesser(val);
				ce.set_mass2(val);
			}
			list.push_back(ce);
		}

		void handle_typrohr(DOM::AttributeList& att)
		{
			dump_att("TypRohr", att);
	/*
	TypRohr
	-> DnId = 100
	-> Id = 2130284792
	-> P1_X = 1283.085700
	-> P1_Y = 6262.035100
	-> P1_Z = 0.000000
	-> P2_X = 1283.085700
	-> P2_Y = 5762.035100
	-> P2_Z = 0.000000
	*/		
			CadElement& e = list.back();
			e.typrohr.dn = att2int(att, "DnId");
			e.typrohr.id = att2int64(att, "Id");
			e.typrohr.p1 = Point( att2dbl(att, "P1_X"), att2dbl(att, "P1_Y"), att2dbl(att, "P1_Z") );
			e.typrohr.p2 = Point( att2dbl(att, "P2_X"), att2dbl(att, "P2_Y"), att2dbl(att, "P2_Z") );

			if(e.get_id() == 30 /*ROHR_Q0*/) // Nicht definertes Rohrqualität
			{
				e.initStammdaten(e.typrohr.dn, 0, e.get_id());
			}
		}


		void handle_anschluesse(DOM::AttributeList& att)
		{
			dump_att("Anschluesse", att);
			parse_anschluesse = true;
		}

		void handle_anbauteile(DOM::AttributeList& att)
		{
			dump_att("Anbauteile", att);
			parse_anschluesse = false;
		}

		void handle_anschluss(DOM::AttributeList& att)
		{
			/**
			Tag erzeugt aus Datenbanktabelle 'Anschluss'
			Enthält die nötigen Tags aus den XML Tags Anschluesse/Anschluss/Anschlusspunkt 
			und darunter Punkt, Richtung, Rohrdn
			*/
			dump_att("Anschluss", att);
			if (list.empty())
				return;
			CadElement& e = list.back();
	/*
		Anschluss
	-> AnschlussId = 2130284632
	-> DnId = 100
	-> P_X = 2096.562600
	-> P_Y = 6414.035100
	-> P_Z = 0.000000
	-> R_X = 0.000000
	-> R_Y = 1.000000
	-> R_Z = 0.000000
	-> VbArt = 0
	*/
			__int64 cadid = att2int64(att, "AnschlussId");
			if (!cadid)
				cadid = att2int64(att, "Id");

			double rx,ry,rz;
			
			rx =  att2dbl(att, "R_X") * 100000;
			ry =  att2dbl(att, "R_Y")* 100000;
			rz =  att2dbl(att, "R_Z")* 100000;

			//Anschluss(int rid = 0, int rdn = 0, int rvbart = 0, Point rp = Point(), Point rr = Point() )
			Anschluss a( cadid, att2int(att, "DnId"), att2int(att, "VbArt"), att2int(att, "Rohrverlaengerung")
				, Point(att2dbl(att, "P_X"),att2dbl(att, "P_Y"),att2dbl(att, "P_Z"))
				, Point(rx, ry, rz));

			if (parse_anschluesse)
				e.anslst.push_back(a);
			else
				e.abglst.push_back(a);
		}

		void handle_anschlusspunkt1(DOM::AttributeList& att)
		{
			/**
			Tag aus XML Modell des Rohrnetzkonfigurators
			*/
			dump_att("Anschlusspunkt", att);
			CadElement& e = list.back();

			int vbart = att2int(att, "VbArt");
			int rverl = att2int(att, "Rohrverlaengerung");
						
			if (parse_anschluesse)
			{

				e.anslst.back().vbart = vbart;
				e.anslst.back().rohrverlaengerung = rverl;
			}
			else
			{
				e.abglst.back().vbart = vbart;
				e.abglst.back().rohrverlaengerung = rverl;
			}
		}
		
		void handle_anschlusspunkt2(DOM::AttributeList& att)
		{
			/**
			Tag aus XML Modell des Rohrnetzkonfigurators
			*/
			dump_att("Punkt", att);
			CadElement& e = list.back();

			double x,y,z;
			x =  att2dbl(att, "X");
			y =  att2dbl(att, "Y");
			z =  att2dbl(att, "Z");

			if (parse_anschluesse)
				e.anslst.back().p = Point(x, y, z);
			else
				e.abglst.back().p = Point(x, y, z);
		}

	

		void handle_anschlussrichtung(DOM::AttributeList& att)
		{
			/**
			Tag aus XML Modell des Rohrnetzkonfigurators
			*/
			dump_att("Richtung", att);
			CadElement& e = list.back();
			
			double x,y,z;
			
			x =  att2dbl(att, "X") * 100000;
			y =  att2dbl(att, "Y")* 100000;
			z =  att2dbl(att, "Z")* 100000;

			if (parse_anschluesse)
				e.anslst.back().r = UnitVector(Point((int)x, (int)y, (int)z));
			else
				e.abglst.back().r = UnitVector(Point((int)x, (int)y, (int)z));
		}

		void handle_anschluss_rohrdn(DOM::AttributeList& att)
		{
			/**
			Tag aus XML Modell des Rohrnetzkonfigurators
			*/
			dump_att("RohrDN", att);
			CadElement& e = list.back();

			int dnid = att2int(att, "DnId");

			if (parse_anschluesse)
			{
				if (e.anslst.empty())
					e.typrohr.dn = dnid;
				else
					e.anslst.back().dn = dnid;
			}
			else
			{
				if (e.abglst.empty())
					e.typrohr.dn = dnid;
				else
					e.abglst.back().dn = dnid;
			}
		}

		void handle_rohrpoint(DOM::AttributeList& att)
		{
			/**
			Tag aus XML Modell des Rohrnetzkonfigurators
			*/
			dump_att("Point", att);
			CadElement& e = list.back();

			double x,y,z;
			x =  att2dbl(att, "X");
			y =  att2dbl(att, "Y");
			z =  att2dbl(att, "Z");
			
			if (e.typrohr.p1.is_null())
				e.typrohr.p1 = Point(x, y, z);
			else
				e.typrohr.p2 = Point(x, y, z);
		}
	};
}