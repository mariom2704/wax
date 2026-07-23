#pragma once

#include "druckliste.h"
#include "maschinenlisten.h"

namespace WaxDruck
{
	class DrucklisteZuschnitt :	public Druckliste
	{
	public:
		DrucklisteZuschnitt(HtmlDruck& file, WaxPart* pt, int id);

		virtual ~DrucklisteZuschnitt();

		virtual bool create();

		

		enum SCHWEISSUNG
		{
			both = -1,
			anlage = 1,
			hand = 0,
		};

		/// Fügt alle Zuschnittsdrucklisten hinzu, die ausgegeben werden können (weil in der betreffenden Maschinenliste
		/// mindestens ein Eintrag vorhanden ist)
		void add_drucklisten(vector<int>& menuitems) const;

	protected:

		/// Liefert die zur Druckliste id gehörige Maschinenliste für die Druckausgabe
		CObList* listefromid(int id);
		/// ID der auszugebenen Zuschnittsliste
		int listid;
	private:

		
		/// Schweissung per Hand, Anlage oder beides, siehe enum SCHWEISSUNG
		int schweissung;
		
		Maschinenlisten maschinenlisten;

		

		/// Durchläuft die Materialisten der Aufträge um eine Rohrleitung zu finden
		Rohrleitung* scan_rohr(bool& mehrereRohrquali);

		/// Rohr aus Materialliste für APAX Liste
		Rohrleitung* default_rohr;
	};
}
