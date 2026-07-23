#pragma once

#include "DialogHtmlDruck.h"


namespace WaxDruck
{
	/**
	Basisklasse für DialogDrucklisten
	*/
	class WAXPRJDLL DialogDruckliste
	{
	public:

		DialogDruckliste(DialogHtmlDruck& file, WaxPart* pt);
		DialogDruckliste(DialogHtmlDruck& file, CObList* ptWerkauf);	
		virtual ~DialogDruckliste();

		/// Muss in abgeleiteten Klassen überschrieben werden
		virtual bool create(Werkauftrag* werkauf) = NULL;

		/// Sortiert eine Liste mit Rohrelementen+Anfang&Ende auf- oder absteigend nach Laenge
		static void sort_liste(CObList &liste, bool aufsteigend = true);

		int get_AnzahlRohreProPos();
		

		static bool loadResource(CString logo, HRSRC rclog, HMODULE module = NULL);
	protected:

		bool dn2zoll;

		/// Maximale Länge der Rohre 
		int maxlaenge;

		WaxPart* waxpart;

		typedef vector<Werkauftrag*>  Auftragsliste;

		/// die Aufträge durchzuarbeiten geht mit einem Vector eleganter
		Auftragsliste auftraege;
		
		DialogHtmlDruck& html;	

		void insert_rohr_quali(const Rohrleitung* rohr, int stutzenAnlage2440);

		/// mehrere Rohrqualiäten im Auftrag
		bool twoquali;

	private:
		
		/// Kopiert die Auftragspointer in den vector auftraege
		void init_auftragsliste();

		/// liefert die grösste Rohrlänge 
		int scan_maxlaenge() const;
	};
}
