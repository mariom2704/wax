#include "stdafx.h"


#include "befehlshowerror.h"
#include "DialogStringListe.h"
#include "resource.h"

#include "../rohrkonst/mxerror.h"

using namespace MxErrorHandling;

BefehlShowError::BefehlShowError(void)
{
}

BefehlShowError::~BefehlShowError(void)
{
}


bool BefehlShowError::start()
{
	if (DialogStringListe::is_active())
		return false;

	const vector<MxError>& erListe = getErrorListe();

	CStringArray arError;


	if (erListe.empty())
		return true;

	//Liste in Stringarray umwandeln
	for (vector<MxError>::const_iterator it = erListe.begin(); it != erListe.end(); it++)
		arError.Add(it->getErrorDescription().c_str());
	
	CString dialogText;
	dialogText = getStammdaten().getText("IDS_DIALOG_FEHLER").c_str();

	DialogStringListe dialog;
	dialog.setWerte(arError,dialogText);
	

	dialog.DoModal();
	return true;
}