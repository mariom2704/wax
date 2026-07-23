#pragma once

#include "drucklistenverwaltung.h"
namespace WaxDruck
{
	class GewinderohrrucklisteZuschnitt :
		public DrucklisteZuschnitt
	{
	public:
		GewinderohrrucklisteZuschnitt(HtmlDruck& file, WaxPart* pt, CObList* pWalist, int id, DrucklistenVerwaltung* pDrucklistenVerwaltung);
		~GewinderohrrucklisteZuschnitt(void);
		virtual bool create();
		void createGewinderohrlaengen();
		void create_Gewinderohrkopf();
		void createGewinderohrliste();

	private:
		CObList* m_pWalist;
		DrucklistenVerwaltung* m_pDrucklistenVerwaltung;

	};
}
