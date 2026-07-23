// Copyright (C) 2001 Minimax GmbH

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_RADIOBUTTONDATA_3E3792FC036E_INCLUDED
#define _INC_RADIOBUTTONDATA_3E3792FC036E_INCLUDED

//Um die 6 Radio Buttons einfacher abfragen und setzen zu können sind über diese 
//Struktur die IDs den Elementids zugeordnet.
class RadioButtonData 
{
public:
	void set_dlgid(int index, int value);

	//Initialisierung der Elementeids. Die dlgids werden für die Benutzung in 
	//PumpenverteilerEingabe initialisiert. Bei Benutzung in anderen Klassen muss 
	//set_dlgid() aufgerufen werden.
	RadioButtonData();

	//Setzt den richtigen Radio Button
	void setRadioButtons(int elemidAbgangA, int elemidAbgangB);

	int getRadioButton(bool abgangA);

	void set_dlg(CDialog* value);

private:
	//Dialog IDs der 6 Radiobuttons
	int dlgid[6];

	//Elementids der 6 Radio Buttons
	int elemid[6];

	CDialog* dlg;

};

#endif /* _INC_RADIOBUTTONDATA_3E3792FC036E_INCLUDED */
