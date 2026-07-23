#include "stdafx.h"

#include "material.h"
#include "MaterialListe.h"
#include "Element.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif 

MaterialListe::MaterialListe()
{
}

MaterialListe::~MaterialListe()
{
	clean();
}

bool MaterialListe::isEmpty() const
{
	return matlist.IsEmpty() ? true : false;
}

MxCObList& MaterialListe::get_matlist()
{
	return matlist;
}

const MxCObList& MaterialListe::get_matlist() const
{
	return matlist;
}

void MaterialListe::serialize(BinaryArchive& ar)
{
	if (ar.is_storing())
	{
		for (POSITION pos = matlist.GetHeadPosition();pos;)
		{
			ar << (int)1;
			((Material*)matlist.GetNext(pos))->serialize(ar);
		}
		ar << (int)0;
	}
	else
	{
		clean();
		int dummy;
		ar >> dummy;
		while (dummy)
		{
			Material *mat = new Material;
			mat->serialize(ar);
			matlist.AddTail(mat);
			ar >> dummy;
		}
	}
}

void MaterialListe::copy(const MaterialListe& other)
{
	// Materialliste kopieren
	clean();
	merge(other);
}

void MaterialListe::merge(const MaterialListe &other)
{
	const CObList& mlist = other.get_matlist();
	for (POSITION pos = mlist.GetHeadPosition(); pos != NULL;)	
	{
		const Material *material = dynamic_cast<const Material*>(mlist.GetNext(pos));
		XASSERT(material);
		if (material)
		{
			Material* mat = new Material;
			mat->copy(*material);
			matlist.AddTail(mat);
		}
	}
}

//Löscht alle Materialobjekte aus der Materialliste
void MaterialListe::clean()
{
	matlist.deleteAllElemente();
}



//Liefert das Gesamtgewicht des Materials
double MaterialListe::get_gewicht(int teile, bool anz) const
{
	// teile : 0=alle, 1=Einkauf, 2=Lager, 3=ohne Preis
	double gew = 0.0;
	for (POSITION pos = matlist.GetHeadPosition(); pos;)
	{
		const Material *m = dynamic_cast<const Material *>(matlist.GetNext(pos));
		XASSERT(m);
		if (!m)
			break;
		
		if (teile == 1 &&  m->get_einkauf()
		 || teile == 2 && !m->get_einkauf()
		 || teile == 3 &&  m->get_festPreis() == 0
		 || teile == 0) 
			gew += m->getGewicht(anz);
	}
	return gew;
}

#ifdef __OLDAPAX
// PR 31.3.2008 Erstmal deaktiviert
//Lädt das Material aus einem alten Wax Arbeitsplan
bool MaterialListe::serializeOldApax(BinaryArchive& ar)
{
	// Filepointer zurücksetzen und temporäres Archiv anlegen, da sonst nicht auf den Anfang zurückgegriffen
	// werden kann (MFC Bug)
	ar.GetFile()->SeekToBegin();
	BinaryArchive xar(ar.GetFile(), BinaryArchive::load);

	CString cont;
	xar.ReadString(cont);
	if (cont.Find("#APX:") == -1)
		return false; // Keine Waxdatei

	clean();

	while (cont.Left(2) != "**")
	{
		if (!xar.ReadString(cont))
			break;
	}
	
	do 
	{
		if (cont.GetLength() > 21)
		{
			long artn = atoi(cont.Mid(31, 6));
			
			Material *mat = new Material;
			if (artn)
			{
				mat->set_artikelnr(artn);
			}
			mat->set_bezeichnung(cont.Left(31));
			mat->set_anzahl(atoi(cont.Right(2)));
			matlist.AddTail(mat);
		}
	} while (xar.ReadString(cont));
	return true;
}
#endif


//Liefert alle Profile aus der Materialliste. Falls keine gefunden wurden, ist 
//die Liste unverändert und es wird false zurückgeliefert. Die Elemente der Liste 
//dürfen nicht gelöscht werden.
bool MaterialListe::get_profilliste(CObList &proflist, int profilkennung)
{
	int count = 0;
	for (POSITION pos = matlist.GetHeadPosition(); pos;)
	{
		Material *mat = dynamic_cast<Material*>(matlist.GetNext(pos));
		XASSERT(mat);
		if (!mat)
			break;

		int matkennung = mat->get_profilkennung();
		if (matkennung & profilkennung)
		{
			proflist.AddTail(mat);
			count++;
		}
	}
	TRACE("%i Profile in Materialliste\n", count);
	return count ? true : false;
}

///Sortiert die Materialliste
void MaterialListe::sortMaterial()
{
	sortMaterial(matlist);
}

///Sortiert die Materialliste
void MaterialListe::sortMaterial(CObList &matliste)
{
	///Todo: sortieren nach GruppenID (Attribut nach Material verschieben) statt Atikelnr. 
	///od. einfach nur alphabetisch
	///jetzt erst nach ID, dann alphabetisch 
	POSITION pos, nextpos; 
   
	// Erst sortieren...
	int i;
	for (i = 0; i < matliste.GetCount()-1; i++)	
	{
		pos = matliste.FindIndex(i);
		Material *first = dynamic_cast<Material *>(matliste.GetAt(pos));
		nextpos = matliste.FindIndex(i+1);
		Material *second = dynamic_cast<Material *>(matliste.GetAt(nextpos));

		XASSERT(first && second);
		if (!first || !second)
			return;
		//if (first->get_id() > second->get_id() || first->get_id() == second->get_id() && first->get_gruppenid() > second->get_gruppenid())
		//if (first->get_id() > second->get_id() || first->get_id() == second->get_id() && first->get_artikelnr() > second->get_artikelnr())
		if (first->get_id() > second->get_id() || first->get_id() == second->get_id() && first->get_kurztext() > second->get_kurztext())
		{	
			// Material werden vertauscht
			matliste.SetAt(nextpos, first);
			matliste.SetAt(pos, second);
			i = -1; // Wieder von vorn beginnen
		}
	}
	// dann Dummynummern ans Ende stellen
	for (i = 0; i < matliste.GetCount(); i++)	
	{
		pos = matliste.FindIndex(i);
		Material *mat = dynamic_cast<Material *>(matliste.GetAt(pos));
		if (mat->get_artikelnr() < 100000 || mat->get_einkauf())
		{
			matliste.RemoveAt(pos);
			matliste.AddTail(mat);
		}
	}
}
