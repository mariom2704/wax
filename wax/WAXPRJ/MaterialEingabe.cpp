#include "stdafx.h"


#include "MaterialEingabe.h"


BEGIN_MESSAGE_MAP(MaterialEingabe, CPropertySheet)
	ON_COMMAND(ID_REBUILD_TABS, onRebuildTabs)
END_MESSAGE_MAP()

MaterialEingabe::MaterialEingabe(Material *material, bool profiltab)
: CPropertySheet("Materialeingabe"), editmat(material), profil(material)
{
	EnableStackedTabs(TRUE);
	mat = material;
	active_profiltab = profiltab;
	initTabs();
}

MaterialEingabe::~MaterialEingabe()
{
}

BOOL MaterialEingabe::OnInitDialog()
{
	if (!CPropertySheet::OnInitDialog())
		return false;
	return true;
}


void MaterialEingabe::initTabs()
{
	if (GetPageIndex(&editmat) < 0)
		AddPage(&editmat);

	if (GetPageIndex(&profil) >= 0)
		RemovePage(&profil);
	
	if (!mat->get_profilkennung())
	{

		CString typ;
		for (POSITION pos = getStammdaten().get_profile().GetHeadPosition(); pos;)
		{
			const Profile::data *prof = dynamic_cast<const Profile::data*>(getStammdaten().get_profile().GetNext(pos));
			XASSERT(prof);
			if (!prof)
				continue;
			if (prof->get_artikelnr() == mat->get_artikelnr())
			{
				typ = prof->get_typ();
				break;
			}
		}
	
		if (typ.GetLength())
		{
			mat->set_profilkennung(typ);
			profil.set_typ(typ);

			AddPage(&profil);
		}
	}
	else
	{
		profil.set_typ(mat->get_profilkennung());
		AddPage(&profil);
		if (active_profiltab)
			SetActivePage(GetPageIndex(&profil));
	}
}


void MaterialEingabe::onRebuildTabs()
{
	initTabs();
	SetActivePage(1);
}

