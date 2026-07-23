#pragma once

#include "parameter.h"

class RegParameter : public Parameter
{
public:
	///Konstruktor für einen std::stringparameter
	RegParameter(int parakey, const std::string& regordner, const std::string& regentry, const std::string& def_str, bool do_load = true);
	
	/// Konstruktor für einen Intparameter
	RegParameter(int parakey, const std::string& regordner, const std::string& regentry, int def_int, bool do_load = true);
	
	///Destruktor
	~RegParameter();

	/// Laden des Parameter, woher auch immer
    virtual void load();

	/// Speichern des Parameters
	virtual void save();

	/// Liefert true wenn der Parameter nicht bearbeitet werden darf
	virtual bool is_readonly() const;

	/// Mögliche Gültigkeitsbereiche
	virtual int valid_namespaces() const;

protected:
	/// Parametergruppe
	std::string ordner;
};

