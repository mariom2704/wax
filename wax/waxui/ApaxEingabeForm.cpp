#include "stdafx.h"

#include "resource.h"
#include "ApaxEditArbeitsfolge.h"
#include "ApaxEingabeForm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(ApaxEingabeForm, ArbeitEingabeForm)
END_MESSAGE_MAP()

IMPLEMENT_DYNCREATE(ApaxEingabeForm, ArbeitEingabeForm)


ApaxEingabeForm::ApaxEingabeForm()
{
}

ApaxEingabeForm::~ApaxEingabeForm()
{
}


BOOL ApaxEingabeForm::OnInitDialog()
{
	ArbeitEingabeForm::OnInitDialog();
	commitUpdate();
	return TRUE;
}
bool ApaxEingabeForm::editAfo()
{
	POSITION pos = list.GetFirstSelectedItemPosition();
	if (!pos)
		return false;
	int index = list.GetNextSelectedItem(pos);
	Arbeitsfolge af;
	readArbeitsfolge(index, af);

	ApaxEditArbeitsfolge e(af, this);
	if (e.DoModal() == IDOK)
	{
		writeArbeitsfolge(af, index);
		return true;
	}
	return false;
}

