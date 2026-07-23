#pragma once

#include "rohrleitung.h"
#include "cadmodell_cadelement.h"

namespace CadModell
{
	/**
	Rohrleitung, kann Hauptleitung sein oder Abgang.
	*/
	class Pipe
	{
	public:
		Pipe() { }
		
		Pipe(const Pipe& other)
		{
			*this = other;
		}

		bool m_Twisted;

		virtual ~Pipe()
		{
			for (vector<CadElement*>::iterator it = elelist.begin(); it != elelist.end(); it++)
				delete *it;
		}

		void operator = (const Pipe& other)
		{
			for (vector<CadElement*>::iterator it = elelist.begin(); it != elelist.end(); it++)
				delete *it;
			for (vector<CadElement*>::const_iterator it = other.elelist.begin(); it != other.elelist.end(); it++)
				elelist.push_back(new CadElement(**it));
		}

		
		vector<CadElement*> elelist;

		bool check_elelist_for_duplicates() const
		{
			/**
			Prüft die Elementliste auf Duplikate. Ein Duplikat ist ein Zeichen für eine Rekursion im CAD Modell.
			*/
			vector<__int64> cadid;
			for (vector<CadElement*>::const_iterator it = elelist.begin(); it != elelist.end(); it++)
			{
				for (vector<__int64>::iterator itc = cadid.begin(); itc != cadid.end(); itc++)
					if (*itc == (*it)->cadid)
						return true;
				cadid.push_back((*it)->cadid);
			}
			return false;
		}

		int get_rohrquali() const
		{
			/**
			Liefert die Rohrquali der Rohrleitung. Es wird das erste Rohrelement der Liste 
			benutzt.
			*/
			int rohrq = 0;
			for (vector<CadElement*>::const_iterator it = elelist.begin(); it != elelist.end(); it++)
			{
				CadElement *cur = *it;
				if (cur->is_rohr())
				{
					rohrq = cur->get_id();
					break;
				}
			}
			TRACE("rohrqualität = %i\n", rohrq);
			return rohrq >= 30 ? rohrq -30 : Rohrleitung::DEFAULT_ROHRQUALI;
		}

		void calc_absAbst()
		{
			int curabs = 0;
			for (vector<CadElement*>::iterator it = elelist.begin(); it != elelist.end(); it++)
			{
				CadElement* ele = *it;
				if (curabs > 0)
					curabs += ele->get_zmass();
				ele->set_abstAbs(curabs);
				
				if (ele->get_zmass())
					curabs += ele->get_zmass();
				else
					curabs += ele->get_laenge();
			}
		}
#if 0
		int normalize()
		{
			/**
			Dreht alle Elemente der Leitung so, wie von get_reference_vector vorgegeben
			und gibt den Winkel zurück.
			*/
			UnitVector rv = get_reference_vector();
			for (vector<CadElement*>::iterator it = elelist.begin(); it != elelist.end(); it++)
			{
				(*it)->set_korrektur_winkel(rv.get_angle_yz());
			}
			return rv.get_angle_yz();
		}
#endif
		int get_len() const
		{
			int len = 0;
			for (vector<CadElement*>::const_iterator it = elelist.begin(); it != elelist.end(); it++)
			{
				CadElement *e = *it;
				
				len += e->get_laenge();

				if (e->isEleBogen())
					len += e->get_laenge(); // Bei Bogen wird doppelte Länge addiert
			}
			return len;
		}
	
		int get_anfid(bool gewinderohr = false) const
		{
			const CadElement* e = *(elelist.begin());
			if (!e)
				return GLATT;

			// Bei Gewinderohr kann ein beliebiger Fitting am Anfang stehen wenn
			// Länge Null ist.
			if (gewinderohr && e->get_abstAbs() == 0 && !e->is_rohr())
				return GLATT;
			return e->get_verbid();
		}

		int get_endid() const
		{
			const CadElement* e = elelist.back();
			if (!e)
				return GLATT;
			return e->get_verbid(false);
		}

		bool is_empty() const
		{
			return elelist.empty();
		}

