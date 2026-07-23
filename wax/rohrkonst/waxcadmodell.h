#pragma once

#include "cadmodell.h"
#include "cadmodell_vector.h"

namespace CadModell
{
	class WaxCadModell : public Modell
	{
	public:
		WaxCadModell()
		{
			anzahl = 1;
			kappenfarbe = 0; // Anzahl und kappenfarbe werden direkt aus dem XML Import heraus gesetzt
			sdmaterialnr = 0;
			isHandelslaenge = false;
		};

		virtual ~WaxCadModell() {};		

		int create_waxstrang(int& anfid, int& endid, int& rohrquali, EnContainerRohrElemente& arr, bool gewinderohr, bool* btwisted)
		{
			/**
			Erzeugt das Elementarray und gibt die Länge zurück. In anfid und endid werden die
			Elementid des Anfangselements und des Endeelements zurückgegeben
			*/
			arr.deleteAll();

			//init_rohrverlaengerung();

			Pipe mainpipe =	get_mainpipe();
			if (mainpipe.is_empty())
			{
				//Dann ist es wahrscheinlich ein Apax. Anfang/Ende auf Glatt setzen
				anfid = NUT;
				endid = NUT;
				return 0;
			}


#ifdef _DEBUG
			dump_directions(mainpipe);

#endif
			rohrquali = mainpipe.get_rohrquali();

/*			// Hauptleitung so drehen, dass erstes Element nach oben zeigt
			int drehwinkel = mainpipe.normalize();
			work.normalize(drehwinkel);
*/						
			// Relativen Abstand aller Elemente bestimmen
			mainpipe.init_relabst(gewinderohr); 

			// Elemente des Hauptstrangs werden mit relativem Abstand eingefügt
			merge_mainpipe(mainpipe, arr, gewinderohr);
		
			// Zu allen Elementen in der Hauptleitung aus den rel. Abständen die absoluten berechnen
			arr.calcAbsolutAbst();
					
			mainpipe.calc_absAbst();

			if (gewinderohr)
			{
				init_lastelement(mainpipe.elelist.back(), arr, endid);
			}

			merge_outlets(mainpipe, arr);

			*btwisted = mainpipe.m_Twisted;

			int index = 0;
			arr.sortElements(index);

			arr.calcRelAbst();
#if 0
			if (gewinderohr)
			{
				init_absabst(arr);
			}
#endif
#ifdef _DEBUG
			dump();
#endif
			anfid = mainpipe.get_anfid(gewinderohr);

			//if (!gewinderohr)
			endid = mainpipe.get_endid();
			
			return mainpipe.get_len();
		}
#if 0
		void init_absabst(EnContainerRohrElemente& arr)
		{
			for (int i = 0; i < arr.GetCount(); i++)
			{
				Element* e = arr.get_element_at(i);
				e->set_abstAbs(e->get_abstRel());
			}
		}
#endif
		
		void init_lastelement(const CadElement* ele, EnContainerRohrElemente& arr, int& endid)
		{
			if (ele->is_rohr())
			{
				endid = ele->get_verbid();
				CadElement* new_endeele = new CadElement(endid, ele->typrohr.dn, false);
				new_endeele->set_abstAbs(ele->get_abstAbs() + ele->get_laenge());


				new_endeele->set_abstRel(ele->get_laenge());
				
				new_endeele->set_dn1(ele->typrohr.dn);
				new_endeele->set_id(ROHR_Q1);
				arr.Add(new_endeele);
			}
			else
				endid = ele->isFitting() ? GLATT  : ele->get_id();
		}

#ifdef _DEBUG
		void dump_directions(const Pipe& mainpipe)
		{
			for (vector<CadElement*>::const_iterator it = mainpipe.elelist.begin(); it != mainpipe.elelist.end(); it++)
			{
				CadElement* ele = *it;
				for (CadElement::AnschlussListe::iterator it = ele->anslst.begin(); it != ele->anslst.end(); it++)
				{
					UnitVector r = it->r;
					TRACE("%s: Richtung x=%i,y=%i,z=%i\n", ele->getBezeichnung(), r.B.x, r.B.y, r.B.z); 
				}
			}
		}
#endif

		

		void merge_mainpipe(const Pipe& mainpipe, EnContainerRohrElemente& arr, bool gewinderohr) const
		{
			/**
			Hauptleitung in Elementliste packen
			*/
#ifdef _DEBUG
			mainpipe.dump_list();
#endif
			for (vector<CadElement*>::const_iterator it = mainpipe.elelist.begin(); it != mainpipe.elelist.end(); it++)
			{
				CadElement* ele = *it;
				if (mainpipe.is_mainpipe_ele(ele, gewinderohr))
					arr.Add(new Element(*ele));
			}
		}


		

