#pragma once


#include "cadmodell.h"


namespace CadModell
{
	class MainPipe
	{
	public:
		MainPipe()
		{
		}

		MainPipe(const MainPipe& other)
		{
			*this = other;
		}

		virtual ~MainPipe()
		{
			for (vector<CadElement*>::iterator it = elelist.begin(); it != elelist.end(); it++)
				delete *it;
		}

		void operator = (const MainPipe& other)
		{
			for (vector<CadElement*>::iterator it = elelist.begin(); it != elelist.end(); it++)
				delete *it;
			for (vector<CadElement*>::const_iterator it = other.elelist.begin(); it != other.elelist.end(); it++)
				elelist.push_back(new CadElement(**it));
		}

		vector<CadElement*> elelist;

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

		int get_len() const
		{
			int len = 0;
			for (vector<CadElement*>::const_iterator it = elelist.begin(); it != elelist.end(); it++)
			{
				len += (*it)->get_laenge();
			}
			return len;
		}
	
		int get_anfid() const
		{
			const CadElement* e = *(elelist.begin());
			return e ? e->get_verbid() : GLATT;
		}

		int get_endid() const
		{
			const CadElement* e = elelist.back();
			return e ? e->get_verbid() : GLATT;
		}

		bool is_empty() const
		{
			return elelist.empty();
		}

		bool init_relabst()
		{
			/**
				Relativer Abstand eines Elementes im Rohr ist die Länge des Rohrstückes davor.
				Ausser bei Bogen, dann muss ab dem zweiten Bogen noch die Länge des Bogens addiert 
				werden. 
			*/
			int relabst = 0;
			bool bogen_detected = false;
			for (vector<CadElement*>::iterator it = elelist.begin(); it != elelist.end(); it++)
			{
				CadElement* ele = *it;
				if (ele->is_rohr())
				{
					relabst += ele->typrohr.len();
				}
				else
				{
					if (bogen_detected && ele->isEleBogen())
						ele->set_abstRel(relabst + ele->get_laenge());
					else
						ele->set_abstRel(relabst);
					relabst = ele->get_laenge();
				}
				if (!ele->is_rohr())
					bogen_detected = ele->isEleBogen();
			}
			return true;
		}

		UnitVector start_direction() const
		{
			const CadElement* e = *(elelist.begin());
			if (e && e->anslst.size())
			{
				if (e->anslst[0].cadid)
					return e->anslst[0].r;
				else if (e->anslst.size() > 1)
					return e->anslst[1].r;
			}

			return UnitVector();
		}

		
#ifdef _DEBUG
	public:
		virtual void dump() const
		{
			TRACE("Mainpipe: Länge %i, Anfid %i, Endid %i\n", get_len(), get_anfid(), get_endid());
			for (vector<CadElement*>::const_iterator it = elelist.begin(); it != elelist.end(); it++)
				(*it)->dump();
		}
#endif
	private:
		UnitVector reference_vector;

		UnitVector get_reference_vector() const
		{
			if (is_empty() || reference_vector != UnitVector())
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
	};
}