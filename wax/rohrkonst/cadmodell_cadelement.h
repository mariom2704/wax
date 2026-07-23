#pragma once

#include "cadmodell_anschluss.h"
#include "cadmodell_typrohr.h"
#include "element.h"

namespace CadModell
{
	class CadElement : public Element
	{
	public:
		CadElement(int artnr, int artgrp, __int64 cid, bool anlage, CadModell::Point pmin, CadModell::Point pmax)
		{
			if(artgrp == 30/*ROHR_Q0*/) // Nicht definertes Rohrqualität (DummyRohr)
			{
				artnr = 11126;  // Erstmal einen Artikel annehmen kommt aus dem Rnk falsch keine definierte Nummer
			}					// e.initStammdaten(e.typrohr.dn, 0, e.get_id());

			if (artnr)
			{
				initStammdaten(artnr, artgrp);
			}
			max = pmax;
			min = pmin;
			cadid = cid;
			set_makeAnlage(anlage);
			set_laenge(get_laenge());
			set_urid(get_id());
		}

		CadElement(int elemid, int dn, bool anlage)
		{
			initStammdaten(dn, dn, elemid);
			cadid = 0;
			set_makeAnlage(anlage);
		}

		CadElement(const CadElement& other)
		{
			cadelement_copy(other);
		}

		virtual WaxBase *deepCopy() const
		{
			return new CadElement(*this);
		}

		void autodetect_kopfstueck()
		{
			set_kopfstueck(0);
			if (anslst.size() < 3)
				return; // Nur Fitting mit mehr als zwei Anschlüssen

			int anschluesse = 0;
			CadModell::UnitVector r;
			for (AnschlussListe::const_iterator it = anslst.begin(); it != anslst.end(); it++)
			{
				if (it->cadid)
				{
					r = it->r;
					anschluesse++;
				}
			}
			if (anschluesse != 1) // Kopfstueck kann nur an einem Anschluss 
				return; 


			for (AnschlussListe::const_iterator it = anslst.begin(); it != anslst.end(); it++)
			{
				//if (it->r == r.reverse())
				if (it->r.compareDirection(r.reverse()))
				{
					set_kopfstueck(0);
					return;
				}
			}
			set_kopfstueck(1);
		}

		void cadelement_copy(const CadElement& other)
		{
			copy(other);
			max = other.max;
			min = other.min;
			cadid = other.cadid;
			set_makeAnlage(other.get_makeAnlage());
			anslst = other.anslst;
			abglst = other.abglst;
			typrohr = other.typrohr;
			set_laenge(get_laenge());
		}

		/// Kopieren der Unterelemente, wird in CadElement überschrieben
		virtual void copy_uelementlist(const EnCObListModell& ue)
		{
			get_uelementList().deleteAllElemente();
			

			//Alle Listenelemente kopiern
			MxCObListIterator i(ue);
			while (i.next())
			{
				WaxBase *pBase = NULL;
				pBase = dynamic_cast<CadElement*>(get_uelementList().getAt(i));
				XASSERT(pBase);

				if (!pBase)
					continue;
				pBase = pBase->deepCopy();
				get_uelementList().append(pBase);
			}
		}


		virtual int get_laenge(bool waxcompatibel = true) const
		{
			if (is_rohr())
			{
				int lenele = typrohr.lenAsInt();
				if (waxcompatibel)
				{
					for (POSITION pos = get_uelementList().GetHeadPosition(); pos != NULL; )
					{
						const Element *e = dynamic_cast<const Element*>(get_uelementList().GetNext(pos));
						if (e)
							lenele += e->get_laenge();
					}
				}
				return lenele + get_rohrverlaengerung();
			}
			return Element::get_laenge();
		}

		int get_rohrverlaengerung() const
		{
			int rohrverl = 0;
			for (AnschlussListe::const_iterator it = anslst.begin(); it != anslst.end(); it++)
				rohrverl += it->rohrverlaengerung;

			return rohrverl;
		}
	
		int get_verbid(bool anfang = true) const
		{
			int ret = get_id(); // Elementid ist gleich Verbindungsid bei Flansch o.ä.
			if (is_rohr()) // Bei Rohr muss in der Anschlussliste geschaut werden
			{
				if ((!anslst.at(0).cadid && !anslst.at(1).cadid) ||
					(anslst.at(0).cadid && anslst.at(1).cadid))
				{
					if (anfang)
						ret = anslst.at(0).vbart;
					else
						ret = anslst.at(1).vbart;
				}
				else
				{
					if (!anslst.at(0).cadid)
						ret = anslst.at(0).vbart;
					else
						ret = anslst.at(1).vbart;			
				}
				switch (ret)
				{
				case 1:
					return NUT;
				case 2:
					return FFL;
				case 3:
				case 4:
					return GEW;
				default:
					return GLATT;
				}
			}	
			return ret > 0 ? ret : GLATT;
		}

		bool is_rohr() const
		{
			return typrohr.dn ? true : false;
		}
		typedef vector<CadModell::Anschluss> AnschlussListe;

		TypRohr typrohr; 

		AnschlussListe anslst; // Elemente des Hauptrohres Boegen Reduzierungen usw. 
		AnschlussListe abglst; // sind hier Muffen Bohrungen usw.

		__int64 cadid;

		CadModell::Point max;
		CadModell::Point min;

		Anschluss get_first_abgang() const
		{
			/**
			Sucht aus der Abgangsliste den ersten Abgang (kleinste X Koordinate)
			*/
			double dx = 99999;
			Anschluss ret;
			for (AnschlussListe::const_iterator xit = abglst.begin(); xit != abglst.end(); xit++)
			{
				if (xit->p.x < dx)
				{
					dx = xit->p.x;
					ret = *xit;
				}
			}
			return ret;
		}

		bool init_abschluss()
		{
			/**
			Das Ende eines Rohrstutzens als Abgang muss in der uelement Liste abgelegt werden. Auch für 
			Nut oder Glatt. Dann müssen in der uelement Liste ein Element Glatt oder Nut zu finden sein.
			*/
			if ((is_rohr() || get_id() == ROHR) && get_uelementList().isEmpty())
			{
				try
				{
					CadElement* new_endeele = new CadElement(get_verbid(), typrohr.dn, get_makeAnlage());
					new_endeele->set_winkel(get_winkel());
					get_uelementList().AddTail(new_endeele);
				}
				catch (Element::Error err)
				{
					TRACE("Ausnahmefehler: %s\n", err.get_errorText().c_str());
					return false;
				}
			}
			return true;
		}


#ifdef _DEBUG
		void dump() const
		{
			TRACE("Element: %s,  CADID %i,  %i Anschlüsse, %i Abgänge DN=%i Anschlussdn=%i\n", getBezeichnung(), cadid, anslst.size(), abglst.size(), get_dn(), get_anschlussDN());
		}
#endif
	private:
		/// Korrektur für CAD Elemente
		int korrektur_winkel;
	};
}