		bool init_relabst(bool gewinderohr = false)
		{
			/**
				Relativer Abstand eines Elementes im Rohr ist die Länge des Rohrstückes davor.
				Ausser bei Bogen, dann muss noch die Länge des Bogens addiert werden. 
				Bei Fitting wird noch das Z Mass addiert
			*/
			int relabst = 0;
	
			m_Twisted = FALSE;
			if (gewinderohr)
			{
				if(elelist.size() > 1)
				{
					
					vector<CadElement*>::iterator it = elelist.begin();
					int idFirst = (*it)->get_gruppenid();
					
					if( idFirst == FormstueckeUndFittinge::WINKEL || idFirst == FormstueckeUndFittinge::WINKEL_A1_45 ||
						idFirst == FormstueckeUndFittinge::WINKEL_A4)
					{
						reverse(elelist.begin(), elelist.end());
						m_Twisted = TRUE;
					}
					
				
					it++;
					int idSecond = (*it)->get_gruppenid();

					if( idFirst == FormstueckeUndFittinge::RED_NIPPEL && idSecond == FormstueckeUndFittinge::WINKEL || idSecond == FormstueckeUndFittinge::WINKEL_A1_45 ||
						idSecond == FormstueckeUndFittinge::WINKEL_A4)
					{
						reverse(elelist.begin(), elelist.end());
						m_Twisted = TRUE;
					}
			

					if(m_Twisted)
					{
						for (vector<CadElement*>::iterator it = elelist.begin(); it != elelist.end(); it++)
						{
							(*it)->set_winkel(360 - (*it)->get_winkel(true));
						}
					}
					
				}
			}


			for (vector<CadElement*>::iterator it = elelist.begin(); it != elelist.end(); it++)
			{
				CadElement* ele = *it;
				if (ele->is_rohr())
				{
					relabst += ele->get_laenge();
				}
				else if (ele->isFitting())
				{
					if (relabst > 0)
						ele->set_abstRel(relabst + ele->get_zmass());

					relabst = ele->get_zmass();
				}
				else
				{
					if (ele->isEleBogen())
						ele->set_abstRel(relabst + ele->get_laenge());
					else 
						ele->set_abstRel(relabst);
					relabst = ele->get_laenge();
				}

				if( ele->get_gruppenid() == FormstueckeUndFittinge::RED_NIPPEL || ele->get_gruppenid() == FormstueckeUndFittinge::DOPPELNIPPEL )
				{
					ele->set_abstRel(0);
				}
				
			}
			return true;
		}

		bool is_mainpipe_ele(const CadElement* ele, bool gewinderohr) const
		{
			/**
			Gibt true zurück, wenn Element ein Element im Hauptstrang ist.
			Das können Bogen oder Einziehungen sein. Anfangelement und Endeelement,
			sowie Rohre gehören nicht dazu.
			*/
			if (!gewinderohr)
			{
				int anfid = get_anfid();
				int endid = get_endid();

				if (ele == (*elelist.begin()) && anfid == ele->get_id())
					return false;
				if (ele == elelist.back() && endid == ele->get_id())
					return false;
			}	
			if (ele->is_rohr())
				return false; // Rohre gibt es nicht im Hauptstrang, nur die Stranglänge
			return true; 
		}
		
#ifdef _DEBUG
	public:
		virtual void dump() const
		{
			TRACE("Pipe: Länge %i, Anfid %i, Endid %i\n", get_len(), get_anfid(), get_endid());
			for (vector<CadElement*>::const_iterator it = elelist.begin(); it != elelist.end(); it++)
				(*it)->dump();
		}
#endif
	private:

		UnitVector reference_vector;

		UnitVector get_reference_vector() const
		{
			if (is_empty() || !reference_vector.compare(UnitVector()))
				return reference_vector;

			for (vector<CadElement*>::const_iterator it = elelist.begin(); it != elelist.end(); it++)
			{
				const CadElement* const ele = *it;
				if (ele->isEleBogen()) // Wenn die ersten Elemente nach dem Bogen kommen, ändern wir nichts am Winkel
					break;
				if (ele->is_rohr() && !ele->abglst.empty())
				{
					return ele->get_first_abgang().r;
				}

			}
			return UnitVector();
		}
#ifdef _DEBUG
	public:
		void dump_list() const;
#endif
		
	};
}