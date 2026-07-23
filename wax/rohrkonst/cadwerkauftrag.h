#pragma once

#include "rohrleitung.h"

namespace CadModell
{
	class Modell;
	class WaxCadModell;
}

/**
CAD Werkauftrag aus dem Rohrnetzkonfigurator
*/
class ROHRKONSTDLL CadWerkauftrag : public Rohrleitung  
{
public:
	/// Konstruktor
	CadWerkauftrag();

	/// Destruktor
	virtual ~CadWerkauftrag();

	virtual const CString getBemassungstext() const;

	/// /vk3 Liste erstellen
	virtual bool createVk3Liste();

	/// Speichern und Laden der Rohrleitung. Achtung:
	virtual void serialize(MxFilesystem::BinaryArchive& ar);

	/// Rohrkonstruktion erzeugen
	virtual void createModell();

	/// Hier wird automatisch der Arbeitsplan generiert
	virtual void createAutoArbeit(bool beschichtung = true);

	/// Rohrkonstruktion auf korrekte Werte checken
	virtual CString checkPlausis();

	void set_xml_modell(const string& xml);

	virtual string get_xml_modell() const { return xml_modell; }

	void create_elementarray();

	virtual bool isGewinderohr() const;
	virtual bool isHandelslaenge() const;
	virtual bool isVerteiler() const;
	virtual bool isStrangrohr() const;
	

	void set_dichtmittel(int dm);

	int get_dichtmittel() const;

	virtual bool is_native_cadpos() const { return true; }



protected:
	
	virtual void check_elementArray();
	
	void init_element_array(CadModell::WaxCadModell& lst);

	void init_infos(const CadModell::WaxCadModell& lst);

private:

	bool m_bTwisted;
	/// Setzen der DN der Rohrleitung
	void scan_dnvl(const CadModell::WaxCadModell& celist, const EnContainerRohrElemente& eleArray);

	void set_rohrdaten(const CadModell::Modell& lst);

	/// Datenmodell aus der Datenbank
	string xml_modell;
	
	/// True wenn Auftrag Gewinderohr ist (andere Grafik)
	bool gewinderohr;

	int dichtmittel;

	bool handelslaenge;
};
