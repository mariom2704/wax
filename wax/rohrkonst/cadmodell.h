#pragma once

#include "EnContainerRohrElemente.h"
#include "cadmodell_cadelement.h"
#include "cadmodell_pipe.h"
#include "MxErrorHandler.h"


namespace CadModell
{
	class Modell : public vector<CadModell::CadElement>
	{
	public:
		Modell()
		{
		};

		
		int get_rohrdn() const 
		{ 
			const_iterator it = start();
			if (it != end())
				return it->get_dn();
			return 0;
		};


		const_iterator start() const
		{
			return  get_startindex() == -1 ? begin() : begin() + get_startindex();
		}

		iterator start()
		{
			return  get_startindex() == -1 ? begin() : begin() + get_startindex();
		}

		int get_startindex() const
		{
			/**
			Sucht die DN stärkste Rohrleitung, gibt es mehrere Rohrleitungen gleicher DN,
			wird die längste ausgewählt
			*/
			int startdn = 0;
			double startlen = 0;
			// Rohrleitung finden
			int i = 0;
			int ret = -1;
			unsigned int anz_abg = 0;
			for (const_iterator it = begin(); it != end(); it++, i++)
			{
				if (!it->is_rohr())
					continue; // kein Rohr

				if (it->abglst.size() < anz_abg)
					continue;
				
				if (it->typrohr.dn > startdn 
					|| (it->typrohr.dn == startdn && it->typrohr.len() > startlen)
					|| it->abglst.size() > anz_abg) 
				{
					startdn = it->get_dn();
					startlen = it->typrohr.len();
					anz_abg = it->abglst.size();
					ret = i;
				}
			}

			if( ret == -1 )
			{
				int j = 0;
				for (const_iterator it = begin(); it != end(); it++, j++)
				{
					if (it->isEleFlansch())
					{
						ret = j;
						break;
					}
				}
			}
			return ret;
		}


		const_iterator next(const_iterator xit, const UnitVector& direction) const
		{
			int nextid = get_nextid(xit, direction);
			return nextid == -1 ? end() : begin()+nextid;
		}


		iterator next(const_iterator xit, const UnitVector& direction)
		{
			int nextid = get_nextid(xit, direction);
			return nextid == -1 ? end() : begin()+nextid;
		}

		int get_nextid( const_iterator xit, const UnitVector& direction) const
		{
			/**
			Liefert das nächste Element. Das aktuelle Element muss in der Anschlussliste vorhanden sein
			mit einer entgegengesetzten Anschlussrichtung.
			*/
			int i = 0;
			for (const_iterator it = begin(); it != end(); it++,i++)
			{
				for (CadModell::CadElement::AnschlussListe::const_iterator abgit = it->anslst.begin(); abgit != it->anslst.end(); abgit++)
				{
					if (abgit->cadid == xit->cadid)
					{
						if(it->get_id() != SPRINKLERSCHELLE_VIROTEC) // Sprinklerschelle hat einen Vector in Hauptrohrrichtung, muss desshalb ueberganen werden
						{
							if (direction.compareDirection(abgit->r.reverse()))
								return i;
						}
					}
				}
			}
			return -1;
		}

		

		int get_anschlussid(const_iterator xit, const UnitVector& direction) const
		{
			for (CadModell::CadElement::AnschlussListe::const_iterator abgit = xit->anslst.begin(); abgit != xit->anslst.end(); abgit++)
			{
				//if (abgit->r == direction)
				if (abgit->r.compareDirection(direction))
					return abgit->vbart;
			}
			return 0;
		}

		UnitVector get_next_direction(const_iterator it, const UnitVector& direction) const
		{
			/**
			Liefert die Folgerichtung des Elementes, wenn es zwei Anschlüsse gibt. 
			Bei drei oder mehr Anschlüssen kann die Folgerichtung nicht ermittelt werden. 
			Deshalbt wird die Richtung beibehalten. Bei einem T-Stück z.B. wäre dieses T-Stück 
			das Endeelement.
			*/
			
			return get_next_direction(*it, direction);
		}

		UnitVector get_next_direction(const CadElement& ele, const UnitVector& direction) const
		{
			/**
			Liefert die Folgerichtung des Elementes, wenn es zwei Anschlüsse gibt. 
			Bei drei oder mehr Anschlüssen kann die Folgerichtung nicht ermittelt werden. 
			Deshalbt wird die Richtung beibehalten. Bei einem T-Stück z.B. wäre dieses T-Stück 
			das Endeelement.
			*/
			
			UnitVector ret = direction;
			// Wenn das Element zwei Abgänge hat, kann sich die Richtung ändern (Bogen)
			if (ele.anslst.size() == 2)
			{
				//if (ele.anslst[0].r == direction.reverse())
				if (ele.anslst[0].r.compareDirection(direction.reverse()))
					ret = ele.anslst[1].r;
				else
					ret = ele.anslst[0].r;
			}
			return ret;
		}

