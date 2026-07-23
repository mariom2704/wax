#include "stdafx.h"


#include "WpPreisanfrage.h"
#include "PreisanfrageForm.h"


IMPLEMENT_SERIAL(WpPreisanfrage, WaxPart, VERSIONABLE_SCHEMA|1)

BEGIN_MESSAGE_MAP(WpPreisanfrage, WaxPart)
	ON_COMMAND(ID_FREIGABE, on_freigeben) 
END_MESSAGE_MAP()

WpPreisanfrage::WpPreisanfrage()
{
	/**
	Konstruktor
	*/
	set_icon(IDI_GLIEDERUNG);
	CString titel;
	set_werkauftrag(new Werkauftrag);
	get_werkauftrag()->set_bezeichnung(titel);

	

}

WpPreisanfrage::~WpPreisanfrage()
{
	
}


void WpPreisanfrage::initPopUpMenu()
{	
	CMenu* menu = getPopUpMenu();

	menu_items items[] =
	{
		ADD_MENU_ITEM(WAXID_NEUER_SAMMELAUFTRAG)
		{ID_SEPARATOR, ""},
		{ -1, "" },
	};

	appendMenuItems(menu, items);
	appendStandardMenuItems(menu);
}

void WpPreisanfrage::serialize(BinaryArchive &ar)
{
	/**
	Objekt abspeichern mit allen Unterobjeken
	*/
	if (get_deleted())
		return;
	WaxPart::serialize(ar);
	get_werkauftrag()->serialize(ar);
	if (ar.is_storing())
	{
		ar << kommentar;
	}
	else
	{
		ar >> kommentar;
	}
}




void WpPreisanfrage::getForms(CObList &RuntimeModules)
{
	/**
	Liste mit Bearbeitungsklassen/Masken zurückliefern
	*/
	RuntimeModules.AddTail((CObject*)RUNTIME_CLASS(PreisanfrageForm));
}

#if 0
void WpPreisanfrage::on_freigeben()
{
	/**
	Freigeben eines ganzen Bauabschnitts
	*/
	freigeben();
}
#endif



bool WpPreisanfrage::isPreisanfrage()const
{
	return true;
}


bool WpPreisanfrage::testLeitkarte(bool isPreisanfrage)
{
	/**
	Prüft die Eingaben der Leitkarten aller Unteraufträge auf Vollständigkeit
	*/
	for (POSITION pos = get_subobjects().GetHeadPosition(); pos; )
	{
		WaxPart *sub = dynamic_cast<WaxPart*>(get_subobjects().GetNext(pos));
		XASSERT(sub);
		if (sub)
		{
			if (!sub->testLeitkarte(isPreisanfrage))
				return false;
		}
	}
	return true;
}

CString WpPreisanfrage::getBezeichnung() const
{
	CString angebot;

	const WaxPart *pt = this;
	const SammelAuftrag *sammel = getFirstSammelauftrag(pt);
	if (sammel)
	{
		if (sammel->get_werkauftrag_const()->get_status_const().get_status() == Status::Angebot ||
			sammel->get_werkauftrag_const()->get_status_const().get_status() == Status::Freigegeben ||
			sammel->get_werkauftrag_const()->get_status_const().get_status() == Status::FreigegebenAv ||
			sammel->get_werkauftrag_const()->get_status_const().get_status() == Status::PreisanfrageStorniert ||
			sammel->get_werkauftrag_const()->get_status_const().get_status() == Status::AngebotAbgelehnt ||
			sammel->get_werkauftrag_const()->get_status_const().get_status() == Status::InBearbeitungAv)
		{
			Status stat;
			angebot.Format(" (%s)", sammel->getStatusText());
		}
	}
	
	CString bez;
	bez.Format("PA %i%s", get_werkauftrag_const()->get_leitkarte().get_preisanfrage().get_nummer(), angebot);

	return bez;
}

bool WpPreisanfrage::subobj_allowed() const
{
	/**
	Nur Gliederungen und Sammelaufträge können Unterobjekte enthalten
	*/
	return true;
}

