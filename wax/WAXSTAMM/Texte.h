#pragma once
#include "stammdat.h"

class WAXSTAMMDLL Texte :
	public StammDat
{
public:
	Texte(void);

	virtual ~Texte();

	virtual void load(const std::string& id);

	const std::string& getText(const std::string& id, bool nullIfNotFound = false);

	class WAXSTAMMDLL data : 
public CObject  
	{
	public:
		data();

	
		const std::string& get_bezeichnung()const;
		const std::string& get_id()const;

	private:
		std::string id;

		std::string bezeichnung;

	friend class Texte;
	};


private:
	std::string mDb;

	std::string getSql(const std::string& sprache, const std::string& id);

	void setLineFeed(std::string& val);
	void setTab(std::string& val);

};