		bool init_anschlussdn(vector<CadElement*> elist) const
		{
				
			for (vector<CadElement*>::iterator it = elist.begin(); it != elist.end(); it++)
			{
				CadElement* e = *it;
				if (e->isEleRedEinz())
				{
					if (it != elist.begin())
					{
						int prevdn = (*(it-1))->get_dn();
						int curanschldn = e->get_anschlussDN();
						if ( prevdn != curanschldn)
						{
							int anschlussdn = e->get_anschlussDN();
							e->set_anschlussDN(e->get_dn());
							e->set_dn(anschlussdn);
						}
					}
				}
				else if (e->isEleBogen())
					e->set_anschlussDN(e->get_dn());
			}

			return true;
		}

		

		virtual Pipe get_mainpipe() const
		{
			/**
			Ausgehend vom Startelement wird die Hauptleitung jeweils in beide Richtung gescannt.
			*/
			Pipe p;

			const_iterator it = start(); // Start ist das Rohrstück mit der grössten DN

			if (it == end() || it->anslst.empty())
					return p; // Fehler, Rohr muss zwei Anschlüsse haben
			
			// Zwei Richtungen Anschlüsse
			for (int i = 0; i < 2; i++)
			{
				CadModell::UnitVector richtung = it->anslst[i].r;

				bool rel_prev = i == 0 ? true : false; 				
				
				for (const_iterator x1 = it; x1 != end(); x1 = next(x1, richtung))
				{
					CadElement* ele = new CadElement(*x1);
					
					if( ele->get_id() == SPRINKLERSCHELLE_VIROTEC )
					{	
						continue;
					}
					
					if (rel_prev)
					{
						if (x1 != it)
							p.elelist.insert(p.elelist.begin(), ele); // Element nach links einfügen
						else
						{
							delete ele; // Startelement nur einmal (im else Zweig) einfügen
							ele = NULL;
						}
					}
					else
						p.elelist.push_back(ele); // Element nach rechts einfügen
	
					CadModell::UnitVector next_richtung = get_next_direction(x1, richtung); // Der Rohrleitung folgen, Richtung kann wechseln (Bogen)
					//if (ele)
					//	ele->set_winkel(calc_winkel(richtung, next_richtung));
					richtung = next_richtung;

					// Rekursion im Modell testen. Im Fehlerfall wird leeres Rohr zurückgeliefert
					if (p.check_elelist_for_duplicates())
						return Pipe();
				}
			}

//			init_winkel(p.elelist);
			
			init_anschlussdn(p.elelist);

			if (p.elelist.size())
				(*p.elelist.begin())->autodetect_kopfstueck();
			if (p.elelist.size() > 1)
				(p.elelist.back())->autodetect_kopfstueck();


			return p;
		}

		void changeAnschluesseParallelFirst(CadElement* ele, CadElement* MainPipe)const// CadElement& Maintube)
		{
			CadModell::UnitVector richtungMainPipe = MainPipe->anslst[0].r;
			bool para = richtungMainPipe.compareDirection(ele->anslst[0].r);
			if(!para)
			{
				if(MainPipe->anslst.size() < 2)
				{
					return;
				}
				richtungMainPipe = MainPipe->anslst[1].r;
				para = richtungMainPipe.compareDirection(ele->anslst[0].r);
			}
			if(para)
			{
				if(ele->anslst.size() < 2)
				{
					return;
				}
				CadModell::Anschluss temp = ele->anslst[0];
				ele->anslst[0] = ele->anslst[1];
				ele->anslst[1] = temp;

			}
		}


		Pipe get_pipe(const_iterator start, UnitVector direction) const
		{
			/**
			Erstellt eine Rohrleitung beginnend vom Anfangselement start. Es werden die 
			relativen Abstände gesetzt und nur in eine Richtung gescannt.
			*/
			Pipe p;

			if (start == end())
				return p;
			
			UnitVector richtung = direction.reverse();
		
			CadElement* rohr = NULL;
			for (const_iterator x1 = start; x1 != end(); x1 = next(x1, richtung))
			{
				CadElement* e = new CadElement(*x1);
				if (rohr)
				{
					e->set_anschlussDN(rohr->get_dn());
					rohr->get_uelementList().AddTail(e);
				}
				else
					p.elelist.push_back(e);

				if (e->isEleRohr() || e->get_id() == ROHR) // Rohr oder Rohrstutzen
				{
					e->set_id(ROHR);
					rohr = e;
				}
				

				richtung = get_next_direction(x1, richtung);
			}
			init_anschlussdn(p.elelist);
			p.init_relabst();
			return p;
		}

