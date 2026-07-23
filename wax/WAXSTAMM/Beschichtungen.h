#pragma once

#include "StammDat.h"

/// Dateninhalte der Tabelle Beschichtungen
class WAXSTAMMDLL Beschichtungen : public StammDat  
 {
public:
	Beschichtungen();

	virtual void load(const std::string& db);

	CString getBezeichnung(int indx);
	
	class WAXSTAMMDLL data : 
public CObject  
	{
	public:
		const int get_afo1() const;

		const int get_afo2() const;

		const int get_afo3() const;

		const int get_afo4() const;

		const int get_afo5() const;

		data();

		int get_colour() const;

		int get_serial() const;

		const CString& get_bezeichnung() const;

		int get_index() const;

		int get_ral() const;

	private:
		int afo1;

		int afo2;

		int afo3;

		int afo4;

		int afo5;

		friend class Beschichtungen;


	/// RAL Nummer, falls verfügbar, sonst -1
		int ral;

	/// Position in Combobox, Tabellenfeld index
		int index;

	/// Index in der Farbtabelle (z.Zt. nur Voloview/DWF)
		int colour;

	/// Datenfeld serial: Primary Key für die Serialisierung, 
	/// sprachunabhängig
		int serial;

	/// Datenfeld bezDeutsch oder bezEnglish, je nach Ländereinstellung 
	/// des Programmes
		CString bezeichnung;

	};
};

