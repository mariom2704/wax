#pragma once


#include <mxcontrols/CtrlComboBoxDn.h>

/**
Eingabemaske für Rohreigenschaften
Bearbeitung der Eigenschaften für alle Rohrarten
*/
class WAXUIDLL MuffenEingabe : public WaxPartForm  
{
	DECLARE_DYNCREATE(MuffenEingabe)

public:
	MuffenEingabe();

	virtual ~MuffenEingabe();

	
	virtual bool check_plausis();

protected:

	CtrlComboBoxDn dnComboBox;

	
	virtual void commitUpdate();

	DECLARE_MESSAGE_MAP()

	virtual void DoDataExchange(CDataExchange* pDX);

	virtual BOOL OnInitDialog();

	void OnCbnSelchangeElementType();
	void initSelchangeElementType(bool pos1);

	void OnCbnSelchangeMuffenDn();
	void initSelchangeMuffenDn(bool pos1);


	//Alle Positionen im Sammelauftrag syncronisieren
	void syncSammelauftrag();

private:
	FloatEditCtrl ed_durchmesser;

//	bool MustBeForcedGekehlteMuffen();
	bool ForceCheckGekehlteMuffen();

	double durchmesser; 

	int muffenRestabstand;

	bool m_bMuffenGekehlt;

	int muffenDN;
	int m_MuffenDNPrev;

	int muffenID;

	int muffenRelAbst[Strangrohr::MAX_ANZAHL_STDELEMENTE+1];

	int muffenLaenge;

	/// Fehlermeldung
	CString err;

	Strangrohr* getStrangrohr();

	void DoUpdate();	
};
