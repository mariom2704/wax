#ifndef __PROPPAGEGEPROJEKT_H__
#define __PROPPAGEGEPROJEKT_H__

#include "waxproppage.h"

/**
PropPage Dialogfeld für Pfade und Projektangaben, Datenbank und Leitkarte
*/
class PropPageProjekt : public WaxPropPage
{
	DECLARE_MESSAGE_MAP()
public:
	/// Konstruktor
	PropPageProjekt(MxParameter *para);

	/// Destructor
	~PropPageProjekt();

	virtual BOOL OnApply();

protected:

	virtual BOOL OnInitDialog();

private:

	/// Auswahl des Excelpfades
	void on_excel(); 

	/// Auswahl des Projektpfades
	void on_projekte(); 

	/// Auswahl des Komponentenpfades
	void on_komp(); 

	/// Pfad zur Stammdatenbank auswählen 
	void on_stammdb(); 

	/// Pfad zur Logdatenbank auswählen 
	void on_logdb(); 
};

#endif 

