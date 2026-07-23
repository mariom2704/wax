#pragma once

#include <vector>
#include "../waxprj/WaxPart.h"
#include "../waxprj/GpSapAuftragsdaten.h"



namespace ServerInterface
{

	class GpSammelauftragCreator
	{		

	public:
		
		enum ORDER
		{
			SdNummer = 0,
			Preisanfrage = 1
		};

		/**
		Erstellt aus einer flachen Liste von einfachen Werkaufträgen eine
		Liste mit Sammelaufträgen
		*/
		vector<WaxPart*> create(vector<WaxPart*>& flatlist, GpSammelauftragCreator::ORDER order, ServerDBOpen& WaxDB) const
		{
			/*
			Alle Werkaufträge mit der selben Positions, Auftragsnummer und PSP Element werden
			zu einem Sammelauftrag zusammengefasst.
			*/
			vector<WaxPart*> SammelauftragCollection;


			int sdMatNr;
			for (vector<WaxPart*>::iterator it = flatlist.begin(); it != flatlist.end(); it++)
			{
				WaxPart* pt = *it;
				if (!pt)
					continue;
				vector<WaxPart*> tmp;
				tmp.push_back(pt);
				sdMatNr = pt->get_werkauftrag()->get_leitkarte().get_sdMaterialnummer();

				// Jetzt wird die Liste nach weiteren Teilen durchsucht
				for (vector<WaxPart*>::iterator it2 = flatlist.begin(); it2 != flatlist.end(); it2++)
				{
					WaxPart *testpt = *it2;
					if (!testpt || testpt == pt)
						continue;
					
					if (order == GpSammelauftragCreator::Preisanfrage)
					{
						if (pt->get_werkauftrag()->get_leitkarte().get_preisanfrage().get_nummer() == testpt->get_werkauftrag()->get_leitkarte().get_preisanfrage().get_nummer() &&
							pt->get_werkauftrag()->get_leitkarte().get_preisanfrage().get_position() == testpt->get_werkauftrag()->get_leitkarte().get_preisanfrage().get_position())
						{
							tmp.push_back(testpt);
							*it2 = NULL;
						}
					}
					else
					{
						
						if(!pt->get_werkauftrag()->get_leitkarte().get_ippeMatNr().IsEmpty()&& !testpt->get_werkauftrag()->get_leitkarte().get_ippeMatNr().IsEmpty())
						{
							if(pt->get_werkauftrag()->get_leitkarte().get_IsChoosenIppeNumbers() == testpt->get_werkauftrag()->get_leitkarte().get_IsChoosenIppeNumbers() && 
							pt->get_werkauftrag()->get_leitkarte().get_ippeMatNr() == testpt->get_werkauftrag()->get_leitkarte().get_ippeMatNr()&&
							pt->get_werkauftrag()->get_leitkarte().get_prj_pspElement() == testpt->get_werkauftrag()->get_leitkarte().get_prj_pspElement()
							)
							{
								tmp.push_back(testpt);
								*it2 = NULL;
							}
		
						}
						else
						{
							if (pt->get_werkauftrag()->get_leitkarte().get_sdPositionsnr() == testpt->get_werkauftrag()->get_leitkarte().get_sdPositionsnr() &&
								pt->get_werkauftrag()->get_leitkarte().get_sdAuftragsnr() == testpt->get_werkauftrag()->get_leitkarte().get_sdAuftragsnr() &&
								pt->get_werkauftrag()->get_leitkarte().get_prj_pspElement() == testpt->get_werkauftrag()->get_leitkarte().get_prj_pspElement()&&
								pt->get_werkauftrag()->get_GUID() == testpt->get_werkauftrag()->get_GUID())
							{
								tmp.push_back(testpt);
								*it2 = NULL;
							}
							
						}
					}
				}

				//Bei Sammelauftrag Strangrohr die Aufträge nach länge sortieren
				if (sdMatNr == Leitkarte::StrangrohrGepulvertVerzinkt)
					sortNachLaenge(&tmp);
			
		
				// Alle Teile des Sammelauftrages sind in tmp gespeichert
				WaxPart *sammelauftrag = new SammelAuftrag;
				SammelauftragCollection.push_back(sammelauftrag);

				for (vector<WaxPart*>::iterator sit = tmp.begin(); sit != tmp.end(); sit++)
					sammelauftrag->addChild(*sit, false);
				
				// Sammelauftrag bekommt das Freigabedatum der 1. Pos., 
				// da der Sammel nach der Freigabe auch von dieser das Datum erhaelt.
				WaxPart* erster_sub = sammelauftrag->getLastSubobject();
				sammelauftrag->get_leitkarte().clone(erster_sub->get_leitkarte());
				sammelauftrag->get_werkauftrag()->get_status().copy(erster_sub->get_werkauftrag()->get_status());
				sammelauftrag->get_werkauftrag()->get_beschichtung().copy(erster_sub->get_werkauftrag()->get_beschichtung());
				sammelauftrag->get_werkauftrag()->set_bemerkung(erster_sub->get_werkauftrag()->get_bemerkung());
				sammelauftrag->get_werkauftrag()->set_sichtbereich(erster_sub->get_werkauftrag()->get_sichtbereich());
				sammelauftrag->get_werkauftrag()->set_zeugnis(erster_sub->get_werkauftrag()->get_zeugnis());
				sammelauftrag->get_werkauftrag()->set_erstellungsdatum(erster_sub->get_werkauftrag()->get_erstellungsdatum());
				sammelauftrag->get_werkauftrag()->set_KollilisteExport(erster_sub->get_werkauftrag()->get_KollilisteExport());
			
				
			}
			return SammelauftragCollection;
		}

		/**
			Liste nach Länge sortieren. Längstes Rohr steht als erstes in der Liste
		*/
		void sortNachLaenge(vector<WaxPart*> *auftraege)const
		{
			vector<WaxPart*> tmp;
			
			for (vector<WaxPart*>::iterator sit = auftraege->begin(); sit != auftraege->end(); sit++)
			{
				WaxPart* wp = *sit;			
				Rohrleitung* rohr = dynamic_cast<Rohrleitung*>(wp->get_werkauftrag());
				if (!rohr)
				{
					tmp.insert(tmp.begin(), wp);
					continue;
				}
				
				int laenge = rohr->getStandardRohrlaenge();
				int pos = 0;

				//Position suchen wo der Auftrag eingefügt werden kann
				for (vector<WaxPart*>::iterator it = tmp.begin(); it != tmp.end(); it++)
				{
					WaxPart* wp2 = *it;			
					Rohrleitung* rohr2 = dynamic_cast<Rohrleitung*>(wp2->get_werkauftrag());
					if (!rohr2)
					{
						pos++;
						continue;
					}
					
					int laenge2 = rohr2->getStandardRohrlaenge();
					if (laenge2 <= laenge)
						break;

					pos++;
				}

				tmp.insert(tmp.begin() + pos, wp);
			}

			//Liste übertragen
			auftraege->clear();
			for (vector<WaxPart*>::iterator it = tmp.begin(); it != tmp.end(); it++)
				auftraege->push_back(*it);
		}
	};
}