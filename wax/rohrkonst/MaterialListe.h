#pragma once

#include "MxCOblist.h"

/// Liste mit Material
class ROHRKONSTDLL MaterialListe 
{
public:
	/// Sortiert die uebergebene Liste nach ID
	void sortMaterial();
	
	/// Sortiert die uebergebene Liste nach ID
	void sortMaterial(CObList &matliste);
	
	/// Liefert alle Profile aus der Materialliste. Falls keine gefunden wurden, 
	/// ist die Liste unverändert und es wird false zurückgeliefert. Die Elemente 
	/// der Liste dürfen nicht gelöscht werden.
	bool get_profilliste(CObList &proflist, int profilkennung);

	/// Löscht alle Materialobjekte aus der Materialliste
	void clean();

	/// Lädt das Material aus einem alten Wax Arbeitsplan
	bool serializeOldApax(MxFilesystem::BinaryArchive& ar);

	/// Liefert das Gesamtgewicht des Materials
	double get_gewicht(int teile = 0, bool anz = false) const;

	MaterialListe();

	virtual ~MaterialListe();

	bool isEmpty() const;

	MxCObList& get_matlist();

	const MxCObList& get_matlist() const;

	void serialize(MxFilesystem::BinaryArchive& ar);

	/// / Zusammenführen zweier Materiallisten
	void merge(const MaterialListe& other);

	void copy(const MaterialListe& other);

private:

	MxCObList matlist;

};

