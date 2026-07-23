#include "stdafx.h"



#include "SonderkostenSammelEingabeForm.h"


IMPLEMENT_DYNCREATE(SonderkostenSammelEingabeForm, SonderkostenEingabeForm)

BEGIN_MESSAGE_MAP(SonderkostenSammelEingabeForm, SonderkostenEingabeForm)
END_MESSAGE_MAP()
	
/// Konstruktor
SonderkostenSammelEingabeForm::SonderkostenSammelEingabeForm()
{
}

/// Destruktor
SonderkostenSammelEingabeForm::~SonderkostenSammelEingabeForm()
{
}

void SonderkostenSammelEingabeForm::DoDataExchange(CDataExchange* pDX)
{
	if (!validWerkauftrag())
		return;

	if (!pDX->m_bSaveAndValidate)
		prepare_sokos();
	SonderkostenEingabeForm::DoDataExchange(pDX);
}

void SonderkostenSammelEingabeForm::prepare_sokos()
{
	/**
	Die Sokos aus allen Unteraufträgen zusammengefasst. Ist der Text gleich, werden 
	die Preise addiert. Alle Positionen werden neu durchnummeriert
	*/
	CObList &sokos = get_data()->get_werkauftrag()->get_preis().get_sokolist();
	get_data()->get_werkauftrag()->get_preis().delete_sokolist(true);
	int counter = 0;
	
	for (POSITION pos = get_data()->get_subobjects().GetHeadPosition(); pos != NULL; )
	{
		WaxPart *sub = dynamic_cast<WaxPart*>(get_data()->get_subobjects().GetNext(pos));
		XASSERT(sub);
		if (!sub || sub->get_deleted())
			continue;
		CObList &soks = sub->get_werkauftrag()->get_preis().get_sokolist();
		for (POSITION p = soks.GetHeadPosition(); p; )
		{
			Preis::Sonderkosten *othersoko = dynamic_cast<Preis::Sonderkosten*>(soks.GetNext(p));
			if (othersoko)
			{
				POSITION xp;
				// Testen, ob bereits eine Position besteht
				for (xp = sokos.GetHeadPosition(); xp; )
				{
					Preis::Sonderkosten *ss = dynamic_cast<Preis::Sonderkosten*>(sokos.GetNext(xp));
					if (ss)
					{
						if (ss->get_text() == othersoko->get_text())
						{
							ss->set_preis(ss->get_preis() + othersoko->get_preis());
							othersoko = NULL;
							break;
						}
					}
				}
				if (!xp && othersoko)
				{
					Preis::Sonderkosten *newsoko = new Preis::Sonderkosten(*othersoko);
					newsoko->set_nr(++counter);
					sokos.AddTail(newsoko);
				}
			}
				
		}
	}
}


BOOL SonderkostenSammelEingabeForm::OnInitDialog()
{
	if (!SonderkostenEingabeForm::OnInitDialog())
		return false;
	UINT disabled[] = { IDC_SONDERKOSTEN, IDC_NEW, IDC_EDIT, IDC_DELETE, 0 };

	for (int i = 0; disabled[i]; i++)
		GetDlgItem(disabled[i])->EnableWindow(FALSE);
	return true;
}
