#pragma once


#include "../rohrkonst/cadwerkauftrag.h"

#include "../waxprj/WaxPart.h"
#include "../rohrkonst/element.h"
#include "../waxui/VerteilerDaten.h"
#include "../waxui/StrangrohrData.h"
#include "../waxui/GewinderohrData.h"
#include "../waxui/Apax.h"
#include "../waxui/CadWerkauftragData.h"

#include <vector>

namespace ServerInterface
{
	class GpCadConvert
	{
	private:
		typedef std::vector<WaxPart*>::iterator WpIterator;
	public:

		GpCadConvert(){}
		/**
		Umwandlung aller CadAufträge in Verteiler, Strangrohr usw., soweit möglich.

		*/
		CString convert(std::vector<WaxPart*>& wplst) const
		{
			CString faultMessageReturn = "";
			for (WpIterator it = wplst.begin(); it != wplst.end(); it++)
			{
				CString faultMessage = "";
				if (!(*it)->get_werkauftrag()->is_native_cadpos())
					continue;


				// Alle CADWerkaufträge werden umgewandelt
				if ((*it)->get_werkauftrag()->isHandelslaenge())
				{
					if (!do_conversion<HandelslaengeData>(it, faultMessage))
						continue;
				}
				// Alle CADWerkaufträge werden umgewandelt
				else if ((*it)->get_werkauftrag()->isSdMaterialNrVerteiler())
				{
					if (!do_conversion<VerteilerData>(it, faultMessage))
						continue;
				}
				else if ((*it)->get_werkauftrag()->isSdMaterialNrStrangrohr())
				{
					if (!do_conversion<StrangrohrData>(it, faultMessage))
						continue;

					if(!faultMessage.IsEmpty())
					{
						faultMessageReturn += faultMessage;
					}
					// Falls am Anfang kein Standardelement ist, wird die Rohrleitung gespiegelt
					Rohrleitung *r = (Rohrleitung*)((*it)->get_werkauftrag());

					Element *e = NULL;
					if (r->get_elementArray().GetSize() > 0)
						e = r->get_elementArray().get_element_at(0);
					if (e)
					{
						if (e->get_id() != MUFFE && e->get_id() != BOHR24 && e->get_id() != BOHR50 && e->get_id() != SONDERMUFFE)
							r->elementeingabe_invertieren();
					}
				}
				else if ((*it)->get_werkauftrag()->isSdMaterialNrGewinderohr())
				{
					if (!do_conversion_gewinderohr(it))
						continue;
				}
				else if ((*it)->get_werkauftrag()->isSdMaterialNrApax())
				{
					if (!do_conversion_apax(it))
						continue;
				}
			}
			return faultMessageReturn;
		}

	private:
		template <class TROHR>
		bool do_conversion(WpIterator it, CString& faultMessage) const
		{
			TROHR* vd = new TROHR;
			vd->get_werkauftrag()->set_cadpos(true);
			CadWerkauftrag* dtw = get_cadwa(it);
			if (!dtw)
				return false;
			
			Rohrleitung* r = (Rohrleitung*)vd->get_werkauftrag();
			faultMessage = r->copy(*dtw); // virtuelles copy macht die eigentliche Konvertierung
			replace(it, vd);
			return true;
		}

		bool do_conversion_gewinderohr(WpIterator it) const
		{
			GewinderohrData* vd = new GewinderohrData;
			vd->get_werkauftrag()->set_cadpos(true);

			CadWerkauftrag* dtw = get_cadwa(it);
			if (!dtw)
				return false;
			
			Gewinderohr* r = (Gewinderohr*)vd->get_werkauftrag();
			r->copy(*dtw); // virtuelles copy macht die eigentliche Konvertierung
			if (dtw->get_dichtmittel()) // Wenn kein Dichtmittel gesetzt ist, auf Default lassen
				r->set_dichtmittel(dtw->get_dichtmittel());
			replace(it, vd);
			return true;
		}

		bool do_conversion_apax(WpIterator it) const
		{
			ApaxData* vd = new ApaxData;
			vd->get_werkauftrag()->set_cadpos(true);
			
			CadWerkauftrag* dtw = get_cadwa(it);
			if (!dtw)
				return false;
		
			vd->get_werkauftrag()->copy(*dtw); 
			replace(it, vd);
			return true;
		}

		CadWerkauftrag* get_cadwa(WpIterator it) const
		{
			CadWerkauftragData* cadd = dynamic_cast<CadWerkauftragData*>(*it);
			if (!cadd)
				return NULL;
			CadWerkauftrag* dtw = (CadWerkauftrag*)cadd->get_werkauftrag();
			dtw->create_elementarray();
			return dtw;
		}

		void replace(WpIterator it, WaxPart* newwp) const
		{
			/**
			Ersetzt das Objekt in it durch newwp
			*/
			WaxPart* oldwp = *it; 
			*it = newwp;
			delete oldwp;
		}

	};
}