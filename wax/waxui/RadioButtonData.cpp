#include "stdafx.h"

#include "resource.h"

#include "RadioButtonData.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//Initialisierung der Elementeids. Die dlgids werden für die Benutzung in 
//PumpenverteilerEingabe initialisiert. Bei Benutzung in anderen Klassen muss 
//set_dlgid() aufgerufen werden.
RadioButtonData::RadioButtonData() : dlg(NULL)
{
	int i = 0;
	dlgid[i++] = IDC_FLACHFLANSCH_DN2;
	dlgid[i++] = IDC_NUTSTUTZEN_DN2;
	dlgid[i++] = IDC_GLATT_DN2;
	
	dlgid[i++] = IDC_FLACHFLANSCH_DN3;
	dlgid[i++] = IDC_NUTSTUTZEN_DN3;
	dlgid[i++] = IDC_GLATT_DN3;

	
	i = 0;

	elemid[i++] = FFL;
	elemid[i++] = NUT;
	elemid[i++] = GLATT; 

	elemid[i++] = FFL;
	elemid[i++] = NUT;
	elemid[i++] = GLATT; }

//Setzt den richtigen Radio Button
void RadioButtonData::setRadioButtons(int elemidAbgangA, int elemidAbgangB)
{
	int i;
	for (i = 0; i < 3; i++)
	{
		((CButton*)dlg->GetDlgItem(dlgid[i]))->SetCheck(elemid[i] == elemidAbgangA ? 1 : 0);
	}
	for (i = 3; i < 6; i++)
	{
		((CButton*)dlg->GetDlgItem(dlgid[i]))->SetCheck(elemid[i] == elemidAbgangB ? 1 : 0);
	}
}

int RadioButtonData::getRadioButton(bool abgangA)
{
	int offset = abgangA ? 0 : 3;

	for (int i = offset; i < offset+3; i++)
	{
		if (((CButton*)dlg->GetDlgItem(dlgid[i]))->GetCheck())
			return elemid[i];
	}
	XASSERT(FALSE);
	return 0;
}

void RadioButtonData::set_dlg(CDialog* value)
{
	dlg = value;
}



void RadioButtonData::set_dlgid(int index, int value)
{
	dlgid[index] = value;
}

