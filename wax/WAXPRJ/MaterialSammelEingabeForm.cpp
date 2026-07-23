#include "stdafx.h"


#include "MaterialSammelEingabeform.h"


IMPLEMENT_DYNCREATE(MaterialSammelEingabeForm, MaterialEingabeForm)

BEGIN_MESSAGE_MAP(MaterialSammelEingabeForm, MaterialEingabeForm)
END_MESSAGE_MAP()
	
/// Konstruktor
MaterialSammelEingabeForm::MaterialSammelEingabeForm()
{
}

/// Destruktor
MaterialSammelEingabeForm::~MaterialSammelEingabeForm()
{
}

void MaterialSammelEingabeForm::DoDataExchange(CDataExchange* pDX)
{
	if (!validWerkauftrag())
		return;

	if (!pDX->m_bSaveAndValidate)
		prepare_material();
	MaterialEingabeForm::DoDataExchange(pDX);

	if (pDX->m_bSaveAndValidate)
		move_material();
}

void MaterialSammelEingabeForm::prepare_material()
{
	get_data()->get_werkauftrag()->get_material().clean();
	for (POSITION pos = get_data()->get_subobjects().GetHeadPosition(); pos != NULL; )
	{
		WaxPart *sub = dynamic_cast<WaxPart*>(get_data()->get_subobjects().GetNext(pos));
		XASSERT(sub);
		if (!sub || sub->get_deleted())
			continue;
		get_data()->get_werkauftrag()->get_material().merge(sub->get_werkauftrag()->get_material());
	}
}


void MaterialSammelEingabeForm::move_material()
{
	WaxPart *sub = NULL;
	for (POSITION pos = get_data()->get_subobjects().GetTailPosition(); pos != NULL; )
	{
		sub = dynamic_cast<WaxPart*>(get_data()->get_subobjects().GetPrev(pos));
		XASSERT(sub);
		if (!sub || sub->get_deleted())
			continue;
		sub->get_werkauftrag()->get_material().clean();
	}
	if (get_data()->getFirstSubobject())
	{
		get_data()->getFirstSubobject()->get_werkauftrag()->get_material().copy(get_data()->get_werkauftrag()->get_material());
	}
}

