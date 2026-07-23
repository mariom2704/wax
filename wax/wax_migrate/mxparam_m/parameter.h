#pragma once

#pragma warning(disable:4996)

#include <string>

/**
Abstrakte Klasse für einen Parameter
*/
class Parameter
{
public:
	/// Gültigkeitsbereich des Parameters
	enum VALID_NAMESPACE
	{
		INVALID = 0,
		USER = 1,
		MACHINE = 2,
		LOCATION = 4,
	};
	enum
	{
		UNDEFINED = -9999, /// Defaultwert für nicht initialisierte Intwerte
	};
	///Konstruktor
	Parameter(int para_key);
	
	///Destruktor
	virtual ~Parameter();

	/// Laden des Parameter, woher auch immer
    virtual void load() = 0;

	/// Speichern des Parameters
	virtual void save() = 0;

	/// Liefert true wenn der Parameter nicht bearbeitet werden darf
	virtual bool is_readonly() const = 0;

	/// Liefert den aktuellen Gültigkeitsbereich
	inline enum VALID_NAMESPACE get_valid_namespace() const;

	/// Liefert alle möglichen Gültigkeitsbereiche für diesen Parameter
	virtual int valid_namespaces() const = 0;

	/// Setzt den aktuell möglichen Gültigkeitsbereich
	bool set_valid_namespace(enum VALID_NAMESPACE namesp);
	
	/// Liefert den Defaultwert als std::string
	inline std::string get_def_str() const;

	/// Liefert den Defaultwert als Integer
	inline int get_def_int() const;

	/// Liefert den aktuellen Wert als std::string
	virtual std::string get_val_str() const;

	/// Liefert den aktuellen Wert als Int
	virtual int get_val_int() const;

	/// Setzen des std::stringwertes
	inline void set_val(LPCSTR val);

	/// Setzen des Integerwertes
	inline void set_val(int val);

	/// Setzen des Integerwertes auf bool
	inline void set_val(bool val);

	/// Gibt den Namen des Parameters zurück. Unter diesem Namen ist der Parameter irgendwo abgespeichert.
	inline std::string get_entry() const;

	/// Setzt die Beschreibung
	inline void set_bemerkung(const std::string& val);

	/// Setzt das AV Flag
	inline void set_av(bool val);

	/// Liefert das AV Flag
	inline bool get_av() const;

	std::string getBemerkung() const;

protected:
	/// Gültigkeitsbereich
	enum VALID_NAMESPACE nmspace;

	/// Der Parameterkey als Int-Wert für den Zugriff aus dem Programm
	int key;

	/// Parameterschlüssel als std::string für die Speicherung
	std::string entry;

	/// Defaultwert als std::string
	std::string def_val_str;

	/// Defaultwert als Int
	int def_val_int;

	/// Aktueller Wert als std::string
	mutable std::string val_str;

	/// Aktueller Wert als Int
	mutable int val_int;

	/// Flag, ob der Parameter ein Int oder ein std::string Wert ist
	bool is_string;

	/// Beschreibung des Systemparameters
	std::string bemerkung;

	/// True, wenn nur in der AV relevant
	bool av;
};

inline std::string Parameter::get_def_str() const
{
	if (!is_string)
	{
		char tmp[50];
		sprintf(tmp, "%i", get_def_int());
		return tmp;
	}
	return def_val_str;
}

inline int Parameter::get_def_int() const
{
	if (is_string)
		return atoi(def_val_str.c_str());
	return def_val_int;
}

inline void Parameter::set_val(LPCSTR val)
{
	val_str = val;
	val_int = atoi(val);
}

inline void Parameter::set_val(int val)
{
	val_int = val;
	char tmp[255];
	sprintf(tmp, "%i", val);
	val_str = tmp;
}

inline void Parameter::set_val(bool val)
{
	val_int = val ? 1 : 0;
	val_str = val_int ? "true" : "false";
}

inline std::string Parameter::get_entry() const
{
	return entry;
}


inline void Parameter::set_av(bool val)
{
	av = val;
}

inline bool Parameter::get_av() const
{
	return av;
}

inline void Parameter::set_bemerkung(const std::string& val)
{
	bemerkung = val;
}


inline enum Parameter::VALID_NAMESPACE Parameter::get_valid_namespace() const
{
	return nmspace;
}