		const_iterator find(__int64 cadid) const
		{
			/**
			Sucht aus der Liste das passende CadElement
			*/
			for (const_iterator it = begin(); it != end(); it++)
			{
				if (it->cadid == cadid)
					return it;
			}
			return end();
		}

		/// Abgang bearbeiten muss überschrieben werden
		virtual bool init_abschlusselement(Pipe& abg) const = 0;

		virtual bool init_relAbst(Pipe& abg, const CadElement::AnschlussListe& anschluss) const = 0;

		vector<Pipe> get_outlets(const CadElement* const ele) const
		{
			/**
			Holt die Abgänge für CadElement ele.
			*/
			vector<Pipe> vp;

			for (CadElement::AnschlussListe::const_iterator it = ele->abglst.begin(); it != ele->abglst.end(); it++)
			{
				// Liste mit Elementen für den Abgang lesen
				Pipe p = get_pipe(find(it->cadid), it->r);
				if (p.is_empty())
					continue;

				// Erstes Element muss mit der DN des Basiselementes versehen werden
				p.elelist.front()->set_anschlussDN(ele->get_dn());

				// Setzen der Winkel, Länge usw. der Elemente im Abgang
				if (!init_abschlusselement(p))
					continue; // Fehler: Abgang konnte nicht korrekt initialisiert werden

				if (!init_relAbst(p, ele->abglst))
					continue; // Fehler: Relativer Abstand konnte nicht berechnet werden

								vp.push_back(p);
			}
			return vp;
		}

		int calc_winkel(const UnitVector ref, const UnitVector& other) const
		{
			double cosphi = ref.cosphi(other);

			double wk = acos(cosphi) * 180 / M_PI; 

			int winkel = (int) wk;

			if (other.B.z > 0)
				winkel = 360 - winkel;
			
			return winkel;
			
		}

		void normalize(int drehwinkel) 
		{
			/**
			Setzt für alle Elemente einen Winkeloffset
			*/
			for (iterator it = begin(); it != end(); it++)
				it->set_korrektur_winkel(drehwinkel);
		}

		CadElement::AnschlussListe get_anschluesse(const CadModell::CadElement &ele) const
		{
			CadElement::AnschlussListe anschl;

			for (CadElement::AnschlussListe::const_iterator ansit = ele.anslst.begin(); ansit != ele.anslst.end(); ansit++)
			{
				for (const_iterator itm = begin(); itm != end(); itm++)
				{
					if (itm->cadid != ansit->cadid)
						continue;
					for (CadElement::AnschlussListe::const_iterator it = itm->anslst.begin(); it != itm->anslst.end(); it++)
					{
						if (ele.cadid == it->cadid)
						{
							anschl.push_back(*it);
						}
					}
				}
			}
			return anschl;
		}

		Anschluss get_match(int cadid, const Anschluss& ans) const
		{
			for (const_iterator itm = begin(); itm != end(); itm++)
			{
				if (itm->cadid != ans.cadid)
					continue;
				for (CadElement::AnschlussListe::const_iterator it = itm->anslst.begin(); it != itm->anslst.end(); it++)
				{
					if (cadid == it->cadid)
						return *it;
				}
			}
			return Anschluss();
		}

		//nicht mehr notwendig, da Rohrverlängerung schon an den Anschlusspunkten des Rohres und Bauteils übergeben wird
		/*
		void init_rohrverlaengerung(CadModell::CadElement &ele) const
		{
			for (CadElement::AnschlussListe::iterator ansit = ele.anslst.begin(); ansit != ele.anslst.end(); ansit++)
			{
				Anschluss a = get_match(ele.cadid, *ansit);
				if (a.cadid == ele.cadid) // gefunden
					ansit->rohrverlaengerung = a.rohrverlaengerung;
			}
		}


		void init_rohrverlaengerung()
		{
			for (iterator it = begin(); it != end(); it++)
			{
				if (!it->is_rohr())
					continue;
				init_rohrverlaengerung(*it);
			}
		}
		*/

		MxErrorHandling::MxErrorHandler herror;

		
#ifdef _DEBUG
		
		virtual void dump() const
		{
			Pipe p = get_mainpipe();
			p.dump();
		}
#endif

	};
}