///Ist das Icon für nicht freigegeben zu modifizieren
bool WpPreisanfrage::is_freigegeben() const
{
	/**
	Zeigt an, dass der Werkauftrag einen Status hat, auf den hingewiesen werden muss.
	Dies ist bis jetzt nur: Werkauftrag angelegt aber nicht freigegeben. 
	Überlagertes Icon ist IDI_ICON_NICHT_FREIGEGEBEN (Ausrufezeichen in linker oberer Ecke)
	Es werden auch alle Unterobjekte überprüft, dieses Flag vererbt sich in der Hierarchie nach 
	oben, d.h. Auch eine Gliederung erhält das nicht freigegeben Symbol, wenn auch nur ein Unterobjekt
	nicht freigegeben wurde.
	false := nicht freigegeben -> !-Zeichen setzen
	true := freigegeben       -> kein !-Zeichen
	*/	
	const Werkauftrag* werk = get_werkauftrag_const();
	if (!werk || get_deleted())
		return true;
	
	for (POSITION pos = get_subobjects_const().GetHeadPosition(); pos; )
	{
		const WaxPart *pt = dynamic_cast<const WaxPart*>(get_subobjects_const().GetNext(pos));
		if (!pt)
			continue;
		if (!pt->is_freigegeben())
			return false;
	}
	return true;
}



Status::STATUS WpPreisanfrage::get_status() const
{
	const Werkauftrag* werk = get_werkauftrag_const();
	if (!werk || get_deleted())
		return Status::NichtGesetzt;
	
	for (POSITION pos = get_subobjects_const().GetHeadPosition(); pos; )
	{
		const WaxPart *pt = dynamic_cast<const WaxPart*>(get_subobjects_const().GetNext(pos));
		if (!pt)
			continue;
		//Status von erster Position holen
		return pt->get_status();
	}
	return Status::NichtGesetzt;
}

///Summiert die Preise aller Unterordner
double  WpPreisanfrage::getGesamtpreis(WaxPart* ptHead)
{
	double preis = 0.0;

	if (ptHead->get_deleted())
		return 0.0;

	SammelAuftrag* sammel = dynamic_cast<SammelAuftrag*>(ptHead);
	if (sammel && sammel->get_werkauftrag())
	{
		if (sammel->get_deleted())
			return 0.0;

		preis += sammel->get_werkauftrag()->get_leitkarte().get_preisanfrage().get_preis();
	}
	else if (ptHead->get_subobject_count() > 0)
	{
		for (POSITION pos = ptHead->get_subobjects().GetHeadPosition(); pos != NULL; )
		{
			WaxPart* pt = dynamic_cast<WaxPart*>(ptHead->get_subobjects().GetNext(pos));
			preis += getGesamtpreis(pt);
		}
	}

	return preis;
}

const SammelAuftrag* WpPreisanfrage::getFirstSammelauftrag(const WaxPart* ptHead) const
{
	const SammelAuftrag* sammel = dynamic_cast<const SammelAuftrag*>(ptHead);
	if (sammel && !sammel->get_deleted())
		return sammel;
	else if (ptHead->get_subobject_count() > 0)
	{
		for (POSITION pos = ptHead->get_subobjects().GetHeadPosition(); pos != NULL; )
		{
			const WaxPart* pt = dynamic_cast<const WaxPart*>(ptHead->get_subobjects().GetNext(pos));
			sammel = getFirstSammelauftrag(pt);
			if (sammel)
				return sammel;
		}
	}

	return NULL;
}


/**
Gueltig bis an die Unterordner weiter geben
*/
void WpPreisanfrage::copyGueltigBisToChilds(WaxPart *pt, MXDate gueltigBis)
{
	for (POSITION pos = pt->get_subobjects().GetHeadPosition(); pos != NULL; )
	{
		WaxPart* sub = dynamic_cast<WaxPart*>(pt->get_subobjects().GetNext(pos));
		if (sub && sub->get_werkauftrag())
		{
			sub->get_werkauftrag()->get_leitkarte().get_preisanfrage().set_gueltigBis(gueltigBis);
			
			if (sub->get_subobject_count() > 0)
				copyGueltigBisToChilds(sub, gueltigBis);
		}
	}
}