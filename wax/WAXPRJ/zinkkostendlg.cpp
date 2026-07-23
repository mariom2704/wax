#include "stdafx.h"


#include "afxcoll.h"
#include "sammelauftrag.h"
#include "zinkkostendlg.h"


#include "..\rohrkonst\zinkkosten.h"


#include <mxutil/numericformatter.h>
using namespace MxUtil;

BEGIN_MESSAGE_MAP(ZinkKostenDlg, CDialog)
END_MESSAGE_MAP()

ZinkKostenDlg::ZinkKostenDlg(CObList& sm) : BaseDialog(IDD_PREISAUSGABE)
{
	add_rohre(sm);
}


void ZinkKostenDlg::add_rohre(CObList& p)
{
	// Interne Liste mit Rohren für die Preisberechnung erstellen
	for (POSITION pos = p.GetHeadPosition(); pos != NULL; )
	{
		WaxPart *pt = dynamic_cast<WaxPart*>(p.GetNext(pos));
		if (!pt || pt->get_deleted())
			continue;
		Rohrleitung *r = dynamic_cast<Rohrleitung*>(pt->get_werkauftrag());
		if (r && r->getBeschichtung().isVerzinkt())
		{
			r->createModell();
			rohre.push_back(r);
		}
		// rekursiv alle Unterobjekte erfassen
		add_rohre(pt->get_subobjects());
	}
}

ZinkKostenDlg::~ZinkKostenDlg()
{
}

BOOL ZinkKostenDlg::DestroyWindow()
{
	return CWnd::DestroyWindow();
}

void ZinkKostenDlg::init_ui()
{
	// Dialog ist eh nur fürs Segment Rohr, also brauchts hier auch nicht mehrsprachig zu sein
	SetWindowText("Verzinkungskosten");
	list.InsertColumn(BEZEICHNUNG, "Bezeichnung", LVCFMT_LEFT,  250);
	list.InsertColumn(MENGE, "Menge",	LVCFMT_LEFT,  80);
	list.InsertColumn(PREIS, "Preis",	LVCFMT_LEFT,  80);
	list.InsertColumn(ZUSCHLAG, "Zuschlag", LVCFMT_LEFT, 80);
	list.InsertColumn(BETRAG, "Betrag",	LVCFMT_RIGHT, 100);

	show_values();
}

void ZinkKostenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PREISE, list);
}


void ZinkKostenDlg::show_values()
{
	ZinkKosten gesamt;

	bool bZinkkostenAvailable = false;
	for( vector<const Rohrleitung*>::iterator it = rohre.begin(); it != rohre.end(); it++)
	{
		const Rohrleitung* r = *it;
		if (!r)
			continue;
		const Verzinkung::data* d = getStammdaten().get_verzinkung().query(r->getBeschichtung().get_verzinkerei());
		if (!d)
			continue;

		bZinkkostenAvailable = true;
		ZinkKosten z(d, *r);
		gesamt.SetGesamtVerzinkerei(d);
		gesamt.add(z);
	}

	if( !bZinkkostenAvailable )
	{
		return;
	}
	NumericFormatter nf(get_locale());
	int line = 0;
	list.addItem(line, BEZEICHNUNG, "Rohrkonstruktion");
	list.SetItemText(line, MENGE, (nf.to_string_locale(gesamt.GesamtGewicht()) + (string)" kg").c_str()); // Gesamtgewicht
	list.SetItemText(line, PREIS, (nf.to_string_locale(gesamt.get_euro_pro_kg()) + (string)" €/kg").c_str()); // Euro pro Kg
	list.SetItemText(line, BETRAG, (nf.to_string_locale(gesamt.GesamtGrundkosten()) + (string)" €").c_str()); // Basiskosten fürs Verzinken
	list.addItem(++line, BEZEICHNUNG, "Zinkpreisausgleich");
	list.SetItemText(line, ZUSCHLAG, (nf.to_string_locale(gesamt.get_zuschlagproz()) + (string)" %").c_str());
	list.SetItemText(line, BETRAG, (nf.to_string_locale(gesamt.GesamtZuschlag()) + (string)" €").c_str());
	
	CString stdele;
	stdele.Format("Einbaufähig putzen: %i x Standardelemente", gesamt.get_element_anzahl(ZinkKosten::STDELEMENT));
	list.addItem(++line, BEZEICHNUNG, stdele);
	list.SetItemText(line, MENGE, (nf.to_string_locale(gesamt.get_putz_zeit(ZinkKosten::STDELEMENT)) + (string)" Min").c_str());
	double stdsatz = 0.0;//gesamt.get_stundensatz();
	list.SetItemText(line, PREIS, (nf.to_string_locale(stdsatz) + (string)" €/Std").c_str());
	list.SetItemText(line, BETRAG, (nf.to_string_locale(gesamt.get_putzkosten(ZinkKosten::STDELEMENT)) + (string)" €").c_str());
	
	CString ag;
	ag.Format("Einbaufähig putzen: %i x Aussengewinde", gesamt.get_element_anzahl(ZinkKosten::AUSSENGEW));
	list.addItem(++line, BEZEICHNUNG, ag);
	list.SetItemText(line, MENGE, (nf.to_string_locale(gesamt.get_putz_zeit(ZinkKosten::AUSSENGEW)) + (string)" Min").c_str());
	list.SetItemText(line, PREIS, (nf.to_string_locale(gesamt.get_stundensatz()) + (string)" €/Std").c_str());
	list.SetItemText(line, BETRAG, (nf.to_string_locale(gesamt.get_putzkosten(ZinkKosten::AUSSENGEW)) + (string)" €").c_str());
	list.addItem(++line, BEZEICHNUNG, "Zink mit MGK(7%)");
	list.SetItemText(line, BETRAG, (nf.to_string_locale(gesamt.GesamtMGK()) + (string)" €").c_str());
	list.addItem(++line, BEZEICHNUNG, "VK Aufschlag");
	list.SetItemText(line, BETRAG, (nf.to_string_locale(gesamt.GesamtVKZuschlag()) + (string)" €").c_str());
	list.addItem(++line, BEZEICHNUNG, "Gesamt");
	list.SetItemText(line, BETRAG, (nf.to_string_locale(gesamt.GesamtKosten()) + (string)" €").c_str());
}

