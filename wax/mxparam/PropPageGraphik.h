#ifndef __PROPPAGEGEGRAPHIK_H__
#define __PROPPAGEGEGRAPHIK_H__

#include "waxproppage.h"

/// PropPage Dialogfeld
class PropPageGraphik : public WaxPropPage
{
public:
	/// Konstruktor
	PropPageGraphik(MxParameter *para);

	/// Destructor
	~PropPageGraphik();

	virtual BOOL OnApply();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    ///  DDX/DDV-Unterstützung

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

	/// Default-Werte setzen
	virtual void set_default();


private:
	/// Fontgroesse
	int paraFontgroesse;

	/// Bemassungslinie an/aus
	int paraBemassungslinie;
	
	/// Trennlinie beim Ausdruck zwischen den Graphiken
	int paraTrennlinie;
	
	/// Rahmen beim Ausdruck um die Graphiken
	int paraRahmen;

	/// Die Rohrstuecke mit Laengenangaben versehen (Gewinderohrfertigung)
	int paraBeschriftunglaenge;  	
};

#endif ///  __PROPPAGEGEGRAPHIK_H__

