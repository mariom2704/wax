#pragma once

#include "StammDat.h"

/**
Dateninhalte der Tabelle Niederlassungen
Die Daten werden durch <c NiederlassungenSet> aus
der Datenbank geladen und dann in einer CObList mit Niederlassungen Objekten 
gespeichert.
*/
class WAXSTAMMDLL Niederlassungen : 
public StammDat  
{
public:
	virtual void load(const std::string& db);

	Niederlassungen();
	
	class WAXSTAMMDLL data : 
	public CObject  
		  
		{
		public:
			data();

			 int get_nlnummer() const;

			const CString& get_bezeichnung() const;

			 int get_nRegion() const;

		private:
		friend class Niederlassungen;

		/// Niederlassungsbezeichnung
			CString bezeichnung;

		/// Ib/Nl Nummer
			int nlnummer;

			int region;

		};


	

};

