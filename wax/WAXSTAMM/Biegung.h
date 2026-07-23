#pragma once

#include "StammDat.h"

class WAXSTAMMDLL Biegung : public StammDat
{
public:
	Biegung(void);

	/// Laden der Daten aus der Stammdatenbank, Tabelle Bauelemente
	virtual void load(const std::string& db);

	bool getAbstaende(long dn, long rohrId, long& minWinkel, long& maxWinkel, long& minAbst, long& minEleAbst);

	bool getRadRolle(long dn, long rohrId, long& rad);

	/// Datenfelder der Tabelle Bauelemente
	class WAXSTAMMDLL data : 
	public CObject  
		  
		{
		public:
			data();

			const long get_dn() const;

			const long get_rohrId() const;

			const long get_minWinkel() const;

			const long get_maxWinkel() const;

			const long get_minAbst() const;

			const long get_minEleAbst() const;

			const long get_radRolle() const;

		private:
			long dn;

			long rohrId;

			long maxWinkel;

			long minWinkel;

			long minAbst;

			long minEleAbst;

			long radRolle;
		
		friend class Biegung;
		};
};
