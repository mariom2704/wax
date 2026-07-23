#pragma once

#include "StammDat.h"

/**
Dateninhalte der Tabelle Brandschutztechniken
Die Daten werden durch <c BrandschutztechnikenSet> aus
der Datenbank geladen und dann in einer CObList mit Brandschutztechniken Objekten 
gespeichert.
*/
class WAXSTAMMDLL Brandschutztechniken : 
public StammDat  
{
public:
	virtual void load(const std::string& db);

	Brandschutztechniken();
	
	class WAXSTAMMDLL data : 
	public CObject  
		  
		{
		public:
			data();

			 int get_Nummer() const;

			const CString& get_bezeichnung() const;

			

		private:
		friend class Brandschutztechniken;

		/// Niederlassungsbezeichnung
			CString BezeichnungDeutsch;
			CString BezeichnungEnglisch;
			int Nummer;

		};


	

};