		void merge_outlets(const Pipe& mainpipe, EnContainerRohrElemente& arr) const
		{
			/**
			Abgänge und Unterelementlisten einfügen
			Bedingung für den Aufruf: für mainpipe-Elemente müssen die absoluten Abstände bereits
			berechnet worden sein. 
			*/	

			int ausgleich_bogen = 0; // relativer Abstand von Elementen nach einem Bogen = rel.Abst. + Bogenlänge
			for (vector<CadElement*>::const_iterator eleit = mainpipe.elelist.begin(); eleit != mainpipe.elelist.end(); eleit++)
			{
				CadElement* ele = *eleit;
				
				if (!ele->is_rohr()) // Nur Abgänge auf Rohr berücksichtigen 
				{
					if (ele->isEleBogen())
						ausgleich_bogen += ele->get_laenge();
					continue; // @todo Prüfung einbauen: Abgänge auf Nichtrohrelementen können nicht gezeichnet werden
				}

				vector<Pipe> vp = get_outlets(ele);
				
				for (vector<Pipe>::iterator it = vp.begin(); it != vp.end(); it++)
				{
					for (vector<CadElement*>::iterator xit = it->elelist.begin(); xit != it->elelist.end(); xit++)
					{
						CadElement* abgele = new CadElement(**xit);
						
						if( abgele->get_id() == SPRINKLERSCHELLE_VIROTEC )
						{	
							changeAnschluesseParallelFirst(abgele, ele);
						}
						
						int absrel = calc_relabst(eleit == mainpipe.elelist.begin() ? NULL : *(eleit-1), ele, abgele) + ausgleich_bogen;
						abgele->set_abstRel(absrel);
						abgele->set_abstAbs(ele->get_abstAbs() + abgele->get_abstRel());

						int winkel = recalc_winkel(eleit == mainpipe.elelist.begin() ? NULL : *(eleit-1), ele, abgele);
						abgele->set_winkel(winkel);

						arr.Add(abgele);
						
						for (POSITION pos = abgele->get_uelementList().GetHeadPosition(); pos != NULL;)
						{
							CadElement* ue = dynamic_cast<CadElement*>(abgele->get_uelementList().GetNext(pos));
							if (!ue)
								continue;
							ue->set_abstAbs(abgele->get_abstAbs());
							ue->set_winkel(abgele->get_winkel());
							ue->set_abstRel(0);
							arr.Add(new CadElement(*ue));
						}
						abgele->get_uelementList().deleteAllElemente();
						
					}
				}
			}
		}

		int recalc_winkel(const CadElement* const prevele, const CadElement* const ele, const CadElement* const abgele) const
		{
			int index = 0;
			if (!prevele)
				index = ele->anslst[0].cadid == 0 ? 0 : 1; // Erstes Element in der Liste: Winkel OK wenn anslst[0] leer ist
			else
				index = ele->anslst[0].cadid == prevele->cadid ? 0 : 1;

			if (index)
				return 360-abgele->get_winkel();
			return abgele->get_winkel();
		}

		int calc_relabst(const CadElement* const prevele, const CadElement* const ele, const CadElement* const abgele) const
		{
			/**
			Errechnet den Abstand des Abgangs zum Beginn des Elementes
			*/
			int index = 0;
			if (!prevele)
				index = ele->anslst[0].cadid == 0 ? 0 : 1;
			else
				index = ele->anslst[0].cadid == prevele->cadid ? 0 : 1;
				
			Vector anschluss_vec(ele->anslst[index].r.A, abgele->anslst[0].r.B);
			Vector abgang_vec(ele->anslst[index].p, abgele->anslst[0].p);

			double lenprodukt = anschluss_vec.len() *  abgang_vec.len();

			if (lenprodukt < 0.01)
				return 0; // Nicht berechenbar


			double cosphi = (double)anschluss_vec.skalarprodukt(abgang_vec) / lenprodukt;
			double sinphi = sqrt(1.0 - pow(cosphi, 2.0)); 

			double len = sinphi * abgang_vec.len();
		
			return (int)(len + ele->anslst[0].rohrverlaengerung + 0.5);
		}



		bool init_abschlusselement(Pipe& abg) const
		{
			/**
			Wenn als letztes Element in der Liste ein Rohr steht, dann
			Endeelement anhängen: Glatt, Nut, Gewinde bei Rohrstutzen
			*/
			CadElement* endeele = abg.elelist.back();

			if (!endeele || !endeele->init_abschluss()) // Für ein korrektes Abschlusselement sorgen bei Rohrstutzen
				return false;

			return true;
		}


				/// Setzen von Winkel der Abgangselemente
		virtual bool init_relAbst(Pipe& abg, const CadElement::AnschlussListe& anschluss) const
		{
			/**
			Berechnet den relativen Abstand des Abgangelementes vom Beginn des Rohrstückes
			*/
			for (vector<CadElement*>::iterator elex = abg.elelist.begin(); elex != abg.elelist.end(); elex++)
			{
				int abst = calc_abstRel((*elex), anschluss.begin()->p);
				(*elex)->set_abstRel(abst);

			}
			return true;
		}

		int calc_abstRel(const CadElement* const ele, CadModell::Point start) const
		{
		/**
			Berechnet den absoluten Abstand eines Elementes. 
			Element muss Abgang vom Hauptrohr sein und das Rohr muss korrekt ausgerichtet sein.
		*/
			double abstabs1 = ele->min.x - start.x;
			double abstabs2 = ele->max.x - start.x;

			double tmp = abs((abstabs1 + abstabs2) / 2.0);

			return (int)(tmp + 0.5);
		}

		int kappenfarbe;	
		// Werte werden durch Grafikerzeugung gesetzt (XML Format aus RNK)
		std::string ferthinweis;
		std::string posnr;
		int anzahl;
		int dichtmittel;
		bool isHandelslaenge;
		bool m_MitUeberlaenge;
		bool m_IsApaxBauteil;

		int get_sdmaterialnr() const
		{
			return sdmaterialnr;
		}
		void set_sdmaterialnr(int sd)
		{
			sdmaterialnr = sd;
		}
		private:
			unsigned int sdmaterialnr;
	};

}